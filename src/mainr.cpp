#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include<vector>
#include<iostream>
#include<math.h>
#include<opencv/highgui.h>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include"../include/align.h"
#include"../include/uart.h"
#include"../include/commons.h"
#include"../include/ColDetect.h"
#define thresharea 10000; //area to be decided
using namespace std;
using namespace cv;
void buoystop()
{
	int master=1;
	Mat src;
	Mat dst;
	double area;
	while(master!=0)
	{
		dst=getcam(1);//1 front cam assuming
		src=YellowDet(dst);
		area=contourArea(findcontours(src));
		if(area>thresharea)
		{
			uartw(1,P);
			master=0;
		}	
	}
}
void bouyfunction()
{
	int px,py;
	int ny=0;
	int nx=0;
	float mx=4.0;
	float my=1.0;
	int xref;
	int yref;
	int master=10;
	float off;
	float dep; 
	Mat dst;
	Mat src; //Cam Feed
	 /*while(1)
	{
		src=getcam(1);//1 front cam assuming		
		if(CContours(RedDet(src)))
		break;
		//FOLLOW LINE
	}
	uartw(2,'h');
	while(uartr(2)!='H');
	//AYDO CALL
	uartao(1.5);//metres
		while(uartr(2)!='O'); 
	uartad(1);//metres
		while(uartr(2)!='D');
	while(1)
	{
		src=getcam(1);//1 front cam assuming
		if(CContours(YellowDet(src)))
		break;		
		//AYDO YAW
	}
	uartw(2,'h');
	while(uartr(2)!='H');*/
	while(master!=0)
	{
		dst=getcam(1);//1 front cam assuming
		src=YellowDet(dst);
		cv::Point2f center;
		center=getCenter(findcontours(src),src);
		xref = 300 - center.x;
		yref = 300 - center.y;

		if(yref>0) 
			py=1;	
		else if(yref==0) 
			py=0;
		else 
			py=-1;


		if(xref>0) 
			px=1;
		else if(xref==0) 
			px=0;
		else 
			px=-1;
		if(my>0.5)//feet value randomly assigned hai height/2 hogi
		{
			dep=depth(py,ny,my);//ny is initially 0,my is intially 1 feet
			uartad(dep);
			while(uartr(2)!='D');
		}
		else
		py=0;
		if(mx>0.5)//feet value breadth/2 assign karni hai
		{
			off=offset(px,nx,mx);//nx is initially 0,mx is intially 4 feet
			uartao(off);
			while(uartr(2)!='O');
		}
		else
		px=0;
		
		master=0;
		
		if(px<0)
			master=master-px;
		else
			master=master+px;
		if(py<0)
			master=master-py;
		else
			master=master+py;
		//aydo ko command	
	}

//MOVE STRAIGHT HIT BUOY
}
//AYDO OFFSET -1.5 , Depth

int bin(Mat src){
    float x,y;
    Mat dst, color_dst;
    char a;
    if(!src.data){
        cout<<"No input";
        return -1;
    }
	src=RedDet(src);
    Canny(src,dst, 50, 200, 3 );

        vector<Point>Largest_contour;
	if(CContours(dst)==0)
		return -1;
    	Largest_contour=findcontours(dst);
               
                   vector<vector<Point> > tcontour;
                   tcontour.push_back(Largest_contour);
                               drawContours(dst,tcontour,-1,Scalar(255,0,0),2);

        cvtColor( dst,color_dst, CV_GRAY2BGR );

       vector<Vec4i> lines;
        HoughLinesP( dst, lines, 1, CV_PI/180,80, 60, 0 );
         size_t i = 0;
         int l,max=0,s,li;
         if(lines.size()==0){
                  return 5;
                 }
         for(i=0;i<lines.size();i++){
                s=(lines[i][2]-lines[i][0])*(lines[i][2]-lines[i][0])+(lines[i][3]-lines[i][1])*(lines[i][3]-lines[i][1]);
                l=sqrt(s);
                if(l>max){
                        max=l;
                        li=i;
                }
            }
         cout<<lines.size();
           line( color_dst, Point(lines[li][0], lines[li][1]),
               Point(lines[li][2], lines[li][3]), Scalar(0,0,255), 3, 8 );
            x=(lines[li][0]+lines[li][2])/2;
         //   y=(lines[li][1]+lines[li][3])/2;
            if(x<300){
                   a='l';
                   x=300-x;
		if(x<20)
			return 0;
                   return -1;
                  }
            else{
                   a='r';
                   x=x-300;
		if(x<20)
			return 0;
                   return 1;
                  }
		

}
cv::Point2f octagon(Mat src)
{
 
 Point2f centre;  
    centre.x=5;
    centre.y=5;

   resize(src,src,Size(600,600));
 if(src.empty())
    {
        cout<<"Can't Read\n";
     
		return centre;
    }


src=YellowDet(src);
if(CContours(src)){

	vector<vector<Point> > contours;
findContours(src, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);  // Find the contours


      int i;
      std::vector<cv::Point2f> center(1);
      std::vector<float> radius(1);
       Mat circles = src;

    for(i=0;i<contours.size();i++){

      minEnclosingCircle(contours[i], center[i], radius[i]);
     centre.x=centre.x+center[i].x;
      centre.y=centre.y+center[i].y;

      Point2f pt;

      pt.x = 600;  // size of screen //320
      pt.y = 600;                     //240
      //float distanc

      circle(circles, center[i], radius[i], cv::Scalar(0, 250, 0), 1, 8, 0);  // minenclosing circle
      circle(circles, center[i], 4, cv::Scalar(0, 250, 0), -1, 8, 0);         // center is made on the screen
      circle(circles, pt, 4, cv::Scalar(150, 150, 150), -1, 8, 0);
}
 centre.x=centre.x/4;
 centre.y=centre.y/4;
if(centre.x<300){
        centre.x=300-centre.x;
		if(centre.x<20)
			centre.x=0;
        else
        	centre.x=-1;
}
else{
		centre.x=centre.x-300;
		if(centre.x<20)
			centre.x=0;
        else
        	centre.x=1;

  }
if(centre.y<300){
        centre.y=300-centre.y;
		if(centre.y<20)
			centre.y=0;
        else
        	centre.y=-1;
}
else{
		centre.y=centre.y-300;
		if(centre.y<20)
			centre.y=0;
        else
        	centre.y=1;

  }
}
return centre;
}


