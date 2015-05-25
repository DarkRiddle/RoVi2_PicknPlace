#ifndef __DETECTCOLOUR_H
#define __DETECTCOLOUR_H

#include "inc.hpp"
#include "util.hpp"

class DetectColour {
	private:
		static const int iLowH	= 0;
		static const int iHighH = 179;

		static const int iLowS	= 130; //162, 130=better, org=0
		static const int iHighS = 255;

		static const int iLowV	= 80;	//org.=0
		static const int iHighV = 255;
	public:
		DetectColour(void) {};
		~DetectColour(void) {};
		Mat Process(Mat);
};

#endif
