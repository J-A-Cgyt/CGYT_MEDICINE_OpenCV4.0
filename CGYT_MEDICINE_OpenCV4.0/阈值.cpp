#include "stdafx.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <stdio.h>
#include <stdlib.h>


using namespace cv;
using namespace std;

int threshold_value = 0;
int threshold_type = 3;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
Mat src, src_gray, dst;
const char* window_name = "Threshold Demo";
const char* trackbar_type = "Type: \n 0:Binary \n 1:Binary Inverted \n 2:Truncate \n 3:To Zero \n 4:To Zero Inverted";
const char* trackbar_value = "Value";

void Threshold_CGYT(int, void*);

int main(int, char** argv)
{
	src = imread("G:\\003.1.jpg", IMREAD_COLOR);
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	namedWindow(window_name, WINDOW_AUTOSIZE);
	createTrackbar(trackbar_type, window_name, &threshold_type, max_type, Threshold_CGYT);
	createTrackbar(trackbar_value, window_name, &threshold_value, max_value, Threshold_CGYT);

	Threshold_CGYT(0, 0);
	waitKey(0);
	return 0;
}

void Threshold_CGYT(int, void*)
{
	/*
	0:二进制
	1:二进制反向
	2:截断
	3:阈值为0
	4:反向阈值为0
	*/
	threshold(src_gray, dst, threshold_value, max_BINARY_value, threshold_type);
	imshow(window_name, dst);
}