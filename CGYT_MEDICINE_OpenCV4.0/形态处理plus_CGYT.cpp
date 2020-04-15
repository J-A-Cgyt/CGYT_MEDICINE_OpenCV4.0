#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

//全局变量
Mat src, dst;
int morph_elem = 0;
int morph_size = 0;
int morph_operator = 0;
int const max_operator = 4;
int const max_elem = 2;
int const max_kernel_size = 21;
const char* Window_name = "Morphology Transformation Demo";

void Morphology_Operations(int, void*);

int main(int, char** argv)
{
	src = imread("G:\\003.1.jpg",IMREAD_GRAYSCALE); //图形文件加载
	if(!src.data)
	{
		return -1;
	}
	namedWindow(Window_name, WINDOW_AUTOSIZE); //建立窗口
	createTrackbar("Operator\n 0: Opening - 1: Cross \n 2: Gardient - 3: Top Hat \n 4: Black Hat",
		            Window_name, &morph_operator, max_operator,
		            Morphology_Operations); //建立滑杆选择Morphology运算
	createTrackbar("Element:\n 0: Rect - 1: Cross - 2: Ellipse", 
		            Window_name, &morph_elem, max_elem, 
		            Morphology_Operations); //建立滑杆选择kernel 类型
	createTrackbar("Kernel size:\n 2n +1",
		            Window_name, &morph_size, max_kernel_size,
		            Morphology_Operations); //建立滑杆选择kernel size
	
	Morphology_Operations(0, 0); //主运算开始

	waitKey(0);
	return 0;
}

void Morphology_Operations(int, void*)
{
	int operation = morph_operator + 2;
	Mat element = getStructuringElement(morph_elem, 
		          Size(morph_size * 2 + 1, morph_size * 2 + 1),
		          Point(morph_size, morph_size));
	morphologyEx(src, dst, operation, element);
	imshow(Window_name, dst);
}

/*
关于形态学处理的若干注释
高级形态学变换：
开运算：Open
先腐蚀，再膨胀，可清除一些小东西(亮的)，放大局部低亮度的区域

闭运算：Closing
先膨胀，再腐蚀，可清除小黑点

形态学梯度：Morphology Gardient
膨胀图与腐蚀图之差，提取物体边缘

顶帽：Top-Hat
原图像-开运算图，突出原图像中比周围亮的区域

黑帽：Black_
闭运算图-原图像，突出原图像中比周围暗的区域

*/