import socket

HOST = '127.0.0.1'
PORT = 5432

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

with open("test.badsql", "r") as f:
    data = f.read()
    s.sendall(data.encode())

response = s.recv(1024).decode()
print(response)