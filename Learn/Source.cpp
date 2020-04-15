#include <iostream>
#include "VidOps.h"
#include "ImgOps.h"



int main() {

	ImgOps imgOps("E:\\Project Resources\\yasuo.png");
	imgOps.detectContours();
	cv::waitKey(0); 
	return 0;
}

