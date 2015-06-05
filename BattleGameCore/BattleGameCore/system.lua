system = {}

dofile("utilities.lua")
dofile("list.lua")
dofile("game.lua")

system.isRunning = true
system.messageList = list.new();
system.answerList = list.new();

system.getRunningStatus = function()
	if system.isRunning then
		return RUNNING;
	else
		return STOPPED;
	end
end

system.hasNewMessage = function()
	if system.messageList:size() > 0 then
		return true;
	else
		return false;
	end
end

system.getNewMessage = function()
	local msg = system.messageList:outelm()
	if msg then
		return msg;
	else
		return "SFALL THROUGH message."
	end
end

function system.acceptAnswer(answer)
	--DEBUG_OUT(string.format('The answer got in acceptAnswer is %s',answer))
	system.answerList:inelm(answer)
end

function system.getAnswer()
	return system.answerList:outelm()
end

system.inited = false
function system.init()
	if not system.inited then
		system.inited = true
		system.messageList:inelm("sը�ѣ�������Ŀ�����Ϸ��")
		system.messageList:inelm("s��Ϸ������ʼ��")
	end
end

system.counter = 0

system.mainLoop = function()
	system.init()
	if system.counter == 0 then
		--local ans = system.getAnswer()
		--if ans then
		--	system.messageList:inelm(string.format('s��������ǣ�%d',ans))
		--	system.counter = system.counter + 1
		--end
		system.messageList:inelm("s��Ƭ����" .. cards:size() .. "��")
		local i = 1
		while cards:size() ~= 0 do
			local card = cards:outelm()
			system.messageList:inelm("s��" .. i .. "�ŵ������ǣ�" .. card)
			system.messageList:inelm("s��" .. i .. "���ǣ�" .. _G[card].getText())
		end
		system.counter = system.counter + 2
	elseif system.counter == 1 then
		local ans = system.getAnswer()
		if ans then
			if ans == 1 then
				ans = "����"
			else
				ans = "Ů��"
			end
			system.messageList:inelm(string.format('s����Ա��ǣ�%s',ans))
			system.counter = system.counter + 1
		end
	elseif system.counter == 2 then
		system.messageList:inelm(string.format('s��������������ɣ�%d',math.random(1000)))
		system.counter = system.counter + 1
	elseif system.counter == 3 then
		system.messageList:inelm('s��Ϸ��ʼ����')
		game.begin()
		system.counter = system.counter + 1
	else
		system.isRunning = false
	end
end
