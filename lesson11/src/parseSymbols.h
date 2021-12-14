#pragma once

#include <opencv2/highgui.hpp>

#include <vector>

std::vector<cv::Mat> splitSymbols(cv::Mat img, std::vector<cv::Rect> boxes);
