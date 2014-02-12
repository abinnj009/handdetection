#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;
int x=0,y=0,temp_x,temp_y,flag=0,thumbfing_x,thumbfing_y,middlefing_x,middlefing_y,smallfing_x,smallfing_y;
int indexfing_x,indexfing_y,ringfing_x,ringfing_y;
int dis;
int dis_new;
int bio1;//wrist length
int bio2;//reference point to middle finger top length
int ref_x,ref_y;//reference points of hand
int a,a1,a2,a3,ref;//areas of triangles for valley point calculation 	
int v1_x,v1_y,v2_x,v2_y,v3_x,v3_y,v4_x,v4_y,v5_x,v5_y;//valley points
int e1_x,e1_y,e2_x,e2_y; //left side end points of hand
int main( int argc, char** argv )
{	
    if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image,image1;
    image1 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);   // Read the file as greyscale
    threshold(image1,image, 60, 255, CV_THRESH_BINARY); //converted to binary

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

hand_end_detect:
e1_x=x;e1_y=y;//hand starting points
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
e2_x=x;e2_y=y;//hand end points
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

dis_new=445;
a=abs(((smallfing_x*(ringfing_y-ref_y))+(ringfing_x*(ref_y-smallfing_y))+(ref_x*(smallfing_y-ringfing_y)))/2);

for(y=447;y>0;y--)
{
	for(x=0;x<567;x++)
	{
		a1=abs((smallfing_x*(ringfing_y-y)+x*(smallfing_y-ringfing_y)+ringfing_x*(y-smallfing_y))/2);
		
      		a2=abs((smallfing_x*(y-ref_y)+x*(ref_y-smallfing_y)+ref_x*(smallfing_y-y))/2);
      		
           	a3=abs((ringfing_x*(ref_y-y)+ref_x*(y-ringfing_y)+x*(ringfing_y-ref_y))/2);
           	
           	if(a==(a1+a2+a3))
           	{	
           // 	cout<<"a1:"<<a1<<endl;
           //	cout<<"a2:"<<a2<<endl;
           //	cout<<"a3:"<<a3<<endl;
         		dis=sqrt((x-ref_x)^2+(y-ref_y)^2);
           		color=image.at<uchar>(Point(x,y));
           		
              		
               		if(dis<dis_new&&color.val[0]==0)
               		{
               		
               		//cout<<"dis"<<dis<<endl;
               			
                   			v1_x=x;v1_y=y;
                   			dis_new=dis;
                   			
                   			
                   			
                   	}
                 }
	}
}

cout<<"first valley point("<<v1_x<<","<<v1_y<<")"<<endl;
cout<<"ref point("<<ref_x<<","<<ref_y<<")"<<endl;

//second valley point 

dis_new=445;
a=abs(((ringfing_x*(middlefing_y-ref_y))+(middlefing_x*(ref_y-ringfing_y))+(ref_x*(ringfing_y-middlefing_y)))/2);

for(y=447;y>0;y--)
{
	for(x=0;x<567;x++)
	{
		a1=abs((ringfing_x*(middlefing_y-y)+x*(ringfing_y-middlefing_y)+middlefing_x*(y-ringfing_y))/2);
		
      		a2=abs((ringfing_x*(y-ref_y)+x*(ref_y-ringfing_y)+ref_x*(ringfing_y-y))/2);
      		
           	a3=abs((middlefing_x*(ref_y-y)+ref_x*(y-middlefing_y)+x*(middlefing_y-ref_y))/2);
           	
           	if(a==(a1+a2+a3))
           	{	
           // 	cout<<"a1:"<<a1<<endl;
           //	cout<<"a2:"<<a2<<endl;
           //	cout<<"a3:"<<a3<<endl;
         		dis=sqrt((x-ref_x)^2+(y-ref_y)^2);
           		color=image.at<uchar>(Point(x,y));
           		
              		
               		if(dis<dis_new&&color.val[0]==0)
               		{
               		
               		//cout<<"dis"<<dis<<endl;
               			
                   			v2_x=x;v2_y=y;
                   			dis_new=dis;
                   			
                   			
                   			
                   	}
                 }
	}
}

cout<<"second valley point("<<v2_x<<","<<v2_y<<")"<<endl;
cout<<"ref point("<<ref_x<<","<<ref_y<<")"<<endl;   
 
 
 //third valley point
 dis_new=445;
a=abs(((middlefing_x*(indexfing_y-ref_y))+(indexfing_x*(ref_y-middlefing_y))+(ref_x*(middlefing_y-indexfing_y)))/2);

for(y=447;y>0;y--)
{
	for(x=0;x<567;x++)
	{
		a1=abs((middlefing_x*(indexfing_y-y)+x*(middlefing_y-indexfing_y)+indexfing_x*(y-middlefing_y))/2);
		
      		a2=abs((middlefing_x*(y-ref_y)+x*(ref_y-middlefing_y)+ref_x*(middlefing_y-y))/2);
      		
           	a3=abs((indexfing_x*(ref_y-y)+ref_x*(y-indexfing_y)+x*(indexfing_y-ref_y))/2);
           	
           	if(a==(a1+a2+a3))
           	{	
           // 	cout<<"a1:"<<a1<<endl;
           //	cout<<"a2:"<<a2<<endl;
           //	cout<<"a3:"<<a3<<endl;
         		dis=sqrt((x-ref_x)^2+(y-ref_y)^2);
           		color=image.at<uchar>(Point(x,y));
           		
              		
               		if(dis<dis_new&&color.val[0]==0)
               		{
               		
               		//cout<<"dis"<<dis<<endl;
               			
                   			v3_x=x;v3_y=y;
                   			dis_new=dis;
                   			
                   			
                   			
                   	}
                 }
	}
}

cout<<"Third valley point("<<v3_x<<","<<v3_y<<")"<<endl;
cout<<"ref point("<<ref_x<<","<<ref_y<<")"<<endl;


 
 //fourth valley point
 

dis_new=445;
a=abs(((indexfing_x*(thumbfing_y-e1_y))+(thumbfing_x*(e1_y-indexfing_y))+(e1_x*(indexfing_y-thumbfing_y)))/2);

for(y=447;y>0;y--)
{
	for(x=0;x<567;x++)
	{
		a1=abs((indexfing_x*(thumbfing_y-y)+x*(indexfing_y-thumbfing_y)+thumbfing_x*(y-indexfing_y))/2);
		
      		a2=abs((indexfing_x*(y-e1_y)+x*(e1_y-indexfing_y)+e1_x*(indexfing_y-y))/2);
      		
           	a3=abs((thumbfing_x*(e1_y-y)+e1_x*(y-thumbfing_y)+x*(thumbfing_y-e1_y))/2);
           	
           	if(a==(a1+a2+a3))
           	{	
           // 	cout<<"a1:"<<a1<<endl;
           //	cout<<"a2:"<<a2<<endl;
           //	cout<<"a3:"<<a3<<endl;
         		dis=sqrt((x-e1_x)^2+(y-e1_y)^2);
           		color=image.at<uchar>(Point(x,y));
           		
              		
               		if(dis<dis_new&&color.val[0]==0)
               		{
               		
               		//cout<<"dis"<<dis<<endl;
               			
                   			v4_x=x;v4_y=y;
                   			dis_new=dis;
                   			
                   			
                   			
                   	}
                 }
	}
}

cout<<"Fourth valley point("<<v4_x<<","<<v4_y<<")"<<endl;
cout<<"ref point("<<e1_x<<","<<e1_y<<")"<<endl;
//Window display
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.


    imshow( "Display window", image );                   // Show our image inside it.

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}

