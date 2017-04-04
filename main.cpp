
#include "opencv2/opencv.hpp"
#include "BackgroundSubtract.h"
#include "Detector.h"
#include <opencv2/highgui/highgui_c.h>
#include "Ctracker.h"
#include <iostream>
#include <vector>
#include "Tuner.h"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/videoio/videoio.hpp"

 #define ExampleNum 1

int main(int argc, char** argv)//int argc, char** argv
{
	//video name
    std::string inFile("5thFloor.mp4");
	//std::string outFile("bungeeOut.mp4");
	int X;
	int Y;
	int width;
	int height;

	if (argc > 1)
	{
		inFile = argv[1];
	}
	
	
/*if (argc > 7)
	{
		outFile = argv[6];
	}
	if (argc > 6)
	{
		X = (int)argv[2];
		Y = (int)argv[3];
		width = (int)argv[4];
		height = (int)argv[5];
	}*/
	

	cv::VideoWriter writer;
	
	int fourcc = cv::VideoWriter::fourcc('M', 'P', '4', '2');

#if ExampleNum
	//array with colors
	cv::Scalar Colors[] = { cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255), cv::Scalar(255, 255, 0), cv::Scalar(0, 255, 255), cv::Scalar(255, 0, 255), cv::Scalar(255, 127, 255), cv::Scalar(127, 0, 255), cv::Scalar(127, 0, 127) };
	
	// put video file in stream
	cv::VideoCapture capture(inFile);

	//stop the programm if unaible to read this frame
	if (!capture.isOpened())
	{
		return 1;
	}
	//create window for video
	cv::namedWindow("Video");
	
	cv::Mat frame;
	cv::Mat gray;
	
	// If true then trajectories will be more smooth and accurate
	// But on high resolution videos with many objects may be to slow
    bool useLocalTracking = true;

	// initialize tracker (useLocalTracking, dt(Шаг времени опроса фильтра), 
	// Accel noise mag, dist thres, maximum allowed skipped frames, max trace length)
	CTracker tracker(useLocalTracking, kalmanStep, accelNoise, distThresh, skipFrames, maxTraceLength);
	
	// take one frame from stream
	capture >> frame;
	
	X = 0;
	Y = 0;
	width = frame.cols;
	height = frame.rows;

	cv::Mat croppedImage = frame(cv::Rect(X, Y, width, height));

	//make small windows to see many simulteneously and reduce computing(really??)
	//cv::resize(croppedImage, croppedImage, cv::Size(390, 300));
	
	//change color space from RGB usually to gray to reduce computing ( how to work with RGB??, is the information loss/computation tradeoff)
	cv::cvtColor(croppedImage, gray, cv::COLOR_BGR2GRAY);

	// initialize detecor object (collect points? ~ useLocalTrackin, our changed to gray frame)
	// not in while loop cause it needed just once
	CDetector detector(useLocalTracking, gray);

	// seting minimum size of object to be detected relative our frame size
	// not in while loop cause it needed just once
	detector.SetMinObjectSize(cv::Size(gray.cols*minObjectWidth, gray.rows*minObjectHeight));
	
	//?????????????????? lol wut 
	int k = 0;

	//  number of clock-cycles per second
	double freq = cv::getTickFrequency();

	//initializing variable for mesuaring time nedded to process  wut???
	int64 allTime = 0;

	//??????
    bool manualMode = false;

	// stackoverflow magic number
	while (k != 27)
	{
		// get next frame from a videofile
		capture >> frame;
		
		cv::Mat croppedImage = frame(cv::Rect(X, Y, width, height));
		
		if (croppedImage.empty())
		{
			//capture.set(cv::CAP_PROP_POS_FRAMES, 0);
			//continue;
			break;
		}
		// resize and make gray
		//cv::resize(croppedImage, croppedImage, cv::Size(390, 300));
		cv::cvtColor(croppedImage, gray, cv::COLOR_BGR2GRAY);
		/*if (!writer.isOpened())
		{
			writer.open(outFile, capture.get(cv::CAP_PROP_FOURCC), capture.get(cv::CAP_PROP_FPS), croppedImage.size(), true);
		}
		*/
		// number of clock-cycles after a reference event (like the moment machine was switched ON) to the moment this function is called
		int64 t1 = cv::getTickCount();
		const std::vector<Point_t> & centers = detector.Detect(gray);
        const regions_t & regions = detector.GetDetects();

		//??????????????
        tracker.Update(centers, regions, CTracker::RectsDist, gray);

		int64 t2 = cv::getTickCount();
		
		// the time of execution in seconds,
		allTime += t2 - t1;

		// draw object centers
		for (auto p : centers)
		{
			cv::circle(croppedImage, p, 3, cv::Scalar(0, 255, 0), 1, CV_AA);
		}
		// prints out how much tracks we have on each frame
		std::cout << tracker.tracks.size() << std::endl;

		// draw bounding boxes and traces 
        for (size_t i = 0; i < tracker.tracks.size(); i++)
		{
			cv::rectangle(croppedImage, tracker.tracks[i]->GetLastRect(), cv::Scalar(0, 255, 0), 1, CV_AA);

			if (tracker.tracks[i]->trace.size() > 1)
			{
				for (size_t j = 0; j < tracker.tracks[i]->trace.size() - 1; j++)
				{
					cv::line(croppedImage, tracker.tracks[i]->trace[j], tracker.tracks[i]->trace[j + 1], Colors[tracker.tracks[i]->track_id % 9], 2, CV_AA);
				}
			}	
		}
		// show modified frame
		cv::imshow("Video", croppedImage);

        int waitTime = manualMode ? 0 : 20;
        k = cv::waitKey(waitTime);

		//?????????
        if (k == 'm' || k == 'M')
        {
            manualMode = !manualMode;
        }

		//if writer is opened add one more frame
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
