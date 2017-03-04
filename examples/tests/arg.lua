-- test script argument passing to the "arg" table:
--     loaf -v arg.lua hello world 1 2 3
-- argments are only passed from the commandline, not via drag and drop

-- index 0 is the script filename, does not count toward #arg
print("script name: "..arg[0]:match("([^/]+)$"))
print("script dir: "..arg[0]:gsub('\\','/'):match('.*/'))

-- indices 1..n are arguments passed from loaf
print("arg table: "..#arg)
for i = 0,#arg do
	print("  "..i.." "..arg[i])
end
