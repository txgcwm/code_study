# -*- coding: utf-8 -*-


from urllib import request, parse


url = 'https://hz.esf.fang.com/house/c61-kw%cf%d0%c1%d6%c9%bd%cb%ae/'
headers = {
    'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36'
}

try:
	req = request.Request(url=url, headers=headers)
	response = request.urlopen(req)
	print(response.read().decode('utf-8'))
except Exception as e:
	print(e)
