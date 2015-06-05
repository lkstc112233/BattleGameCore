utilities = {}

function utilities.copyTable(tableToCopy)
	local temp={}
	for key,var in pairs(tableToCopy) do
		temp[key] = var
	end
	return temp
end