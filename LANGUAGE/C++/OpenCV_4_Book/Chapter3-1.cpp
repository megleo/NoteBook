//
// Created by ts on 23-7-31.
//
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// Opencv include
#include <opencv2/core.hpp>
#include "opencv2/highgui.hpp"

using namespace cv;

int main(int argc, char** argv)
{
  // Read images
  Mat lena = imread("/home/ts/vscode/NoteBook/LANGUAGE/C++/OpenCV_4_Book/imgs/lena.jpg");
  // checking if lena image has been loaded/
  if (!lena.data) {
    cout << "Lena images missing." << endl;
    return -1;
  }

  Mat photo = imread("/home/ts/vscode/NoteBook/LANGUAGE/C++/OpenCV_4_Book/imgs/photo.jpg");
  // checking if lena image has been loaded/
  if (!photo.data) {
    cout << "photo images missing." << endl;
    return -1;
  }

  // Create windows
  namedWindow("Lena", WINDOW_NORMAL);
  namedWindow("photo", WINDOW_GUI_EXPANDED);

  moveWindow("Lena", 10, 10);
  moveWindow("photo", 520, 10);
  imshow("Lena", lena);
  imshow("photo", photo);

  resizeWindow("Lena", 512, 512);
  waitKey(0);
  // Destory the windows
  destroyWindow("Lena");
  destroyWindow("photo");

// create 10 windows
  for (int i = 0; i < 10; ++i) {
    ostringstream ss;
    ss << "photo" << i;
    namedWindow(ss.str());
    moveWindow(ss.str(), 20 *i, 20*i);
    imshow(ss.str(), photo);
    waitKey(600);
  }
  waitKey(0);
  destroyAllWindows();
  return 0;
}