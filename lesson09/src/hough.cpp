#include "hough.h"

#include <libutils/rasserts.h>

#include <opencv2/imgproc.hpp>

using namespace std;

double toRadians(double degrees)
{
    const double PI = 3.14159265358979323846264338327950288;
    return degrees * PI / 180.0;
}

double estimateR(double x0, double y0, double theta0radians)
{
    double r0 = x0 * cos(theta0radians) + y0 * sin(theta0radians);
    return r0;
}


cv::Mat buildHough(cv::Mat sobel) {
    // проверяем что входная картинка - одноканальная и вещественная:
    rassert(sobel.type() == CV_32FC1, 237128273918006);

    // Эта функция по картинке с силами градиентов (после свертки оператором Собеля) строит пространство Хафа
    // Вы можете либо взять свою реализацию из прошлого задания, либо взять эту заготовку:

    int width = sobel.cols;
    int height = sobel.rows;

    // решаем какое максимальное значение у параметра theta в нашем пространстве прямых
    int max_theta = 360;

    // решаем какое максимальное значение у параметра r в нашем пространстве прямых:
    int max_r = width + height;

    // создаем картинку-аккумулятор, в которой мы будем накапливать суммарные голоса за прямые
    // так же известна как пространство Хафа
    cv::Mat accumulator(max_r, max_theta, CV_32FC1, 0.0f); // зануление аккумулятора через указание значения по умолчанию в конструкторе

    // проходим по всем пикселям нашей картинки (уже свернутой оператором Собеля)
    for (int y0 = 0; y0 < height; ++y0) {
        for (int x0 = 0; x0 < width; ++x0) {
            // смотрим на пиксель с координатами (x0, y0)
            float strength = sobel.at<float>(y0, x0);

            // теперь для текущего пикселя надо найти все возможные прямые которые через него проходят
            // переберем параметр theta по всему возможному диапазону (в градусах):
            for (int theta0 = 0; theta0 + 1 < max_theta; ++theta0) {

                double theta0radians = toRadians(theta0);
                int r0 = (int) round(estimateR(x0, y0, theta0radians)); // оцениваем r0 и округляем его до целого числа
                if (r0 < 0 || r0 >= max_r)
                    continue;

                int theta1 = theta0 + 1;
                double theta0radians1 = toRadians(theta1);
                int r1 = (int) round(estimateR(x0, y0, theta0radians1)); // оцениваем r0 и округляем его до целого числа
                if (r1 < 0 || r1 >= max_r)
                    continue;



                int r_Min = min(r0, r1);
                int r_Max = max(r0, r1);
                int length = r_Max - r_Min;
                double koef = 1;
                for(int r = r_Min; r < r_Max; r++) {
                    accumulator.at<float>(r, theta0) += strength * koef;
                    accumulator.at<float>(r, theta1) += strength * (1 - koef);

                    koef -= 1.0 / length;
                }


                //                        // чтобы проверить не вышли ли мы за пределы картинки-аккумулятора - давайте явно это проверим
                //                        rassert(i >= 0, 237891731289044);
                //                        rassert(i < accumulator.cols, 237891731289045);
                //                        rassert(j >= 0, 237891731289046);
                //                        rassert(j < accumulator.rows, 237891731289047);
                //                        // теперь легко отладить случай выхода за пределы картинки
                //                        // просто поставьте точку остановки внутри rassert:
                //                        // нажмите Ctrl+Shift+N -> rasserts.cpp
                //                        // и поставьте точку остановки на 8 строке: "return line;"
                //
                //                        // и добоавляем в картинку-аккумулятор наш голос с весом strength (взятый из картинки свернутой Собелем)
            }
        }
    }

    return accumulator;
}

