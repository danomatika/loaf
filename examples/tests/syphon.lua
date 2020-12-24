-- 2020 Dan Wilcox <danomatika@gmail.com>

-- this sets up custom Syphon server and client instances
--
-- if all you need is to send the current screen output, use the built-in
-- Syphon server instead via
-- * loaf.startSyphon(): start server to publish screen each frame
-- * loaf.stopSyphon(): stop server
-- * loaf.setSyphonName(): set server name
-- * loaf.isSyphonPublishing(): is the server publishing right now?
-- * loaf.getSyphonServer(): get the built-in server instance
--
-- the main difference is that the built-in server can keep running between
-- script reloads

local server = syphon.Server()
local client = syphon.Client()
local mod = 0

function setup()
	of.setWindowTitle("kaa")
	of.background(100)
	of.setCircleResolution(40)

	-- to (re)use the built-in server instance directly:
	--server = loaf.getSyphonServer()

	-- name the server, stream name will be "loaf - Screen Output"
	server:setName("Screen Output")

	-- receive from ofxSyphon example-Basic
	client:setup()
	client:set("", "Test Server")
end

function update()
	mod = mod + 0.025
end

function draw()

	-- draw circle
	of.setColor(255)
	of.drawCircle(of.getWidth()/2, of.getHeight()/2, 200 * of.noise(mod))

	-- draw other client's screen, if connected
	client:draw(50, 50)

	-- update screen to server
	server:publishScreen()
end
