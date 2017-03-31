#include "Detector.h"
#include <memory>

CDetector::CDetector(bool collectPoints, cv::Mat& gray)
	: m_collectPoints(collectPoints) {
	m_fg = gray.clone();
	// create background subtruct
	m_backgroundSubst = std::make_unique<BackgroundSubtract>(gray.channels());

	m_minObjectSize.width = std::max(5, gray.cols / 100);
	m_minObjectSize.height = m_minObjectSize.width;
}

CDetector::~CDetector(void)
{
}

void CDetector::SetMinObjectSize(cv::Size minObjectSize)
{
	m_minObjectSize = minObjectSize;
}

// global variables 
const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
const cv::Scalar SCALAR_YELLOW = cv::Scalar(0.0, 255.0, 255.0);
const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 200.0, 0.0);
const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);

void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName) {
	cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);

	cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);

	cv::imshow(strImageName, image);
}
void CDetector::DetectContour()
{
	m_regions.clear();
	m_centers.clear();
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::findContours(m_fg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point());
	//_____________________________________________________________________________________________________
	std::vector<std::vector<cv::Point> > convexHulls(contours.size());

	for (unsigned int i = 0; i < contours.size(); i++) {
		cv::convexHull(contours[i], convexHulls[i]);
	}
	
	drawAndShowContours(m_fg.size(), convexHulls, "imgConvexHulls");
	//______________________________________________________________________________________________________
	if (contours.size() > 0)
	{
		for (size_t i = 0; i < contours.size(); i++)
		{
			cv::Rect r = cv::boundingRect(convexHulls[i]);

			if (r.width >= m_minObjectSize.width &&
				r.height >= m_minObjectSize.height)
			{
				CRegion region(r);
				cv::Point2f center(r.x + 0.5f * r.width, r.y + 0.5f * r.height);

				if (m_collectPoints)
				{
					const int yStep = 3;
					const int xStep = 3;

					for (int y = r.y; y < r.y + r.height; y += yStep)
					{
						cv::Point2f pt(0, static_cast<float>(y));
						for (int x = r.x; x < r.x + r.width; x += xStep)
						{
							pt.x = static_cast<float>(x);

							if (cv::pointPolygonTest(contours[i], pt, false) > 0)
							{
								region.m_points.push_back(pt);
							}
						}
					}
					if (region.m_points.empty())
					{
						region.m_points.push_back(center);
					}
				}
				m_regions.push_back(region);
				m_centers.push_back(Point_t(center.x, center.y));
			}
		}
	}
}
const std::vector<Point_t>& CDetector::Detect(cv::Mat& gray)
{
	m_backgroundSubst->subtract(gray, m_fg);
	DetectContour();
	return m_centers;
}

const regions_t& CDetector::GetDetects() const
{
	return m_regions;
}