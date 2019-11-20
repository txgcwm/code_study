#!/usr/bin/python
# -*- coding: UTF-8 -*-


import threading
import time,random,math
 

def print_num(idx):
    for num in range(idx):
        print("{0}\tnum={1}".format(threading.current_thread().getName(), num))
        delay = math.ceil(random.random() * 2)
        time.sleep(delay)
 
if __name__ == '__main__':
    th1 = threading.Thread(target=print_num, args=(2,), name="test1")
    th2 = threading.Thread(target=print_num, args=(3,), name="test2")

    th1.start()
    th2.start()

    th1.join()
    th2.join()
    print("{0} over".format(threading.current_thread().getName()))
