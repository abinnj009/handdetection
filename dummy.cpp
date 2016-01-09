#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
/* Compilation tip : use *g++ -o test_1 test_1.cpp `pkg-config opencv --cflags --libs`*  
 vishnuthankappan3@gmail.com*/
using namespace cv;
using namespace std;
int x=0,y=0,temp_x,temp_y,e1_x,e1_y,e2_x,leaf2_start,leaf2_width,leaf1_width;
main( int argc, char** argv )
{	

	Mat image,image1;
	VideoCapture cap(0); // open the default camera
    	if(!cap.isOpened())  // check if we succeeded
        	return -1;
	 cap >> image1; // get a new frame from camera to image1
	threshold(image1,image, 60, 255, CV_THRESH_BINARY); //converted to binary

	Scalar color;

	if(! image.data ) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl ;
		return -1;
	}

	x=2;y=440;//y is the approximate y resolution of input pic

	for(x;x<599;x++)
	{
		color= image.at<uchar>(Point(x,y));

		if(color.val[0]>90)
		{
			temp_x=x;
			goto first_leaf_start;
		}
	}

first_leaf_start:
	e1_x=x;e1_y=y;//first leaf starting points
	for(x=temp_x;x<599;x++)
	{
		color=image.at<uchar>(Point(x,y));
		if(color.val[0]<90) //first leaf ends
		{	
			leaf1_width = x - e1_x;
			break;
		}
	}

	cout<<"first leaf width:"<<leaf1_width<<endl;
	
	if(leaf1_width > 90) //leaf1 is the target 
	{
		cout<<"Cut down leaf 1"<<endl;
	}

//Window display
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.


    imshow( "Display window", image ); // Show our image inside it.

    waitKey(0); // Wait for a keystroke in the window
    return 0;
}
