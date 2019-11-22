#!/usr/bin/python
# -*- coding: utf-8 -*-


import socket

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) 
 
addr = ('127.0.0.1', 10021)

while True:
	data = raw_input('send:')
	if data == 'quit':
		break

	s.sendto(data.encode(), addr)
	recvdata, addr = s.recvfrom(1024)
	print(recvdata.decode('utf-8'))

s.close()

