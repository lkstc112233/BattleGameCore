c00000003 = {}

function c00000003.getText() 
	return "Æ½ÃñµÄ¿¨Æ¬£¡"
end

function c00000003.getID()
	return 0
end

function c00000003.anti(toAnti)
	if toAnti.getId() == 1 then
		return -1
	elseif toAnti.getId() == 1 then
		return 1
	else
		return 0
	end
end