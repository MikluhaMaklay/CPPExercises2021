#include "line.h"

#include <libutils/rasserts.h>

#include <opencv2/imgproc.hpp>

#include <random>

double Line::getDist(const cv::Point2f& p) const{
    return pow(a*p.x + b*p.y + c, 2)/(a*a + b*b);
}

double Line::getYFromX(double x)
{
    rassert(b != 0.0, 2734832748932790061); // случай вертикальной прямой не рассматривается для простоты

    // TODO 01
    double y = - (a * x + c) / b;

    return y;
}

std::vector<cv::Point2f> Line::generatePoints(int n,
                                              double fromX, double toX,
                                              double gaussianNoiseSigma)
{
    std::vector<cv::Point2f> points;

    // пусть зерно случайности порождающее последовательность координат будет однозначно опредляться по числу точек
    unsigned int randomSeed = n;
    std::mt19937 randomGenerator(randomSeed); // это генератор случайных чисел (см. https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine )

    // TODO 01 доделайте этот метод:
    //  - поправьте в коде ниже количество точек которые создадутся
    //  - диапазон x в котором создаются точки
    for (int i = 0; i < n; ++i) {
        // это правило генерации случайных чисел - указание какие мы хотим координаты x - равномерно распределенные в диапазоне от fromX  до toX
        std::uniform_real_distribution<> xDistribution(fromX, toX);

        double x = xDistribution(randomGenerator);

        // найдем идеальную координату y для данной координаты x:
        double idealY = getYFromX(x); // TODO 01 воспользуйтесь методом getYFromX (сначала его надо доделать)

        // указание какую мы хотим координату y - распределенную около idealY в соответствии с распределением Гаусса (т.н. нормальное распределение)
        std::normal_distribution<> yDistribution(idealY, gaussianNoiseSigma);
        double y = yDistribution(randomGenerator);

        points.push_back(cv::Point2f(x, y));
    }

    return points;
}

// эта функция рисует на картинке указанные точки
// при этом если картинка пустая - эта функция должна увеличить картинку до размера в который впишутся все точки
// TODO 02 поправьте в этой функции цвет которым рисуются точки (нужно использовать аргумент color)
void plotPoints(cv::Mat &img, std::vector<cv::Point2f> points, double scale, cv::Scalar color)
{
    rassert(points.size() > 0, 347238947320012);

    if (img.empty()) {
        // если картинка пустая - нужно увеличить картинку до размера в который впишутся все точки
        float maxX = 0.0f;
        float maxY = 0.0f;
        for (int i = 0; i < points.size(); ++i) {
            maxX = std::max(maxX, points[i].x);
            maxY = std::max(maxY, points[i].y);
        }
        cv::Scalar black(0, 0, 0);
        // увеличим на 10% размер картинки чтобы точки были не совсем на краю
        maxX *= 1.1;
        maxY *= 1.1;
        // создаем картинку нужного размера (и сразу указываем что она заполнена черным)
        int nrows = (int) (maxY * scale);
        int ncols = (int) (maxX * scale);
        img = cv::Mat(nrows, ncols, CV_8UC3, black);

        // рисуем текст для указания координат в углах картинки
        cv::Scalar white(255, 255, 255);
        float textHeight = cv::getTextSize("0;0", cv::FONT_HERSHEY_DUPLEX, 1.0, 1, nullptr).height;
        cv::putText(img, "0;0", cv::Point(0, textHeight), cv::FONT_HERSHEY_DUPLEX, 1.0, white);

        cv::putText(img, "0;" + std::to_string(maxY), cv::Point(0, nrows - 5), cv::FONT_HERSHEY_DUPLEX, 1.0, white);

        std::string textTopRight = std::to_string(maxX) + ";0";
        float textWidth = cv::getTextSize(textTopRight, cv::FONT_HERSHEY_DUPLEX, 1.0, 1, nullptr).width;
        cv::putText(img, textTopRight, cv::Point(ncols-textWidth, textHeight), cv::FONT_HERSHEY_DUPLEX, 1.0, white);
    } else {
        rassert(img.type() == CV_8UC3, 34237849200017);
    }

    for (int i = 0; i < points.size(); ++i) {
        // TODO 02 и обратите внимание что делает scale (он указывает масштаб графика)
        cv::circle(img, points[i] * scale, 5, color, 2);
    }
}

