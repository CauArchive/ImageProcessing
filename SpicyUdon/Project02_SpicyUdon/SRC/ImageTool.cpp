#include "pch.h"
#include "ImageTool.h"

BITMAPINFO* ImageTool::fillBitmap(int bitsPerPixel, cv::Mat mat)
{
	BITMAPINFO* bitmap = new BITMAPINFO();

	bitmap->bmiHeader.biBitCount = bitsPerPixel;
	bitmap->bmiHeader.biWidth = mat.cols;
	bitmap->bmiHeader.biHeight = -mat.rows;
	bitmap->bmiHeader.biPlanes = 1;
	bitmap->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmap->bmiHeader.biCompression = BI_RGB;
	bitmap->bmiHeader.biClrImportant = 0;
	bitmap->bmiHeader.biClrUsed = 0;
	bitmap->bmiHeader.biSizeImage = 0;
	bitmap->bmiHeader.biXPelsPerMeter = 0;
	bitmap->bmiHeader.biYPelsPerMeter = 0;
	if (bitsPerPixel == 8) {
		RGBQUAD* palette = bitmap->bmiColors;
		for (int i = 0; i < 256; i++)
		{
			palette[i].rgbBlue = (BYTE)i;
			palette[i].rgbGreen = (BYTE)i;
			palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
	return bitmap;
}

void ImageTool::setupInformation(int & border, cv::Mat mat, int bitsPerPixel)
{
	if (bitsPerPixel < 32) {
		border = 4 - (mat.cols % 4);
	}
}

CImage * ImageTool::matToImage(cv::Mat mat, BITMAPINFO* bitmap, cv::Size size, int border)
{
	CImage * image = new CImage();

	image->Create(size.width, size.height, 24);

	if (mat.cols == size.width && mat.rows == size.height) {

		SetDIBitsToDevice(image->GetDC(),
			0, 0, size.width, size.height,
			0, 0, 0, mat.rows,
			mat.data, bitmap, DIB_RGB_COLORS);
	} else {
		int xDest = 0;
		int yDest = 0;
		int destWidth = size.width;
		int destHeight = size.height;
		int xSrc = 0;
		int ySrc = 0;
		int srcWidth = mat.cols - border;
		int srcHeight = mat.rows;

		StretchDIBits(image->GetDC(),
			xDest, yDest, destWidth, destHeight,
			xSrc, ySrc, srcWidth, srcHeight,
			mat.data, bitmap, DIB_RGB_COLORS, SRCCOPY);
	}
	return image;
}

cv::Mat ImageTool::resizeMat(cv::Mat original, int width, int height)
{
	cv::Mat mat;
	if (original.type() == CV_8UC3) {
		mat = cv::Mat(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
	}
	else {
		mat = cv::Mat(height, width, CV_8UC4, cv::Scalar(0, 0, 0, 0));
	}
	float scaleX = float(width) / float(original.size().width);
	float scaleY = float(height) / float(original.size().height);
	for (int index = 0; index < height; index++) {
		for (int idx = 0; idx < width; idx++) {
			int x = int(float(idx) / scaleX);
			int y = int(float(index) / scaleY);
			if (x >= original.cols)
				x = original.cols - 1;
			else if (x < 0)
				x = 0;
			if (y >= original.rows)
				y = original.rows - 1;
			else if (y < 0)
				y = 0;
			if (original.type() == CV_8UC3) {
				cv::Vec3b& color = original.at<cv::Vec3b>(y, x);
				mat.at<cv::Vec3b>(index, idx) = color;
			}
			else {
				cv::Vec4b& color = original.at<cv::Vec4b>(y, x);
				mat.at<cv::Vec4b>(index, idx) = color;
			}
		}
	}
	return mat;
}

std::vector<std::string> ImageTool::split(const std::string &line, const std::string &delimiter)
{
    std::string buffer;
    std::vector<std::string> result;
    bool found = false;

    for (char index : line) {
        found = false;
        for (char idx : delimiter) {
            if (index == idx)
                found = true;
        }
        if (found) {
            result.emplace_back(buffer);
            buffer.clear();
        } else if (!found){
            buffer.push_back(index);
        }
    }
    if (!buffer.empty())
        result.emplace_back(buffer);
    return result;
}
