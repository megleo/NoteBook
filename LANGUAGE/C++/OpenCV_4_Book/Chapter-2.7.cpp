#include <iostream>
#include <string>
#include <sstream>

// OpenCV includes
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
using namespace cv;

int main(int argc, char **argv) {
  // Read images
  Mat color = imread("/home/ts/vscode/NoteBook/LANGUAGE/C++/OpenCV_4_Book/imgs/street.jpg");
  Mat gray = imread("/home/ts/vscode/NoteBook/LANGUAGE/C++/OpenCV_4_Book/imgs/street.jpg", IMREAD_GRAYSCALE);
  if (!color.data) // Check for invalid input
  {
    std::cout << "Could not open or find the image." << std::endl;
    return -1;
  }

  // write images
  imwrite("/home/ts/vscode/NoteBook/LANGUAGE/C++/OpenCV_4_Book/imgs/street_gray.jpg", gray);

  Mat screenshot = imread("/home/ts/vscode/NoteBook/LANGUAGE/C++/OpenCV_4_Book/imgs/screenshot.png");
  // get some pixel with opencv function.
  int myRow = screenshot.rows;
  int myCol = screenshot.cols;
  Vec3b pixel = screenshot.at<Vec3b>(myRow * 2, myCol * 2);
  std::cout << "Pixel value (BGR): " << (int) pixel[0] << ", "
            << (int) pixel[1] << ", "
            << (int) pixel[2] << std::endl;

  std::cout << "row: " << myRow << std::endl;
  std::cout << "col: " << myCol << std::endl;

  imshow("color", color);
  imshow("gray", gray);
  waitKey(0);
  return 0;
}
