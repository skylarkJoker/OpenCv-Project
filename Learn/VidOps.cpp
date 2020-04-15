#include <iostream>
#include "VidOps.h"

VidOps::VidOps(std::string videoFile) {
	vidCapture = cv::VideoCapture (videoFile);
	if (!vidCapture.isOpened()) {
		std::cerr << "Unable to open: " << videoFile << std::endl;
	}
}

VidOps::VidOps(int index) {
	vidCapture = cv::VideoCapture (index);
	vidCapture.set(cv::CAP_PROP_FRAME_WIDTH, width);
	vidCapture.set(cv::CAP_PROP_FRAME_HEIGHT, height);

	if (!vidCapture.isOpened()) {
	std::cerr << "Unable to open camera " << std::endl;
	}
}

void VidOps::display(std::string winName) {
	while (true) {
		vidCapture >> frame;
		if (frame.empty())
			break;

		cv::namedWindow(winName, cv::WINDOW_NORMAL);
		cv::imshow(winName, frame);

		int keyboard = cv::waitKey(30);
		if (keyboard == 'q' || keyboard == 27)
			break;
	}

}