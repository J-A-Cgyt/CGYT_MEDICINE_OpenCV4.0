#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

//ȫ�ֱ���
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
	src = imread("G:\\003.1.jpg",IMREAD_GRAYSCALE); //ͼ���ļ�����
	if(!src.data)
	{
		return -1;
	}
	namedWindow(Window_name, WINDOW_AUTOSIZE); //��������
	createTrackbar("Operator\n 0: Opening - 1: Cross \n 2: Gardient - 3: Top Hat \n 4: Black Hat",
		            Window_name, &morph_operator, max_operator,
		            Morphology_Operations); //��������ѡ��Morphology����
	createTrackbar("Element:\n 0: Rect - 1: Cross - 2: Ellipse", 
		            Window_name, &morph_elem, max_elem, 
		            Morphology_Operations); //��������ѡ��kernel ����
	createTrackbar("Kernel size:\n 2n +1",
		            Window_name, &morph_size, max_kernel_size,
		            Morphology_Operations); //��������ѡ��kernel size
	
	Morphology_Operations(0, 0); //�����㿪ʼ

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
������̬ѧ���������ע��
�߼���̬ѧ�任��
�����㣺Open
�ȸ�ʴ�������ͣ������һЩС����(����)���Ŵ�ֲ������ȵ�����

�����㣺Closing
�����ͣ��ٸ�ʴ�������С�ڵ�

��̬ѧ�ݶȣ�Morphology Gardient
����ͼ�븯ʴͼ֮���ȡ�����Ե

��ñ��Top-Hat
ԭͼ��-������ͼ��ͻ��ԭͼ���б���Χ��������

��ñ��Black_
������ͼ-ԭͼ��ͻ��ԭͼ���б���Χ��������

*/