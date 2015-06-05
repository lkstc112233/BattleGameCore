vector = {}

vector.m_size = 0

function vector.new()
	local ins = {}
	for key,var in pairs(vector) do
		ins[key]=var
	end
	ins.new = nil
	ins.store = {}
	return ins
end

function vector.push_back(self,elm)
	self.store[self.m_size] = elm
	self.m_size = self.m_size + 1
end

function vector.pop_back(self)
	if self.m_size == 0 then
		return nil
	end
	self.m_size = self.m_size - 1
	local ins = self.store[self.m_size]
	self.store[self.m_size] = nil
	return ins
end

function vector.at(self,ind)
	return self.store[ind]
end

function vector.insert(self,elm,pos)
	local ins = {}
	for key,var in pairs(self.store) do
		if key < pos then
			ins[key] = var
		else
			ins[key + 1] = var
		end
	end
	ins[pos] = elm
	self.store = elm
	self.m_size = self.m_size + 1
end

function vector.remove(self,pos)
	local ins = {}
	local temp = self.store[pos]
	for key,var in pairs(self.store) do
		if key <= pos then
			ins[key] = var
		else
			ins[key - 1] = var
		end
	end
	self.m_size = self.m_size - 1
	return temp
end

function vector.size(self)
	return self.m_size
end