float ldistance=1;
//int checkcontour=0;
	    
int main()
{

do{
	char c=uartr(1);
	if(c!='\0'){

		if(c=='o')
		{
			char d='O';
			uartw(1,d);
		}
		else if(c=='a')
		{
			do
			{
				Mat src=getcam(0);
				Mat dst;
				dst=OrangeDet(src);
				if(CContours(dst))
				{
					char d='A';
					uartw(1,d);
					break;
				}
			}while(1);
		}
		else if(c=='l')
		{
			
			int y=-1,o=5,check=-1,co=0;
			do
			{
				Mat src=getcam(0);
				y=yaw(src);
				if(y!=-1)
				{
					if(check==-1)
					{
						co++;
						check=y;
					}
					else if((check-y<20&&check>y)||(y-check<20&&y>check))
						co++;
				}
			}while(y==-1||co<2);
			uartay(check);
			while(uartr(2)!='Y');

			while(1)
			{
				Mat src=getcam(0);
				int pointer=5,ppointer=0;
				while(pointer==5)
				{
					pointer=getlCenter(src);
				}
					if(pointer==0)
						break;
					float o=offset(pointer,ppointer,ldistance);//ppointer -1 for left and 1 for right
					uartao(o);
						
			}
			while(uartr(2)!='O');

			char d='L';
			uartw(1,d);	

		}
		else if(c=='q'){
			while(1)
			{	
				Mat src=getcam(0);
				Mat dst=OrangeDet(src);
					if(!CContours(dst))
						break;

					int y=-1,o=5,check=-1,co=0;

						y=yaw(src);
						int pointer=5,ppointer=0;
						pointer=getlCenter(src);
							
						if(pointer!=0&&pointer!=5)
							float o=offset(pointer,ppointer,ldistance);//ppointer -1 for left and 1 for right
						if(y>30||o>0.3)
						{
							char d='h';
							uartw(2,d);
							while(uartr(2)!='H');
							break;
						}

				

			}					
				

				char d='Q';
				uartw(1,d);		
					
		}
		else if(c=='c')
		{
				do
				{
					Mat src=getcam(0);
					Mat dst;
					dst=RedDet(src);
					if(CContours(dst))
					{
						char d='C';
						uartw(1,d);
						break;
					}
				}while(1);
						
		}
		else if(c=='d'){
			do{
				Mat src=getcam(0);
				Mat dst;
				dst=YellowDet(src);
				if(CContours(dst)){
				char d='D';
				uartw(1,d);
				break;
				}
			}while(1);
		}

		else if(c=='m'){
			bouyfunction();
		
			char d = 'M';
			uartw(2,d);		
		}
		else if(c=='r'){
			while(1){
				Mat src=getcam(0);
				int pointer=5,ppointer=0;
				while(pointer==5){
					pointer=bin(src);
				}
					if(pointer==0)
						break;
					float o=offset(pointer,ppointer,ldistance);//ppointer -1 for left and 1 for right
					uartao(o);
					
			}
			while(uartr(2)!='R');

			char d='R';
			uartw(1,d);		


		}

		else if(c=='s'){
			
				
				int pointerx=5,ppointerx=0,pointery=5,ppointery=0;
				while(1){
					Mat src=getcam(0);
					while(pointerx==5){
							cv::Point2f cen=octagon(src);
							pointerx=cen.x;
						}
							if(pointerx==0)
								break;
							float o=offset(pointerx,ppointerx,ldistance);//ppointer -1 for left and 1 for right
							uartao(o);
					}	
					while(uartr(2)!='H');	//CHECK

			while(1){
					Mat src=getcam(0);
					while(pointery==5){
							cv::Point2f cen=octagon(src);
							pointery=cen.y;
						}
							if(pointery==0)
								break;
							float o=offset(pointery,ppointery,ldistance);//ppointer -1 for left and 1 for right
							uartao(o);
					}	
					while(uartr(2)!='H'); //CHECK

			char d='S';
			uartw(1,d);
		}
			
	}

}while(1);
return 0;
}
