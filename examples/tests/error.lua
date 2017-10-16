-- error test
-- press any key to cause a script error

function keyPressed(key)
	-- error: attempt to perform arithmetic on a nil value (global 'x')
	local i = 1 + x
end
