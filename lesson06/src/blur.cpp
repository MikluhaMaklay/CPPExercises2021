#include "blur.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include <libutils/rasserts.h>

cv::Mat blur(cv::Mat img, double sigma, int radius) {
    cv::Mat weights(2 * radius + 1, 2 * radius + 1, CV_32F);

    for (int rows = 0; rows < weights.rows; rows++){
        for (int cols = 0; cols < weights.cols; cols++){
            weights.at<double>(rows, cols) = G(rows - radius, cols - radius, sigma);
        }
    }

    cv::Mat res = img.clone();

    for (int rows = 0; rows < res.rows; rows++){
        for (int cols = 0; cols < res.cols; cols++){
            cv::Vec3b value(0,0,0);
            double sum = 0;
            for (int i = - radius; i <= radius; i++){
                for(int j = - radius; j <= radius; j++){
                    if (i + rows < 0 || i + rows >= res.rows || j + cols < 0 || j + cols >= res.cols)
                        continue;

                    double weight  = weights.at<double>(i + radius, j + radius);
                    cv::Vec3b current = img.at<cv::Vec3b>(i + rows, j + cols);
                    value = current * weight;
                    sum += weight;
                }
            }
            value /= sum;
            res.at<cv::Vec3b>(rows, cols) = value;
         }
    }

    // https://ru.wikipedia.org/wiki/%D0%A0%D0%B0%D0%B7%D0%BC%D1%8B%D1%82%D0%B8%D0%B5_%D0%BF%D0%BE_%D0%93%D0%B0%D1%83%D1%81%D1%81%D1%83
    return res;
}

double G(int x, int y, double sigma){
    return exp(- (x * x + y * y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
}
