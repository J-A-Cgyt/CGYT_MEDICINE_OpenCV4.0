//����ͼ��ģ�������Ĳ��ԣ�ǿ���𽥱��
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <opencv2/features2d/features2d.hpp>

using namespace std;
using namespace cv;

//����ȫ�ֱ���

int Delay_Caption = 1500;
int Delay_Bulr = 500;
int Max_Kernel_Length = 31;

Mat Src;
Mat Dst;

char Window_name[] = "Smoothing Demo";

int display_caption(const char* caption); //��������
int display_dst(int delay);

int main()
{
	namedWindow(Window_name, WINDOW_AUTOSIZE);
	Src = imread("G:\\003.1.jpg",IMREAD_GRAYSCALE);//ͼ�����
	
	if(display_caption("Original Image") != 0) //ԭʼͼ��
	{
		return 0;
	}
	Dst = Src.clone();
	if (display_dst(Delay_Bulr) != 0)
	{
		return 0;
	}

	if (display_caption("Homogeneous Bulr") != 0) //Homogeneousģ��
	{
		return 0;
	}
	for (int i = 1; i < 99; i = i + 8)
	{
		blur(Src, Dst, Size(i, i));
		if (display_dst(Delay_Bulr) != 0)
		{
			return 0;
		}
	}

	if (display_caption("Gaussian Bulr") != 0) //��˹ģ��
	{
		return 0;
	}
	for (int i = 1; i < Max_Kernel_Length; i = i + 2)
	{
		GaussianBlur(Src, Dst, Size(i, i), 0, 0);
		if (display_dst(Delay_Bulr) != 0)
		{
			return 0;
		}
	}

	if (display_caption("Median Bulr") != 0) //��ֵģ����
	{
		return 0;
	}
	for (int i = 1; i < Max_Kernel_Length; i = i + 2)
	{
		medianBlur(Src, Dst, i);
		if (display_dst(Delay_Bulr) != 0)
		{
			return 0;
		}
	}

	if (display_caption("Bilateral Bulr") != 0) //bilateralģ��
	{
		return 0;
	}
	for (int i = 1; i < Max_Kernel_Length; i = i + 2)
	{
		bilateralFilter(Src, Dst, i, i * 2, i / 2);
		if (display_dst(Delay_Bulr) != 0)
		{
			return 0;
		}
	}

	display_caption("End");
	waitKey(0);
	return 0;
}

int display_caption(const char* caption) //������ʾ����
{
	Dst = Mat::zeros(Src.size(), Src.type());
	putText(Dst, caption, 
		    Point(Src.cols / 4, Src.rows / 2), 
		    FONT_HERSHEY_COMPLEX, 1, 
	    	Scalar(255, 255, 255));
	imshow(Window_name, Dst);
	int c = waitKey(Delay_Caption);
	if (c >= 0)
	{
		return -1;
	}
	return 0;
}

int display_dst(int delay) //��ʾͼ��
{
	imshow(Window_name, Dst);
	int c = waitKey(delay);
	if (c >= 1)
	{
		return -1;
	}

	return 0;
}