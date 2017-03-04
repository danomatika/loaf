-- test changing the window size & position

timestamp = 0
bigWindow = true

centerX = 0
centerY = 0

noiseX = 0.0
noiseY = 0.5

function setup()
	setCenterPos()
	of.setWindowPosition(centerX, centerY)
	of.setWindowTitle("window test")
end

function draw()

	-- move window semi-randomly each frame using noise
	local posX = centerX + of.signedNoise(noiseX) * 100
	local posY = centerY + of.signedNoise(noiseY) * 100
	of.setWindowPosition(posX, posY)
	noiseX = noiseX + 0.01 -- update animation variables
	noiseY = noiseY + 0.01

	-- change size every second
	if of.getElapsedTimef() - timestamp > 1.0 then
		if bigWindow then
			of.setWindowShape(640, 480) -- small
		else
			of.setWindowShape(800, 600) -- big
		end
		bigWindow = not bigWindow -- invert
		timestamp = of.getElapsedTimef()
	end

end

function windowResized(w, h)
	-- update center position if window is changed
	setCenterPos()
end

-- custom functions

-- set center position based on window & screen size
function setCenterPos()
	centerX = of.getScreenWidth()/2 - of.getWindowWidth()/2
	centerY = of.getScreenHeight()/2 - of.getWindowHeight()/2
end
