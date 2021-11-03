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
  // Image read
  img_input = imread("1.png", IMREAD_COLOR);
  if (img_input.empty()) {
    cout << "이미지가 없습니다\n";
    return -1;
  }
  //이미지를 grayscale로 변환
  cvtColor(img_input, img_gray, COLOR_BGR2GRAY);
  //바이너리 이미지로 변환
  Mat binary_image;
  threshold(img_gray, img_gray, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
  //이미지의 Contour를 찾습니다.
  vector<vector<Point> > contours;
  findContours(img_gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
  //찾은 Contour를 근사화합니다.
  vector<Point2f> approx;
  img_result = img_input.clone();
  for (size_t i = 0; i < contours.size(); i++) {
    approxPolyDP(Mat(contours[i]), approx,
                 arcLength(Mat(contours[i]), true) * 0.02, true);
    if (fabs(contourArea(Mat(approx))) >
        100)  //이미지의 면적인 특정 크기보다 커야 감지합니다
    {
      int size = approx.size();
      // Contour를 근사화한 결과를 이용해서 사각형인지 직사각형인지 판단합니다.
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

      //근사화한 Contour의 크기가 3이면 삼각형입니다
      if (size == 3) setLabel(img_result, "triangle", contours[i]);
      //근사화한 Contour의 크기가 4이면 사각형입니다
      else if (size == 4 && isContourConvex(Mat(approx)))
        setLabel(img_result, "rectangle", contours[i]);
      //근사화한 Contour의 크기가 5이면 오각형입니다
      else if (size == 5 && isContourConvex(Mat(approx)))
        setLabel(img_result, "pentagon", contours[i]);
      //근사화한 Contour의 크기가 6이면 육각형입니다
      else if (size == 6 && isContourConvex(Mat(approx)))
        setLabel(img_result, "hexagon", contours[i]);
      else if (size == 10 && isContourConvex(Mat(approx)))
        setLabel(img_result, "decagon", contours[i]);
      else
        setLabel(img_result, to_string(approx.size()), contours[i]);
    }
  }

  imshow("input", img_input);
  imshow("result", img_result);

  waitKey(0);

  return 0;
}
