-- event callback tests
-- prints to the console

updateOnce = false
drawOnce = false

function setup() 
	of.setWindowTitle("callbacks test")
	print("setup")
	print("mouseX "..mouseX.." mouseY "..mouseY)
	of.sendMessage("hello world")
end

function update()
	if not updateOnce then
		print("update")
		updateOnce = true
	end
end

function draw()
	if not drawOnce then
		print("draw")
		drawOnce = true
	end
end

function exit()
	print("exit")
end

function keyPressed(key)
	print("keyPressed "..key)
end

function keyReleased(key)
	print("keyReleased "..key)
end

function mouseMoved(x, y)
	-- check mouseX & mouseY globals
	print("mouseMoved "..x.." "..y.." (mouseX & mouseY "..mouseX.." "..mouseY..")")
end

function mouseDragged(x, y, button) 
	print("mouseDragged "..x.." "..y.." "..button)
end

function mousePressed(x, y, button) 
	print("mousePressed "..x.." "..y.." "..button)
end

function mouseReleased(x, y, button) 
	print("mouseReleased "..x.." "..y.." "..button)
end

function mouseEntered(x, y) 
	print("mouseEntered "..x.." "..y)
end

function mouseExited(x, y) 
	print("mouseExited "..x.." "..y)
end

function windowResized(w, h)
	print("windowResized "..w.." "..h)
end

function dragEvent(dragInfo)
	print("dragEvent "..dragInfo.files[0].." "..tostring(dragInfo.position))
end

function gotMessage(msg)
	print("gotMessage "..msg)
end

function oscReceived(msg)
	print("oscReceived "..msg.address)
end
