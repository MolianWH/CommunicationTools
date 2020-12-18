// WriteMem.cpp : 此文件为写共享内存
// Author : Jiejing.Ma
// Update : 2020/11/27

#include <iostream>
#include "ShareMemory.h"

using namespace std;
using namespace cv;

// 读图片或视频
void send_img(SHAREDMEMORY sharedsend)
{
	int index = 0;
	int64 t0 = cv::getTickCount();;
	int64 t1 = 0;
	string fps = "";
	int nFrames = 0;
	
	cv::Mat frame;

	cout << "Opening video..." << endl;
	VideoCapture cap("test.flv");
	while (cap.isOpened()) 
	{
		cap >> frame;
		if (frame.empty())
		{
			std::cerr << "ERROR: Can't grab video frame." << endl;
			break;
		}
		resize(frame, frame, Size(FRAME_W, FRAME_H));

		nFrames++;
		
		if (!frame.empty()) {
			if (nFrames % 10 == 0)
			{
				const int N = 10;
				int64 t1 = cv::getTickCount();
				fps = " Send FPS:" + to_string((double)getTickFrequency() * N / (t1 - t0)) + "fps";	
				t0 = t1;
			}
			cv::putText(frame, fps, Point(100, 100), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 255, 255),1);
		}
		sharedsend.SendMat(frame, index * FRAME_NUMBER);
		

		if ((waitKey(1) & 0xFF) == 'q') break;
	}
}


int main()
{
	SHAREDMEMORY sharedmem;
	char str[] = "hello";
	if (sharedmem.state == INITSUCCESS) send_img(sharedmem);
	//if (sharedmem.state == INITSUCCESS) sharedmem.SendStr(str);

	return 0;
}