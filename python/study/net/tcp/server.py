#!/usr/bin/python
# -*- coding: utf-8 -*-


import socket


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('127.0.0.1', 10021))
s.listen(1)                      
                               
def tcp_connect(sock, addr):
	data = sock.recv(1024)
	print(data)
	sock.close()

while True:
	sock, addr = s.accept()
	tcp_connect(sock, addr)
