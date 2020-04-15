//ͼ����Ҷ�任
#include "stdafx.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
	const char* filename = "G:\\001.jpg";
	Mat I = imread(filename, IMREAD_GRAYSCALE);
	if (I.empty())
	{
		cout << "ͼƬ��ȡʧ��" << endl;
		return -1;
	}

	Mat padded;

	//����ͼ�������ʴ�С
	int m = getOptimalDFTSize(I.rows);
	int n = getOptimalDFTSize(I.cols);

	//��ͼ��߿�0
	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

	imshow("000", padded);
	waitKey(0); //������ʾ����

	//�����ǲ�����ͩ�Ĵ����޸����ԣ�
	Mat K;
	padded.convertTo(K, CV_32F);
	Mat planes[] = { K,Mat::zeros(padded.size(),CV_32F) };
	//������ͩ���޲����뵽�˽���

	//Mat planes[] = { Mat_<float>(padded),Mat::zeros(padded.size(),CV_32F) };

	Mat complexI;
	merge(planes, 2, complexI); // Ϊ�������ͼ������һ����ʼ��Ϊ0��ͨ��
	                            //merge()�����ǰѶ����ͨ���������ӳ�1����ͨ������

	dft(complexI, complexI); //��ɢ����Ҷ�任�����������ԭ������

	split(complexI, planes);//split()�������෴����1����ͨ�������ֽ�ɶ����ͨ��������

	magnitude(planes[0], planes[1], planes[0]); //�ú��������ά������ģ����ά�����Ķ�Ӧ��������������ֱ�����

	Mat magI = planes[0];

	magI += Scalar::all(1);                    // ת���������߶�
	log(magI, magI);                           //���ͨ������ֵ

	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
 /* ���ǵ������ڵ�һ��ʱ������ͼ����? ��������ʱ������ӵ������޳��ˡ�
	Ϊ�˷�����ʾ������Ҳ�������·ֲ�����ͼ����λ��
	(ע�������岽�õ��ķ���ͼ���м仮���õ�����1/4��ͼ�񣬽�ÿ����ͼ�񿴳ɷ���ͼ��һ�����ޣ�
	���·ֲ������ĸ��ǵ��ص���ͼƬ����)�� �����Ļ�ԭ��(0,0)��λ�Ƶ�ͼ�����ġ�
	*/
    int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Ϊÿһ�����޴���ROI
	Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat tmp;                           // �������� (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // �������� (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);


	normalize(magI, magI, 0, 1, NORM_MINMAX); // ��float���͵ľ���ת��������ʾͼ��Χ
	char* name = "001";
	namedWindow(name,WINDOW_AUTOSIZE);


	imshow(name, magI);

	waitKey(0);
	return 0;

}
