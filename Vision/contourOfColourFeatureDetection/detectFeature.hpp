#ifndef __DETECTFEATURE_H
#define __DETECTFEATURE_H

#include "inc.hpp"
#include "util.hpp"

class DetectFeature {
	private:

	public:
		DetectFeature(void) {};
		~DetectFeature(void) {};
		Mat Process(Mat,Mat);
};

#endif
