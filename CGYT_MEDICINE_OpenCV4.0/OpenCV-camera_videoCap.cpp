#include "stdafx.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;
using namespace cv;

string intToString(int num)
{
	stringstream ss;
	ss << num;
	return ss.str();
}

int main(int argc, char** argv)
{
	VideoCapture cap(0); //打开编号为0的摄像头

	if (!cap.isOpened()) //若启动失败则结束程序
	{
		cout << "启动摄像头失败" << endl;
		return -1;
	}

	char* WinName = "Camera";
	char filename[50];  //保存图形文件的文件名，最多50个文件

	int n = 0;
	namedWindow(WinName, WINDOW_AUTOSIZE);
	while(1)
	{
		Mat frame; //从摄像头读取帧
		bool IsSuccess = cap.read(frame);
		if (!IsSuccess)
		{
			cout << "无法从摄像头读取帧" << endl;
			break;
		}
		imshow(WinName, frame); //显示从摄像头读取的帧

		switch (waitKey(10))
		{
		case 27: //ESC被按下时退出程序（ESC的ASCII码为27）
			return 0;
			break;
		case 13: //ENTER被按下时保存帧（ENTER的ASCII码为13）
			sprintf_s(filename, "G:\\VideoFrame%d.jpg", n++);/*sprintf指的是字符串格式化命令，主要功能是把格式化的数据写入某个字符串中。
			                                                sprintf 是个变参函数。
		                                                 	使用sprintf 对于写入buffer的字符数是没有限制的，
		                                                  	这就存在了buffer溢出的可能性。
		                                                  	解决这个问题，可以考虑使用 snprintf函数，该函数可对写入字符数做出限制。*/			
			imwrite(filename, frame);
			break;
		}
	}
	return 0;
}