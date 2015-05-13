#include "findContour.hpp"

Mat FindContour::Process(String filename,bool convert)
{
	Mat image;
	image = imread(filename.c_str(),1);
	return Process(image, convert);
}

Mat FindContour::Process(Mat image,bool convert)
{
    if(convert) {
		Mat gray;
		cvtColor(image, gray, CV_BGR2GRAY);
		//cvtColor(image, gray, CV_RGB2GRAY);
		Canny(gray, image, 100, 200, 3);
	}
    /// Find contours   
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //RNG rng(12345);
    findContours( image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    /// Draw contours
    Mat drawing = Mat::zeros( image.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        //Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        Scalar color = Scalar( 255,255,255);
        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }
    printMatType(drawing,"drawing");
	return drawing;
}
