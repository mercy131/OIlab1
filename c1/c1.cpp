#define MAX 255
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
#include"time.h"
#include "math.h"

using namespace cv;
using namespace std;

double oupsnr(const Mat& I1, const Mat& I2)
{
	    /******Calculating absolute difference between I1 and I2******/
	Mat s1;
	absdiff(I1, I2, s1); 
	s1.convertTo(s1, CV_32F); 
	s1 = s1.mul(s1); 
	double result;
	    /******Summing all chanels******/
	Scalar s = sum(s1); 
	double sse = s.val[0] + s.val[1] + s.val[2]; 
	try {
		if (sse < 1e-10) throw "too small value";
		/******clalculate MSE******/

		double mse = sse / (double)(I1.channels() * I1.total());
		/******return psnr*****/
		result = 10.0 * log10((MAX * MAX) / mse);
	}
	catch (const char* exception)
	{
		std::cerr << "Error: " << exception << endl;
	}
	return result;
}
Mat ouphotoshop(Mat im1)
{
	int r, g, b;
	Mat i1(im1);
	int ph = 0;
	for (int i = 0; i < im1.rows; i++)
	{
		for (int j = 0; j < im1.cols; j++)
		{
			r = im1.at<Vec3b>(i, j)[0];
			g = im1.at<Vec3b>(i, j)[1];
			b = im1.at<Vec3b>(i, j)[2];
			ph = 0.3 * r + 0.59 * g + 0.11 * b;
			i1.at<Vec3b>(i, j)[0] = ph;
			i1.at<Vec3b>(i, j)[1] = ph;
			i1.at<Vec3b>(i, j)[2] = ph;
		}
	}
	return i1;

}
void rgb2ycbcr(Mat im1, Mat im2)
{
	int r, g, b;
	double y = 0;
	double cb = 0;
	double cr = 0;
	try {
		if (im1.size != im2.size) throw "wrong size";
		for (int i = 0; i < im1.rows; i++)
		{
			for (int j = 0; j < im1.cols; j++)
			{
				r = (double)im1.at<Vec3b>(i, j)[0];
				g = (double)im1.at<Vec3b>(i, j)[1];
				b = (double)im1.at<Vec3b>(i, j)[2];
				y = 0.299 * r + 0.587 * g + 0.114 * b;
				cb = 0.5 * r - 0.4187 * g - 0.0813 * b + 128;
				cr = -0.1687 * r - 0.3313 * g + 0.5 * b + 128;
				im2.at<Vec3b>(i, j)[0] = y;
				im2.at<Vec3b>(i, j)[1] = cb;
				im2.at<Vec3b>(i, j)[2] = cr;
			}
		}
		
	}
	catch (const char* exception) 
	{
		std::cerr << "Error: " << exception << endl;
	}
}
void yrgb(Mat im1)
{
	/*******increase brightness by RGB******/
	for (int i = 0; i < im1.rows; i++)
	{
		for (int j = 0; j < im1.cols; j++)
		{

			im1.at<Vec3b>(i, j)[0] += 40;
			im1.at<Vec3b>(i, j)[1] += 40;
			im1.at<Vec3b>(i, j)[2] += 40;
		}
	}
}
void yycbrcr(Mat im1)
{
	/*******increase brightness by YCbCr******/
	for (int i = 0; i < im1.rows; i++)
	{
		for (int j = 0; j < im1.cols; j++)
		{

			im1.at<Vec3b>(i, j)[0] += 80;

		}
	}
}

int main()
{
	/******Test psnr*****/
	Mat img4 = imread("rr.jpg");
	Mat img5 = imread("yy.jpg");
	clock_t t1, t2,t3,t4;
	t1 = clock();
	double ourPSNR = oupsnr(img4, img5);
	t2 = clock();
	auto optime = t2 - t1;
	t3 = clock();
	double iPSNR = cv::PSNR(img4, img5);
	t4 = clock();
	auto itime = t4 - t3;
	cout <<"iPSNR: " <<iPSNR<<"->time: "<<itime<< endl;
	cout << "ourPSNR: "<<ourPSNR <<"->time: "<< optime << endl;
	/******Test photoshop******/
	Mat img6 = imread("ss.jpg");
	clock_t t5, t6;
	t5 = clock();
	Mat rez = ouphotoshop(img6);
	t6 = clock();
	imshow("rezPhotoshop", rez);
	cvWaitKey(2000);
	auto delta = t6 - t5;
	cout <<"time_Phoshop: "<<delta << endl;
	/******Test rgb to ycbcr******/
	clock_t t7, t8, t9, t10;
	Mat img10 = imread("ss.jpg");
	Mat img7=imread("ss.jpg");
	Mat img8 = imread("ss.jpg");
	Mat img9=imread("ss.jpg");
	t7 = clock();
	rgb2ycbcr(img10, img7);
	t8 = clock();
	imshow("ourRGB2YCBCR", img7);
	cvWaitKey(2000);
	t9 = clock();
	cv::cvtColor(img8, img9, COLOR_RGB2YCrCb);
	t10 = clock();
	imshow("defoult YCBCR", img9);
	cvWaitKey(2000);
	auto delta2 = t8 - t7;
	auto delta3 = t10 - t9;
	cout << "ourTime:  " << delta2 << "   defoultTime:  " << delta3 << endl;
	/******Test increase brightness******/
	Mat img11 = imread("ss.jpg");
	Mat img12 = imread("ss.jpg");
	yrgb(img11);
	yycbrcr(img12);
	imshow("increase brightness by RGB", img11);
	waitKey(2000);
	imshow("increase brightness by RGB", img12);
	waitKey(2000);
	

}

