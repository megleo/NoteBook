//
// Created by ts on 23-7-31.
//

#include "opencv2/opencv.hpp"
#include "iostream"
using namespace cv;

int main(int, char** argv)
{
  // create our writer
  FileStorage fs("/home/ts/vscode/NoteBook/LANGUAGE/C++/OpenCV_4_Book/test.yml", FileStorage::WRITE);
  // Save an int
  int fps = 5;
  fs << "fps" << fps;
  // create some example
  Mat m1 = Mat::eye(2, 3, CV_32F);
  Mat m2 = Mat::ones(3, 2, CV_32F);
  Mat result = (m1 + 1).mul(m1+3);
  // Write the result
  fs << "Result " << result;
  // Release the result
  fs.release();

  FileStorage fs2("/home/ts/vscode/NoteBook/LANGUAGE/C++/OpenCV_4_Book/test.yml", FileStorage::READ);
  Mat r;
  fs2["Result"] >> r;
  std::cout << r << std::endl;
  fs2.release();
  return 0;
}