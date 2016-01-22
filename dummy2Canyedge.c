#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
/* Compilation tip : use *g++ -o test_1 test_1.cpp `pkg-config opencv --cflags --libs`*  
*/
using namespace cv;
using namespace std;
int x=0,y=0,temp_x,temp_y,e1_x,e1_y,e2_x,leaf2_start,leaf2_width,leaf1_width;
main( int argc, char** argv )
{	

	Mat image,image1,imgThreshed;
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

cv::Mat hsv,input = image1;
cv::cvtColor(input,hsv,CV_BGR2HSV);

std::vector<cv::Mat> channels;
cv::split(hsv, channels);

cv::Mat H = channels[0];
cv::Mat S = channels[1];
cv::Mat V = channels[2];
cv::Mat shiftedH = H.clone();
int shift = 25; // in openCV hue values go from 0 to 180 (so have to be doubled to get to 0 .. 360) because of byte range from 0 to 255
for(int j=0; j<shiftedH.rows; ++j)
    for(int i=0; i<shiftedH.cols; ++i)
    {
        shiftedH.at<unsigned char>(j,i) = (shiftedH.at<unsigned char>(j,i) + shift)%180;
    }
cv::Mat cannyH;
cv::Canny(shiftedH, cannyH, 250, 240);//adjust canny values//ebin
// extract contours of the canny image:
std::vector<std::vector<cv::Point> > contoursH;
std::vector<cv::Vec4i> hierarchyH;
cv::findContours(cannyH,contoursH, hierarchyH, CV_RETR_TREE , CV_CHAIN_APPROX_SIMPLE);

// draw the contours to a copy of the input image:
cv::Mat outputH = input.clone();
for( int i = 0; i< contoursH.size(); i++ )
 {
   cv::drawContours( outputH, contoursH, i, cv::Scalar(0,0,255), 2, 8, hierarchyH, 0);
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


    imshow( "Display window", image1 ); // Show our image inside it.
namedWindow("Sampled image",WINDOW_AUTOSIZE);
imshow("Sampled image",outputH);

    waitKey(0); // Wait for a keystroke in the window
    return 0;
}
