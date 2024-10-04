-- tcp_client.lua
local socket = require("socket")

local client = socket.tcp()

client:connect("127.0.0.1", 12345)

client:send("Hello from client!\n")

local response, err = client:receive()
if not err then
    print("Received from server: " .. response)
else
    print("Error receiving from server: " .. err)
end

client:close()
