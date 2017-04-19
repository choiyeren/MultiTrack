#include "CameraStreamer.h"
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui_c.h>
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2\highgui.hpp"
#include <string>
#include "Tuner.h"
#include "Detector.h"
#include "Ctracker.h"

//global variables
// If true then trajectories will be more smooth and accurate
// But on high resolution videos with many objects may be to slow
bool useLocalTracking = true;

std::vector<cv::Mat> firstGray;
cv::Scalar Colors[] = { cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255), cv::Scalar(255, 255, 0), cv::Scalar(0, 255, 255), 
						cv::Scalar(255, 0, 255), cv::Scalar(255, 127, 255), cv::Scalar(127, 0, 255), cv::Scalar(127, 0, 127) };

CameraStreamer::CameraStreamer(vector<string> stream_source)
{
	camera_source = stream_source;
	camera_count = camera_source.size();
	isUSBCamera = false;
	
	startMultiCapture();
}

CameraStreamer::~CameraStreamer()
{
	stopMultiCapture();
}

void CameraStreamer::captureFrame(int index)
{
	//number of clock-cycles per second
	double freq = cv::getTickFrequency();

	//initializing variable for mesuaring time nedded to process  wut???
	int64 allTime = 0;
	//??????
	bool manualMode = false;
	//?????????????????? a
	int k = 0;
	
	VideoCapture *capture = camera_capture[index];
	// initialize tracker(useLocalTracking, dt(Шаг времени опроса фильтра),
	// Accel noise mag, dist thres, maximum allowed skipped frames, max trace length)
	CTracker tracker(useLocalTracking, kalmanStep[index], accelNoise[index], distThresh[index], skipFrames[index], maxTraceLength[index]);
	// initialize detecor object (collect points? ~ useLocalTrackin, our changed to gray frame)
	// not in while loop cause it needed just once
	CDetector  detector(useLocalTracking, firstGray[index]);
	
	// seting minimum size of object to be detected relative our frame size
	cv::Size minObjSize = cv::Size(minObjectWidth[index], minObjectHeight[index]);
	detector.SetMinObjectSize(minObjSize, index);

	while (k = !27)
	{
		Mat frame;
		Mat gray;

		//Grab frame from camera capture
		(*capture) >> frame;

		if (frame.empty())
		{
			break;
		}
		// number of clock-cycles after a reference event (like the moment machine was switched ON) to the moment this function is called
		int64 t1 = cv::getTickCount();

		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
		
		const std::vector<Point_t> & centers = detector.Detect(gray, index);
		const regions_t & regions = detector.GetDetects();

		//??????????????
		tracker.Update(centers, regions, CTracker::RectsDist, gray);

		int64 t2 = cv::getTickCount();

		// the time of execution in seconds,
		allTime += t2 - t1;

		// draw object centers
		for (auto p : centers)
		{
			cv::circle(frame, p, 3, cv::Scalar(0, 255, 0), 1, CV_AA);
		}
		// prints out how much tracks we have on each frame
		std::cout << tracker.tracks.size() << std::endl;

		// draw bounding boxes and traces 
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
		// show modified frame
		cv::imshow("Video", frame);

		int waitTime = manualMode ? 0 : 20;
		k = cv::waitKey(waitTime);

		//?????????
		if (k == 'm' || k == 'M')
		{
			manualMode = !manualMode; 
		}
	}

	std::cout << "work time = " << (allTime / freq) << std::endl;
}

void CameraStreamer::startMultiCapture()
{
	VideoCapture *capture;
	thread *t;

	for (int i = 0; i < camera_count; i++)
	{
		Mat frame;
		Mat gray;
		
		//Make VideoCapture instance
		string url = camera_source[i];
		capture = new VideoCapture(url);
		cout << "Camera Setup: " << url << endl;
		
		//get first frame
		(*capture) >> frame;
		
		//change color space from RGB usually to gray to reduce computing ( how to work with RGB??, information loss/computation tradeoff)
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
		firstGray.push_back(gray);
		
		//Put VideoCapture to the vector
		camera_capture.push_back(capture);

		//Make thread instance
		t = new thread(&CameraStreamer::captureFrame, this, i);

		//Put thread to the vector
		camera_thread.push_back(t);
	}
}

void CameraStreamer::stopMultiCapture()
{
	VideoCapture *cap;
	for (int i = 0; i < camera_count; i++) {
		cap = camera_capture[i];
		if (cap->isOpened()) {
			//Relase VideoCapture resource
			cap->release();
			cout << "Capture " << i << " released" << endl;
		}
	}
}