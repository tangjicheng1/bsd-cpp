-- tcp_server.lua
local socket = require("socket")

local server = socket.bind("127.0.0.1", 12345)
print("TCP server listening on port 12345...")

while true do
    local client = server:accept()
    print("Client connected!")

    local line, err = client:receive()
    if not err then
        print("Received from client: " .. line)
        client:send("Hello from server!\n")
    else
        print("Error receiving from client: " .. err)
    end

    client:close()
end
