#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;
int x=0,y=0,temp_x,temp_y,flag=0,thumbfing_x,thumbfing_y,middlefing_x,middlefing_y,smallfing_x,smallfing_y;
int indexfing_x,indexfing_y,ringfing_x,ringfing_y;
int dis;
int bio1;//wrist length
int bio2;//reference point to middle finger top length
int ref_x,ref_y;//reference points of hand
int a,a1,a2,a3,ref;//areas of triangles for valley point calculation 	
int v1_x,v1_y,v2_x,v2_y,v3_x,v3_y,v4_x,v4_y,v5_x,v5_y;//valley points
int e1_x,e1_y;//left side end points of hand
int main( int argc, char** argv )
{	
    if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image,image1;
    image1 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);   // Read the file as greyscale
    threshold(image1,image, 100, 255, CV_THRESH_BINARY); //converted to binary

Scalar color;



    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    
    x=2;y=440;//y is the approximate y resolution of input pic
for(x=0;x<599;x++)
{
	color= image.at<uchar>(Point(x,y));
	
	if(color.val[0]>90)
	{
		temp_x=x;
		goto hand_end_detect;
	}
}
e1_x=x;e1_y=y;
hand_end_detect:
for(x=temp_x;x<599;x++)
{
	color=image.at<uchar>(Point(x,y));
	if(color.val[0]<90)
	{	bio1=x-temp_x;
		ref_x=x-(x-temp_x)/2;
		
		ref_y=y;
		break;
	}
}
cout<<"reference point(x,y)"<<ref_x<<ref_y<<"val:" <<color.val[0]<<endl;
//hand middle finger top detection
x=0;y=0;
for(y=0;y<200;y++)
{
	for( x=0;x<560;x++)
	{
		color=image.at<uchar>(Point(x,y));
		if(color.val[0]>90)
		{
			temp_x=x;
			temp_y=y;
			flag=1;
			goto middle_finger_top_detected;
			break;
		}
		
	}
}

middle_finger_top_detected:
cout<<"Middle finger top:"<<temp_x<<temp_y<<endl;
middlefing_x=temp_x;
middlefing_y=temp_y;
bio2=ref_y-temp_y;//length from reference point to middle finger tip

//small finger top detection
for(x=0;x<400;x++)
{
	for( y=0;y<400;y++)
	{
		color=image.at<uchar>(Point(x,y));
		if(color.val[0]>90)
		{
			temp_x=x;
			temp_y=y;
			flag=1;
			goto small_finger_top_detected;
			break;
		}
		
	}
}

small_finger_top_detected:
cout<<"Small finger top:"<<temp_x<<temp_y<<endl;
smallfing_x=temp_x;
smallfing_y=temp_y;
//thumb top detection

for(x=567;x>300;x--)
{
	for( y=0;y<400;y++)
	{
		color=image.at<uchar>(Point(x,y));
		if(color.val[0]>90)
		{
			temp_x=x;
			temp_y=y;
			flag=1;
			goto thumb_finger_top_detected;
			break;
		}
		
	}
}

thumb_finger_top_detected:
cout<<"thumb finger top:"<<temp_x<<temp_y<<endl;
thumbfing_x=temp_x;
thumbfing_y=temp_y;

//index finger tip detection

for(y=middlefing_y;y<400;y++)
{
	for(x=middlefing_x+30;x<thumbfing_x;x++)
	{
		color=image.at<uchar>(Point(x,y));
		if(color.val[0]>90)
		{
			temp_x=x;
			temp_y=y;
			goto index_finger_detected;
			break;
		}	
	}	
}
index_finger_detected:
cout<<"index finger top:"<<temp_x<<temp_y<<endl;
indexfing_x=temp_x;
indexfing_y=temp_y;

//ring finger tip detection

for(y=middlefing_y;y<400;y++)
{
	for(x=middlefing_x-30;x>smallfing_x;x--)
	{
		color=image.at<uchar>(Point(x,y));
		if(color.val[0]>90)
		{
			temp_x=x;
			temp_y=y;
			goto ring_finger_detected;
			break;
		}
	}
	
}
ring_finger_detected:
cout<<"ring finger top:"<<temp_x<<temp_y<<endl;
ringfing_x=temp_x;
ringfing_y=temp_y;

			/*	VALLEY POINT DETECTION*/
			
			
			
