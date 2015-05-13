#include "inc.hpp"
//#include "combined.hpp"
#include "findContour.hpp"
#include "detectColour.hpp"
#include "detectFeature.hpp"
#include "util.hpp"

//using namespace std;
//using namespace cv;

int main(int, char**) {
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
	VideoCapture camera(0);
	
	if(!camera.isOpened()) {
		cout << "Camera two not initialized."<<endl;
		return 1;
	}
	int kernel_length=3;
	Mat camFrame;
	
	DetectColour dc;
	DetectFeature df;
	Mat vid_colour;
	Mat vid_matches;

	while(1) {
		camera.read(camFrame);
		if( camFrame.empty() ) break;
		frameCount++;

		printMatType(camFrame,"camFrame");
		vid_colour = dc.Process(camFrame);
		printMatType(vid_colour,"vid_colour");
		//vid_contour = fc.Process(vid_colour,false);
		vid_contour = fc.Process(vid_colour,true);
		//imshow("Video",vid_contour);
		
		//feature
		printMatType(vid_contour,"vid_contour");
		printMatType(obj_contour,"obj_contour");
		vid_matches = df.Process(obj_contour, vid_contour);
		imshow("Video",vid_matches);
		
		char c = waitKey( 33 );
		if( c == 27 ) break;
	}

	camera.release();
	destroyAllWindows();
	
    return 0;
}
