c00000001 = {}

function c00000001.getText() 
	return "»ÊµÛµÄ¿¨Æ¬£¡"
end

function c00000001.getID()
	return 1
end

function c00000001.anti(toAnti)
	if toAnti.getId() == -1 then
		return -1
	else
		return 1
	end
end