// BattleGameCore.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "lua.hpp"

#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
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

wstring StringToWString(const std::string &str)
{
	std::wstring wstr;
	int nLen = (int)str.length();
	wstr.resize(nLen, L' ');

	int nResult = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), nLen, (LPWSTR)wstr.c_str(), nLen);

	if (nResult == 0)
		return L"";

	return wstr;
}

string WStringToString(const std::wstring &wstr)
{
	std::string str;
	int nLen = (int)wstr.length();
	str.resize(nLen, ' ');
	int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);
	if (nResult == 0)
		return "";
	return str;
}

vector<string> DirectoryList(LPCWSTR Path)
{
	vector<string> files;
	WIN32_FIND_DATA FindData;
	HANDLE hError;
	int FileCount = 0;
	TCHAR FilePathName[MAX_PATH_LENGTH];
	// 构造路径
	TCHAR FullPathName[MAX_PATH_LENGTH];
	_tcscpy_s(FilePathName, Path);
	_tcscat_s(FilePathName, TEXT("\\*.*"));
	hError = FindFirstFile(FilePathName, &FindData);
	if (hError == INVALID_HANDLE_VALUE)
		return vector<string>();
	while (::FindNextFile(hError, &FindData))
	{
		// 过虑.和..
		if (_tcscmp(FindData.cFileName, TEXT(".")) == 0 || _tcscmp(FindData.cFileName, TEXT("..")) == 0)
			continue;

		// 构造完整路径
		wsprintf(FullPathName, TEXT("%s\\%s"), Path, FindData.cFileName);
		FileCount++;
		// 输出本级的文件
#ifdef UNICODE
		files.push_back(WStringToString(FullPathName));
#else
		files.push_back(FullPathName);
#endif // UNICODE

//		printf("\n%d  %s  ", FileCount, FullPathName);

		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			auto temp=DirectoryList(FullPathName);
			files.insert(files.end(), temp.begin(),temp.end());
		}
			
	}
	return files;
}

void addCard(const char* name)
{
	lua_getglobal(L, "cards");
	lua_getfield(L, -1, "inelm");
	lua_getglobal(L, "cards");
	lua_pushstring(L, name);
	if (lua_pcall(L, 2, 0, NULL))
		cerr << "ERROR:" << lua_tostring(L, -1) << "at line:" << __LINE__ << endl;;
	lua_pop(L,1);
}

int loadFilesInFolder(lua_State *l)
{
	if (lua_gettop(l) < 1)
		return 0;
	string fileName=lua_tostring(l, 1);
	auto files=DirectoryList(StringToWString(fileName).c_str());
	for (auto f : files)
	{
		auto p = f.rfind(".");
		if (p == std::string::npos)
			continue;
		auto tail = f.substr(p + 1);
		transform(tail.begin(), tail.end(), tail.begin(), ::tolower);
		if (tail != "lua")
			continue;
		luaL_dofile(L, f.c_str());
		auto rp = f.rfind("\\");
		auto name = f.substr(rp + 1, p - rp - 1);
		addCard(name.c_str());
	}
}

void initRunningStatus()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	lua_register(L, "DEBUG_OUT", debug_out);
	lua_register(L, "LOAD_FOLDER", loadFilesInFolder);
	lua_pushinteger(L, RUNNING);
	lua_setglobal(L, "RUNNING");
	lua_pushinteger(L, STOPPED);
	lua_setglobal(L, "STOPPED");
	lua_getglobal(L, "math");
	lua_getfield(L, -1, "randomseed");
	lua_pushinteger(L, rand());
	lua_pcall(L, 1, 0, NULL);
	lua_pop(L, 1);

	if(luaL_dofile(L, "system.lua"))
	{
		cerr << "File open ERROR." << endl;
		cerr << lua_tostring(L, -1) << endl;
		exit(-1);
	}
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
	srand((unsigned)(time(NULL)));
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

