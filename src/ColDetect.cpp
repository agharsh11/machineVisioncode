//File : ColDetect.h
#include<iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "../include/ColDetect.h"
using namespace std;
using namespace cv;

//Default for Yellow Buoy Detection
Mat YellowDet(Mat src) //OUTPUT : HSV Image
{
  cvtColor(src, src, CV_BGR2HSV);
  Scalar hsv_min = cv::Scalar(23,41,133, 0);
  Scalar hsv_max = cv::Scalar(40,150,255, 0);

inRange(src, hsv_min, hsv_max, src);
return src;
}

Mat BlueDet(Mat src) //OUTPUT : HSV Image
{
  cvtColor(src, src, CV_BGR2HSV);
  Scalar hsv_min = cv::Scalar(23,41,133, 0);
  Scalar hsv_max = cv::Scalar(40,150,255, 0);

inRange(src, hsv_min, hsv_max, src);
return src;
}

Mat RedDet(Mat src) //OUTPUT : HSV Image
{
  cvtColor(src, src, CV_BGR2HSV);
  Scalar hsv_min = cv::Scalar(23,41,133, 0);
  Scalar hsv_max = cv::Scalar(40,150,255, 0);

inRange(src, hsv_min, hsv_max, src);
return src;
}

Mat OrangeDet(Mat src) //OUTPUT : HSV Image
{
  cvtColor(src, src, CV_BGR2HSV);
  Scalar hsv_min = cv::Scalar(0,134,0);
  Scalar hsv_max = cv::Scalar(80,240,255);

inRange(src, hsv_min, hsv_max, src);
return src;
}

