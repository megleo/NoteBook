//
// Created by ts on 23-7-28.
//

#include <vector>
#include <iostream>
#include <string>
#include <sstream>

// OpenCV includes
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"

// CommandLineParser类
// Opencv command line parser functions
// Keys accepted by command line parser

const char* keys =
    {
    "{help h usage ? || print this message}"
    "{@video || Video file, if not defined try to use webcamera}"
    };

int main(int argc, char** argv)
{
    cv::CommandLineParser parser(argc, argv, keys);
    parser.about("Chater 2. v1.0.0");

    if (parser.has("help"))
    {
      parser.printMessage();
      return 0;
    }

    auto videoFile = parser.get<cv::String>(0);

    // Check if params are correctly parsed in his variables.
  if (!parser.check())
  {
    parser.printErrors();
    return 0;
  }

  cv::VideoCapture cap; // open the default camera
  if (videoFile != "")
    cap.open(videoFile);
  else
    cap.open(0);
  if (!cap.isOpened())
    return -1;

  cv::namedWindow("Video", 1);
  for (;;) {
    cv::Mat frame;
    cap >> frame; // get a new frame from camera
    if (frame.empty())
      return 0;
    cv::imshow("Video", frame);
    if (cv::waitKey(30) >= 0)
      break;
  }
  cap.release();
}


