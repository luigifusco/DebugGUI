import socket
import time

port = int(input())

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.connect(("0.0.0.0", port))

s.sendall(b'GET / HTTP/1.1\t\nHost: ws://0.0.0.0/\t\nUpgrade: websocket\t\nConnection: Upgrade\t\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\t\nSec-WebSocket-Version: 13\t\nSec-WebSocket-Protocol: dpl\t\n\t\n')

print(s.recv(2048))
s.sendall(b'hello world')
time.sleep(5)
s.close()