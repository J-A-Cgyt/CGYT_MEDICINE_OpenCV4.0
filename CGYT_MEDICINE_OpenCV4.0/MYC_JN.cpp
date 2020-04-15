#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

//公共变量声明
  Mat Src;  
  Mat Gray; 
//Mat mohu; 
  Mat BianYuan; 
  Mat Changing; //中途监控用输出矩阵

  vector<vector<Point>> YaoPian; //边缘点集（轮廓）向量组
  vector<Point> ZuoBiao;         //轮廓重心坐标（由矩计算得）
  Point HangLie;                 //药板的行数和列数

  char* TestPath = "G:\\Pictures\\Test For Programming\\003.1.jpg";
  char* Window_Name = "YB_Test";

//函数声明
  void 前置处理(Mat src, Mat dst);
  void 边缘检测_Canny(Mat src, Mat dst);
  vector<vector<Point>> 药片辨识(Mat src);
  vector<Point> 坐标计算(vector<vector<Point>> YP);
  Point 行列计数(vector<Point> Centers);
//void FindBiggestArea(Mat &input, Mat &output);

int main(int argc, char** argv)
{
	//图像读取
	Src = imread(TestPath, IMREAD_ANYCOLOR);
	if (!Src.data)
	{
		cout << "图片读取失败" << endl;
		return -1;
	}

	resize(Src, Src, Size(800, 600));
	//颜色转换
	cvtColor(Src, Gray, COLOR_BGR2GRAY);
	imshow(Window_Name, Gray);
	waitKey(0);

	Mat QianZhi = Gray.clone();

	前置处理(Gray, QianZhi);
	//边缘检测_Canny(Gray, QianZhi);

	//vector<vector<Point>> YaoPian;
	YaoPian = 药片辨识(QianZhi);

	//vector<Point> ZuoBiao;
	ZuoBiao = 坐标计算(YaoPian);

	Point CR;
	CR = 行列计数(ZuoBiao);

	waitKey(0);//2019-3-18添加，为获取PPT素材所得的药片像素坐标

	return 0;
}

void 前置处理(Mat src, Mat dst) //高斯模糊+形态处理+阈值
{

	Mat Step1, Step2, Step3, Step4;

	//高斯模糊操作参数
	int GaussKernel = 11;              //高斯模糊1的核的大小
	int GaussKerne2 = 11;              //高斯模糊2的核的大小

	//形态处理操作参数
	int morph_elem = 2;               //Element:\n 0: Rect - 1: Cross - 2: Ellipse
	int morph_size = 2;               //2n +1
	int morph_operator = 2;           //Operator\n 0: Opening - 1: Cross \n 2: Gardient - 3: Top Hat \n 4: Black Hat

	//阈值1操作参数
	int threshold_value1 = 55;          //阈值
	int const max_BINARY_value1 = 255; //最大值
	int threshold_type1 = 0;           //阈值操作类型	0:二进制，1:二进制反向，2:截断，3:阈值为0，4:反向阈值为0

	//阈值2操作参数
	int threshold_value2 = 50;          //阈值
	int const max_BINARY_value2 = 255; //最大值
	int threshold_type2 = 0;           //阈值操作类型	0:二进制，1:二进制反向，2:截断，3:阈值为0，4:反向阈值为0


	//形态处理
	Mat element = getStructuringElement(morph_elem,
		                                Size(morph_size * 2 + 1, morph_size * 2 + 1),
		                                Point(morph_size, morph_size));
	morphologyEx(src, Step1, morph_operator + 2, element);
	imshow(Window_Name, Step1);
	waitKey(0);

	//高斯模糊1-2019/1/13-添加
	GaussianBlur(Step1, Step2, Size(GaussKernel, GaussKernel), 0, 0);
	imshow(Window_Name, Step2);
	waitKey(0);

	//阈值1
	threshold(Step2, Step3, threshold_value1, max_BINARY_value1, threshold_type1);
	imshow(Window_Name, Step3);
	waitKey(0);

//2019-1-10添加
	//高斯模糊2
	GaussianBlur(Step3, Step4, Size(GaussKerne2, GaussKerne2), 0, 0);
	imshow(Window_Name, Step4);
	waitKey(0);

	//阈值2
	threshold(Step4, dst, threshold_value2, max_BINARY_value2, threshold_type2);
	imshow(Window_Name, dst);
    waitKey(0);    

}