// метод прямой позволяющий нарисовать ее на картинке (т.е. на простом графике)
void Line::plot(cv::Mat &img, double scale, cv::Scalar color)
{
    rassert(!img.empty(), 3478342937820055);
    rassert(img.type() == CV_8UC3, 34237849200055);

    // TODO 03 реализуйте отрисовку прямой (воспользуйтесь getYFromX и cv::line(img, cv::Point(...), cv::Point(...), color)), будьте осторожны и не забудьте учесть scale!
    cv::line(img, cv::Point(0, getYFromX(0) * scale), cv::Point(img.cols, getYFromX(img.cols / scale) * scale), color);
}

Line fitLineFromTwoPoints(cv::Point2f a, cv::Point2f b)
{
    if(a.x == b.x)
        return Line(4,5,5);
    rassert(a.x != b.x, 23892813901800104); // для упрощения можно считать что у нас не бывает вертикальной прямой


    // (x - x1) * (y2 - y1) - (y - y1) * (x2 - x1) = 0
    // (y2 - y1) = a
    // - (x2 - x1) = b
    // c = - x1 * y2 + y1 * x2


    // TODO 04 реализуйте построение прямой по двум точкам
    return Line(b.y - a.y, - b.x + a.x, - a.x * b.y + a.y * b.x);
}

Line fitLineFromNPoints(std::vector<cv::Point2f> points)
{

    double minDist = DBL_MAX;
    Line resultLine = fitLineFromTwoPoints(points[0], points[1]);
    for(int i = 0; i < points.size(); i++){
        for(int j = 0; j < points.size(); j++){
            if(points[i] == points[j])
                continue;
            Line line = fitLineFromTwoPoints(points[i], points[j]);
            double dist = 0;
            for(auto & point : points){
                dist += line.getDist(point);
            }
            if(minDist > dist) {
                minDist = dist;
                resultLine = line;
            }
        }
    }
    // TODO 05 реализуйте построение прямой по многим точкам (такое чтобы прямая как можно лучше учитывала все точки)
    return resultLine;
}

Line fitLineFromNNoisyPoints(std::vector<cv::Point2f> points, int k)
{
    int rad = 10;
    int bestVotes = 0;
    Line bestLine = fitLineFromTwoPoints(points[0], points[1]);
    for(int i = 0; i < pow(1 / k, 2); i++){
        cv::Point a;
        cv::Point b = a;
        while(a == b) {
            int aInd = rand() % points.size();
            int bInd = rand() % points.size();
            a = points[aInd];
            b = points[bInd];
        }
        Line line = fitLineFromTwoPoints(a, b);
        int votes = 0;
        for(auto point: points){
            if (line.getDist(point) <= rad){
                votes++;
            }
        }
        if(bestVotes < votes){
            bestVotes = votes;
            bestLine = line;
        }

    }
    // TODO 06 БОНУС - реализуйте построение прямой по многим точкам включающим нерелевантные (такое чтобы прямая как можно лучше учитывала НАИБОЛЬШЕЕ число точек)
    return bestLine;
}

std::vector<cv::Point2f> generateRandomPoints(int n,
                                              double fromX, double toX,
                                              double fromY, double toY)
{
    std::vector<cv::Point2f> points;

    // пусть зерно случайности порождающее последовательность координат будет однозначно опредляться по числу точек
    unsigned int randomSeed = n;
    std::mt19937 randomGenerator(randomSeed); // это генератор случайных чисел (см. https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine )

    for (int i = 0; i < n; ++i) {
        // это правило генерации случайных чисел - указание какие мы хотим координаты x - равномерно распределенные в диапазоне от fromX  до toX
        std::uniform_real_distribution<> xDistribution(fromX, toX);
        std::uniform_real_distribution<> yDistribution(fromY, toY);

        double x = xDistribution(randomGenerator);
        double y = yDistribution(randomGenerator);

        points.push_back(cv::Point2f(x, y));
    }

    return points;
}

std::ostream& operator << (std::ostream& os, const Line& line)
{
    os << line.a << "*x + " << line.b << "*y + " << line.c << " = 0";
    return os;
}
