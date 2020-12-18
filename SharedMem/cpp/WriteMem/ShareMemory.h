#pragma once
// ShareMemory.h : 此文件包含共享内存数据定义、大小确定、位置分配、信息定义
// Author : Jiejing.Ma
// Update : 2020/11/27
#ifndef ShareMemory_H
#define ShareMemory_H

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>  // cv::Canny()
#include <opencv2/opencv.hpp>

#include <Windows.h>
#include <vector>

//=================================共享内存数据定义=================================
typedef struct {
	int x;
	int y;
	int width;
	int height;
	int track_id;
}TrackBox;    //跟踪框BOX 20B

typedef struct {
	int width;
	int height;
	int type;
}ImgInf;       //图像信息


//=================================共享内存大小确定=================================
// 为图像分配空间
#define FRAME_NUMBER         1               // 图像路数
#define FRAME_W              1920
#define FRAME_H              1080
#define FRAME_W_H            FRAME_W*FRAME_H
// 图像分辨率：彩色图（3通道）+图像信息结构体
#define FRAME_SIZE           FRAME_W_H*sizeof(unsigned char)*3+sizeof(ImgInf)

//#define MEMORY_SIZE          FRAME_NUMBER*FRAME_SIZE

#define MAX_BOX_N            10           // 最多跟踪人数
#define TRACK_BOX_SIZE       sizeof(TrackBox)
#define MEMORY_SIZE          TRACK_BOX_SIZE*MAX_BOX_N

//=================================共享内存位置分配=================================
//pass

//=================================共享内存信息定义=================================
#define INITSUCCESS      0
#define CREATEMAPFAILED  1
#define MAPVIEWFAILED    2

class SHAREDMEMORY
{
public:
	SHAREDMEMORY();
	~SHAREDMEMORY();

	//void SendBox(TrackBox& BOX);
	//void RecBox(TrackBox& BOX);
	//void SendVectorBox(vector<TrackBox>& VTrackBox);
	void RecieveVectorBox(std::vector<TrackBox>& VTrackBox);
	void SendMat(cv::Mat img, char indexAddress);
	cv::Mat  ReceiveMat(char indexAddress);
	void SendStr(const char data[]);
	char* ReceiveStr();

public:
	int state;
private:
	HANDLE hShareMem;                               //共享内存句柄
	TCHAR sShareMemName[30] = TEXT("DDTrackBox");   // 共享内存名称
	LPCTSTR pBuf;	
};

#endif // !ShareMemory_H
