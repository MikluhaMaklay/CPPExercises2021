#include <iostream>
#include <filesystem>
#include <libutils/rasserts.h>

#include "disjoint_set.h"
#include "morphology.h"

#include <opencv2/highgui.hpp>

// чтобы их потестировать - постарайтесь дописать сюда много разных интересных случаев:
void testingMyDisjointSets() {
    DisjointSet set(5);
    rassert(set.count_differents() == 5, 2378923791);
    for (int element = 0; element < 5; ++element) {
        rassert(set.get_set(element) == element, 23892803643);
        rassert(set.get_set_size(element) == 1, 238928031);

    }

    rassert(set.union_sets(1, 2) == 1, 23872936)
    rassert(set.union_sets(1, 3) == 1, 294730247)
    rassert(set.get_set(3) == 1, 395862397)
    rassert(set.count_differents() == 3, 234349)
    rassert(set.get_set_size(1) == 3, 2638754)
}

struct MyVideoContent {
    cv::Mat frame;
    std::vector<std::vector<int>> clicks = {{}, {}};
    bool isInverted = false;
    cv::Vec3b currentColor;
    cv::Mat background;
};

cv::Mat largeCastleUpscale(cv::Mat image, MyVideoContent content){
    cv::Mat newImage(content.frame.rows, content.frame.cols, CV_8UC3, cv::Scalar(256,56,0));

    std::cout << content.frame.rows << " " << content.frame.cols;
    rassert(!newImage.empty(), 3829694326)

    for (int rows = 0; rows < newImage.rows; rows++){
        for (int cols = 0; cols < newImage.cols; cols++){
            int y = rows*1.0 / newImage.rows * image.rows;
            int x = cols*1.0 / newImage.cols * image.cols;

            newImage.at<cv::Vec3b>(rows, cols) = image.at<cv::Vec3b>(y, x);
        }
    }

    return newImage;
}

// TODO 200 перенесите сюда основную часть кода из прошлого задания про вычитание фона по первому кадру, но:
// 1) добавьте сохранение на диск визуализации картинок:
// 1.1) картинка эталонного фона
// 1.2) картинка текущего кадра
// 1.3) картинка визуализирующая маску "похож ли пиксель текущего кадра на эталонный фон"
// 1.4) картинка визуализирующая эту маску после применения волшебства морфологии
// 1.5) картинка визуализирующая эту маску после применения волшебства СНМ (системы непересекающихся множеств)
// 2) сохраняйте эти картинки визуализации только для тех кадров, когда пользователем был нажат пробел (код 32)
// 3) попробуйте добавить с помощью нажатия каких то двух кнопок "усиление/ослабление подавления фона"
// 4) попробуйте поменять местами морфологию и СНМ
// 5) попробуйте добавить настройку параметров морфологии и СНМ по нажатию кнопок (и выводите их значения в консоль)
void backgroundMagickStreaming() {

    cv::VideoCapture video(1);

    rassert(video.isOpened(), 3423948392481)

    MyVideoContent content;

    std::string resultsDir = "lesson04/resultsData/";
    if (!std::filesystem::exists(resultsDir)) { // если папка еще не создана
        std::filesystem::create_directory(resultsDir); // то создаем ее
    }

    cv::Mat castle = cv::imread("lesson03/data/castle_large.jpg");

    rassert(!castle.empty(), 3573290865)

    video.read(content.frame);

    cv::Mat largeCastle = largeCastleUpscale(castle, content);

    rassert(!largeCastle.empty(), 8237489369265)

    video.read(content.background);

    std::string filename = resultsDir + "background";
    cv::imwrite(filename, content.background);

    while(video.isOpened()){
        bool isSuccess = video.read(content.frame);
        rassert(isSuccess, 348792347819);
        rassert(!content.frame.empty(), 3452314124643)

        makeBackgroundDisappear(content.background, largeCastle, content);

        cv::imshow("video", content.frame);

        int key = cv::waitKey(10);
        if (key == 32 || key == 27) break;
    }



}

int main() {
    try {
//        testingMyDisjointSets();
        backgroundMagickStreaming();
        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}
