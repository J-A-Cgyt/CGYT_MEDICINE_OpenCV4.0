// ҩ��.cpp : �������̨Ӧ�ó������ڵ㡣

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/ml/ml.hpp>
#include <iostream>
using namespace cv;
using namespace std;

void FindRedHSV(Mat &input, Mat &output) //�ҵ���ɫ���򲢱����������ڣ�������������������ⲿ���벢������ֵ
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
			if (((h <150) && (h >0)) && (s< 30) && (s >0) && (v  > 46) && (v < 200))//�ں�ɫ�����ϣ�����
			{
				//�˴���������
			}
			else
			{
				output.at<Vec3b>(row, col).val[0] = 0;
				output.at<Vec3b>(row, col).val[1] = 0;
				output.at<Vec3b>(row, col).val[2] = 0;//���ǵģ��������
			}
		}
	}
	/*�Ա�Ե��ȡ��ǰ�ò����������ҿ����ƺ���Щ���ʣ�
	�����ĺ�ɫͨ���ƺ������ܺܺõĶԱ�Ե���з������ȡ*/
}

void FindBiggestArea1(Mat &input, Mat &output)
{
	Mat temp = input.clone();
	Mat gray;
	Mat bin;
	cvtColor(temp, gray, COLOR_BGR2GRAY);    //ɫ�ռ�ת����RGB->�Ҷ�
	threshold(gray,bin,100,255,THRESH_OTSU); //��ֵ���룬������ֵ100�����ͼ�����ֵ255��
                                             /*THRESH_OTSU��THRESH_TRIANGLE����Ϊ�Ż��㷨���
											 THRESH_BINARY��THRESH_BINARY_INV��THRESH_TRUNC��THRESH_TOZERO�Լ�THRESH_TOZERO_INV��ʹ�õġ�
	                                         ��ʹ����THRESH_OTSU��THRESH_TRIANGLE������־ʱ������ͼ�����Ϊ��ͨ����*/
	vector<vector<Point>> contours;          //vector�����������һ��vector��������������ŵ�
	vector<Rect> boxfinal;                   //vector�����з���һ�����α߿� ����width * height   from  λ��(x*y)
	double maxArea = 0;   //������
	int index = 0;
	findContours(bin, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE); //Ѱ�ұ߿� Դ��bin��������������㼯������ⷽʽ���������Ʒ�����method��
                                                                   /* RETR_EXTERNAL:��ʾֻ����������������������������hierarchy[i][2]=hierarchy[i][3]=-1 
                                                                      RETR_LIST:��ȡ������������������list�У����������������ȼ���ϵ
                                                                      RETR_CCOMP : ��ȡ��������������������֯��˫��ṹ(two - level hierarchy), ����Ϊ��ͨ�����Χ�߽磬�β�λ�ڲ�߽�
                                                                      RETR_TREE : ��ȡ�������������½�����״�����ṹ
	                                                                  RETR_FLOODFILL������û�н��ܣ�Ӧ���Ǻ�ˮ��䷨
														   	  method��
																      CHAIN_APPROX_NONE����ȡÿ��������ÿ�����أ����ڵ������������λ�ò����1
																	  CHAIN_APPROX_SIMPLE��ѹ��ˮƽ���򣬴�ֱ���򣬶Խ��߷����Ԫ�أ�ֵ�����÷�����ص����꣬���һ����������ֻ��4����������������Ϣ
																      CHAIN_APPROX_TC89_L1��CHAIN_APPROX_TC89_KCOSʹ��Teh-Chinl���ƽ��㷨�е�һ��																													
																	 */											 
	for (size_t i = 0; i < contours.size(); i++)
	{
	    RotatedRect box = minAreaRect(contours[i]); 
		/*
		������minAreaRect����ȥ��һ����Ӿ���ʱ��
		�������ھ��γ��Ϳ��ѡ��ȡ���������Ӿ��εı���ˮƽ��ĽǶȾ��룬
		����ˮƽ����ʱ����ת����������ƽ�еıߵĳ�����Ϊ���width������һ������Ϊheight��
		���Ƕ���ֱ��ȡ�ñ���ˮƽ��ļнǡ�
		Ӧ����ʶ������minAreaRect�����У�RotatedRect��width��height��ѡȡ�����εĳߴ��޹أ�
		���ǳ��ľ���height���̵ľ���width����

		�ǶȲ���angle �Ǿ���������ĵ㣨y�������P[0]������ƽ����x������ߣ���ʱ����ת���������ĵ�һ���ߵļнǣ�����ߵı߳�����Ϊwidth����ȡֵ��Χ[-90~0]��

		*/
		if (maxArea <= box.size.area())
		{
			index = i;
			maxArea = box.size.area();
		}
	}
	RotatedRect rect = minAreaRect(contours[index]);
//ת���Ժ�����ȡ����Ȥ�ķ��򼴷�������
	Point2f center = rect.center;//��Ӿ������ĵ�����
	Mat rot_mat = getRotationMatrix2D(center, 270-rect.angle, 1.0);//����ת���� ��ת���ģ��Ƕȣ�����ϵ��
	Mat rot_image;
	Size dst_sz(temp.size());
	warpAffine(temp, rot_image, rot_mat, dst_sz);//ԭͼ����ת
	output = rot_image(Rect(center.x - (rect.size.height / 2), center.y - (rect.size.width / 2), rect.size.height, rect.size.width)); //��ȡROI(region of interest;��Ȥ����)
	/*
	���Կ�������Rect���±��0��ʼ��
	��һ������Ϊ��ʼ���У��ڶ�������Ϊ��ʼ���У�
	����������Ϊ�Ӿ�������������ĸ�����Ϊ�Ӿ����������
	����������Ӷ��ԣ��Ӿ���A����ȡtestMat�����һ�еڶ��е�һ�й��������ݡ�-���ϵ����£��ĸ�
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
	double maxArea = 0;   //������
	int index = 0;
	findContours(bin, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	for (size_t i = 0; i < contours.size(); i++) //contours.size ������Ԫ�صĸ���
	{
		Rect box = boundingRect(contours[i]); // boundingRect(contours[i]) ���������Ĵ�ֱ�߽���С���Σ���������ͼ�����±߽�ƽ�е�-���ԾͲ�������ת��
		if (maxArea <= box.area())
		{
			index = i;
			maxArea = box.area();
		}
	}
	Rect rect = boundingRect(contours[index]);
	output = temp(rect);//��ȡROI������һ������˶���Ȥ�������ȡ���ü���Ϊһ����ͼ
}

int main()
{
	Mat src;// = imread("C:/Users/wangshichao/Desktop/ҩ��/ҩ��3.jpg");
//	resize(src, src, Size(src.cols/4 , src.rows/4)); //�ߴ���С��1/4
//	imshow("src", src);                              //��ʾԭͼ��
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		//�������ͷû�����򷵻�-1
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
		cvtColor(ROI, gray, COLOR_BGR2GRAY); //��Ȥ�����̻Ҷ�ͼ
		cv::imshow("gray", gray);
		medianBlur(gray, med, 11); //ksize�˲�ģ��ߴ��С��>=1������
		cv::imshow("med1", med);
		double m = mean(med)[0];//ͼ���һ��ͨ���ľ�ֵ�������ǻҶ�ͼ��
		/*cv::Scalar mean = cv::mean(image, mask);
          mask����iamgeһ����С�ľ������е���ֵΪ0����1��Ϊ1�ĵط���
		  �����image������Ԫ�صľ�ֵ��Ϊ0 �ĵط���������*/
		cout << "��ֵ" << m << endl;

		for (size_t row = 0; row < med.rows; row++) //�ֶ���ѭ����ֵ������������ֵ�Ķ�ֵ�� ����ֻȡ������һ����
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
			cout << "�������̫С" << endl;
		}
		Rect rect = Rect(k, k, med.cols - 2 * k, med.rows - 2 * k);  /*�����������вü�������ҩ��ԭʼͼ
																	 Ƭ���������һȦ�����Ϊ15����*/
		med = med(rect);
		vector<vector<Point>> contours_all; //�㼯����
		findContours(med, contours_all, RETR_TREE, CHAIN_APPROX_NONE); //��ȡ��������ڵ㼯������,�˴���ȡ��������
		vector<vector<Point>> contours_aviable;

		if (contours_all.size() < 5 || contours_all.size() > 200)//����͹�С������������ҩƬ
		{
			//continue;
			cout << "δ��⵽Ŀ��" << endl;
		}

		for (int j = 0; j < contours_all.size(); j++)  //�޳�С����
		{
			int tem_size = contours_all[j].size();
			if (tem_size >100)
			{
				contours_aviable.push_back(contours_all[j]); //push_back��vector��������Ϊ��vectorβ������һ�����ݡ�
			}
		} //ѭ��ѹ��ջ�Ժ���Ȼ��contours_avaliable.size()ͳ�Ƴ�ҩƬ����

		drawContours(ROI, contours_aviable, -1, Scalar(0, 0, 255), 1, 8, Mat(), 100, Point(k, k)); 
		//��ͼ���Ծ���ԭ�еİ���ҩ������������Ȥ���򻭹ʴ˼��Ͽ��ƫ��

		int num = contours_aviable.size();
		cout << "ҩƬ����" << num << "��" << endl<<endl;
		//20181230

		double xArray[200];
		double yArray[200]; //ͼ��Ҷ����Ķ�ά��������
		//���������� 		
		Moments mu;
		for (size_t i = 0; i <= contours_aviable.size() - 1; i++)
		{
			mu = moments(contours_aviable[i], false); //ͼ������ľأ��Ҷ�ֵ���������������һ���������еľض�����
			xArray[i] = mu.m10 / mu.m00;
			yArray[i] = mu.m01 / mu.m00; 
			
			/*���Ѿ���ֵ����ͼ����ԣ�m00����������Χ��������²�m01��m10�ֱ��Ƕ�x���y��ľأ���˳�����������λ��*/
			
			circle(ROI,Point(xArray[i]+k,yArray[i]+k),2,Scalar(0,0,255),2); //����ʵ�ĵ�Ȧ͹��ͼ�����ĵ�λ��

			cout << "��"<<i+1<<"��ҩƬ����x����:" << xArray[i]+k << " y����:" << yArray[i]+k << endl;
		}

		cout << endl;
		
		double xStd = xArray[0];
		double yStd = yArray[1];
		double rows = 0;
		double cols = 0;
		for (size_t i = 0; i <= contours_aviable.size() - 1; i++) //���м���
		{   //����ͬһ�����xy����������м�����Ϊ�Σ�������Ϊ���ظ�������
			if (abs(xArray[i] - xStd) < 20)
			{
				rows++;
			}
			if (abs(yArray[i] - yStd) < 20)
			{
				cols++;
			}
		}
		if (rows*cols == contours_aviable.size()) //����������������������
		{
			cout << "ҩƬ��" << rows << "�У�" << cols << "��" << endl;
		}
		else
		{
			cout << "ʶ�����" << endl;
			
		}

		for (int i = 0; i < contours_aviable.size() - 1; i++) //x�������򣬵��ǲ����
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
                   /*����x����ĵ�ֵ�������У�������ð�ݣ���*/
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

 