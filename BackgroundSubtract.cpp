#include "BackgroundSubtract.h"
#include "Tuner.h"

BackgroundSubtract::BackgroundSubtract(
	int channels,
	int samples,
	int pixel_neighbor,
	int distance_threshold,
	int matching_threshold,
	int update_factor
	)
	:
	m_channels(channels)
{
}

void BackgroundSubtract::subtract(const cv::Mat& image, std::vector<cv::Mat>& foreground)
{
	auto GetImg = [&]() -> cv::Mat
	{
		if (image.channels() != m_channels)
		{
			if (image.channels() == 1)
			{
				cv::Mat newImg;
				cv::cvtColor(image, newImg, CV_GRAY2BGR);
				return newImg;
			}
			else if (image.channels() == 3)
			{
				cv::Mat newImg;
				cv::cvtColor(image, newImg, CV_BGR2GRAY);
				return newImg;
			}
		}
		return image;
	};
	
	static cv::Mat imgFrame1;
	static cv::Mat imgFrame2;
	
	if (isFirstFrame) {
		imgFrame1 = foreground;
		isFirstFrame = false;
	}
	
	imgFrame2 = image;

	cv::Mat imgFrame1Copy = imgFrame1.clone();
	cv::Mat imgFrame2Copy = imgFrame2.clone();

	cv::Mat imgDifference;
	cv::Mat imgThresh;
	
	cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(7, 7), 0);
	cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(7, 7), 0);
	cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);

	cv::threshold(imgDifference, imgThresh, subtractThresh, 255.0, CV_THRESH_BINARY);

	cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
	cv::Mat structuringElement15x15 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));

	for (unsigned int i = 0; i < 2; i++) {
		cv::dilate(imgThresh, imgThresh, structuringElement3x3);
		cv::dilate(imgThresh, imgThresh, structuringElement7x7);
		cv::erode(imgThresh, imgThresh, structuringElement5x5);
	}

	cv::imshow("after", imgThresh);

	foreground = imgThresh.clone();
	imgFrame1 = imgFrame2.clone();
	
}