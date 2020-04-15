//图像傅里叶变换
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
		cout << "图片读取失败" << endl;
		return -1;
	}

	Mat padded;

	//扩张图像至合适大小
	int m = getOptimalDFTSize(I.rows);
	int n = getOptimalDFTSize(I.cols);

	//在图像边框补0
	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

	imshow("000", padded);
	waitKey(0); //至此显示正常

	//以下是昌国梧桐的代码修复尝试：
	Mat K;
	padded.convertTo(K, CV_32F);
	Mat planes[] = { K,Mat::zeros(padded.size(),CV_32F) };
	//昌国梧桐的修补代码到此结束

	//Mat planes[] = { Mat_<float>(padded),Mat::zeros(padded.size(),CV_32F) };

	Mat complexI;
	merge(planes, 2, complexI); // 为延扩后的图像增添一个初始化为0的通道
	                            //merge()函数是把多个单通道数组连接成1个多通道数组

	dft(complexI, complexI); //离散傅里叶变换，结果保存在原数组中

	split(complexI, planes);//split()函数则相反，把1个多通道函数分解成多个但通道函数。

	magnitude(planes[0], planes[1], planes[0]); //该函数计算二维向量的模，二维向量的对应参数由两个矩阵分别输入

	Mat magI = planes[0];

	magI += Scalar::all(1);                    // 转换到对数尺度
	log(magI, magI);                           //求各通道对数值

	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
 /* 还记得我们在第一步时延扩了图像吗? 那现在是时候将新添加的像素剔除了。
	为了方便显示，我们也可以重新分布幅度图象限位置
	(注：将第五步得到的幅度图从中间划开得到四张1/4子图像，将每张子图像看成幅度图的一个象限，
	重新分布即将四个角点重叠到图片中心)。 这样的话原点(0,0)就位移到图像中心。
	*/
    int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - 为每一个象限创建ROI
	Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat tmp;                           // 交换象限 (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // 交换象限 (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);


	normalize(magI, magI, 0, 1, NORM_MINMAX); // 将float类型的矩阵转换到可显示图像范围
	char* name = "001";
	namedWindow(name,WINDOW_AUTOSIZE);


	imshow(name, magI);

	waitKey(0);
	return 0;

}
