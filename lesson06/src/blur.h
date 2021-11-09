#pragma once

#include <opencv2/highgui.hpp>

cv::Mat blur(cv::Mat img, double sigma, int radius);
double G(int x, int y, double sigma);
