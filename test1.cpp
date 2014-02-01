#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include<fstream.h>

using namespace cv;
using namespace std;
int x=0,y=0,temp_x,temp_y,flag=0,thumbfing_x,thumbfing_y,middlefing_x,middlefing_y,smallfing_x,smallfing_y;
int bio1;//wrist length
int bio2;//reference point to middle finger top length
int ref_x,ref_y;//reference points of hand	
int main( int argc, char** argv )
{	
    if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image,image1;
    image1 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);   // Read the file
    threshold(image1,image, 100, 255, CV_THRESH_BINARY);

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

//small finger detection
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








//Window display
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.


    imshow( "Display window", image );                   // Show our image inside it.

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}
