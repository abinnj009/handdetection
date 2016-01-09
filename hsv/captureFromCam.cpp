/**
 * @file compareHist_Demo.cpp
 * @brief Sample code to use the function compareHist
 * @author OpenCV team
 * http://www.teuniz.net/RS-232/ - serial communication
 */
 

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace std;
using namespace cv;

/**
 * @function main
 */
int main( int argc, char** argv )
{
    Mat src_base, hsv_base;
    Mat src_test1, hsv_test1;

    /// Load three images with different environment settings
    if( argc < 1 )
    {
        printf("** Error. Usage: ./<output_name> <base_image> <test sample>\n");
        return -1;
    }
cout<<"capturing base image\n";
VideoCapture cap(0); // open the default camera
    	if(!cap.isOpened())  // check if we succeeded
        	return -1;
	 cap >> src_base; // get a new frame from camera to image1
cout<<"capturing test image\n";
sleep(5);
	 cap >> src_test1; // get a new frame from camera to image1

    /// Convert to HSV
    cvtColor( src_base, hsv_base, COLOR_BGR2HSV );
    cvtColor( src_test1, hsv_test1, COLOR_BGR2HSV );


    /// Using 50 bins for hue and 60 for saturation
    int h_bins = 50; int s_bins = 60;
    int histSize[] = { h_bins, s_bins };

    // hue varies from 0 to 179, saturation from 0 to 255
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };

    const float* ranges[] = { h_ranges, s_ranges };

    // Use the o-th and 1-st channels
    int channels[] = { 0, 1 };


    /// Histograms
    MatND hist_base;
    MatND hist_test1;

    /// Calculate the histograms for the HSV images
    calcHist( &hsv_base, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false );
    normalize( hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat() );

    calcHist( &hsv_test1, 1, channels, Mat(), hist_test1, 2, histSize, ranges, true, false );
    normalize( hist_test1, hist_test1, 0, 1, NORM_MINMAX, -1, Mat() );

    /// Apply the histogram comparison methods
    for( int i = 0; i < 4; i++ )
    {
        int compare_method = i;
        double base_base = compareHist( hist_base, hist_base, compare_method );
        double base_test1 = compareHist( hist_base, hist_test1, compare_method );

        printf( " Method [%d] Perfect, Base-Test(1) : %f, %f \n", i, base_base, base_test1 );
    }

    printf( "Done \n" );

    /// Display base
  namedWindow("Base Image", CV_WINDOW_AUTOSIZE );
  imshow("base", hist_base );
namedWindow("Test Image", CV_WINDOW_AUTOSIZE );
  imshow("test", hist_test1 );
  waitKey(0);
return 0;
}
