#include "BackgroundSubtract.h"

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

void BackgroundSubtract::subtract(const cv::Mat& image, cv::Mat& foreground)
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

	cv::imshow("before", foreground);
	cv::GaussianBlur(foreground, foreground, cv::Size(5, 5), 0);

	//cv::Mat dilateElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7,7), cv::Point(-1, -1));
	cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
	cv::Mat structuringElement15x15 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));
	//cv::dilate(foreground, foreground, dilateElement, cv::Point(-1, -1), 1);	
	cv::erode(foreground, foreground, structuringElement15x15, cv::Point(-1, -1), 1);
	cv::imshow("after", foreground);
}