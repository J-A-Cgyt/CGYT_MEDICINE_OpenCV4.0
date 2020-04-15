// 药板.cpp : 定义控制台应用程序的入口点。

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/ml/ml.hpp>
#include <iostream>
using namespace cv;
using namespace std;

void FindRedHSV(Mat &input, Mat &output) //找到红色区域并保留，其余变黑，输入矩阵和输出矩阵都由外部输入并不返回值
{
	Mat imghsv;
	cvtColor(input, imghsv, COLOR_BGR2HSV);
	output = input.clone();
	for (int row = 0; row < input.rows; row++)
	{
		for (int col = 0; col < input.cols; col++)
		{
			int h = imghsv.at<Vec3b>(row, col).val[0];
			int s = imghsv.at<Vec3b>(row, col).val[1];
			int v = imghsv.at<Vec3b>(row, col).val[2];
			if (((h <150) && (h >0)) && (s< 30) && (s >0) && (v  > 46) && (v < 200))//在红色区间上，保留
			{
				//此处不做操作
			}
			else
			{
				output.at<Vec3b>(row, col).val[0] = 0;
				output.at<Vec3b>(row, col).val[1] = 0;
				output.at<Vec3b>(row, col).val[2] = 0;//不是的，让它变黑
			}
		}
	}
	/*对边缘提取的前置操作，但在我看来似乎有些草率，
	单纯的红色通道似乎并不能很好的对边缘进行分类和提取*/
}

void FindBiggestArea1(Mat &input, Mat &output)
{
	Mat temp = input.clone();
	Mat gray;
	Mat bin;
	cvtColor(temp, gray, COLOR_BGR2GRAY);    //色空间转换，RGB->灰度
	threshold(gray,bin,100,255,THRESH_OTSU); //阈值分离，分离阈值100，输出图像最大值255。
                                             /*THRESH_OTSU和THRESH_TRIANGLE是作为优化算法配合
											 THRESH_BINARY、THRESH_BINARY_INV、THRESH_TRUNC、THRESH_TOZERO以及THRESH_TOZERO_INV来使用的。
	                                         当使用了THRESH_OTSU和THRESH_TRIANGLE两个标志时，输入图像必须为单通道。*/
	vector<vector<Point>> contours;          //vector容器里面放了一个vector容器，子容器里放点
	vector<Rect> boxfinal;                   //vector容器中放置一个矩形边框即 像素width * height   from  位置(x*y)
	double maxArea = 0;   //最大面积
	int index = 0;
	findContours(bin, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE); //寻找边框 源：bin，输出向量集（点集），检测方式，轮廓近似方法（method）
                                                                   /* RETR_EXTERNAL:表示只检测最外层轮廓，对所有轮廓设置hierarchy[i][2]=hierarchy[i][3]=-1 
                                                                      RETR_LIST:提取所有轮廓，并放置在list中，检测的轮廓不建立等级关系
                                                                      RETR_CCOMP : 提取所有轮廓，并将轮廓组织成双层结构(two - level hierarchy), 顶层为连通域的外围边界，次层位内层边界
                                                                      RETR_TREE : 提取所有轮廓并重新建立网状轮廓结构
	                                                                  RETR_FLOODFILL：官网没有介绍，应该是洪水填充法
														   	  method：
																      CHAIN_APPROX_NONE：获取每个轮廓的每个像素，相邻的两个点的像素位置差不超过1
																	  CHAIN_APPROX_SIMPLE：压缩水平方向，垂直方向，对角线方向的元素，值保留该方向的重点坐标，如果一个矩形轮廓只需4个点来保存轮廓信息
																      CHAIN_APPROX_TC89_L1和CHAIN_APPROX_TC89_KCOS使用Teh-Chinl链逼近算法中的一种																													
																	 */											 
	for (size_t i = 0; i < contours.size(); i++)
	{
	    RotatedRect box = minAreaRect(contours[i]); 
		/*
		在利用minAreaRect函数去求一个外接矩形时，
		函数对于矩形长和宽的选择取决于这个外接矩形的边与水平轴的角度距离，
		即将水平轴逆时针旋转，最先与其平行的边的长度作为宽度width，另外一条边则为height，
		而角度则直接取该边与水平轴的夹角。
		应该意识到，在minAreaRect函数中，RotatedRect的width和height的选取跟矩形的尺寸无关，
		并非长的就是height，短的就是width。！

		角度参数angle 是矩形最下面的点（y坐标最大）P[0]发出的平行于x轴的射线，逆时针旋转，与碰到的第一个边的夹角（这个边的边长就作为width），取值范围[-90~0]。

		*/
		if (maxArea <= box.size.area())
		{
			index = i;
			maxArea = box.size.area();
		}
	}
	RotatedRect rect = minAreaRect(contours[index]);
//转正以后再提取感兴趣的方框即分析区域
	Point2f center = rect.center;//外接矩形中心点坐标
	Mat rot_mat = getRotationMatrix2D(center, 270-rect.angle, 1.0);//求旋转矩阵 旋转中心，角度，缩放系数
	Mat rot_image;
	Size dst_sz(temp.size());
	warpAffine(temp, rot_image, rot_mat, dst_sz);//原图像旋转
	output = rot_image(Rect(center.x - (rect.size.height / 2), center.y - (rect.size.width / 2), rect.size.height, rect.size.width)); //提取ROI(region of interest;兴趣区域)
	/*
	可以看出函数Rect的下标从0开始，
	第一个参数为起始的列，第二个参数为起始的行，
	第三个参数为子矩阵的列数，第四个参数为子矩阵的行数。
	对于这个例子而言，子矩阵A即提取testMat矩阵第一列第二行的一列共两行数据。-左上到右下，四个
	*/
}

