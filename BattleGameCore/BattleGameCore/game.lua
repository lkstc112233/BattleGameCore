game = {}

dofile("player.lua")

cards = list.new()

LOAD_FOLDER("Cards")

game.player1=player.new()
game.player2=player.new()

function game.beginas(buf)
	game.player1=player.new(buf)
	game.player2=player.new(not buf)
	local player1s=""
	local player2s=""
	if buf then
		player1s = "�ʵ۷�"
		player2s = "ū����"
	else
		player2s = "�ʵ۷�"
		player1s = "ū����"
	end
	system.messageList:inelm("s���1��" .. player1s)
	system.messageList:inelm("s���2��" .. player2s)
end

function game.begin()
	local buf = math.random(2) == 1
	game.beginas(buf)
end

function game.get(player1s,player2s)
	
end