#pragma once
#include "BackgroundSubtract.h"
#include <iostream>
#include <vector>
#include "defines.h"
#include <memory>

class CDetector
{
public:
	CDetector(bool collectPoints, std::vector<cv::Mat&> grayFrames);
	~CDetector(void);
	
	void SetMinObjectSize(std::vector<cv::Size> minObjectSize);

	const std::vector<Point_t>& Detect(cv::Mat& gray);
	const regions_t& GetDetects() const;

private:
		void DetectContour();

		std::unique_ptr<BackgroundSubtract> m_backgroundSubst;
	regions_t m_regions;
		std::vector<Point_t> m_centers;
		std::vector<cv::Mat> m_fg;
		std::vector<cv::Size> m_minObjectSize;
		bool m_collectPoints;
};
