#include "inc.hpp" 
#include <ros/ros.h>
#include <image_transport/image_transport.h>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <iostream>
#include <fstream>
#include <cv_bridge/cv_bridge.h>
//vision
#include "findContour.hpp"
#include "detectColour.hpp"
#include "detectFeature.hpp"
#include "util.hpp"
//using namespace cv;	//already defined
using namespace ros;
using namespace image_transport;

void imageCallback(const sensor_msgs::ImageConstPtr& msg){
	FindContour fc;
	DetectColour dc;
	DetectFeature df;
	try{
		Mat camFrame;
		camFrame=cv_bridge::toCvShare(msg, "bgr8")->image;
		vid_colour = dc.Process(camFrame);
		vid_contour = fc.Process(vid_colour,true);
		//imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
		//cvtColor(cv_bridge::toCvShare(msg, "bgr8")->image,cv_HSV,COLOR_RGB2HSV);
		waitKey(1);		//curious
	}
	catch (cv_bridge::Exception& e){
		ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
	}
}

int main(int argc, char **argv){
	//ros stuff//
	init(argc, argv, "image_listener");
	NodeHandle nh;
	namedWindow("view",WINDOW_NORMAL);

	startWindowThread();
	ImageTransport it(nh);
	
	//vision stuff//
	namedWindow("ObjectContour");
	namedWindow("Video");
	
	//image
	FindContour fc;
	Mat obj_contour;
	obj_contour = fc.Process("0001.png",true);
	
	imshow("ObjectContour",obj_contour);
	
	//video
	Mat vid_contour;
	int frameCount=0;
	//VideoCapture camera(0);
	/*if(!camera.isOpened()) {
		cout << "Camera two not initialized."<<endl;
		return 1;
	}*/
	int kernel_length=3;
	Mat camFrame;
	
	Mat vid_colour;
	Mat vid_matches;
	
	//Subscriber sub = it.subscribe("stereo_camera/left/image_raw", 1, imageCallback);
	
	//try
	while(1){
		frameCount++;
		Subscriber sub = it.subscribe("stereo_camera/left/image_raw", 1, imageCallback);
		vid_matches = df.Process(obj_contour, vid_contour);
		imshow("Video",vid_matches);
		
		char c = waitKey( 33 );
		if( c == 27 ) break;
	}
	
	spin();
	destroyAllWindows();
	
    return 0;
}
