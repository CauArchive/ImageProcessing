#include <iostream>
#include <string>

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

void setLabel(Mat& image, string str, vector<Point> contour) {
  int fontface = FONT_HERSHEY_SIMPLEX;
  double scale = 0.5;
  int thickness = 1;
  int baseline = 0;
  Size text = getTextSize(str, fontface, scale, thickness, &baseline);
  Rect r = boundingRect(contour);
  Point pt(r.x + ((r.width - text.width) / 2),
           r.y + ((r.height + text.height) / 2));
  rectangle(image, pt + Point(0, baseline),
            pt + Point(text.width, -text.height), CV_RGB(200, 200, 200),
            FILLED);
  putText(image, str, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
}

int main(int, char**) {
  Mat img_input, img_result, img_gray;
  // Image Load
  img_input = imread("1.png", IMREAD_COLOR);
  if (img_input.empty()) {
    cout << "No Image Found\n";
    return -1;
  }
  // Convert Image to GrayScale
  cvtColor(img_input, img_gray, COLOR_BGR2GRAY);
  // Convert Image to Binary Image
  Mat binary_image;
  threshold(img_gray, img_gray, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
  // Find Contours of Images
  vector<vector<Point> > contours;
  findContours(img_gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
  // Approximate Contours
  vector<Point2f> approx;
  img_result = img_input.clone();
  for (size_t i = 0; i < contours.size(); i++) {
    approxPolyDP(Mat(contours[i]), approx,
                 arcLength(Mat(contours[i]), true) * 0.02, true);
    if (fabs(contourArea(Mat(approx))) > 100)  // Detect Image with Area > 100
    {
      int size = approx.size();
      // Decide whether the contour is a triangle or a rectangle
      if (size % 2 == 0) {
        line(img_result, approx[0], approx[approx.size() - 1],
             Scalar(0, 255, 0), 3);
        for (int k = 0; k < size - 1; k++)
          line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);
        for (int k = 0; k < size; k++)
          circle(img_result, approx[k], 3, Scalar(0, 0, 255));
      } else {
        line(img_result, approx[0], approx[approx.size() - 1],
             Scalar(0, 255, 0), 3);
        for (int k = 0; k < size - 1; k++)
          line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);

        for (int k = 0; k < size; k++)
          circle(img_result, approx[k], 3, Scalar(0, 0, 255));
      }
      // If approximated Contour size is over 3, it is a triangle
      if (size == 3) setLabel(img_result, "triangle", contours[i]);
      // If approximated Contour size is over 4, it is a rectangle
      else if (size == 4 && isContourConvex(Mat(approx)))
        setLabel(img_result, "rectangle", contours[i]);
      // If approximated Contour size is over 5, it is a pentagon
      else if (size == 5 && isContourConvex(Mat(approx)))
        setLabel(img_result, "pentagon", contours[i]);
      // If approximated Contour size is over 6, it is a hexagon
      else if (size == 6 && isContourConvex(Mat(approx)))
        setLabel(img_result, "hexagon", contours[i]);
      // Else Just Show the Num of Edges
      else
        setLabel(img_result, to_string(approx.size()), contours[i]);
    }
  }
  imshow("input", img_input);
  imshow("result", img_result);
  waitKey(0);
  return 0;
}
