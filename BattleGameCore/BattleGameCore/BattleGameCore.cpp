// BattleGameCore.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "lua.hpp"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

lua_State *L = NULL;

enum MachineStatus
{
	RUNNING, STOPPED,
};

enum MessageType
{
	SENTENCE, ASK, ASKEND,
};

enum QuestionType
{
	INTEGER, STRING,
};

class Message
{
private:
	MessageType type;
	string messageBody;
public:
	inline MessageType getMessageType(){ return type; }
	inline string getMessageBody(){ return messageBody; }
	Message(string MessageBody, MessageType Type = SENTENCE) :type(Type), messageBody(MessageBody){}
};

int debug_out(lua_State* l)
{
	if (lua_gettop(l) <= 0)
		return 0;
	cerr << lua_tostring(l, 1) << endl;
	return 0;
}

bool checkAMessage()
{
	lua_getglobal(L, "system");
	lua_getfield(L, -1, "hasNewMessage");
	lua_remove(L, -2);
	if (lua_pcall(L, 0, 1,NULL))
		cerr<<lua_tostring(L,-1)<<endl;
	bool temp=lua_toboolean(L, -1);
	lua_pop(L, 1);
	return temp;
}

Message pullAMessage()
{
	lua_getglobal(L, "system");
	lua_getfield(L, -1, "getNewMessage");
	lua_remove(L, -2);
	if (lua_pcall(L, 0, 1, NULL))
		cerr << lua_tostring(L, -1) << endl;
	string str = lua_tostring(L, -1);
	lua_pop(L, 1);
	switch (str.at(0))
	{
	case 'a':
		return Message(str.substr(1), ASK);
		break;
	case 'A':
		return Message(str.substr(1), ASKEND);
		break;
	default:
		return Message(str.substr(1));
		break;
	}
}

void initRunningStatus()
{
	L = luaL_newstate();
	luaL_openlibs(L);
	if(luaL_dofile(L, "system.lua"))
	{
		cerr << "File open ERROR." << endl;
		cerr << lua_tostring(L, -1) << endl;
		exit(-1);
	}
	//switch (luaL_loadfile(L, "system.lua"))
	//{
	//case LUA_OK:
	//	cerr << "LUA_OK" << endl;
	//	break;
	//case LUA_ERRSYNTAX:
	//	cerr << "LUA_ERRSYNTAX" << endl;
	//	break;
	//case LUA_ERRMEM:
	//	cerr << "LUA_ERRMEM" << endl;
	//	break;
	//case LUA_ERRGCMM:
	//	cerr << "LUA_ERRGCMM" << endl;
	//	break;
	//case LUA_ERRFILE:
	//	cerr << "LUA_ERRFILE" << endl;
	//	break;
	//}
	
	lua_register(L, "DEBUG_OUT", debug_out);
	lua_pushinteger(L, RUNNING);
	lua_setglobal(L, "RUNNING");
	lua_pushinteger(L, STOPPED);
	lua_setglobal(L, "STOPPED");
}

bool checkRunningStatus()
{
	lua_getglobal(L, "system");
	lua_getfield(L, -1, "getRunningStatus");
	lua_remove(L, -2);
	if (lua_pcall(L, 0, 1, NULL))
		cerr << lua_tostring(L, -1) << " At line " << __LINE__ << endl;
	int st = lua_tointeger(L, -1);
	lua_pop(L, 1);
	MachineStatus status = static_cast<MachineStatus>(st);
	switch (status)
	{
	case RUNNING:
		return true;
	case STOPPED:
		return false;
	default:
		cerr << "BattleGameCore.cpp : An unexpected result is returned in function 'checkRunningStatus' at line " << __LINE__ << endl;
		cerr << "BattleGameCore.cpp : Case taken as STOPPED." << endl;
		return false;
	}
}

vector<pair<string, QuestionType> > questions;

void addQuestion(string str,QuestionType type=INTEGER)
{
	questions.push_back(make_pair(str,type));
}

class Answer
{
public:
	QuestionType type;
	string str;
	int integer;
};

void askQuestions()
{
	vector<Answer> ans;
	for (auto pair : questions)
	{
		Answer a;
		a.type = pair.second;
		cout << pair.first;
		switch (pair.second)
		{
		case INTEGER:
			cin >> a.integer;
			break;
		case STRING:
			cin >> a.str;
			break;
		}
		ans.push_back(a);
	}
	lua_getglobal(L, "system");
	for (auto a : ans)
	{
		lua_getfield(L, -1, "acceptAnswer");
		switch (a.type)
		{
		case INTEGER:
			lua_pushinteger(L, a.integer);
			break;
		case STRING:
			lua_pushstring(L, a.str.c_str());
			break;
		}
		if (lua_pcall(L, 1, 0, NULL))
		{
			cerr << "BattleGameCore.cpp:" << __LINE__ << lua_tostring(L, -1) << endl;
			exit(-1);
		}
	}
	lua_pop(L, 1);
}

int _tmain(int argc, _TCHAR* argv[])
{
	initRunningStatus();
	while (checkRunningStatus())
	{
		lua_getglobal(L, "system");
		lua_getfield(L, -1, "mainLoop");
		lua_pcall(L, 0, 0, NULL);
		lua_pop(L, 1);
		while (checkAMessage())
		{
			Message msg = pullAMessage();
			switch (msg.getMessageType())
			{
			case SENTENCE:
				cout << msg.getMessageBody() << endl;
				break;
			case ASK:
				addQuestion(msg.getMessageBody());
				break;
			case ASKEND:
				askQuestions();
				break;
			}
		}
	}
	while (checkAMessage())
	{
		Message msg = pullAMessage();
		switch (msg.getMessageType())
		{
		case SENTENCE:
			cout << msg.getMessageBody() << endl;
			break;
		default:
			break;
		}
	}
	lua_close(L);
	cerr << "BattleGameCore.cpp : Program exited successfully." << endl;
	return 0;
}

