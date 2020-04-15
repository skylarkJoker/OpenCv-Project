#pragma once
#include <string>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

class VidOps {
private:
	cv::VideoCapture vidCapture;
	cv::Mat frame;
	const int width = 640;
	const int height = 480;
public:
	VidOps(std::string);
	VidOps(int);
	void display(std::string);
};
