#ifndef MYTOOLS_HPP
#define MYTOOLS_HPP
#include <opencv2/core/core.hpp>
using namespace cv;

void showImage(char* windowName, Mat &img, bool wait = true);
string getImgType(int imgTypeInt);

#endif // MYTOOLS_HPP
