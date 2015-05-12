#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cv.h>
#include <cvaux.h>
#include <cxcore.h>
#include <highgui.h>
#include <math.h>
#include <string.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>


using namespace std;
using namespace cv;

int main(int, char**)
{

    cvNamedWindow("Sobel", CV_WINDOW_AUTOSIZE); 
	CvCapture* capture = cvCaptureFromCAM(0);

	IplImage* frame;
	
	int kernel_length = 3;
	
	//sobel
	Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	int c;

    while(1) {
        frame = cvQueryFrame( capture );

        int depth_img	=frame->depth;
        int height_img	=frame->height;
        int width_img	=frame->width;
        int size_img	=frame->imageSize;
        int nchan_img	=frame->nChannels;
        int nsize_img	=frame->nSize;

        cout << setw(15) << "depth"		<<  depth_img	<< endl;
        cout << setw(15) << "height"	<<  height_img	<< endl;
        cout << setw(15) << "width"		<<  width_img	<< endl;
        cout << setw(15) << "size"		<<  size_img	<< endl;
        cout << setw(15) << "nchan"		<<  nchan_img	<< endl;
        cout << setw(15) << "nsize"		<<  nsize_img	<< endl;


        IplImage* out = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 3 );
        IplImage* gray_out = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
        //IplImage* billy_out = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
        IplImage* canny_out = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
        
        cvSmooth( frame, out, CV_GAUSSIAN, 11, 11 );
        cvCvtColor(out , gray_out, CV_RGB2GRAY);
        
        // bilateralFilter heeft cv::Mat als input nodig i.p.v. IplImage*... dit is een mix van C en C++...
        //Bilateral blur:
		//bilateralFilter(gray_out, billy_out, kernel_length, (double)(kernel_length*2), (double)(kernel_length/2));
		
		//canny edge
        //cvCanny( gray_out, canny_out, 10, 10, 3 );
        
        /// Generate grad_x and grad_y
		Mat grad_x, grad_y;
		Mat abs_grad_x, abs_grad_y;
		
		/// Gradient X
		//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
		Sobel( gray_out, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
		convertScaleAbs( grad_x, abs_grad_x );

		/// Gradient Y
		//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
		Sobel( gray_out, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
		convertScaleAbs( grad_y, abs_grad_y );

		/// Total Gradient (approximate)
		addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
        
        
        if( !frame ) break;
        cvShowImage( "Sobel", grad );
        char c = cvWaitKey(33);
        if( c == 27 ) break;
    }
    cvReleaseCapture( &capture );
    cvDestroyWindow( "Sobel" );
    return 0;
}
