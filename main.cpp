#include "opencv2/opencv.hpp"
#include "BackgroundSubtract.h"
#include "Detector.h"
#include <opencv2/highgui/highgui_c.h>
#include "Ctracker.h"
#include <iostream>
#include <vector>

 #define ExampleNum 1

int main(int argc, char** argv)
{
    std::string inFile("output.avi");
    /*if (argc > 1)
	{
		inFile = argv[1];
	}

	std::string outFile;
	if (argc > 2)
	{
		outFile = argv[2];
	}*/

	cv::VideoWriter writer;

#if ExampleNum
	cv::Scalar Colors[] = { cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255), cv::Scalar(255, 255, 0), cv::Scalar(0, 255, 255), cv::Scalar(255, 0, 255), cv::Scalar(255, 127, 255), cv::Scalar(127, 0, 255), cv::Scalar(127, 0, 127) };
	cv::VideoCapture capture(inFile);
	if (!capture.isOpened())
	{
		return 1;
	}
	cv::namedWindow("Video");
	cv::Mat frame;
	cv::Mat gray;

    bool useLocalTracking = true;

    CTracker tracker(useLocalTracking, 0.2f, 0.1f, 60.0f, 10, 50);
		
	capture >> frame;
	cv::resize(frame, frame, cv::Size(390, 300));
	cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
	CDetector detector(BackgroundSubtract::MOG_ALG, useLocalTracking, gray);
	detector.SetMinObjectSize(cv::Size(gray.cols /10, gray.rows /10));
	int k = 0;

	double freq = cv::getTickFrequency();

	int64 allTime = 0;

    bool manualMode = false;

	while (k != 27)
	{
		capture >> frame;
		if (frame.empty())
		{
			//capture.set(cv::CAP_PROP_POS_FRAMES, 0);
			//continue;
			break;
		}
		cv::resize(frame, frame, cv::Size(390, 300));
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

		/*if (!writer.isOpened())
		{
			writer.open(outFile, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), capture.get(cv::CAP_PROP_FPS), frame.size(), true);
		}*/

		int64 t1 = cv::getTickCount();

		const std::vector<Point_t>& centers = detector.Detect(gray);
        const regions_t& regions = detector.GetDetects();

        tracker.Update(centers, regions, CTracker::RectsDist
			, gray);

		int64 t2 = cv::getTickCount();

		allTime += t2 - t1;

		for (auto p : centers)
		{
			cv::circle(frame, p, 3, cv::Scalar(0, 255, 0), 1, CV_AA);
		}

		std::cout << tracker.tracks.size() << std::endl;

        for (size_t i = 0; i < tracker.tracks.size(); i++)
		{
			cv::rectangle(frame, tracker.tracks[i]->GetLastRect(), cv::Scalar(0, 255, 0), 1, CV_AA);

			if (tracker.tracks[i]->trace.size() > 1)
			{
				for (size_t j = 0; j < tracker.tracks[i]->trace.size() - 1; j++)
				{
					cv::line(frame, tracker.tracks[i]->trace[j], tracker.tracks[i]->trace[j + 1], Colors[tracker.tracks[i]->track_id % 9], 2, CV_AA);
				}
			}
		}

		cv::imshow("Video", frame);

        int waitTime = manualMode ? 0 : 20;
        k = cv::waitKey(waitTime);

        if (k == 'm' || k == 'M')
        {
            manualMode = !manualMode;
        }

        if (writer.isOpened())
		{
			writer << frame;
		}
	}

	std::cout << "work time = " << (allTime / freq) << std::endl;

#else
	// other option
#endif

	cv::destroyAllWindows();
	return 0;
}
