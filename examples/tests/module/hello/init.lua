-- a custom lua module aka script library
-- this method creates the tables locally so as to avoid global scope by default

-- create local table to hold module contents
local hello = {}

-- add data & functions to the module table
hello.message = "hello"
function hello.hello()
	print(hello.message)
end

-- add subtables/classes from other files within the module folder,
-- note the use of a Java-style period and lack of .lua extension
hello.World = require "hello.World"

-- return the module table
return hello
