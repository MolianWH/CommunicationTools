import mmap
import os
import cv2
import numpy as np

# -----------------Define info in ShareMemory.h-----------------
IMG_HEAD_OFFSET = 12
# typedef struct {
# 	int width;
# 	int height;
# 	int type;
# }ImgInf;       //图像信息12字节

FRAME_NUMBER = 1
FRAME_W = 1920
FRAME_H = 1080
FRAME_W_H = FRAME_W * FRAME_H
FRAME_SIZE = FRAME_W_H * 3
MEMORY_SIZE = (FRAME_SIZE + IMG_HEAD_OFFSET) * FRAME_NUMBER

sShareMemName = "DDUE4Media"

if __name__ == "__main__":
    # file = os.open(str('shareMemName'), os.O_RDWR)
    fpx = mmap.mmap(-1, FRAME_SIZE+IMG_HEAD_OFFSET, "DDUE4Media")
    # read img head as string
    # img_head = fpx.read(IMG_HEAD_OFFSET).translate(None,b'\x00').decode()

    # read img as numpy
    cv2.namedWindow("python_sharedmem_show",0)
    t0 = cv2.getTickCount()
    N = 50
    nFrame = 0
    fps = 0
    while 1:
        nFrame += 1
        img = np.frombuffer(fpx, dtype=np.uint8)
        img = img[IMG_HEAD_OFFSET:FRAME_SIZE+IMG_HEAD_OFFSET]
        img = img.reshape((FRAME_H,FRAME_W,3))
        #cv2.resize(img,(FRAME_W,FRAME_H,1),img)
        # img=cv2.cvtColor(img,cv2.COLOR_YUV2BGR_NV12)

        # if nFrame % 50 == 0:
        #     t1 = cv2.getTickCount()
        #     fps = N*cv2.getTickFrequency() / (t1 - t0)
        #     t0 = t1
        # cv2.putText(img, "Average FPS:" + str(fps) + "fps", (100, 200), cv2.FONT_HERSHEY_COMPLEX, 1, (100, 200, 200), 1)
        cv2.imshow("python_sharedmem_show", img)
        img = None
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cv2.destroyAllWindows()
