player = {}

--list.store = {}

function player.new(belonging)
	local ins = {}
	for key,var in pairs(player) do
		ins[key]=var
	end
	ins.new = nil
	ins.belonging = belonging
	ins.cards = vector.new()
	return ins
end

function player.playcard(self,index)
	
end

--function list.inelm(self,elm)
--	self.store[self.head] = elm
--	self.head = self.head + 1
--end

--function list.outelm(self)
--	if self.tail == self.head then 
--		return nil
--	end
--	local ins = self.store[self.tail]
--	self.store[self.tail] = nil
--	self.tail = self.tail + 1
--	return ins
--end

--function list.size(self)
--	return self.head - self.tail;
--end

