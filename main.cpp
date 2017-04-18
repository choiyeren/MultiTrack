
#include "BackgroundSubtract.h"
#include "Detector.h"
#include "Ctracker.h"
#include <iostream>
#include <vector>
#include "Tuner.h"
#include "CameraStreamer.h"

 #define ExampleNum 1

int main()
{
	// If true then trajectories will be more smooth and accurate
	// But on high resolution videos with many objects may be to slow
	bool useLocalTracking = true;

	// initialize tracker (useLocalTracking, dt(Шаг времени опроса фильтра), 
	// Accel noise mag, dist thres, maximum allowed skipped frames, max trace length)
	CTracker tracker(useLocalTracking, kalmanStep, accelNoise, distThresh, skipFrames, maxTraceLength);

	//array with colors
	cv::Scalar Colors[] = { cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255), cv::Scalar(255, 255, 0), cv::Scalar(0, 255, 255), 
							cv::Scalar(255, 0, 255), cv::Scalar(255, 127, 255), cv::Scalar(127, 0, 255), cv::Scalar(127, 0, 127) };

#if ExampleNum
	//IP camera URLs
	vector<string> capture_source = {
		"rtsp://admin:12345@192.168.1.88:554/MPEG-4/ch12/main/av_stream",
		"rtsp://admin:12345@192.168.1.88:554/MPEG-4/ch13/main/av_stream"
	};

	//Highgui window titles
	vector<string> label;
	for (int i = 0; i < capture_source.size(); i++)
	{
		string title = "CCTV " + to_string(i);
		label.push_back(title);
	}

	//Make an instance of CameraStreamer
	CameraStreamer cam(capture_source);

	while (waitKey(20) != 27)
	{
		//Retrieve frames from each camera capture thread
		for (int i = 0; i < capture_source.size(); i++)
		{
			Mat frame;
			{
				//Show frame on Highgui window
				imshow(label[i], frame);
			}
		}
	}


#else
	// other option
#endif

	cv::destroyAllWindows();
	return 0;
}



//int argc, char** argv

/*X = 100;
Y =100;
width = frame.cols-101;
height = frame.rows-101;*/
//cv::Mat croppedImage = frame(cv::Rect(X, Y, width, height));
//make small windows to see many simulteneously and reduce computing(really??)
//cv::resize(croppedImage, croppedImage, cv::Size(390, 300));

//video name
//std::string inFile("5thFloor.mp4");
//std::string outFile("bungeeOut.mp4");

/*if (argc > 1)
{
inFile = argv[1];
}*/
/*
int X;
int Y;
int width;
int height;
if (argc > 7)
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

/*cv::VideoWriter writer;
int fourcc = cv::VideoWriter::fourcc('M', 'P', '4', '2');*/

/*// resize and make gray
//cv::resize(croppedImage, croppedImage, cv::Size(390, 300));
cv::cvtColor(croppedImage, gray, cv::COLOR_BGR2GRAY);
if (!writer.isOpened())
{
writer.open(outFile, capture.get(cv::CAP_PROP_FOURCC), capture.get(cv::CAP_PROP_FPS), croppedImage.size(), true);
}

//if writer is opened add one more frame
if (writer.isOpened())
{
writer << frame;
}
*/