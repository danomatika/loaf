-- hello world example with animated circle
--
-- control circle position using OSC messages:
--   /circle/x float
--   /circle/y float
--
-- try test Pure Data patch: helloworld.pd

angle = 0
xOffset = 0
yOffset = 0

function setup()
	of.setCircleResolution(50);
	of.background(50)
	
	of.setWindowTitle("hello world")
	print("hello world")

	-- start OSC listener
	loaf.setListenPort(9999)
	loaf.startListening()
end

function draw()

	-- draw circle using size computed with the sine function
	of.setColor(255, 200, 10)
	of.drawCircle(xOffset + of.getWidth()/2,
		          yOffset + of.getHeight()/2,
		          math.sin(angle) * 100 + 200)

	-- update the animation variable
	angle = angle + 0.01
end

function oscReceived(message)
	
	-- print received message
	print(tostring(message))

	-- parse messages based on address
	if message.address == "/circle/x" then
		xOffset = message:getArgAsFloat(0)
	elseif message.address == "/circle/y" then
		yOffset = message:getArgAsFloat(0)
	end
end
