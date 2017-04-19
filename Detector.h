#pragma once
#include "BackgroundSubtract.h"
#include <iostream>
#include <vector>
#include "defines.h"
#include <memory>

class CDetector
{
public:
	CDetector(bool collectPoints, cv::Mat & gray);
	~CDetector(void);
	
	void SetMinObjectSize(cv::Size minObjectSize, int index);

	const std::vector<Point_t>& Detect(cv::Mat& gray, int index);
	const regions_t& GetDetects() const;

private:
		void DetectContour(int index);

		bool m_collectPoints;
		regions_t m_regions;
		std::unique_ptr<BackgroundSubtract> m_backgroundSubt;
		std::vector<Point_t> m_centers;
		std::vector<cv::Mat> m_fgs;
		std::vector<cv::Size> m_minObjectSizes;
		
};
