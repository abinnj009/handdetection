#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <linux/errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>    /* For O_RDWR */
#include <errno.h>
/* Compilation tip : use *g++ -o test_1 test_1.cpp `pkg-config opencv --cflags --libs`*  
   vishnuthankappan3@gmail.com*/
using namespace cv;
using namespace std;
char buf[100];
int x=0,y=0,temp_x,temp_y,e1_x,e1_y,e2_x,leaf2_start,leaf2_width,leaf1_width;
/* set interface attributes */
int set_interface_attribs (int fd, int speed, int parity)
{
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0)
	{
		return 0;
	}

	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
	// disable IGNBRK for mismatched speed tests; otherwise receive break
	// as \000 chars
	tty.c_iflag &= ~IGNBRK;         // disable break processing
	tty.c_lflag = 0;                // no signaling chars, no echo,
	// no canonical processing
	tty.c_oflag = 0;                // no remapping, no delays
	tty.c_cc[VMIN]  = 0;            // read doesn't block
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

	tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
	// enable reading
	tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
	tty.c_cflag |= parity;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	if (tcsetattr (fd, TCSANOW, &tty) != 0)
	{
		return 0;
	}
	return 0;
}
/* set blocking or none blocking */
int set_blocking (int fd, int should_block)
{
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0)
	{
		return 0;
	}

	tty.c_cc[VMIN]  = should_block ? 1 : 0;
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	if (tcsetattr (fd, TCSANOW, &tty) != 0);//error
}

char *portname = "/dev/ttyUSB0"; //the camera entry point



/* Send commands to the controller */
int agri_send_cmd(void  *x)
{
	int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if(fd < 0)
	{
		return 0;
	}

	set_interface_attribs (fd, B9600, 0);  // set speed to 9600bps
	set_blocking (fd, 0);                // set no blocking
	write (fd, "hello!\n", 7);           // send 7 character greeting
	usleep ((7 + 25) * 100);             // sleep enough to transmit the 7 plus
	// receive 25:  rounding up 100 uS per char transmit
	close(fd);
	return 1;
}
int  agri_receive_cmd()
{
	int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0)
	{
		return 0;
	}
	set_interface_attribs (fd, B9600, 0);
	set_blocking (fd, 0);                // set no blocking
	int n = read (fd, buf, sizeof buf);
	usleep ((7 + 25) * 100); 
	close(fd);
	return  1;
}

/* Capture and process the image */
int agri_process_img()
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

	/*write captured image */
	imwrite("/home/ubuntu/share/sample.png", image);
	return 1;
}

int main()
{
char *x = "hi";
	agri_process_img();
	agri_send_cmd(x);
	waitKey(0); // Wait for a keystroke in the window
	return 0;
}
