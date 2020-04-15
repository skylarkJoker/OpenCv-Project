#include <iostream>
#include "ImgOps.h"

ImgOps::ImgOps(std::string imageFile) {
	image = cv::imread(imageFile, cv::IMREAD_COLOR);
	if (!image.data) {
		std::cerr << "Unable to open: " << imageFile << std::endl;
	}
}

ImgOps::ImgOps(cv::Mat imageMat) {
	image = imageMat.clone();
}

void ImgOps::display(std::string winName) {

		cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
		cv::imshow(winName, image);

}

cv::Mat ImgOps::getImageGreyScale() {
	cv::Mat gris;
	cv::cvtColor(image, gris,  cv::COLOR_BGR2GRAY);
	return gris;
}

cv::Mat ImgOps::getEdges() {
	cv::Mat edges;
	cv::Canny(image, edges, 100, 100);
	return edges;
}

cv::Mat ImgOps::dilate(cv::Mat image, int interations) {
	cv::Mat dilated;
	cv::Mat element = cv::getStructuringElement(0, cv::Size(3, 3), cv::Point(-1, -1));
	cv::dilate(image, dilated, element,cv::Point(-1, -1), interations);

	return dilated;
}

cv::Mat ImgOps::getImage() {
	return image;
}

cv::Mat ImgOps::resize(int width, int height) {
	cv::Mat newSize;
	cv::resize(image, newSize, cv::Size(width, height));
	return newSize;
}

cv::Mat ImgOps::crop(int top, int left, int width, int height) {
	cv::Mat newRange;
	
	newRange = image(cv::Range(top, top + height), cv::Range(left, left + width));

	return newRange;
}

void ImgOps::draw() {
	cv::Mat temp(512, 512, CV_8UC3, cv::Scalar(255, 255, 255));
//	temp(cv::Range(256, 306), cv::Range(256, 306)) = cv::Scalar(50, 200, 157);
	cv::line(temp, cv::Point(0, 0), cv::Point(256, 256), cv::Scalar(230, 190, 0));
	cv::rectangle(temp, cv::Point(0, 0), cv::Point(256, 256), cv::Scalar(230, 190, 0) , 6);
	cv::circle(temp, cv::Point(temp.size[0]/2, temp.size[1]/2), 100, cv::Scalar(230, 190, 0) , 1);
	cv::putText(temp, "ENTITY 1:0", cv::Point(50, 253), cv::FONT_HERSHEY_SIMPLEX, .48f, cv::Scalar(0, 0, 0), 1);
	cv::imshow("draw", temp );
}

cv::Mat ImgOps::deWarp(cv::Point2f tleft, cv::Point2f tright, cv::Point2f bleft, cv::Point2f bright) {
	cv::Point2f temp[] = { tleft, tright, bleft, bright };
	cv::Point2f target[] = { cv::Point2f(0,0), cv::Point2f(440, 0), cv::Point2f(0,660), cv::Point2f(440, 660)};
	cv::Mat matrix = cv::getPerspectiveTransform(temp, target);
	
	cv::Mat dewarped;
	cv::warpPerspective(image, dewarped, matrix, cv::Size(440, 660));

	return dewarped;
}

void ImgOps::on_trackbar(int , void*) {
	hMin = cv::getTrackbarPos("Hue Min", "THub");
	hMax = cv::getTrackbarPos("Hue Max", "THub");
	sMin = cv::getTrackbarPos("Sat Min", "THub");
	sMax = cv::getTrackbarPos("Sat Max", "THub");
	vMin = cv::getTrackbarPos("Val Min", "THub");
	vMax = cv::getTrackbarPos("Val Max", "THub");
}

void ImgOps::detectContours() {

	std::vector<std::vector<cv::Point>> contour;
	std::vector<std::vector<cv::Point>> shapes;
	std::vector<cv::Vec4i> hierarchy;
	std::vector<cv::Point> poly;

	double area, perimeter = 0;

	cv::Mat blurred, edges;
	cv::Mat gris = getImageGreyScale();
	cv::GaussianBlur(gris, blurred, cv::Size(7, 7), 1, 1);
	cv::Canny(blurred, edges, 100, 100);

	cv::findContours(edges, contour, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	for (int idx = 0; idx >= 0; idx = hierarchy[idx][0]) {
		area = std::fabs(cv::contourArea(contour.at(idx)));
		if (area > 12) {
			perimeter = cv::arcLength(contour.at(idx), false);
			cv::approxPolyDP(contour.at(idx), poly, perimeter*0.02, false);
			if (poly.size() >= 3)
				shapes.push_back(poly);

		}

	}

	for (int i = 0; i < shapes.size(); i++) {
		const cv::Point *p = &shapes[i][0];
		int n = (int)shapes[i].size();
		cv::polylines(image , &p, &n, 1, false, cv::Scalar(200, 190, 0), 2, cv::LINE_AA);
	}


	imshow("Original", image);
	//imshow("Gris", gris);
	//imshow("Blurred", blurred);
	imshow("Edge", edges);
	return;
}
