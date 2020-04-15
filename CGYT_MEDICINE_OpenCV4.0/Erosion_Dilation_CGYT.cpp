//OpenCV组头文件
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/ml/ml.hpp>
//标准组头文件
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

//全局变量
Mat src, erosion_dst, dilation_dst;
char text[] = "Element: 0:Rect 1:Cross 2:Ellipse";
int erosion_elem = 0; //腐蚀用结构元素
int erosion_size = 0;
int dilation_elem = 0; //膨胀用结构元素
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;

//函数声明
void Erosion(int, void*);
void Dilation(int, void*);

int main(int, char** argv)
{
	src = imread("G:\\003.1.jpg", IMREAD_COLOR);//图像读取
	if (!src.data)
	{
		return -1;
	}
	namedWindow("膨胀", WINDOW_AUTOSIZE);//窗口建立
	namedWindow("腐蚀", WINDOW_AUTOSIZE);

	createTrackbar("Element:", "腐蚀", &erosion_elem, max_elem, Erosion);  //建立腐蚀元素形态滑杆
	createTrackbar("Kernel:", "腐蚀", &erosion_size, max_kernel_size, Erosion); //建立腐蚀强度滑杆

	createTrackbar("Element:", "膨胀", &dilation_elem, max_elem, Dilation);  //建立膨胀元素形态滑杆
	createTrackbar("Kernel:", "膨胀", &dilation_size, max_kernel_size, Dilation); //建立膨胀强度滑杆

	//主运算开始
	Erosion(0, 0);
	Dilation(0, 0);

	waitKey(0);
	return 0;
}

void Erosion(int, void*)
{
	int erosion_type = 0;
	if (erosion_elem == 0)
	{
		erosion_type = MORPH_RECT;
	}
	else if (erosion_elem == 1)
	{
		erosion_type = MORPH_CROSS;
	}
	else if (erosion_elem == 2)
	{
		erosion_type = MORPH_ELLIPSE;
	}

	Mat element = getStructuringElement(erosion_type, Size(2 * erosion_size + 1, 2 * erosion_size + 1));
	Point(erosion_size, erosion_size);

	//使用腐蚀运算
	erode(src, erosion_dst, element);
    //取得文字中心点
	Point textOrg(10, 25);
	putText(erosion_dst, text, textOrg, FONT_HERSHEY_SIMPLEX, 1, 3);
	imshow("腐蚀", erosion_dst);
}

void Dilation(int, void*)
{
	int dilation_type = 0;
	if (dilation_elem == 0)
	{
		dilation_type = MORPH_RECT;
	}
	else if (dilation_elem == 1)
	{
		dilation_type = MORPH_CROSS;
	}
	else if (dilation_elem == 2)
	{
		dilation_type = MORPH_ELLIPSE;
	}

	Mat element = getStructuringElement(dilation_type, Size(2 * dilation_size + 1, 2 * dilation_size + 1));
	Point(dilation_size, dilation_size);

	//使用膨胀运算
	dilate(src, dilation_dst, element);
	//取得文字中心点
	Point textOrg(10, 25);
	putText(dilation_dst, text, textOrg, FONT_HERSHEY_SIMPLEX, 1, 3);
	imshow("膨胀", dilation_dst);
}