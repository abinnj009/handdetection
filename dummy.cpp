#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
/* Compilation tip : use *g++ -o test_1 test_1.cpp `pkg-config opencv --cflags --libs`*  
*/
using namespace cv;
using namespace std;
int x=0,y=0,temp_x,temp_y;

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
//calculation of second leaf start
e2_x = x+10;
for(x=e2_x;x<599;x++)
{
	color=image.at<uchar>(Point(x,y));
	if(color.val[0]>90) //second leaf start
	{	
		leaf2_start = x ;
		break;
	}
}
//calculating second leaf end
for(x = leaf2_start +1; x < 599; x++)
{
	color = image.at<uchar>(Point(x,y));
	if(color.val[0]<90)//second leaf end
	{
		leaf2_width = x - leaf2_start;
		break;
	}
}
if(leaf1_width > 90) //leaf1 is the target 
{
	cout<<"Cut down leaf 1"<<endl;
}
if(leaf2_width > 90) //leaf2 is the target
{

	cout<<"Cut down leaf 2"<<endl;
}

//Window display
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.


    imshow( "Display window", image ); // Show our image inside it.

    waitKey(0); // Wait for a keystroke in the window
    return 0;
}
