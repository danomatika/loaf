-- custom lua module layout & loading example

-- import the module into a module table,
-- require remembers tables which are already loaded
hello = require "hello"

-- use the module table
print("default message: "..hello.message)
hello.hello()

-- create an instance of the hello.World class
world = hello.World()
world:hello() -- call a class method using :
