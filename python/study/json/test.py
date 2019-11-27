# -*- coding: utf-8 -*-

import io
import json


with io.open('test.json', 'r', encoding='utf-8') as f:
	data = json.load(f);

	mlist = data["data"]

	for i in mlist:
		directors = i["directors"]
		for j in directors:
			print(j)

		rate = i["rate"]
		print(rate)
