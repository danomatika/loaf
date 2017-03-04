-- creates a custom class for use within the "hello" module
local World = class()

-- constructor & attributes
function World:__init()
	self.message = "hello world"
end

-- class method
function World:hello()
	print(self.message)
end

-- return
return World