std::vector<PolarLineExtremum> findLocalExtremums(cv::Mat houghSpace)
{
    rassert(houghSpace.type() == CV_32FC1, 234827498237080);

    const int max_theta = 360;
    rassert(houghSpace.cols == max_theta, 233892742893082);
    const int max_r = houghSpace.rows;

    std::vector<PolarLineExtremum> winners;

    for (int theta = 0; theta < max_theta; ++theta) {
        for (int r = 1; r < max_r - 1; ++r) {
            bool extremum = true;
            float maxVotes = houghSpace.at<float>(r, theta);
            for (int dr = -1; dr <= 1; dr++) {
                for(int dtheta = -1;  dtheta <= 1; dtheta++) {
                    int ntheta = (theta + dtheta + max_theta) % max_theta;
                    float curVotes = houghSpace.at<float>(r + dr, ntheta);
                    if (curVotes >= maxVotes && (dr != 0 || dtheta != 0)) extremum = false;
                }
            }
            if (extremum) {
                PolarLineExtremum line(theta, r, maxVotes);
                winners.push_back(line);
            }
        }
    }

    return winners;}

std::vector<PolarLineExtremum> filterStrongLines(std::vector<PolarLineExtremum> allLines, double thresholdFromWinner, int width)
{
    std::vector<PolarLineExtremum> strongLines;

    // Эта функция по множеству всех найденных локальных экстремумов (прямых) находит самую популярную прямую
    // и возвращает только вектор из тех прямых, что не сильно ее хуже (набрали хотя бы thresholdFromWinner голосов от победителя, т.е. например половину)

    PolarLineExtremum strongestLine = allLines[0];
    for(PolarLineExtremum line: allLines){
        if(line.votes > strongestLine.votes) strongestLine = line;
    }

    //    std::vector<PolarLineExtremum> differentStrongLines;
    //
    //    for (PolarLineExtremum line: strongLines){
    //        for (PolarLineExtremum anotherLine: strongLines){
    //            if (line.votes != anotherLine.votes && abs(line.theta - anotherLine.theta) <= 5 && abs(line.r - anotherLine.r) <= 0.1 * width){
    //
    //            }
    //        }
    //    }


    for(PolarLineExtremum line: allLines){
        if(strongestLine.votes * thresholdFromWinner <= line.votes){
            strongLines.push_back(line);
        }
    }

    return strongLines;

}

cv::Mat drawCirclesOnExtremumsInHoughSpace(cv::Mat houghSpace, std::vector<PolarLineExtremum> lines, int radius)
{
    // TODO Доделайте эту функцию - пусть она скопирует картинку с пространством Хафа и отметит на ней красным кружком указанного радиуса (radius) места где были обнаружены экстремумы (на базе списка прямых)

    // делаем копию картинки с пространством Хафа (чтобы не портить само пространство Хафа)
    cv::Mat houghSpaceWithCrosses = houghSpace.clone();

    // проверяем что пространство состоит из 32-битных вещественных чисел (т.е. картинка одноканальная)
    rassert(houghSpaceWithCrosses.type() == CV_32FC1, 347823472890137);

    // но мы хотим рисовать КРАСНЫЙ кружочек вокруг найденных экстремумов, а значит нам не подходит черно-белая картинка
    // поэтому ее надо преобразовать в обычную цветную BGR картинку
    cv::cvtColor(houghSpaceWithCrosses, houghSpaceWithCrosses, cv::COLOR_GRAY2BGR);
    // проверяем что теперь все хорошо и картинка трехканальная (но при этом каждый цвет - 32-битное вещественное число)
    rassert(houghSpaceWithCrosses.type() == CV_32FC3, 347823472890148);

    for (auto line : lines) {
        // Пример как рисовать кружок в какой-то точке (закомментируйте его):
        cv::Point point(line.theta, line.r);
        cv::Scalar color(0, 0, 255); // BGR, т.е. красный цвет
        cv::circle(houghSpaceWithCrosses, point, radius, color);

        // TODO отметьте в пространстве Хафа красным кружком радиуса radius экстремум соответствующий прямой line
    }

    return houghSpaceWithCrosses;
}

