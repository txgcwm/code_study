#!/usr/bin/python
# -*- coding: UTF-8 -*-

import threading
import time,random,math


class MutliThread(threading.Thread):
    def __init__(self, threadName, num):
        threading.Thread.__init__(self)
        self.name 	= threadName
        self.num 	= num
 
    def run(self):
        for i in range(self.num):
            print("{0} i={1}".format(threading.current_thread().getName(), i))
            delay = math.ceil(random.random() * 2)
            time.sleep(delay)

if __name__ == '__main__':
    thr1 = MutliThread("test1", 3)
    thr2 = MutliThread("test2", 2)

    thr1.start()
    thr2.start()

    thr1.join()
    thr2.join()
    print("{0} over".format(threading.current_thread().getName()))