void FindBiggestArea(Mat &input, Mat &output)
{
	Mat temp = input.clone();
	Mat gray;
	Mat bin;
	cvtColor(temp, gray, COLOR_BGR2GRAY);
	threshold(gray, bin, 100, 255, THRESH_OTSU);
	vector<vector<Point> > contours;
	vector<Rect> boxfinal;
	double maxArea = 0;   //最大面积
	int index = 0;
	findContours(bin, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	for (size_t i = 0; i < contours.size(); i++) //contours.size 容器中元素的个数
	{
		Rect box = boundingRect(contours[i]); // boundingRect(contours[i]) 计算轮廓的垂直边界最小矩形，矩形是与图像上下边界平行的-所以就不用再旋转了
		if (maxArea <= box.area())
		{
			index = i;
			maxArea = box.area();
		}
	}
	Rect rect = boundingRect(contours[index]);
	output = temp(rect);//提取ROI，在这一步完成了对兴趣区域的提取并裁剪成为一张新图
}

int main()
{
	Mat src;// = imread("C:/Users/wangshichao/Desktop/药板/药板3.jpg");
//	resize(src, src, Size(src.cols/4 , src.rows/4)); //尺寸缩小至1/4
//	imshow("src", src);                              //显示原图像
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		//如果摄像头没开，则返回-1
		return -1;
	}
	Mat frame;
//	Mat src;
	Mat ROI;
	Mat gray;
	Mat bin;
	Mat med;
	while (1)
	{
		cap >> frame;
		src = frame.clone();
		cv::imshow("frame", frame);
		waitKey(100);
		//continue;
		FindBiggestArea(src, ROI);
		cv::imshow("ROI1", ROI);
		cvtColor(ROI, gray, COLOR_BGR2GRAY); //兴趣区域编程灰度图
		cv::imshow("gray", gray);
		medianBlur(gray, med, 11); //ksize滤波模板尺寸大小，>=1的奇数
		cv::imshow("med1", med);
		double m = mean(med)[0];//图像第一个通道的均值，这里是灰度图像
		/*cv::Scalar mean = cv::mean(image, mask);
          mask是与iamge一样大小的矩阵，其中的数值为0或者1，为1的地方，
		  计算出image中所有元素的均值，为0 的地方，不计算*/
		cout << "均值" << m << endl;

		for (size_t row = 0; row < med.rows; row++) //手动的循环二值化？？根据中值的二值化 但是只取了其中一部分
		{
			for (size_t col = 0; col < med.cols; col++)
			{
				if ((med.at<uchar>(row, col) < m + 10) && (med.at<uchar>(row, col) > 100))
				{
					med.at<uchar>(row, col) = 0;
				}
				else
				{
					med.at<uchar>(row, col) = 255;
				}
			}
		}

		int k = 15;
		if ((med.cols <= 2 * k) || med.rows <= 2 * k)
		{
			//continue;
			cout << "最大轮廓太小" << endl;
		}
		Rect rect = Rect(k, k, med.cols - 2 * k, med.rows - 2 * k);  /*对外轮廓进行裁剪，即把药板原始图
																	 片从外面剪掉一圈，宽度为15像素*/
		med = med(rect);
		vector<vector<Point>> contours_all; //点集容器
		findContours(med, contours_all, RETR_TREE, CHAIN_APPROX_NONE); //获取轮廓存放在点集容器中,此处提取所有轮廓
		vector<vector<Point>> contours_aviable;

		if (contours_all.size() < 5 || contours_all.size() > 200)//过大和过小的轮廓都不是药片
		{
			//continue;
			cout << "未检测到目标" << endl;
		}

		for (int j = 0; j < contours_all.size(); j++)  //剔除小轮廓
		{
			int tem_size = contours_all[j].size();
			if (tem_size >100)
			{
				contours_aviable.push_back(contours_all[j]); //push_back在vector类中作用为在vector尾部加入一个数据。
			}
		} //循环压完栈以后自然由contours_avaliable.size()统计出药片个数

		drawContours(ROI, contours_aviable, -1, Scalar(0, 0, 255), 1, 8, Mat(), 100, Point(k, k)); 
		//画图是仍旧在原有的包括药板外轮廓的兴趣区域画故此加上宽度偏移

		int num = contours_aviable.size();
		cout << "药片共有" << num << "个" << endl<<endl;
		//20181230

		double xArray[200];
		double yArray[200]; //图像灰度质心二维坐标数组
		//计算轮廓矩 		
		Moments mu;
		for (size_t i = 0; i <= contours_aviable.size() - 1; i++)
		{
			mu = moments(contours_aviable[i], false); //图像的中心矩（灰度值），这个函数好像一口气把所有的矩都算了
			xArray[i] = mu.m10 / mu.m00;
			yArray[i] = mu.m01 / mu.m00; 
			
			/*对已经二值化的图像而言，m00就是轮廓包围的面积，猜测m01和m10分别是对x轴和y轴的矩，借此除法计算质心位置*/
			
			circle(ROI,Point(xArray[i]+k,yArray[i]+k),2,Scalar(0,0,255),2); //画个实心的圈凸显图像质心的位置

			cout << "第"<<i+1<<"个药片质心x坐标:" << xArray[i]+k << " y坐标:" << yArray[i]+k << endl;
		}

		cout << endl;
		
		double xStd = xArray[0];
		double yStd = yArray[1];
		double rows = 0;
		double cols = 0;
		for (size_t i = 0; i <= contours_aviable.size() - 1; i++) //行列计数
		{   //不用同一个点的xy坐标进行行列计数是为何？？是因为会重复计数吗
			if (abs(xArray[i] - xStd) < 20)
			{
				rows++;
			}
			if (abs(yArray[i] - yStd) < 20)
			{
				cols++;
			}
		}
		if (rows*cols == contours_aviable.size()) //如果数量无误则输出行列数
		{
			cout << "药片有" << rows << "行，" << cols << "列" << endl;
		}
		else
		{
			cout << "识别错误" << endl;
			
		}

		for (int i = 0; i < contours_aviable.size() - 1; i++) //x坐标排序，但是不输出
		{
			for (int j = 0; j < contours_aviable.size() - 1 - i; j++)
			{
				if (xArray[j] > xArray[j + 1])
				{
					double temp = xArray[j];
					xArray[j] = xArray[j + 1];
					xArray[j+1] = temp;
					temp = yArray[j];
					yArray[j] = yArray[j + 1];
					yArray[j + 1] = temp;
                   /*按照x坐标的的值升序排列（好像是冒泡？）*/
				}

				/*if (yArray[j] > yArray[j + 1])
				{
					double temp = yArray[j];
					yArray[j] = yArray[j + 1];
					yArray[j + 1] = temp;
				}*/
			}
		}

		/*for (int i = 0; i < contours_aviable.size() ; i++)
		{
		//	cout << xArray[i] << "  "<<yArray[i]<< endl;
		}*/


		cv::imshow("bin", med);
		cv::imshow("roi", ROI);
		char inputC = waitKey(100);
		if (inputC == 27)
		{
			break;
		}
	}

	waitKey(0);
    return 0;
}

 