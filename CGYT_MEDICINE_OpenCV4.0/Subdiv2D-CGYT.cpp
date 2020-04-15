#include "stdafx.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;


static void draw_subdiv_point(Mat& img, Point2f fp, Scalar color)
{
	circle(img, fp, 3, color, FILLED, 8, 0);
}

static void draw_subdiv(Mat& img, Subdiv2D& subdiv, Scalar delaunay_color)
{
#if 1
	vector<Vec6f> trianglelist;
	subdiv.getTriangleList(trianglelist);
	vector<Point> pt(3); //创建点的容器 是创建三个 还是创建初值为3的

	for (size_t i = 0; i < trianglelist.size(); i++)
	{
		Vec6f t = trianglelist[i];
		pt[0] = Point(cvRound(t[0]), cvRound(t[1])); // cvRound 返回跟参数最接近的整数值；
		pt[1] = Point(cvRound(t[2]), cvRound(t[3])); // cvFloor 返回不大于参数的最大整数值；
		pt[2] = Point(cvRound(t[4]), cvRound(t[5])); // cvCeil 返回不小于参数的最小整数值。

		line(img, pt[0], pt[1], delaunay_color, 1, LINE_AA, 0);
		line(img, pt[1], pt[2], delaunay_color, 1, LINE_AA, 0);
		line(img, pt[2], pt[0], delaunay_color, 1, LINE_AA, 0);
	}
#else
	vector<Vec4f> edgeList;
	subdiv.getEdgeList(edgeList);
	for (size_t i = 0; i < edgeList.size(), i++)
	{
		Vec4f e = edgeList;
		Point pt0 = Point(cvRound(e[0]), cvRound(e[1]));
		Point pt1 = Point(cvRound(e[2]), cvRound(e[3]));
		line(img, pt0, pt1, delaunay_color, 1, LINE_AA, 0);
	}
#endif
}

static void locate_point(Mat& img, Subdiv2D& subdiv, Point2f fp, Scalar active_color)
{
	int e0 = 0, vertex = 0;
	subdiv.locate(fp, e0, vertex);
	if (e0 > 0)
	{
		int e = e0;
		do {
			Point2f org, dst;
			if (subdiv.edgeOrg(e, &org) > 0 && subdiv.edgeDst(e, &dst))
			{
				line(img, org, dst, active_color, 3, LINE_AA, 0);

				e = subdiv.getEdge(e, Subdiv2D::NEXT_AROUND_LEFT);
			}
		} while (e != e0);
	}
	draw_subdiv_point(img, fp, active_color);
}

static void paint_voronoi(Mat& img, Subdiv2D& subdiv)
{
	//subdiv2D OpenCV二维三角网格类型 该类似乎包括了各种对三角网格进行处理的方法

	vector<vector<Point2f> > facets;
	vector<Point2f> centers;
	subdiv.getVoronoiFacetList(vector<int>(), facets, centers);

	//facet小平面
	//voronoi小平面，面片

	vector<Point> ifacet;
	vector<vector<Point> > ifacets(1); //括号中的数字初始化该容器的size，最外层的数量

	for (size_t i = 0; i < facets.size(); i++)
	{
		ifacet.resize(facets[i].size());
		for (size_t j = 0; j < facets[i].size(); j++)
		{
			ifacet[j] = facets[i][j];
		}
		Scalar color;
		color[0] = rand() & 255;
		color[1] = rand() & 255;
		color[2] = rand() & 255;
		fillConvexPoly(img, ifacet, color, LINE_AA, 0); //填充图多边形区域

		ifacets[0] = ifacet; /*由于polylines函数的参数类型要求，
							 InputArrayOfArrays pt，即点数组的数组，
							 故此才存在ifacets用于再将点数组ifacet存于其中，
							 因此ifacets只需要1维即可*/
		polylines(img, ifacets, true, Scalar(), 1, LINE_AA, 0);
		circle(img, centers[i], 3, Scalar(), -1, LINE_AA, 0);
	}

}

int main()
{
	Scalar active_facet_color(0, 0, 255) //当前活跃的三角面片为红色
		, delaunay_color(255, 255, 255);

	Rect rect(0, 0, 600, 600);

	Subdiv2D subdiv(rect);
	Mat img(rect.size(), CV_8UC3);

	img = Scalar::all(0); //矩阵中全部单元的所有分量值均至零
	string windows = "Delaunay Demo";
	imshow(windows, img);

	for (int i = 0; i < 100; i++)
	{
		Point2f fp((float)(rand() % (rect.width - 10) + 5),
			(float)(rand() % (rect.height - 10) + 5));

		locate_point(img, subdiv, fp, active_facet_color);
		imshow(windows, img);

		if (waitKey(100) >= 0)
		{
			break;
		}

		subdiv.insert(fp);

		img = Scalar::all(0);
		draw_subdiv(img, subdiv, delaunay_color);
		imshow(windows, img);

		if (waitKey(100) >= 0)
		{
			break;
		}
	}
	img = Scalar::all(0);
	paint_voronoi(img, subdiv);
	imshow(windows, img);

	waitKey(0);

	return 0;
}