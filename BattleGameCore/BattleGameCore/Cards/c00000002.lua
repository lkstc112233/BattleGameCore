c00000002 = {}

function c00000002.getText() 
	return "Å«Á¥µÄ¿¨Æ¬£¡"
end

function c00000002.getID()
	return -1
end

function c00000002.anti(toAnti)
	if toAnti.getId() == 1 then
		return 1
	else
		return -1
	end
end