// OpenCV-1.cpp: 定义控制台应用程序的入口点。
//苗雨晨的第一个OpenCV程序，因为对VS和C++有过接触，所以hello world就不写了


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

/*
int main(int argc, char** argv) //图像读取
{
	argv[1] = "G:\\Pictures\Saved Pictures/DSC_17338.jpg";

	Mat Picture1; //定义矩阵类型变量（即图片）
	
	Picture1 = imread(argv[1], IMREAD_COLOR);

	if (!Picture1.data) //检查能否打开文件
	{
		cout << "无法打开或找不到文件" << endl;

		waitKey(0);

		return -1;
	}

	namedWindow("Picture1"); //创建窗口

	imshow("Picture1", Picture1); //显示图片

	waitKey(0);

    return 0;
}
*/ 


/*
int main(int argc, char** argv) 部分数据结构与性质，相关内容记录在txt中
{
	//构造函数建立
	Mat M(2,2,CV_8UC3,Scalar(0,0,255));
	cout << M << endl;

	//create方法建立
	M.create(2, 2, CV_8U);
	cout << endl << M << endl;

	//创建单位矩阵
	Mat B = Mat::eye(4,4,CV_8U);
	cout << endl << B << endl;
	
	return 0;

}
*/


/*
int main(int argc, char** argv) //随机的往图里面加一些点，非动态
{
	argv[1] = "G:\\001.jpg";

	Mat image;

	image = imread(argv[1], IMREAD_COLOR);
    if(!image.data)
	{
		cout << "文件读取失败" << endl;
		return -1;
	}

	namedWindow("原图", WINDOW_AUTOSIZE);
	namedWindow("效果图", WINDOW_AUTOSIZE);

	imshow("原图", image);

	//颗粒数
	int i = 100;

	for (int k = 0; k < i; k++)
	{
		int c = rand() % image.cols;
		int r = rand() % image.rows;
		if (image.channels() == 1) //下面的那几行if就是想把点涂大一点，怕光涂一个像素你他妈看不见
		{
			image.at<uchar>(r, c) = 0;
			if (c < (int)image.cols)
			{
				image.at<uchar>(r + 1, c) = 0;
			}
			if (r < (int)image.rows)
			{
				image.at<uchar>(r, c + 1) = 0;
			}
			if (c < (int)image.cols && r < (int)image.rows)
			{
				image.at<uchar>(r + 1, c + 1) = 0;
			}
		}

		else if (image.channels() == 3)
		{
			image.at<cv::Vec3b>(r, c) = (0, 255, 0);
			if (c < (int)image.cols)
			{
				image.at<cv::Vec3b>(r + 1, c) = (0, 255, 0);
			}
			if (r < (int)image.rows)
			{
				image.at<cv::Vec3b>(r, c + 1) = (0, 255, 0);
			}
			if (c < (int)image.cols && r < (int)image.rows)
				{
					image.at<cv::Vec3b>(r + 1, c + 1) = (0, 255, 0);//直接这么写也是没问题的 但是具体如何对应，为何没有红色的
				}
			}
			else
			{
				return -1;
			}
		}

		imshow("效果图", image);
		waitKey(0);
	
	return 0;
}
*/


/*int main(int argc, char** argv) //图像色空间转换
{
	argv[1] = "G:\\001.jpg";

	Mat image = imread(argv[1], IMREAD_COLOR);

	Mat Gray_img;
	
	cvtColor(image,Gray_img,COLOR_BGR2GRAY,1);

	namedWindow("效果", WINDOW_AUTOSIZE);
	imshow("效果",Gray_img);

	waitKey(0);

	return 0;
}*/


double alpha; //对比控制
int beta;     //明度控制

int main()
{
	Mat image = imread("G:\\001.jpg");
	Mat new_image = Mat::zeros(image.size(), image.type());

	cout << "基本线性转换" << endl;
	cout << "----------------------------" << endl;
	
	cout << "输入alpha值[0.0-3.0]:";
	cin >> alpha;

	cout << "输入beta值[0-100]:";
	cin >> beta;

	//转换公式 new_image(i,j) = alpha*image(i,j) + beta
	for (int y = 0; y < image.rows; y++)
	{
		for (int x = 0; x < image.cols; x++)
		{
			for (int c = 0; c < 3; c++)
			{
				new_image.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(image.at<Vec3b>(y, x)[c]) + beta);
			}
		}
	}

	namedWindow("原图", WINDOW_AUTOSIZE);
	namedWindow("效果图", WINDOW_AUTOSIZE);

	imshow("原图", image);
	imshow("效果图",new_image);

	waitKey(0);

	return 0;
}
