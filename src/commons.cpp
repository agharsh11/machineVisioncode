#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/photo/photo.hpp>
#include<iostream>
#include<vector>

using namespace cv;
using namespace std;

cv::Mat image_clahe;
VideoCapture cap1(0); //capture the video from web cam
VideoCapture cap2(1); //capture the video from web cam

Mat getcam(int port){
	Mat src;
	if(port==0)
		cap1>>src;
	else
		cap2>>src;
	return src;
}
		

void Erosion( int, void* )
{
	int erosion_elem = 0;
int erosion_size = 0;

    Mat dst1;
  int erosion_type;

  if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
  else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
  else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( erosion_type,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );

  /// Apply the erosion operation
  erode( image_clahe, image_clahe, element,Point(0,0),3000 );
  imwrite("/home/sukhad/main1.jpg",image_clahe);
}
/** @function Dilation */
void Dilation( int, void* )
{
	int dilation_elem = 0;
	int dilation_size = 0;

  int dilation_type;
  namedWindow( "Dilation Demo", CV_WINDOW_AUTOSIZE );
  if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( dilation_type,
                                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                       Point( dilation_size, dilation_size ) );
  /// Apply the dilation operation
  dilate( image_clahe, image_clahe, element,Point(0,0),3000 );
  imshow( "Dilation Demo", image_clahe );
}

    
void Enhance(Mat bgr_image){
        
        cv::Mat lab_image;
            if(!bgr_image.data)
                    return;
            resize(bgr_image,bgr_image,Size(640,480));
        cv::cvtColor(bgr_image, lab_image, CV_BGR2Lab);
cv::Mat dst1;
        // Extract the L channel
        std::vector<cv::Mat> lab_planes(3);
        cv::split(lab_image, lab_planes);  // now we have the L image in lab_planes[0]

        // apply the CLAHE algorithm to the L channel
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
        clahe->setClipLimit(4);
        cv::Mat dst;
        clahe->apply(lab_planes[0], dst);

        // Merge the the color planes back into an Lab image
        dst.copyTo(lab_planes[0]);
        cv::merge(lab_planes, lab_image);

       // convert back to RGB
       cv::cvtColor(lab_image, image_clahe, CV_Lab2BGR);
       namedWindow( "image original", CV_WINDOW_AUTOSIZE );
       namedWindow( "image CLAHE", CV_WINDOW_AUTOSIZE );


       // display the results  (you might also want to see lab_planes[0] before and after).

       Erosion( 0, 0 );
         Dilation( 0, 0 );


         imshow("image original", bgr_image);
  imshow("image CLAHE", image_clahe);
    }



vector<Point> findcontours(Mat threshy) //Input HSV IMAGE AFTER COLORDETECTION !! 
{
GaussianBlur(threshy, threshy, cv::Size(9, 9), 0, 0, 0);

 std::vector< std::vector<cv::Point> > contours;
  cv::Mat thresholded_Mat;

threshy.copyTo(thresholded_Mat);
findContours(thresholded_Mat, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);  // Find the contours

double largest_area = 0;
int largest_contour_index = 0;

for (int i = 0; i < contours.size(); i++)  // iterate through each contour.
      {
        double a = contourArea(contours[i], false);
        if (a > largest_area)
        {
          largest_area = a;
          largest_contour_index = i;
        }
      }
return contours[largest_contour_index];
}



int CContours(Mat threshy) //Input HSV IMAGE AFTER COLORDETECTION !! 
{
GaussianBlur(threshy, threshy, cv::Size(9, 9), 0, 0, 0);

 std::vector< std::vector<cv::Point> > contours;
  cv::Mat thresholded_Mat;

threshy.copyTo(thresholded_Mat);
findContours(thresholded_Mat, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);  // Find the contours

double largest_area = 0;
int largest_contour_index = 0;

if(contours.empty())
    {
        cout<<"No contours found!\n";
        return 0;
    }

for (int i = 0; i < contours.size(); i++)  // iterate through each contour.
      {
        double a = contourArea(contours[i], false);
        if (a > largest_area)
        {
          largest_area = a;
          largest_contour_index = i;
        }
      }
return largest_contour_index;
}



 Point2f getCenter(vector<cv::Point> contour, Mat circles) //Vector passed by reference &&& Circles = Source image (for drawing circle)
 {
      std::vector<Point2f> center(1);
      std::vector<float> radius(1);

      minEnclosingCircle(contour, center[0], radius[0]);

      Point2f pt;

      pt.x = 600;  // size of screen //320
      pt.y = 600; 

      circle(circles, center[0], radius[0], cv::Scalar(0, 250, 0), 1, 8, 0);  // minenclosing circle
      circle(circles, center[0], 4, cv::Scalar(0, 250, 0), -1, 8, 0);         // center is made on the screen
      circle(circles, pt, 4, cv::Scalar(150, 150, 150), -1, 8, 0);

      namedWindow("circle",1);
      imshow("circle", circles);
      
return center[0];
}
