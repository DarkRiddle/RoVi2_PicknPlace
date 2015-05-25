#ifndef __FINDCONTOUR_H
#define __FINDCONTOUR_H

#include "inc.hpp"
#include "util.hpp"

class FindContour {
	public:
		FindContour(void) {};
		~FindContour(void) {};
		Mat Process(String filename,bool convert=false);
		Mat Process(Mat image,bool convert=false);
};

#endif
