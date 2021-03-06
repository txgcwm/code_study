#!/usr/bin/python
# -*- coding: UTF-8 -*-

import threading, time
 
def run(taskName):
    print("task:", taskName)
    time.sleep(2)
    print("{0} over".format(taskName))
 
if __name__ == '__main__':
    start_time = time.time()
    for i in range(3):
        thr = threading.Thread(target=run, args=("task-{0}".format(i),))
        thr.setDaemon(True)
        thr.start()

    thrName  = threading.current_thread().getName()
    thrCount = threading.active_count()

    print("{0} over，thread_num={1}".format(thrName, thrCount))
    print("cost time:", time.time() - start_time)
