-- some osc functionality tests
-- the ofxOsc bindings are found in the "osc" Lua module

-- print out hostname
print("hostname: "..loaf.getHostname())

-- construct a message with all argument types
message = osc.Message()
message:setAddress("/hello")
message:addIntArg(1)
message:addInt32Arg(2)
message:addInt64Arg(3)
message:addFloatArg(4.56)
message:addDoubleArg(7.89)
message:addStringArg("world")
message:addCharArg("c")
message:addBoolArg(true)
message:addBoolArg(false)
message:addTriggerArg() -- also known as impluse and infinitum
message:addTimetagArg(0) -- currently converted to a bool due to bug in ofxOSC
message:addBlobArg(of.Buffer("hello\0world")) -- binary data as string
message:addMidiMessageArg(tonumber("90404000", 16)) -- convert hex string
message:addSymbolArg("foobar")
message:addNoneArg()
message:addRgbaColorArg(tonumber("FF00FFFF", 16)) -- convert hex string

-- print the message
local types = ""
for i = 0,message:getNumArgs()-1 do
	types = types..message:getArgType(i)
end
if message:getArgType(0) == osc.TYPE_INT32 then
	print("arg 0 is an int32: "..message:getArgAsInt32(0))
end
print("types: "..types)
print("message: "..tostring(message))

-- start loaf's internal listener
loaf.setSendPort(8880)
loaf.setListenPort(8880)
loaf.startListening()

-- loaf osc listener callback
-- use with loaf.setListenPort(#) and loaf.startListening()
function oscReceived(message)
	-- print received message
	print("loaf receiver: "..tostring(message))
end

-- check loaf convenience senders
loaf.send("/multi", 1, 2, 3, "hello")
loaf.sendBang("/bang")
loaf.sendFloat("/float", 1.23)
loaf.sendInt("/int", 12345)
loaf.sendString("/string", "kaaaaaw")

message = osc.Message()
message:setAddress("/test")
message:addIntArg(123)
message:addFloatArg(456.789)
message:addStringArg("foobar")
loaf.sendMessage(message)

bundle = osc.Bundle()
bundle:addMessage(message)
loaf.sendBundle(bundle)
print("bundle: "..tostring(bundle))

-- receive messages manually
receiver = osc.Receiver()
settings = osc.ReceiverSettings()
settings.port = 9990
settings.start = false -- don't start yet
receiver:setup(settings)
receiver:start() -- start manually
print("receiver: "..tostring(receiver))

function update()
	-- process messages from the receiver manually
	while receiver:hasWaitingMessages() do
		local message = osc.Message()
		receiver:getNextMessage(message)
		if message then
			-- print received message
			print("receiver: "..tostring(message))
		end
	end
end

-- send message manually
sender = osc.Sender()
sender:setup("127.0.0.1", 9990)
sender:sendMessage(message)
print("sender: "..tostring(sender))
