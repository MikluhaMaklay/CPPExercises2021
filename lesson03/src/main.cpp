#include <iostream>
#include <filesystem> // это нам понадобится чтобы создать папку для результатов
#include <libutils/rasserts.h>

#include "helper_functions.h"

#include <opencv2/highgui.hpp> // подключили часть библиотеки OpenCV, теперь мы можем читать и сохранять картинки

void task1() {
    cv::Mat imgUnicorn = cv::imread("lesson03/data/unicorn.png");  // загружаем картинку с единорогом
    rassert(!imgUnicorn.empty(), 3428374817241); // проверяем что картинка загрузилась (что она не пустая)

    int width = imgUnicorn.cols; // как в ООП - у картинки есть поля доступные через точку, они называются cols и rows - попробуйте их
    int height = imgUnicorn.rows;
    std::cout << "Unicorn image loaded: " << width << "x" << height << std::endl;

    // создаем папку в которую будем сохранять результаты - lesson03/resultsData/
    std::string resultsDir = "lesson03/resultsData/";
    if (!std::filesystem::exists(resultsDir)) { // если папка еще не создана
        std::filesystem::create_directory(resultsDir); // то создаем ее
    }

    cv::Mat blueUnicorn = makeAllBlackPixelsBlue(imgUnicorn.clone());
    std::string filename = resultsDir + "01_blue_unicorn.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename, blueUnicorn);

    cv::Mat invertedUnicorn = invertImageColors(imgUnicorn.clone());
    filename = resultsDir + "02_inv_unicorn.jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename, invertedUnicorn);

    cv::Mat castle = cv::imread("lesson03/data/castle.png");
    rassert(!castle.empty(), 232637253725)
    cv::Mat unicornInCastle = addBackgroundInsteadOfBlackPixels(imgUnicorn.clone(), castle.clone());
    filename = resultsDir + "03_unicorn_castle.jpg";
    cv::imwrite(filename, unicornInCastle);

    cv::Mat largeCastle = cv::imread("lesson03/data/castle_large.jpg");
    rassert(!largeCastle.empty(), 3947826395)
    cv::Mat unicornInLargeCastle = addBackgroundInsteadOfBlackPixelsLargeBackground(imgUnicorn.clone(), largeCastle.clone());
    filename = resultsDir + "04_unicorn_large_castle.jpg";
    cv::imwrite(filename, unicornInLargeCastle);

    cv::Mat lotsOfUnicorns = drawManyTimes(imgUnicorn.clone(), largeCastle.clone(), 1);
    filename = resultsDir + "05_unicorns_otake.jpg";
    cv::imwrite(filename, lotsOfUnicorns);

    cv::Mat largeUnicorn = unicornUpscale(imgUnicorn.clone(), largeCastle.clone());
    filename = resultsDir + "06_unicorn_upscale.jpg";
    cv::imwrite(filename, largeUnicorn);
}

void task2() {
    cv::Mat imgUnicorn = cv::imread("lesson03/data/unicorn.png");
    rassert(!imgUnicorn.empty(), 3428374817241);

//     cv::waitKey - функция некоторое время ждет не будет ли нажата кнопка клавиатуры, если да - то возвращает ее код
    int updateDelay = 10; // указываем сколько времени ждать нажатия кнопки клавиатуры - в миллисекундах
    while (cv::waitKey(updateDelay) != 32) {
        // поэтому если мы выполняемся до тех пор пока эта функция не вернет код 32 (а это код кнопки "пробел"), то достаточно будет нажать на пробел чтобы закончить работу программы

        // кроме сохранения картинок на диск (что часто гораздо удобнее конечно, т.к. между ними легко переключаться)
        // иногда удобно рисовать картинку в окне:

        cv::imshow("lesson03 window", epilepsy(imgUnicorn.clone()));
    }
}

struct MyVideoContent {
    cv::Mat frame;
    std::vector<std::vector<int>> clicks = {{}, {}};
    bool isInverted = false;
    cv::Vec3b currentColor;
    cv::Mat background;
};

