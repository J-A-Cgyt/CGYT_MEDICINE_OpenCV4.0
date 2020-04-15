#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

//全局变量声明
Mat src, src_gray;
Mat dst, detected_edges;
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int Ratio = 3;
int kernel_size = 3;
const char* Window_name = "Edge Map";

//trackbar回调函数，因无需传入函数故此不写变量名
static void CannyThreshold(int, void*)
{	
	blur(src_gray, detected_edges, Size(kernel_size,kernel_size));//以3*3的矩形为核降噪
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * Ratio, kernel_size);//canny边缘检测
	dst = Scalar::all(0);
	src.copyTo(dst, detected_edges);//将检测所得的结果作为掩码使源图像拷贝至输出矩阵
	imshow(Window_name, dst);//显示图像
}

int main(int, char** argv)
{
	src = imread("G:\\003.1.jpg", IMREAD_COLOR);
	if(!src.data)
	{
		return -1;
	}
	dst.create(src.size(), src.type());
	cvtColor(src, src_gray, COLOR_BGR2GRAY); //色域转换
	namedWindow(Window_name, WINDOW_AUTOSIZE);
	createTrackbar("Min Threshold:", Window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

	CannyThreshold(0, 0);

	waitKey(0);
	return 0;
}