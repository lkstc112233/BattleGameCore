list = {}

list.head = 0
list.tail = 0

function list.new()
	local ins = {}
	for key,var in pairs(list) do
		ins[key]=var
	end
	ins.new = nil
	return ins
end

function list.inelm(self,elm)
	self[self.head] = elm
	self.head = self.head + 1
end

function list.outelm(self)
	if self.tail == self.head then
		return nil
	end
	local ins = self[self.tail]
	self[self.tail] = nil
	self.tail = self.tail + 1
	return ins
end

function list.size(self)
	return self.head - self.tail
end

