import socket

def client(filename):
    # 创建 TCP socket
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('localhost', 8080))
    
    # 发送文件名
    s.sendall((filename + '\n').encode())
    
    # 接收文件内容
    while True:
        data = s.recv(4096)
        if not data:
            continue
        print(data.decode(), end='')

    s.close()

# 使用文件名调用客户端
client('1.txt')
