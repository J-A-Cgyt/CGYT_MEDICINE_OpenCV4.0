#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

//������������
  Mat Src;  
  Mat Gray; 
//Mat mohu; 
  Mat BianYuan; 
  Mat Changing; //��;������������

  vector<vector<Point>> YaoPian; //��Ե�㼯��������������
  vector<Point> ZuoBiao;         //�����������꣨�ɾؼ���ã�
  Point HangLie;                 //ҩ�������������

  char* TestPath = "G:\\Pictures\\Test For Programming\\003.1.jpg";
  char* Window_Name = "YB_Test";

//��������
  void ǰ�ô���(Mat src, Mat dst);
  void ��Ե���_Canny(Mat src, Mat dst);
  vector<vector<Point>> ҩƬ��ʶ(Mat src);
  vector<Point> �������(vector<vector<Point>> YP);
  Point ���м���(vector<Point> Centers);
//void FindBiggestArea(Mat &input, Mat &output);

int main(int argc, char** argv)
{
	//ͼ���ȡ
	Src = imread(TestPath, IMREAD_ANYCOLOR);
	if (!Src.data)
	{
		cout << "ͼƬ��ȡʧ��" << endl;
		return -1;
	}

	resize(Src, Src, Size(800, 600));
	//��ɫת��
	cvtColor(Src, Gray, COLOR_BGR2GRAY);
	imshow(Window_Name, Gray);
	waitKey(0);

	Mat QianZhi = Gray.clone();

	ǰ�ô���(Gray, QianZhi);
	//��Ե���_Canny(Gray, QianZhi);

	//vector<vector<Point>> YaoPian;
	YaoPian = ҩƬ��ʶ(QianZhi);

	//vector<Point> ZuoBiao;
	ZuoBiao = �������(YaoPian);

	Point CR;
	CR = ���м���(ZuoBiao);

	waitKey(0);//2019-3-18��ӣ�Ϊ��ȡPPT�ز����õ�ҩƬ��������

	return 0;
}

void ǰ�ô���(Mat src, Mat dst) //��˹ģ��+��̬����+��ֵ
{

	Mat Step1, Step2, Step3, Step4;

	//��˹ģ����������
	int GaussKernel = 11;              //��˹ģ��1�ĺ˵Ĵ�С
	int GaussKerne2 = 11;              //��˹ģ��2�ĺ˵Ĵ�С

	//��̬�����������
	int morph_elem = 2;               //Element:\n 0: Rect - 1: Cross - 2: Ellipse
	int morph_size = 2;               //2n +1
	int morph_operator = 2;           //Operator\n 0: Opening - 1: Cross \n 2: Gardient - 3: Top Hat \n 4: Black Hat

	//��ֵ1��������
	int threshold_value1 = 55;          //��ֵ
	int const max_BINARY_value1 = 255; //���ֵ
	int threshold_type1 = 0;           //��ֵ��������	0:�����ƣ�1:�����Ʒ���2:�ضϣ�3:��ֵΪ0��4:������ֵΪ0

	//��ֵ2��������
	int threshold_value2 = 50;          //��ֵ
	int const max_BINARY_value2 = 255; //���ֵ
	int threshold_type2 = 0;           //��ֵ��������	0:�����ƣ�1:�����Ʒ���2:�ضϣ�3:��ֵΪ0��4:������ֵΪ0


	//��̬����
	Mat element = getStructuringElement(morph_elem,
		                                Size(morph_size * 2 + 1, morph_size * 2 + 1),
		                                Point(morph_size, morph_size));
	morphologyEx(src, Step1, morph_operator + 2, element);
	imshow(Window_Name, Step1);
	waitKey(0);

	//��˹ģ��1-2019/1/13-���
	GaussianBlur(Step1, Step2, Size(GaussKernel, GaussKernel), 0, 0);
	imshow(Window_Name, Step2);
	waitKey(0);

	//��ֵ1
	threshold(Step2, Step3, threshold_value1, max_BINARY_value1, threshold_type1);
	imshow(Window_Name, Step3);
	waitKey(0);

//2019-1-10���
	//��˹ģ��2
	GaussianBlur(Step3, Step4, Size(GaussKerne2, GaussKerne2), 0, 0);
	imshow(Window_Name, Step4);
	waitKey(0);

	//��ֵ2
	threshold(Step4, dst, threshold_value2, max_BINARY_value2, threshold_type2);
	imshow(Window_Name, dst);
    waitKey(0);    

}

