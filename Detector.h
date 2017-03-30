#pragma once
#include "BackgroundSubtract.h"
#include <iostream>
#include <vector>
#include "defines.h"

class CDetector
{
public:
	CDetector(bool collectPoints, cv::Mat& gray);
	~CDetector(void);
	
	void SetMinObjectSize(cv::Size minObjectSize);

	const std::vector<Point_t>& Detect(cv::Mat& gray);
	const regions_t& GetDetects() const;

private:
	void DetectContour();

	std::unique_ptr<BackgroundSubtract> m_backgroundSubst;
	regions_t m_regions;
	std::vector<Point_t> m_centers;
	cv::Mat m_fg;
	cv::Size m_minObjectSize;
	bool m_collectPoints;
};
