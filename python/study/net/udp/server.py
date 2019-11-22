#!/usr/bin/python
# -*- coding: utf-8 -*-


import socket

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('127.0.0.1', 10021))

while True:                                          
	data, addr = s.recvfrom(1024)
	print(data)
	s.sendto(data.decode('utf-8').upper().encode(), addr)