cv::Point PolarLineExtremum::intersect(PolarLineExtremum that)
{
    // Одна прямая - наш текущий объект (this) у которого был вызван этот метод, у этой прямой такие параметры:
    double theta0 = this->theta;
    double r0 = this->r;

    // Другая прямая - другой объект (that) который был передан в этот метод как аргумент, у этой прямой такие параметры:
    double theta1 = that.theta;
    double r1 = that.r;

    // TODO реализуйте поиск пересечения этих двух прямых, напоминаю что формула прямой описана тут - https://www.polarnick.com/blogs/239/2021/school239_11_2021_2022/2021/11/02/lesson8-hough-transform.html
    // после этого загуглите как искать пересечение двух прямых, пример запроса: "intersect two 2d lines"
    // и не забудьте что cos/sin принимают радианы (используйте toRadians)
    double a1 = cos(toRadians(theta0));
    double a2 = cos(toRadians(theta1));
    double b1 = sin(toRadians(theta0));
    double b2 = sin(toRadians(theta1));
    double c1 = r0;
    double c2 = r1;


    int x = 0;
    int y = 0;
    double determinant = a1 * b2 - a2 * b1;
    if (determinant != 0)
    {
        x = (c1*b2 - c2*b1)/determinant;
        y = (a1*c2 - a2*c1)/determinant;
    }

    return {x, y};
}

// TODO Реализуйте эту функцию - пусть она скопирует картинку и отметит на ней прямые в соответствии со списком прямых
cv::Mat drawLinesOnImage(cv::Mat img, std::vector<PolarLineExtremum> lines)
{
    // делаем копию картинки (чтобы при рисовании не менять саму оригинальную картинку)
    cv::Mat imgWithLines = img.clone();

    // проверяем что картинка черно-белая (мы ведь ее такой сделали ради оператора Собеля) и 8-битная
    rassert(imgWithLines.type() == CV_8UC1, 45728934700167);

    // но мы хотим рисовать КРАСНЫЕ прямые, а значит нам не подходит черно-белая картинка
    // поэтому ее надо преобразовать в обычную цветную BGR картинку с 8 битами в каждом пикселе
    cv::cvtColor(imgWithLines, imgWithLines, cv::COLOR_GRAY2BGR);
    rassert(imgWithLines.type() == CV_8UC3, 45728934700172);

    // выпишем размер картинки
    int width = imgWithLines.cols;
    int height = imgWithLines.rows;

    for (auto line : lines) {
        // нам надо найти точки на краях картинки
        cv::Point pointA;
        cv::Point pointB;

        // TODO создайте четыре прямых соответствующих краям картинки (на бумажке нарисуйте картинку и подумайте какие theta/r должны быть у прямых?):
        // напоминаю - чтобы посмотреть какие аргументы требует функция (или в данном случае конструктор объекта) - нужно:
        // 1) раскомментировать эти четыре строки ниже
        // 2) поставить каретку (указатель где вы вводите новые символы) внутри скобок функции (или конструктора, т.е. там где были три вопроса: ???)
        // 3) нажать Ctrl+P чтобы показать список параметров (P=Parameters)
        PolarLineExtremum leftImageBorder(0, 0, 0);
        PolarLineExtremum bottomImageBorder(90, height, 0);
        PolarLineExtremum rightImageBorder(0, width, 0);
        PolarLineExtremum topImageBorder(90, 0, 0);

        // TODO воспользуйтесь недавно созданной функций поиска пересечения прямых чтобы найти точки пересечения краев картинки:
        if (line.theta >= 45 && line.theta <= 135 || line.theta >= 180 && line.theta <= 270)
        {
            pointA = line.intersect(leftImageBorder);
            pointB = line.intersect(rightImageBorder);
        }
        // TODO а в каких случаях нужно использовать пересечение с верхним и нижним краем картинки?
        if (line.theta > 270 && line.theta <= 360 || line.theta >= 0 && line.theta < 45  || line.theta >= 135 && line.theta <= 180) {
            pointA = line.intersect(topImageBorder);
            pointB = line.intersect(bottomImageBorder);
        }
        // TODO переделайте так чтобы цвет для каждой прямой был случайным (чтобы легче было различать близко расположенные прямые)
        cv::Scalar color(rand() % 256, rand() % 256, rand() % 256);
        cv::line(imgWithLines, pointA, pointB, color);
    }

    return imgWithLines;
}
