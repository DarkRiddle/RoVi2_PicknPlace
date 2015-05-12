#include <opencv2/opencv.hpp>

#include <iostream>
#include <iomanip>
#include <cmath>

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>

using namespace std;
using namespace cv;

int main(int, char**)
{
	namedWindow("Test");
	
	int frameCount=0;
	VideoCapture cameraTwo(0);
	
	if(!cameraTwo.isOpened()) {
		cout << "Camera two not initialized."<<endl;
		return 1;
	}
	
	int kernel_length=3;

	Mat camFrame;
	Mat gray;
	Mat canny;
	
	Mat test;
    
    Mat camFrameTwo;
    Mat CannyCamTwo;
    
    while(1) {
		cameraTwo.read(camFrame);
        int depth_img =camFrame.depth();
        int height_img =camFrame.rows;
        int width_img =camFrame.cols;
        int nchan_img =camFrame.channels();

		cvtColor(camFrame, gray, CV_RGB2GRAY);
		Canny(gray, gray, 100, 200, 3);
		
		/// Find contours   
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		RNG rng(12345);
		findContours( gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		
		/// Draw contours
		Mat drawing = Mat::zeros( gray.size(), CV_8UC3 );
		for( int i = 0; i< contours.size(); i++ )
		{
			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
		}
		
		//camera two, webcam laptop
		cameraTwo.read(camFrameTwo);
		Canny(camFrameTwo,CannyCamTwo,10,10,3);
		
		frameCount++;
		if( camFrame.empty() ) break;
		imshow("Test",drawing);
		
		//cvMoveWindow("Test",0,600);
		
		char c = waitKey( 33 );
		if( c == 27 ) break;
		
    }
	cameraTwo.release();
	destroyAllWindows();
    return 0;
}
