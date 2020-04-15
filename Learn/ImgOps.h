#pragma once
#include <string>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

class ImgOps {
private:
	cv::Mat image;
	int hMin = 35;
	int hMax = 179;
	int sMin = 58;
	int sMax = 255;
	int vMin = 0;
	int vMax = 255;

	void on_trackbar(int, void*);

public:
	ImgOps(std::string);
	ImgOps(cv::Mat);
	void display(std::string);
	
	cv::Mat getImage();
	cv::Mat getEdges();
	cv::Mat getImageGreyScale();
	cv::Mat dilate(cv::Mat image, int);
	cv::Mat resize(int, int);
	cv::Mat crop(int, int, int, int);
	void draw();

	cv::Mat deWarp(cv::Point2f, cv::Point2f, cv::Point2f, cv::Point2f);
	void detectContours();
};