void 边缘检测_Canny(Mat src, Mat dst) //高斯模糊+阈值+边缘检测
{
	Mat temp, mohu;
	Mat detected_edges;
	temp = src.clone();	
	mohu = dst.clone();

	
	int GaussKernel = 35;              //高斯模糊的核的大小	
	//高斯模糊	
	GaussianBlur(temp, mohu, Size(GaussKernel, GaussKernel), 0, 0);
/*	
	int ksize = 7; //中值模糊核尺寸
	medianBlur(temp, mohu,ksize);
*/	
	imshow(Window_Name, mohu);
	waitKey(0);

	
	//阈值
	int threshold_value = 100;         //阈值
	int const max_BINARY_value = 255; //最大值
	int threshold_type = 3;           //阈值操作类型	0:二进制，1:二进制反向，2:截断，3:阈值为0，4:反向阈值为0
	threshold(mohu, mohu, threshold_value, max_BINARY_value, threshold_type);
	imshow(Window_Name, mohu);
	waitKey(0);
	
    //Canny边缘检测
	int Ratio = 5;
	int kernel_size = 3;
	int Threshold = 10; //Canny边缘检测低阈值
	
	Canny(mohu, detected_edges, Threshold, Threshold * Ratio, kernel_size);
	dst = Scalar::all(0);
	src.copyTo(dst, detected_edges); //将检测所得的结果作为掩码使源图像拷贝至输出矩阵
	imshow("Canny_边缘检测", dst);   //显示图像
	waitKey(0);
	
}

vector<vector<Point>> 药片辨识(Mat src)
{
	Mat temp;
	temp = src.clone();

	int k = 30;
	if ((temp.cols <= 2 * k) || temp.rows <= 2 * k)
	{
		//continue;
		cout << "最大轮廓太小" << endl;
	}
	Rect rect = Rect(k, k, temp.cols - 2 * k, temp.rows - 2 * k);        /*
																		 对外轮廓进行裁剪，即把药板原始图
																         片从外面剪掉一圈，宽度为15像素
																		 */

	temp = temp(rect); //裁剪

	vector<vector<Point>> LunKuo_all;
	vector<vector<Point>> YP;

	findContours(temp, LunKuo_all, RETR_TREE, CHAIN_APPROX_NONE); //获取轮廓存放在点集容器中,此处提取所有轮廓

	for (int i = 0; i < LunKuo_all.size(); i++)
	{
		int Num = LunKuo_all[i].size();
		if (Num < 800 && Num > 400)
		{
			YP.push_back(LunKuo_all[i]); //push_back在vector类中作用为在vector尾部加入一个数据。
		}
	}
	if (YP.size() == 0)
	{
		cout << "检测失败" << endl;
		return YP;
	}

	Changing = Src.clone();
	drawContours(Changing, YP, -1, Scalar(0, 0, 255), 1, 8, Mat(), 100, Point(k, k));
	imshow(Window_Name, Changing);
	waitKey(0);
	//YaoPian = YP;
	return YP;
}

vector<Point> 坐标计算(vector<vector<Point>> YP)
{
	int Label = YP.size();

	vector<Point> ZB;
	Moments mu;
	for (int i = 0; i < Label; i++)
	{
		double x, y;

		mu = moments(YP[i], false); //图像的中心矩（灰度值），这个函数好像一口气把所有的矩都算了
		x = mu.m10 / mu.m00;
		y = mu.m01 / mu.m00;

		ZB.push_back(Point(x, y));

		/*对已经二值化的图像而言，m00就是轮廓包围的面积，猜测m01和m10分别是对x轴和y轴的矩，借此除法计算质心位置*/
		cout << "药片x坐标=" << x << " 药片y坐标=" << y << endl;
	}
	cout << "共有药片" << ZB.size() << "个" << endl;
	//ZuoBiao = ZB;
	return ZB;
}

Point 行列计数(vector<Point> Centers)
{
	int sum = Centers.size();
	
	Point cr;
	cr.x = 0; cr.y = 0;
	
	vector<int> xList;
	vector<int> yList;

	xList.push_back(Centers[0].x);
	yList.push_back(Centers[0].y);

	for (int i = 1; i < sum; i++)
	{
		bool flagx = true;
		bool flagy = true;

		for (int k = 0; k < xList.size(); k++)//x计数
		{
			int xDelta = abs(Centers[i].x - xList[k]);
			if (xDelta < 15)
			{
				flagx = false;
			}
		}
		if (flagx)
		{
			xList.push_back(Centers[i].x);
		}

		for (int j = 0; j < yList.size(); j++)//y计数
		{
			int yDelta = abs(Centers[i].y - yList[j]);
			if (yDelta < 15)
			{
				flagy = false;
			}
		}
		if (flagy)
		{
			yList.push_back(Centers[i].y);
		}
	}

	cr.x = xList.size();
	cr.y = yList.size();

	//HangLie = cr;
	cout << "有" << cr.x << "行；" << cr.y << "列。" << endl;

	return cr;
}

/*
对适用工况的说明：用于测试的药板有iPhone7拍摄，缩减至尺寸850*567，
但是需要在拍摄是提供较强的光源辅助，在针对不同拍摄条件的药板进行检测时，明显发现不同的参数调整无法兼容，
若是能检测出一种，则在对另一种情况下拍摄的药板样照会出错或效果不好。
*/