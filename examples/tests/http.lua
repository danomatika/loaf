-- try using ofURLFileLoader for basic HTTP GET and POST requests

of.setLogLevel("ofURLFileLoader", of.LOG_VERBOSE)

-- http get
-- check with: curl -X GET "http://httpbin.org/get
local http = of.URLFileLoader()
local res = http:get("http://httpbin.org/get")
print("http get: "..res.status)
if res.data ~= "" then
  print(res.data:getText())
end

-- http post
-- check with: curl -X POST -d '{"foo":"bar","baz":123}' "http://httpbin.org/post"
local req = of.HttpRequest("http://httpbin.org/post", "test")
req.body = '{"foo":"bar","baz":123}'
req.contentType = "application/json"
req.method = of.HttpRequest_POST
req.verbose = true
res = http:handleRequest(req)
print("http post: "..res.status)
if res.data ~= "" then
  print(res.data:getText())
end
