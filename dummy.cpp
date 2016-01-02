#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
/* Compilation tip : use *g++ -o test_1 test_1.cpp `pkg-config opencv --cflags --libs`*  
*/
using namespace cv;
using namespace std;
int x=0,y=0,temp_x,temp_y,flag=0,thumbfing_x,thumbfing_y,middlefing_x,middlefing_y,smallfing_x,smallfing_y;
int indexfing_x,indexfing_y,ringfing_x,ringfing_y;
int smallfing_width,ringfing_width,middlefing_width,indexfing_width,thumbfing_width,left_end;
int dis;
int dis_new,smallfing_width_1,ringfing_width_1,middlefing_width_1,palm_width;
int bio1;//wrist length
int bio2;//reference point to middle finger top length
int ref_x,ref_y,ymax;//reference points of hand
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
    image1 = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE); // Read the file as greyscale
    threshold(image1,image, 60, 255, CV_THRESH_BINARY); //converted to binary

Scalar color;



    if(! image.data ) // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl ;
        return -1;
    }
    
/*    x=2;y=440;//y is the approximate y resolution of input pic
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
	{	
		bio1=x-temp_x;
		ref_x=x-(x-temp_x)/2;
		ref_y=y;
		break;
	}
}
e2_x=x;e2_y=y;//hand end points
cout<<"reference point(x,y)"<<ref_x<<ref_y<<"val:" <<color.val[0]<<endl;
*/
//Window display
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.


    imshow( "Display window", image ); // Show our image inside it.

    waitKey(0); // Wait for a keystroke in the window
    return 0;
}
