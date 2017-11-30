-- error test
-- press any key to cause a script error

function setup()
	-- random background
	of.background(of.random(0, 255), of.random(0, 255), of.random(0, 255))
end

function keyPressed(key)
	-- error: attempt to perform arithmetic on a nil value (global 'x')
	local i = 1 + x
end
