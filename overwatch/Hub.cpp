#include <iostream>
#include <sstream>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

void grisImg(char* imageName, int i) {
	Mat image;
	image = imread(imageName, IMREAD_COLOR); // Read the file

	if (i != 2 || !image.data)
    {
     cout <<"No image loaded";
	 return;
    }

	Mat gray_image;
	cvtColor(image, gray_image, COLOR_BGR2GRAY);
	
	imwrite("gray_frank.jpg", gray_image);

	namedWindow(imageName, WINDOW_AUTOSIZE);
	namedWindow("Gris", WINDOW_AUTOSIZE);

	imshow("Gray", image);
	imshow("Gris", gray_image);
}

void bgSubtract(string algo, string file) {
	Ptr<BackgroundSubtractor> pBackSub;
	if (algo == "MOG2")
		pBackSub = createBackgroundSubtractorMOG2();
	else
		pBackSub = createBackgroundSubtractorKNN();

	VideoCapture capture(samples::findFile(file));
	if (!capture.isOpened()) {
		cerr << "Unable to open: " << file << endl;
		return;
	}

	Mat frame, fgMask;
	while (true) {
		capture >> frame;
		if (frame.empty())
			break;

		pBackSub->apply(frame, fgMask);

		rectangle(frame, cv::Point(10, 2), 
			cv::Point(100, 20), 
			cv::Scalar(255, 255, 255), -1);

		stringstream ss;
		ss << capture.get(CAP_PROP_POS_FRAMES);
		string frameNumberString = ss.str();
		putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
			FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

		imshow("Frame", frame);
		imshow("FG Mask", fgMask);
		
		int keyboard = waitKey(30);
		if (keyboard == 'q' || keyboard == 27)
			break;
	}

}

void meanShift(string file) {
	VideoCapture capture(file);
	if (!capture.isOpened()) {
		cerr << "Unable to open: " << file << endl;
		return;
	}

	Mat frame, roi, hsv_roi, mask;
	//Get first frame
	capture >> frame;
	
	//starting rect
	Rect track_window(2022, 1088, 174, 490);

	//ROI for tracking
	roi = frame(track_window);
	cvtColor(roi, hsv_roi, COLOR_BGR2HSV);
	inRange(hsv_roi, Scalar(0, 60, 32), Scalar(180, 255, 255), mask);

	float range_[]= { 0, 180 };
	const float* range[] = { range_ };

	Mat roi_hist;
	int histSize[] = { 180 };
	int channels[] = { 0 };
	calcHist(&hsv_roi, 1 ,channels, mask, roi_hist, 1, histSize, range);
	normalize(roi_hist, roi_hist, 0, 255, NORM_MINMAX);

	TermCriteria term_crit(TermCriteria::EPS | TermCriteria::COUNT, 10, 1);

	while (true) {
		Mat hsv, dst;
		capture >> frame;
		if (frame.empty())
			break;
		cvtColor(frame, hsv, COLOR_BGR2HSV);
		calcBackProject(&hsv, 1, channels, roi_hist, dst, range);

		//meanshift
		meanShift(dst, track_window, term_crit);
		
		//draw
		rectangle(frame, track_window, 255, 2);

		namedWindow("img2", WINDOW_NORMAL);
		imshow("img2", frame);

		int keyboard = waitKey(30);
		if (keyboard == 'q' || keyboard == 27)
			break;
	}
	
}

int main(int argc, char** argv)
{
	
	meanShift(argv[1]);

    return 0;
}