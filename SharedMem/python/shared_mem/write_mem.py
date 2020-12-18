#!/usr/bin/python
# -*- coding: utf-8 -*-
import mmap
from jsonmmap import ObjectMmap
import random


def write_json():
    mm = ObjectMmap(-1, 1024 * 1024, access=mmap.ACCESS_WRITE, tagname='share_mmap')
    while True:
        length = random.randint(1, 100)
        # p = list(range(length))
        p=4
        if mm.jsonwrite(p):
            print('*' * 30)
            print(mm.jsonread_master())


def write_int():
    mm = mmap.mmap(-1,1920*1080*3,access=mmap.ACCESS_WRITE, tagname='DDUE4Media')
    li = [100,200,300,400,1]
    str_li = ','.join([str(x) for x in li])
    print(str_li)
    print(type(str_li))
    while 1:
        mm.seek(0)
        mm.write(str_li.encode())
        mm.tell()
if __name__ == '__main__':
    write_int()