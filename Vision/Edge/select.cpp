#include <opencv2/opencv.hpp>

#include <iostream>
#include <iomanip>
#include <cmath>

//test
#include "myTools.hpp"


using namespace std;
using namespace cv;

int main(int, char**)
{
//	namedWindow("Orig");
	//namedWindow("Sobel");
	namedWindow("Canny");
	//namedWindow("Sobel_Canny");
	namedWindow("Bila");
	namedWindow("CannyBilaSobel");
	namedWindow("Test");
	namedWindow("TestTwo");
	namedWindow("TestThree");
	
	namedWindow("TestCombi");
	
	int frameCount=0;
	VideoCapture camera(0);
	VideoCapture cameraTwo(1);
	
	if(!camera.isOpened()) {
		cout << "Camera not initialized."<<endl;
		return 1;
	}
	if(!cameraTwo.isOpened()) {
		cout << "Camera two not initialized."<<endl;
		return 1;
	}
	
	int kernel_length=3;

	Mat camFrame;
	Mat gblur;
	Mat gray;
	Mat canny;
	Mat sobel;
	Mat sobelcanny;
	
	Mat bila;
	Mat bilasobel;
	Mat bilacanny;
	
	Mat cannybilasobel;
	
	Mat test;
	Mat testTwo;
	Mat testToThree;
	Mat testThree;
	
	Mat testCombi;
    Mat testCombiTwo;
    Mat testCombiThree;
    
    Mat camFrameTwo;
    Mat CannyCamTwo;
    //Mat src;
    //Mat color;
    /*vector<vector<Point>> cont;  // Filled by cv::findContours()
	Mat labels = Mat::zeros(image.size(), CV_8UC1);     
	vector<float> cont_avgs(cont.size(), 0.f); // This contains the averages of each contour
    */
    
    while(1) {
		camera.read(camFrame);
        int depth_img =camFrame.depth();
        int height_img =camFrame.rows;
        int width_img =camFrame.cols;
        int nchan_img =camFrame.channels();
		
		//cout << "["<<frameCount<<"]["<<width_img<<","<<height_img<<"]["<<depth_img<<"] "<<nchan_img<<endl;

		//GaussianBlur(camFrame,gblur,Size(3,3),3*2,3/2);
		GaussianBlur(camFrame,gblur,Size(9,9),3*2,9/2);
		cvtColor(gblur, gray, CV_RGB2GRAY);
		
		Sobel(gray,sobel,-1,1,1);
		Canny(gray,canny,10,10,3);
		Canny(sobel,sobelcanny,25,25,7,true);
		
		bilateralFilter(gray, bila, kernel_length, kernel_length*2, kernel_length/2);
		Sobel(bila,bilasobel,-1,1,1);
		Canny(bilasobel,bilacanny,10,10,3);
		
		Canny(bilasobel,cannybilasobel,25,25,3);
		
		test=((cannybilasobel)+canny)/2;
		testTwo=((cannybilasobel)+sobelcanny)/2;
		testToThree=(cannybilasobel+test+testTwo)/3;
		bilateralFilter(testToThree,testThree, kernel_length, kernel_length*2, kernel_length/2);
		
		//detect circles
		//vector<Vec3f> circles;
		//HoughCircles( cannybilasobel, circles, CV_HOUGH_GRADIENT, 1, cannybilasobel.rows/8, 200, 100, 0, 0 );

		//cvtColor(cannybilasobel,color,CV_GRAY2RGB);

		/*for( size_t i = 0; i < circles.size(); i++ )
		{
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);*/
			// circle center
			//circle( color, center, 3, Scalar(0,255,0), -1, 8, 0 );
			// circle outline
			//circle( color, center, radius, Scalar(0,0,255), 3, 8, 0 );
		//}
		//end detect circles
		
		testCombi=(testTwo&bilacanny)|cannybilasobel;
		bilateralFilter(testCombi,testCombiTwo, kernel_length, kernel_length*2, kernel_length/2);
		testCombiThree=testCombiTwo&testThree;
		
		//camera two
		cameraTwo.read(camFrameTwo);
		Canny(camFrameTwo,CannyCamTwo,10,10,3);
		
		frameCount++;
		if( camFrame.empty() ) break;
		//imshow("Sobel",sobel);
		imshow("Canny",canny);
		//imshow("Sobel_Canny",sobelcanny);
		imshow("Bila",bilacanny);
		
		imshow("CannyBilaSobel",cannybilasobel);
		//imshow("CannyBilaSobel",color);
		imshow("Test",test);
		imshow("TestTwo",testTwo);
		imshow("TestThree",testThree);
		
		imshow("TestCombi",testCombiTwo);
		
		imshow("CameraTwo",CannyCamTwo);
		
		cvMoveWindow("CannyBilaSobel",0,0);
		cvMoveWindow("Test",0,600);
		cvMoveWindow("TestTwo",640,0);
		cvMoveWindow("TestThree",640,600);
		cvMoveWindow("Bila",1280,0);
		cvMoveWindow("Canny",1280,600);
		
		cvMoveWindow("CameraTwo",0,0);
		
		char c = waitKey( 33 );
		if( c == 27 ) break;
		
    }
	camera.release();
	cameraTwo.release();
	destroyAllWindows();
    return 0;
}
