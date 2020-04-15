//OpenCV��ͷ�ļ�
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/ml/ml.hpp>
//��׼��ͷ�ļ�
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

//ȫ�ֱ���
Mat src, erosion_dst, dilation_dst;
char text[] = "Element: 0:Rect 1:Cross 2:Ellipse";
int erosion_elem = 0; //��ʴ�ýṹԪ��
int erosion_size = 0;
int dilation_elem = 0; //�����ýṹԪ��
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;

//��������
void Erosion(int, void*);
void Dilation(int, void*);

int main(int, char** argv)
{
	src = imread("G:\\003.1.jpg", IMREAD_COLOR);//ͼ���ȡ
	if (!src.data)
	{
		return -1;
	}
	namedWindow("����", WINDOW_AUTOSIZE);//���ڽ���
	namedWindow("��ʴ", WINDOW_AUTOSIZE);

	createTrackbar("Element:", "��ʴ", &erosion_elem, max_elem, Erosion);  //������ʴԪ����̬����
	createTrackbar("Kernel:", "��ʴ", &erosion_size, max_kernel_size, Erosion); //������ʴǿ�Ȼ���

	createTrackbar("Element:", "����", &dilation_elem, max_elem, Dilation);  //��������Ԫ����̬����
	createTrackbar("Kernel:", "����", &dilation_size, max_kernel_size, Dilation); //��������ǿ�Ȼ���

	//�����㿪ʼ
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

	//ʹ�ø�ʴ����
	erode(src, erosion_dst, element);
    //ȡ���������ĵ�
	Point textOrg(10, 25);
	putText(erosion_dst, text, textOrg, FONT_HERSHEY_SIMPLEX, 1, 3);
	imshow("��ʴ", erosion_dst);
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

	//ʹ����������
	dilate(src, dilation_dst, element);
	//ȡ���������ĵ�
	Point textOrg(10, 25);
	putText(dilation_dst, text, textOrg, FONT_HERSHEY_SIMPLEX, 1, 3);
	imshow("����", dilation_dst);
}