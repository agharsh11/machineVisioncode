#include<iostream>
#include<math.h>
#include<opencv/highgui.h>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include"../include/align.h"
#include"../include/uart.h"
#include"../include/ColDetect.h"
#include"../include/commons.h"

#define offset_end 0.5
#define depth_end 0.25

using namespace cv;
using namespace std;
float offset(int &pointer, int &ppointer,float &ldistance) //initialize at 4feet
{
    if(ldistance < offset_end)
       return 0;
    if(pointer==0)
        return 0;

    if(ppointer==pointer)
        {
            return pointer*ldistance;
        }
    else
        {
            ppointer=pointer;
            ldistance=ldistance/2;
            return pointer*ldistance;
        }
}
float depth(int &pointer, int &ppointer,float &ldistance) //initialize at 1feet
{
  float a;
    if(ldistance < depth_end)
       return 0;
    if(pointer==0)
        return 0;

    if(ppointer==pointer)
        {
            a=pointer*ldistance;
            return a;
        }
    else
        {
            ppointer=pointer;
            ldistance=ldistance/2;
            a=pointer*ldistance;
            return a;
        }
}

double yaw(Mat src)
{
    Mat dst, color_dst;
	int co=0;
    double angle = 0;
    resize(src,src,Size(640,480));

    cv::waitKey(30);
       if(!src.data)
            return -1;

 src=OrangeDet(src);
    namedWindow("Source1",1);
 imshow("Source1",src);
 Canny(src,dst, 100, 300, 3 );

    vector<Point>Largest_contour;
	if(CContours(dst)==0)
		return -1;
    Largest_contour=findcontours(dst);
    vector<vector<Point> > tcontour;
               tcontour.push_back(Largest_contour);
                           drawContours(dst,tcontour,-1,Scalar(255,0,0),2);

    cvtColor( dst,color_dst, CV_GRAY2BGR );
    namedWindow( "Source", 1 );
       imshow( "Source", dst );

   vector<Vec4i> lines;
    int l,max=0,s,li;
    HoughLinesP( dst, lines, 1, CV_PI/180,80, 60, 0 );
   if(lines.size()==0){
           return -1;
          }
     for(unsigned int i=0;i<lines.size();i++){
            s=(lines[i][2]-lines[i][0])*(lines[i][2]-lines[i][0])+(lines[i][3]-lines[i][1])*(lines[i][3]-lines[i][1]);
            l=sqrt(s);
            if(l>max){
                    max=l;
                    li=i;
            }
        }
       line( color_dst, Point(lines[li][0], lines[li][1]),
           Point(lines[li][2], lines[li][3]), Scalar(0,0,255), 3, 8 );

		angle=0;
            angle = atan2((double)lines[li][3] - lines[li][1],
                            (double)lines[li][2] - lines[li][0])-angle;
		

   
     namedWindow( "Source", 1 );
        imshow( "Source", dst );

    namedWindow( "Detected Lines", 1 );
    imshow( "Detected Lines", color_dst );


    //std::cout << angle * 180 / CV_PI<< std::endl;
	angle=angle*180/CV_PI;
	if(angle>0){//left
		angle=90-angle;
		if(angle>20){
				return angle;
				
		}
		else{	 
			return 0;	}
	
	}
	else if(angle<0){//right
		angle=angle+90;
		if(angle>20){
				return angle;
			
		}
		else {
			return 0;
		}	
	}
		
	cout<<angle<<endl;
}


int getlCenter(Mat src){
    Mat dst, color_dst;
	
    resize(src,src,Size(640,480));

    cv::waitKey(30);
       if(!src.data)
            return -1;

 src=OrangeDet(src);
    namedWindow("Source1",1);
 imshow("Source1",src);
 Canny(src,dst, 100, 300, 3 );

    vector<Point>Largest_contour;
	if(CContours(dst)==0)
		return -1;

    Largest_contour=findcontours(dst);
    vector<vector<Point> > tcontour;
               tcontour.push_back(Largest_contour);
                           drawContours(dst,tcontour,-1,Scalar(255,0,0),2);

    cvtColor( dst,color_dst, CV_GRAY2BGR );
    namedWindow( "Source", 1 );
       imshow( "Source", dst );

   vector<Vec4i> lines;
    int l,max=0,s,li,off;
    HoughLinesP( dst, lines, 1, CV_PI/180,80, 60, 0 );
   if(lines.size()==0){
           return 5;
          }
     for(unsigned int i=0;i<lines.size();i++){
            s=(lines[i][2]-lines[i][0])*(lines[i][2]-lines[i][0])+(lines[i][3]-lines[i][1])*(lines[i][3]-lines[i][1]);
            l=sqrt(s);
            if(l>max){
                    max=l;
                    li=i;
            }
        }
       line( color_dst, Point(lines[li][0], lines[li][1]),
           Point(lines[li][2], lines[li][3]), Scalar(0,0,255), 3, 8 );  
               
        off=0;
            off= (lines[li][0]+lines[li][2])/2-300;


         if(off>0){//left
            if(off>20){
		return -1;
            }
        
        else{	
            	return 0;
}

    }
    else if(off<0){//right
        off=(-1)*off;
        if(off>20){
	        return 1;   
	 }
        else {
            return  0;
        }
    }

    cout<<off<<endl;
}