void onMouseClick(int event, int x, int y, int flags, void *pointerToMyVideoContent) {
    MyVideoContent &content = *((MyVideoContent*) pointerToMyVideoContent);
    // не обращайте внимание на предыдущую строку, главное что важно заметить:
    // content.frame - доступ к тому кадру что был только что отображен на экране
    // content.lastClickX - переменная которая вам тоже наверняка пригодится
    // вы можете добавить своих переменных в структурку выше (считайте что это описание объекта из ООП, т.к. почти полноценный класс)

    if (event == cv::EVENT_LBUTTONDOWN) { // если нажата левая кнопка мыши
        std::cout << "Left click at x=" << x << ", y=" << y << std::endl;
//        content.clicks[0].push_back(x);
//        content.clicks[1].push_back(y);

        content.currentColor = content.frame.at<cv::Vec3b>(y, x);
    }
    if (event == cv::EVENT_RBUTTONDOWN){
        content.isInverted = !content.isInverted;
    }
}

cv::Mat makeAllClickedPixelsRed(MyVideoContent content){
    for (int i = 0; i < content.clicks[0].size(); i++) {
        content.frame.at<cv::Vec3b>(content.clicks[1][i], content.clicks[0][i]) = cv::Vec3b(0, 0, 255);
    }
    return content.frame;
}

cv::Mat makeGreenScreenWithClickedColor(MyVideoContent content, cv::Mat image){
    for (int rows = 0; rows < content.frame.rows; rows++){
        for(int cols = 0; cols < content.frame.cols; cols++){
            unsigned char red = content.currentColor[2];
            unsigned char green = content.currentColor[1];
            unsigned char blue = content.currentColor[0];

            cv::Vec3b frameColor = content.frame.at<cv::Vec3b>(rows,cols);

            unsigned char frameRed = frameColor[2];
            unsigned char frameGreen = frameColor[1];
            unsigned char frameBlue = frameColor[0];

            int differenceBetweenColors = 50;

            if (abs(frameRed - red) < differenceBetweenColors && abs(frameBlue - blue) < differenceBetweenColors && abs(frameGreen - green) < differenceBetweenColors){
                content.frame.at<cv::Vec3b>(rows, cols) = image.at<cv::Vec3b>(rows , cols);
            }
        }
    }

    return content.frame;
}

void task3() {
    // давайте теперь вместо картинок подключим видеопоток с веб камеры:
    cv::VideoCapture video(1);
    // если у вас нет вебкамеры - подключите ваш телефон к компьютеру как вебкамеру - это должно быть не сложно (загуглите)
    // альтернативно если у вас совсем нет вебки - то попробуйте запустить с видеофайла, но у меня не заработало - из-за "there is API version mismath: plugin API level (0) != OpenCV API level (1)"
    // скачайте какое-нибудь видео с https://www.videezy.com/free-video/chroma-key
    // например https://www.videezy.com/elements-and-effects/5594-interactive-hand-gesture-sliding-finger-studio-green-screen
    // если вы увидите кучу ошибок в консоли навроде "DynamicLib::libraryLoad load opencv_videoio_ffmpeg451_64.dll => FAILED", то скопируйте файл C:\...\opencv\build\x64\vc14\bin\opencv_videoio_ffmpeg451_64.dll в папку с проектом
    // и укажите путь к этому видео тут:
//    cv::VideoCapture video("lesson03/data/Spin_1.mp4");


    rassert(video.isOpened(), 3423948392481); // проверяем что видео получилось открыть

    MyVideoContent content; // здесь мы будем хранить всякие полезности - например последний видео кадр, координаты последнего клика и т.п.
    // content.frame - доступ к тому кадру что был только что отображен на экране
    // content.lastClickX - переменная которая вам тоже наверняка пригодится
    // вы можете добавить своих переменных в структурку выше (считайте что это описание объекта из ООП, т.к. почти полноценный класс)
    // просто перейдите к ее объявлению - удерживая Ctrl сделайте клик левой кнопкой мыши по MyVideoContent - и вас телепортирует к ее определению

    while (video.isOpened()) { // пока видео не закрылось - бежим по нему
        bool isSuccess = video.read(content.frame); // считываем из видео очередной кадр
        rassert(isSuccess, 348792347819); // проверяем что считывание прошло успешно
        rassert(!content.frame.empty(), 3452314124643); // проверяем что кадр не пустой

        content.frame = makeAllClickedPixelsRed( content);
        if (content.isInverted) content.frame = invertImageColors(content.frame);

        cv::imshow("video", content.frame); // покаызваем очередной кадр в окошке
        cv::setMouseCallback("video", onMouseClick, &content); // делаем так чтобы функция выше (onMouseClick) получала оповещение при каждом клике мышкой

        int key = cv::waitKey(10);
        if (key == 32 || key == 27) break;
    }
}

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

