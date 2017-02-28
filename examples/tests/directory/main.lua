-- test to see if directory with main.lua loading works

function setup()
	of.background(50)
	of.setWindowTitle("directory test")
end

function draw()
	of.setColor(255, 10, 200)
	of.translate(of.getWidth()/2, of.getHeight()/2)
	of.drawBitmapString("it works", 0, 0)
end
