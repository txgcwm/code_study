#!/usr/bin/python
# -*- coding: UTF-8 -*-


import os
import sys
import time



def get_cmd_excute_result(cmd):
	phdl 	= os.popen(cmd)
	result 	= phdl.read()
	phdl.close()
	return result.splitlines(False)

def check_if_product(topdir, chip, product):
	fullpath = topdir + "/" + chip + "/" + product
	isdir = os.path.isdir(fullpath)
	if isdir == False:
		return False

	notinclude = ['mkhdr', 'script', 'user']
	for item in range(len(notinclude)):
		if product == notinclude[item]:
			return False

	print product

	return True

def get_products(topdir, chip):
	cmd		= "ls " + topdir + "/" + chip
	products = get_cmd_excute_result(cmd)

	for type in range(len(products)):
		check_if_product(topdir, chip, products[type])

def main(topdir):
	cmd		= "ls " + topdir
	localtime = time.asctime(time.localtime(time.time()))
	chips = get_cmd_excute_result(cmd)

	print "local_time:", localtime

	for type in range(len(chips)):
		print "chip_type:", (chips[type])
		get_products(topdir, chips[type]);


if __name__ == "__main__":
    main(sys.argv[1])

