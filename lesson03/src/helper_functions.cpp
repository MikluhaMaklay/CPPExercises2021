#include "helper_functions.h"

#include <libutils/rasserts.h>


cv::Mat makeAllBlackPixelsBlue(cv::Mat image) {

    // ниже приведен пример как узнать цвет отдельного пикселя - состоящий из тройки чисел BGR (Blue Green Red)
    // чем больше значение одного из трех чисел - тем насыщеннее его оттенок
    // всего их диапазон значений - от 0 до 255 включительно
    // т.е. один байт, поэтому мы используем ниже тип unsigned char - целое однобайтовое неотрицательное число
    for (int rows = 0; rows < image.rows; rows++){
        for (int cols = 0; cols < image.cols; cols++){
            cv::Vec3b color = image.at<cv::Vec3b>(rows, cols); // взяли и узнали что за цвет в пикселе в 14-ом ряду (т.к. индексация с нуля) и 6-ой колонке
//            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
//            unsigned char green = color[1];
//            unsigned char red = color[2];

            if (color == cv::Vec3b(0, 0, 0)){
                image.at<cv::Vec3b>(rows, cols) = cv::Vec3b(255, 0, 0);
            }
        }
    }

    // как получить белый цвет? как получить черный цвет? как получить желтый цвет?
    // поэкспериментируйте! например можете всю картинку заполнить каким-то одним цветом

    // пример как заменить цвет по тем же координатам
//    red = 255;
    // запустите эту версию функции и посмотрите на получившуюся картинку - lesson03/resultsData/01_blue_unicorn.jpg
    // какой пиксель изменился? почему он не чисто красный?

    return image;
}

cv::Mat invertImageColors(cv::Mat image) {
    // т.е. пусть ночь станет днем, а сумрак рассеется
    // иначе говоря замените каждое значение яркости x на (255-x) (т.к находится в диапазоне от 0 до 255)

    for (int rows = 0; rows < image.rows; rows++){
        for (int cols = 0; cols < image.cols; cols++){
            cv::Vec3b color = image.at<cv::Vec3b>(rows, cols); // взяли и узнали что за цвет в пикселе в 14-ом ряду (т.к. индексация с нуля) и 6-ой колонке
                unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
                unsigned char green = color[1];
                unsigned char red = color[2];

            image.at<cv::Vec3b>(rows, cols) = cv::Vec3b(255 - blue, 255 - green, 255 - red);
        }
    }

    return image;
}

cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background) {
    // TODO реализуйте функцию которая все черные пиксели картинки-объекта заменяет на пиксели с картинки-фона
    // т.е. что-то вроде накладного фона получится

    // гарантируется что размеры картинок совпадают - проверьте это через rassert, вот например сверка ширины:
    rassert(object.cols == background.cols, 341241251251351)
    rassert(object.rows == background.rows, 274825321762864)

    for (int rows = 0; rows < object.rows; rows++){
        for (int cols = 0; cols < object.cols; cols++){
            cv::Vec3b color = object.at<cv::Vec3b>(rows, cols); // взяли и узнали что за цвет в пикселе в 14-ом ряду (т.к. индексация с нуля) и 6-ой колонке

            if (color == cv::Vec3b(0, 0, 0)){
                object.at<cv::Vec3b>(rows, cols) = background.at<cv::Vec3b>(rows, cols);
            }
        }
    }

    return object;
}

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground) {
    // теперь вам гарантируется что largeBackground гораздо больше - добавьте проверок этого инварианта (rassert-ов)

    // TODO реализуйте функцию так, чтобы нарисовался объект ровно по центру на данном фоне, при этом черные пиксели объекта не должны быть нарисованы

    return largeBackground;
}