void ��Ե���_Canny(Mat src, Mat dst) //��˹ģ��+��ֵ+��Ե���
{
	Mat temp, mohu;
	Mat detected_edges;
	temp = src.clone();	
	mohu = dst.clone();

	
	int GaussKernel = 35;              //��˹ģ���ĺ˵Ĵ�С	
	//��˹ģ��	
	GaussianBlur(temp, mohu, Size(GaussKernel, GaussKernel), 0, 0);
/*	
	int ksize = 7; //��ֵģ���˳ߴ�
	medianBlur(temp, mohu,ksize);
*/	
	imshow(Window_Name, mohu);
	waitKey(0);

	
	//��ֵ
	int threshold_value = 100;         //��ֵ
	int const max_BINARY_value = 255; //���ֵ
	int threshold_type = 3;           //��ֵ��������	0:�����ƣ�1:�����Ʒ���2:�ضϣ�3:��ֵΪ0��4:������ֵΪ0
	threshold(mohu, mohu, threshold_value, max_BINARY_value, threshold_type);
	imshow(Window_Name, mohu);
	waitKey(0);
	
    //Canny��Ե���
	int Ratio = 5;
	int kernel_size = 3;
	int Threshold = 10; //Canny��Ե������ֵ
	
	Canny(mohu, detected_edges, Threshold, Threshold * Ratio, kernel_size);
	dst = Scalar::all(0);
	src.copyTo(dst, detected_edges); //��������õĽ����Ϊ����ʹԴͼ�񿽱����������
	imshow("Canny_��Ե���", dst);   //��ʾͼ��
	waitKey(0);
	
}

vector<vector<Point>> ҩƬ��ʶ(Mat src)
{
	Mat temp;
	temp = src.clone();

	int k = 30;
	if ((temp.cols <= 2 * k) || temp.rows <= 2 * k)
	{
		//continue;
		cout << "�������̫С" << endl;
	}
	Rect rect = Rect(k, k, temp.cols - 2 * k, temp.rows - 2 * k);        /*
																		 �����������вü�������ҩ��ԭʼͼ
																         Ƭ���������һȦ�����Ϊ15����
																		 */

	temp = temp(rect); //�ü�

	vector<vector<Point>> LunKuo_all;
	vector<vector<Point>> YP;

	findContours(temp, LunKuo_all, RETR_TREE, CHAIN_APPROX_NONE); //��ȡ��������ڵ㼯������,�˴���ȡ��������

	for (int i = 0; i < LunKuo_all.size(); i++)
	{
		int Num = LunKuo_all[i].size();
		if (Num < 800 && Num > 400)
		{
			YP.push_back(LunKuo_all[i]); //push_back��vector��������Ϊ��vectorβ������һ�����ݡ�
		}
	}
	if (YP.size() == 0)
	{
		cout << "���ʧ��" << endl;
		return YP;
	}

	Changing = Src.clone();
	drawContours(Changing, YP, -1, Scalar(0, 0, 255), 1, 8, Mat(), 100, Point(k, k));
	imshow(Window_Name, Changing);
	waitKey(0);
	//YaoPian = YP;
	return YP;
}

vector<Point> �������(vector<vector<Point>> YP)
{
	int Label = YP.size();

	vector<Point> ZB;
	Moments mu;
	for (int i = 0; i < Label; i++)
	{
		double x, y;

		mu = moments(YP[i], false); //ͼ������ľأ��Ҷ�ֵ���������������һ���������еľض�����
		x = mu.m10 / mu.m00;
		y = mu.m01 / mu.m00;

		ZB.push_back(Point(x, y));

		/*���Ѿ���ֵ����ͼ����ԣ�m00����������Χ��������²�m01��m10�ֱ��Ƕ�x���y��ľأ���˳�����������λ��*/
		cout << "ҩƬx����=" << x << " ҩƬy����=" << y << endl;
	}
	cout << "����ҩƬ" << ZB.size() << "��" << endl;
	//ZuoBiao = ZB;
	return ZB;
}

Point ���м���(vector<Point> Centers)
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

		for (int k = 0; k < xList.size(); k++)//x����
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

		for (int j = 0; j < yList.size(); j++)//y����
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
	cout << "��" << cr.x << "�У�" << cr.y << "�С�" << endl;

	return cr;
}

/*
�����ù�����˵�������ڲ��Ե�ҩ����iPhone7���㣬�������ߴ�850*567��
������Ҫ���������ṩ��ǿ�Ĺ�Դ����������Բ�ͬ����������ҩ����м��ʱ�����Է��ֲ�ͬ�Ĳ��������޷����ݣ�
�����ܼ���һ�֣����ڶ���һ������������ҩ�����ջ�����Ч�����á�
*/