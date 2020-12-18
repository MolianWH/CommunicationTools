# !/usr/bin/python
# -*- coding: utf-8 -*-
import mmap
import os
import cv2
import numpy as np
from jsonmmap import ObjectMmap
import time

# -----------------Define info in ShareMemory.h-----------------
IMG_HEAD_OFFSET = 12
# typedef struct {
# 	int width;
# 	int height;
# 	int type;
# }ImgInf;       //图像信息12字节

FRAME_NUMBER = 1
FRAME_W = 1920
FRAME_H = 1080*3//2
FRAME_W_H = FRAME_W * FRAME_H
FRAME_SIZE = FRAME_W_H * 1
MEMORY_SIZE = (FRAME_SIZE + IMG_HEAD_OFFSET) * FRAME_NUMBER

sShareMemName = "DDUE4Media"

def read_video():
    # file = os.open(str('shareMemName'), os.O_RDWR)
    fpx = mmap.mmap(-1, FRAME_SIZE+IMG_HEAD_OFFSET, sShareMemName)
    # read img head
    img_head = np.frombuffer(fpx,dtype=np.uint32,count=3)
    print(img_head)
    img_w = img_head[0]
    img_h = img_head[1]*3//2
    img_flag = img_head[2]
    img_size = img_w*img_h
    print("img_w:",img_w)
    print("img_h:", img_h)
    print("img_size:", img_size)

    # read img as numpy
    cv2.namedWindow("python_sharedmem_show",cv2.WINDOW_AUTOSIZE)
    #t0 = cv2.getTickCount()
    t0 = time.time()
    N = 50
    nFrame = 0
    fps = 0
    while 1:
        nFrame += 1
        # 前3个数是3*4B=12B，而读数据单位是1B(uint8)，所以offset=12
        img = np.frombuffer(fpx, dtype=np.uint8,offset=IMG_HEAD_OFFSET)

        img = img[:img_size]
        img = img.reshape((img_h,img_w,1))
        #cv2.resize(img,(FRAME_W,FRAME_H,1),img)
        img=cv2.cvtColor(img,cv2.COLOR_YUV2BGR_NV12)

        if nFrame % 25 == 0:
            # t1 = cv2.getTickCount()
            # fps = N*cv2.getTickFrequency() / (t1 - t0)
            # t0 = t1
            t1 = time.time()
            fps = round((25/(t1-t0)),2)
            t0 = time.time()
        cv2.putText(img, "Average FPS:" + str(fps) + "fps", (100, 200), cv2.FONT_HERSHEY_COMPLEX, 1, (100, 200, 200), 1)
        cv2.imshow("python_sharedmem_show", img)
        img = None
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cv2.destroyAllWindows()





def main():
  mm = ObjectMmap(-1, 1024*1024, access=mmap.ACCESS_READ, tagname='share_mmap')
  while True:
      print('*' * 30)
      print(mm.jsonread_follower())

if __name__ == '__main__':
  read_video()