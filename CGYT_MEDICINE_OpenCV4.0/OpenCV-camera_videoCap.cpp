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
	VideoCapture cap(0); //�򿪱��Ϊ0������ͷ

	if (!cap.isOpened()) //������ʧ�����������
	{
		cout << "��������ͷʧ��" << endl;
		return -1;
	}

	char* WinName = "Camera";
	char filename[50];  //����ͼ���ļ����ļ��������50���ļ�

	int n = 0;
	namedWindow(WinName, WINDOW_AUTOSIZE);
	while(1)
	{
		Mat frame; //������ͷ��ȡ֡
		bool IsSuccess = cap.read(frame);
		if (!IsSuccess)
		{
			cout << "�޷�������ͷ��ȡ֡" << endl;
			break;
		}
		imshow(WinName, frame); //��ʾ������ͷ��ȡ��֡

		switch (waitKey(10))
		{
		case 27: //ESC������ʱ�˳�����ESC��ASCII��Ϊ27��
			return 0;
			break;
		case 13: //ENTER������ʱ����֡��ENTER��ASCII��Ϊ13��
			sprintf_s(filename, "G:\\VideoFrame%d.jpg", n++);/*sprintfָ�����ַ�����ʽ�������Ҫ�����ǰѸ�ʽ��������д��ĳ���ַ����С�
			                                                sprintf �Ǹ���κ�����
		                                                 	ʹ��sprintf ����д��buffer���ַ�����û�����Ƶģ�
		                                                  	��ʹ�����buffer����Ŀ����ԡ�
		                                                  	���������⣬���Կ���ʹ�� snprintf�������ú����ɶ�д���ַ����������ơ�*/			
			imwrite(filename, frame);
			break;
		}
	}
	return 0;
}