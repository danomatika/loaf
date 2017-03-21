
-- of.Buffer
buffer = of.Buffer("hello\0world")
print("buffer size (11): "..buffer:size())
buffer:clear()
print("buffer size (0): "..buffer:size())
buffer:set("abc\0de")
print("buffer size (6): "..buffer:size())
buffer:append("fg\0hij")
print("buffer size (12): "..buffer:size())
local data = buffer:getData()
if data then
io.write("buffer data (hex): ")
for i = 1,#data do
  io.write(string.format("%.2X ", string.byte(data:sub(i,i))))
end
io.write("\n")
end
buffer:set("foo bar")
print("buffer size (7): "..buffer:size())
print("buffer text: "..buffer:getText())

-- of.FilePath
print("filepath current dir: "..of.FilePath.getCurrentWorkingDirectory())
print("filepath joining /Users/foo with bar.txt: "..of.FilePath.join("/Users/foo", "bar.txt"))

-- of.File
file = of.File("fileutils.lua")

print("file extension: "..file:getExtension())
print("file name: "..file:getFileName())
print("file basename: "..file:getBaseName())
print("file enclosing directory: "..file:getEnclosingDirectory())
print("file absolute path: "..file:getAbsolutePath())

print("file can read?: "..tostring(file:canRead()))
print("file can write?: "..tostring(file:canWrite()))
print("file can execute?: "..tostring(file:canExecute()))

print("file is file?: "..tostring(file:isFile()))
print("file is link?: "..tostring(file:isLink()))
print("file is directory?: "..tostring(file:isDirectory()))
print("file is device?: "..tostring(file:isDevice()))
print("file is hidden?: "..tostring(file:isHidden()))

print("file size: "..file:getSize())

-- of.Directory
dir = of.Directory("./")
print("dir absolute path: "..dir:getAbsolutePath())
