#define DLL_API extern "C" _declspec(dllexport)
#include "iostream"
#include "vector"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

double det(Point p0,Point p1,Point p2)
{
	return (p1.x-p0.x)*(p2.y-p0.y)-(p1.y-p0.y)*(p2.x-p0.x);
}

double ploygon_area(int n,vector<Point> p)  
{
	double s=0.0;
	int i=1;
	for(;i < n-1;i++)
		s += det(p[0],p[i],p[i+1]);
	return 0.5*fabs(s);
}

DLL_API double TotalContoursConvexHull(uchar* src,int width,int height,uchar* bwhullimage,unsigned int* rgbhullimage);
DLL_API double TotalContoursConvexHull(uchar* src,int width,int height,uchar* bwhullimage,unsigned int* rgbhullimage)
{
	Mat image(height, width, CV_8UC1, src);
	Mat hullimage_bw(height, width, CV_8UC1, bwhullimage);
	Mat hullimage_rgb(height, width, CV_8UC4, rgbhullimage);


	vector<vector<Point>> contours;
	findContours(image,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	vector<Point>   totalcontours; 
	for(unsigned int i=0;i<contours.size();i++)
	{
		totalcontours.insert(totalcontours.end(),contours[i].begin(),contours[i].end());  
	}

	vector<int> hull;
	convexHull( Mat(totalcontours), hull, false);

	vector<Point> hull_points;
	convexHull( Mat(totalcontours), hull_points, false);

	Point poi = totalcontours[hull[hull.size()-1]];
	for(unsigned int i=0;i < hull.size();++i)
	{
		line(hullimage_bw, poi, totalcontours[hull[i]], Scalar(255), 3, CV_AA);
		poi = totalcontours[hull[i]];
		//circle(hullimage,totalcontours[hull[i]],3,Scalar(0,255,0),2,8);
	}

	//Mat drawing = Mat::zeros( image.size(), CV_8UC3 );

	Point poi1 = totalcontours[hull[hull.size()-1]];
	for(unsigned int i=0;i < hull.size();++i)
	{
		line(hullimage_rgb, poi1, totalcontours[hull[i]], Scalar(255,0,0), 3, CV_AA);
		poi1 = totalcontours[hull[i]];
		circle(hullimage_rgb,totalcontours[hull[i]],3,Scalar(0,255,0),3,8);
	}
	for(unsigned int i=0;i<contours.size();i++)
	{
		drawContours( hullimage_rgb, contours,i, Scalar(0,0,255), 1, 8, vector<Vec4i>(), 0);  
	}
	
	

	double areaSum=0.0;
	areaSum=ploygon_area(hull_points.size(),hull_points);

	/*
	namedWindow("result Image",0);
	imshow("result Image",drawing);
	waitKey(0);
	destroyWindow("result Image");
	*/
	return areaSum;
}




