#pragma once

#include "pch.h"

class ImageTool
{
	public:
		static BITMAPINFO* fillBitmap(int bitsPerPixel, cv::Mat mat);
		static void setupInformation(int& border, cv::Mat mat, int bitsPerPixel);
		static CImage * matToImage(cv::Mat mat, BITMAPINFO* bitmap, cv::Size size, int border);
		static cv::Mat resizeMat(cv::Mat original, int width, int height);
		static std::vector<std::string> split(const std::string& line, const std::string& delimiter);
};

