#include "BackgroundSubtract.h"

BackgroundSubtract::BackgroundSubtract(
	BGFG_ALGS algType,
	int channels,
	int samples,
	int pixel_neighbor,
	int distance_threshold,
	int matching_threshold,
	int update_factor
	)
	:
	m_channels(channels),
	m_algType(algType)
{
	switch (m_algType)
	{
	case VIBE_ALG:
		m_modelVibe = std::make_unique<vibe::VIBE>(m_channels, samples, pixel_neighbor, distance_threshold, matching_threshold, update_factor);
		break;

	case MOG_ALG:
		m_modelOCV = cv::bgsegm::createBackgroundSubtractorMOG(115, 7, 0.01, 0); 
		break;

	}
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

	switch (m_algType)
	{
	case VIBE_ALG:
		m_modelVibe->update(GetImg());
		foreground = m_modelVibe->getMask();
		break;

	case MOG_ALG:
		m_modelOCV->apply(GetImg(), foreground);
		break;
	}
	
	cv::imshow("before", foreground);
	cv::GaussianBlur(foreground, foreground, cv::Size(21, 21), 0);

	cv::Mat dilateElement = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7), cv::Point(-1, -1));
	cv::dilate(foreground, foreground, dilateElement, cv::Point(-1, -1), 1);	

	cv::imshow("after", foreground);
}
