#include <opencv2/imgproc.hpp>
#include "parseSymbols.h"

std::vector<cv::Mat> splitSymbols(cv::Mat img, std::vector<cv::Rect> boxes)
{
    std::vector<cv::Mat> symbols;
    for(auto box: boxes) {
        cv::Size size(box.width, box.height);
        cv::Point2f center(box.x + box.width / 2, box.y + box.height / 2);
        cv::Mat dst;
        cv::getRectSubPix(img, size, center, dst, CV_8U);
        symbols.push_back(dst);
    }
    // TODO 101: чтобы извлечь кусок картинки (для каждого прямоугольника cv::Rect вокруг символа) - загуглите "opencv how to extract subimage"
    return symbols;
}
