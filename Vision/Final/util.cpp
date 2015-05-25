#include "util.hpp"

void printMatType(Mat input, String name){
	if (input.depth() == CV_8U){
		cout << name << " : " << "CV_8U" << "C" << input.channels() << endl;
	}else if(input.depth() == CV_8S){
		cout << name << " : " << "CV_8S" << "C" << input.channels() << endl;
	}else if(input.depth() == CV_16U){
		cout << name << " : " << "CV_16U" << "C" << input.channels() << endl;
	}else if(input.depth() == CV_16S){
		cout << name << " : " << "CV_16S" << "C" << input.channels() << endl;
	}else if(input.depth() == CV_32S){
		cout << name << " : " << "CV_32S" << "C" << input.channels() << endl;
	}else if(input.depth() == CV_32F){
		cout << name << " : " << "CV_32F" << "C" << input.channels() << endl;
	}else if(input.depth() == CV_64F){
		cout << name << " : " << "CV_64F" << "C" << input.channels() << endl;
	}else{
		cout << name << " : " << "_ERROR_" << "C" << input.channels() << endl;
	}
}
