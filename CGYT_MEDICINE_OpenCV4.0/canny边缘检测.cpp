#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

//ȫ�ֱ�������
Mat src, src_gray;
Mat dst, detected_edges;
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int Ratio = 3;
int kernel_size = 3;
const char* Window_name = "Edge Map";

//trackbar�ص������������贫�뺯���ʴ˲�д������
static void CannyThreshold(int, void*)
{	
	blur(src_gray, detected_edges, Size(kernel_size,kernel_size));//��3*3�ľ���Ϊ�˽���
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * Ratio, kernel_size);//canny��Ե���
	dst = Scalar::all(0);
	src.copyTo(dst, detected_edges);//��������õĽ����Ϊ����ʹԴͼ�񿽱����������
	imshow(Window_name, dst);//��ʾͼ��
}

int main(int, char** argv)
{
	src = imread("G:\\003.1.jpg", IMREAD_COLOR);
	if(!src.data)
	{
		return -1;
	}
	dst.create(src.size(), src.type());
	cvtColor(src, src_gray, COLOR_BGR2GRAY); //ɫ��ת��
	namedWindow(Window_name, WINDOW_AUTOSIZE);
	createTrackbar("Min Threshold:", Window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

	CannyThreshold(0, 0);

	waitKey(0);
	return 0;
}