#include <opencv2/imgproc.hpp>
#include <iostream>
#include <libutils/rasserts.h>
#include "parseSymbols.h"

cv::Scalar randColors() {
    return cv::Scalar(128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128); // можно было бы брать по модулю 255, но так цвета будут светлее и контрастнее
}

cv::Mat drawContourss(int rows, int cols, std::vector<std::vector<cv::Point>> contoursPoints) {

    // TODO 06 реализуйте функцию которая покажет вам как выглядят найденные контура:

    // создаем пустую черную картинку
    cv::Mat blackImage(rows, cols, CV_8UC3, cv::Scalar(0, 0, 0));
    // теперь мы на ней хотим нарисовать контуры
    cv::Mat imageWithContoursPoints = blackImage.clone();
    for (int contourI = 0; contourI < contoursPoints.size(); ++contourI) {
        // сейчас мы смотрим на контур номер contourI

        cv::Scalar contourColor = randColors(); // выберем для него случайный цвет
        std::vector<cv::Point> points = contoursPoints[contourI]; // TODO 06 вытащите вектор из точек-пикселей соответствующих текущему контуру который мы хотим нарисовать
        for (int i = 0; i < points.size(); ++i) { // TODO 06 пробегите по всем точкам-пикселям этого контура
            cv::Point point = points[i]; // TODO 06 и взяв очередную точку-пиксель - нарисуйте выбранный цвет в этом пикселе картинки:
            imageWithContoursPoints.at<cv::Vec3b>(point.y, point.x) = cv::Vec3b(contourColor[0], contourColor[1], contourColor[2]);
        }

    }

    return imageWithContoursPoints;
}

std::vector<cv::Mat> splitSymbols(cv::Mat img)
{

    std::vector<cv::Rect> boxes;

    // преобразуем в черно-белый цвет и тоже сразу сохраняем
    cv::Mat img2;
    rassert(!img.empty(), 456537647875876896);
    cv::cvtColor(img, img2, cv::COLOR_BGR2GRAY);

    cv::Mat binary;
    cv::threshold(img2, binary, 128, 255, cv::THRESH_BINARY_INV);

    cv::adaptiveThreshold(img2, binary, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 51, 10);

    cv::Mat binary_eroded;
    cv::erode(binary, binary_eroded, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1)));


    // TODO 04 дальше работаем с картинкой после морфологичесокго рашсирения или морфологического сжатия - на ваш выбор, подумайте и посмотрите на картинки
    binary = binary_eroded;

    // TODO 05
    std::vector<std::vector<cv::Point>> contoursPoints; // по сути это вектор, где каждый элемент - это одна связная компонента-контур,
    // а что такое компонента-контур? это вектор из точек (из пикселей)
    cv::findContours(binary, contoursPoints, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE); // TODO подумайте, какие нужны два последних параметра? прочитайте документацию, после реализации отрисовки контура - поиграйте с этими параметрами чтобы посмотреть как меняется результат
    std::cout << "Contours: " << contoursPoints.size() << std::endl;
    cv::Mat imageWithContoursPoints = drawContourss(img2.rows, img2.cols, contoursPoints); // TODO 06 реализуйте функцию которая покажет вам как выглядят найденные контура

    std::vector<std::vector<cv::Point>> contoursPoints2;
    cv::findContours(binary, contoursPoints2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    // TODO:
    // Обратите внимание на кромку картинки - она всё победила, т.к. черное - это ноль - пустота, а белое - это 255 - сам объект интереса
    // как перевернуть ситуацию чтобы периметр не был засчитан как контур?
    // когда подумаете - замрите! и прежде чем кодить:
    // Посмотрите в документации у функций cv::threshold и cv::adaptiveThreshold
    // про некоего cv::THRESH_BINARY_INV, чем он отличается от cv::THRESH_BINARY?
    // Посмотрите как изменились все картинки.
    std::cout << "Contours2: " << contoursPoints2.size() << std::endl;
    cv::Mat imageWithContoursPoints2 = drawContourss(img.rows, img.cols, contoursPoints2);

    // TODO 06 наконец давайте посмотрим какие буковки нашлись - обрамим их прямоугольниками
    cv::Mat imgWithBoxes = img.clone();
    for (int contourI = 0; contourI < contoursPoints.size(); ++contourI) {
        std::vector<cv::Point> points = contoursPoints[contourI]; // перем очередной контур
        cv::Rect box = cv::boundingRect(points); // строим прямоугольник по всем пикселям контура (bounding box = бокс ограничивающий объект)
        boxes.push_back(box);
        cv::Scalar blackColor(0, 0, 0);
        // TODO прочитайте документацию cv::rectangle чтобы нарисовать прямоугольник box с толщиной 2 и черным цветом (обратите внимание какие есть поля у box)
        cv::rectangle(imgWithBoxes, box.tl(), box.br(), blackColor, 2);
    }
    // например если нет прямоугольников - посмотрите в верхний левый пиксель - белый ли он?
    // если не белый, то что это значит? почему так? сколько в целом нашлось связных компонент?


    std::vector<cv::Mat> symbols;
    for(const auto& box: boxes) {
        cv::Size size(box.width, box.height);
        cv::Point2f center(box.x + box.width / 2, box.y + box.height / 2);
        cv::Mat dst;
        cv::getRectSubPix(img, size, center, dst, CV_8U);
        symbols.push_back(dst);
    }
    // TODO 101: чтобы извлечь кусок картинки (для каждого прямоугольника cv::Rect вокруг символа) - загуглите "opencv how to extract subimage"
    return symbols;
}
