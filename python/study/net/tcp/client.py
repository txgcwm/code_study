#!/usr/bin/python
# -*- coding: utf-8 -*-


import socket


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('127.0.0.1', 10021))
data = raw_input("send:")
s.send(data.encode())
s.close()