//first valley point from left(small fing)
ref=300;
a=(((smallfing_y*(ringfing_x-ref_x))+(ringfing_y*(ref_x-smallfing_x))+(ref_y*(smallfing_x-ringfing_x)))/2);
for(y=0;y<ref;y++)
{
	for(x=0;x<300;x++)
	{
		a1=(((y*(ringfing_x-ref_x))+(ringfing_y*(ref_x-x))+(ref_y*(x-ringfing_x)))/2);
      		a2=(((smallfing_y*(x-ref_x))+(y*(ref_x-smallfing_x))+(ref_y*(smallfing_x-x)))/2);
           	a3=(((smallfing_y*(ringfing_x-x))+(ringfing_y*(x-smallfing_x))+(y*(smallfing_x-ringfing_x)))/2);
           	if(a==(a1+a2+a3))
           	{	
           		color=image.at<uchar>(Point(y,x));
              		dis=sqrt((y-ref_y)^2+(x-ref_x)^2);
               		if(dis<ref&&color.val[0]==0)
               		{
                   		v1_x=x;v1_y=y;ref=dis;//valley point 1
                   	}
                 }
	}
}
cout<<"Valley point 1:"<<v1_x<<","<<v1_y<<dis<<endl;
//second valley point 

ref=243;
ringfing_y=1;ringfing_x=1;  
a=(((ringfing_y*(middlefing_x-ref_x))+(middlefing_y*(ref_x-ringfing_x))+(ref_y*(ringfing_x-middlefing_x)))/2);
 for(y=0;y<ref;y++)
 {
       for(x=0;x<205;x++)
       {
           a1=(((x*(middlefing_x-ref_x))+(middlefing_y*(ref_x-y))+(ref_y*(y-middlefing_x)))/2);
           a2=(((ringfing_y*(y-ref_x))+(x*(ref_x-ringfing_x))+(ref_y*(ringfing_x-y)))/2);
           a3=(((ringfing_y*(middlefing_x-y))+(middlefing_y*(y-ringfing_x))+(x*(ringfing_x-middlefing_x)))/2);
           if(a==(a1+a2+a3))
           {
              dis=sqrt((x-ref_y)^2+(y-ref_x)^2);
              color=image.at<uchar>(Point(x,y));
               if((dis<ref&&color.val[0])==0)
               {
                   v2_x=x;v2_y=y;ref=dis;//valley point 2
               }
            }
        }
 }   
 
 
 //third valley point
 
 ref=243;
     middlefing_y=1;middlefing_x=1;  
      a=(((middlefing_y*(indexfing_x-ref_x))+(indexfing_y*(ref_x-middlefing_x))+(ref_y*(middlefing_x-indexfing_x)))/2);
    for(y=0;y<ref;y++)
    {
    	for(x=0;x<205;x++)
    	{
        	a1=(((x*(indexfing_x-ref_x))+(indexfing_y*(ref_x-y))+(ref_y*(y-indexfing_x)))/2);
        	a2=(((middlefing_y*(y-ref_x))+(x*(ref_x-middlefing_x))+(ref_y*(middlefing_x-y)))/2);
        	a3=(((middlefing_y*(indexfing_x-y))+(indexfing_y*(y-middlefing_x))+(x*(middlefing_x-indexfing_x)))/2);
        	if(a==(a1+a2+a3))
        	{
        		dis=sqrt((x-ref_y)^2+(y-ref_x)^2);
        		color=image.at<uchar>(Point(x,y));
               		if((dis<ref&&color.val[0])==0)
                   		v3_x=x;v3_y=y;ref=dis;//valley point 3
                 }
         }
     }
     
 
 //fourth valley point
 
 ref=243;
     thumbfing_y=1;thumbfing_x=1;  
      a=(((v3_x*(thumbfing_x-e1_y))+(thumbfing_y*(e1_y-v3_y))+(e1_x*(v3_y-thumbfing_x)))/2);
    for(y=0;y<ref;y++)
    {
       for(x=0;x<205;x++)
       {
           a1=(((x*(thumbfing_x-e1_y))+(thumbfing_y*(e1_y-y))+(e1_x*(y-thumbfing_x)))/2);
           a2=(((v3_x*(y-e1_y))+(x*(e1_y-v3_y))+(e1_x*(v3_y-y)))/2);
           a3=(((v3_x*(thumbfing_x-y))+(thumbfing_y*(y-v3_y))+(x*(v3_y-thumbfing_x)))/2);
           if(a==(a1+a2+a3))
           {
              dis=sqrt((x-e1_x)^2+(y-e1_y)^2);
              color=image.at<uchar>(Point(x,y));
               if(dis<ref&&color.val[0]==0)
               {
                   v5_x=x;v5_y=y;ref=dis;
               }
            }
        }
     }
//Window display
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.


    imshow( "Display window", image );                   // Show our image inside it.

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}