cv::Mat makeBackgroundDisappear(cv::Mat firstImage, cv::Mat background, MyVideoContent content){

    for (int rows = 0; rows < firstImage.rows; rows++){
        for (int cols = 0; cols < firstImage.cols; cols++){

            cv::Vec3b backgroundColor = content.background.at<cv::Vec3b>(rows,cols);

            unsigned char redBackground = backgroundColor[2];
            unsigned char greenBackground = backgroundColor[1];
            unsigned char blueBackground = backgroundColor[0];

            cv::Vec3b frameColor = content.frame.at<cv::Vec3b>(rows,cols);

            unsigned char redFrame = frameColor[2];
            unsigned char greenFrame = frameColor[1];

            unsigned char blueFrame = frameColor[0];

            int differenceBetweenColors = 30;

            if (abs(redBackground - redFrame) < differenceBetweenColors && abs(greenBackground - greenFrame) < differenceBetweenColors && abs(blueBackground - blueFrame) < differenceBetweenColors){
                content.frame.at<cv::Vec3b>(rows, cols) = background.at<cv::Vec3b>(rows, cols);
            }
        }
    }

    return content.frame;
}

void task4() {
    cv::VideoCapture video(1);

    rassert(video.isOpened(), 3423948392481)

    MyVideoContent content;

    cv::Mat castle = cv::imread("lesson03/data/castle_large.jpg");

    rassert(!castle.empty(), 3573290865)

    video.read(content.frame);

    cv::Mat largeCastle = largeCastleUpscale(castle, content);

    rassert(!largeCastle.empty(), 8237489369265)

    std::cout << largeCastle.rows << " " << largeCastle.cols << std::endl;



    while (video.isOpened()) {
        bool isSuccess = video.read(content.frame);
        rassert(isSuccess, 348792347819);
        rassert(!content.frame.empty(), 3452314124643)

        content.frame = makeAllClickedPixelsRed( content);
        content.frame = makeGreenScreenWithClickedColor(content, largeCastle);
        if (content.isInverted) content.frame = invertImageColors(content.frame);

        cv::imshow("video", content.frame);
        cv::setMouseCallback("video", onMouseClick, &content);

        int key = cv::waitKey(10);
        if (key == 32 || key == 27) break;
    }

    video.read(content.background);

    while(video.isOpened()){
        bool isSuccess = video.read(content.frame);
        rassert(isSuccess, 348792347819);
        rassert(!content.frame.empty(), 3452314124643)

        if (content.isInverted) content.frame = invertImageColors(content.frame);

        makeBackgroundDisappear(content.background, largeCastle, content);

        cv::imshow("video", content.frame);

        int key = cv::waitKey(10);
        if (key == 32 || key == 27) break;
    }



    // TODO подумайте, а как бы отмаскировать фон целиком несмотря на то что он разноцветный?
    // а как бы вы справились с тем чтобы из фотографии с единорогом и фоном удалить фон зная как выглядит фон?
    // а может сделать тот же трюк с вебкой - выйти из вебки в момент запуска программы, и первый кадр использовать как кадр-эталон с фоном который надо удалять (делать прозрачным)
}



int main() {
    try {
//        task1();
//        task2();
//        task3();
        task4();
        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}
