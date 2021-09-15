#include <vector>
#include <iostream>
#include <libutils/rasserts.h>

#include "helper_function.h" // подключаем функцию print2DArray(...) из соседнего файла


void task1() {
    std::vector<int> a; // динамический массив (автоматически увеличивается по мере добавления объектов)
    a.push_back(10); // добавляем в конец число 10, тем самым увеличивая этот динамический массив
    std::cout << "current size is " << a.size() << std::endl; // печатаем в консоль текущий размер

    // чтобы продолжить выполнение задания - раскомментируйте следующие строчки - проще всего это сделать выделив их все мышкой, а затем нажать Ctrl+/ (так же можно и комментировать строки)
    rassert(a.size() == 1,3412421421); // это способ проверить некоторый факт в программе, например здесь мы ожидали бы что размер вектора 1 - давайте явно это проверим, указав код ошибки на случай если это будет не так
    rassert(a.size() == 1, "vector a should of size 1!"); // можно вместо числа писать текст описывающий проблему, но на мой взгляд это слишком долго и лениво

    a.resize(5); // изменяем размер массива, пусть он будет хранить пять чисел
    for (int i = 0; i < 5; ++i) {
        a[i] = i; // тогда мы сможем сразу в каждую из его пяти ячеек что-то положить, например числа от 0 до 4
    }
    std::cout << "current size is " << a.size() << std::endl; // печатаем в консоль текущий размер
    rassert(a.size() == 5, 2327489623);
}

void task2() {
    std::vector<std::vector<int>> array2d; // это динамический массив в котором каждый элемент - тоже динамический массив
    rassert(array2d.empty(), 920193012);

    std::vector<int> row0; // первый ряд чисел для нашего двумерного массива (row = ряд)
    row0.push_back(5);
    row0.push_back(8);
    row0.push_back(10);
    rassert(row0.size() == 3, 291740294);

    array2d.push_back(row0);
    rassert(array2d.size() == 1, 932741089);

    std::cout << "print2DArray(array2d):" << std::endl;
    print2DArray(array2d);

    // чтобы продолжить выполнение задания - раскомментируйте следующие строчки - проще всего это сделать выделив их все мышкой, а затем нажать Ctrl+/ (так же можно и комментировать строки)
    // теперь давайте добавим еще один ряд чисел но другим способом:
    std::vector<int> row1;
    row1.resize(3);
    row1[0] = 23;
    row1[1] = 24;
    row1[2] = 42;
    array2d.push_back(row1);
    rassert(row1.size() == 3, 39902)
    rassert(array2d.size() == 2, 3297490)

    std::cout << "print2DArray(array2d):" << std::endl;
    print2DArray(array2d);


    std::vector<int> row2;
    row2.resize(10);
    for (int i = 0; i < 10; ++i){
        row2[i] = i;
    }
    array2d.push_back(row2);
    std::cout << "print2DArray(array2d):" << std::endl;
    print2DArray(array2d);
}

void task3() {
    // TODO 31 напишите следующую программу:

    std::cout << "Choose number of rows and cols: ";
    int rows;
    int cols;
    std::cin >> rows >> cols;

    rassert(rows < 21 && rows > 0, 297492)
    rassert(cols < 21 && cols > 0, 2974891)

    std::vector<std::vector<int>> matrix;

    matrix.resize(rows);

    for (int i = 0; i < rows; i++){
        matrix[i].resize(cols);
    }

    print2DArray(matrix);

    // TODO 37 ваша программа должна считывать пары чисел i, j в вечном цикле до тех пор пока i и j не отрицательны
    while (true) {
        int i;
        int j;
        std::cin >> i >> j;
        if ( i < 0 || j < 0) break;
        matrix[i][j]++;

        print2DArray(matrix);

        bool isRowed;
        for (int m = 0; m < rows; m++){
            isRowed = true;
            for (int n = 0; n < cols; n++){
                if (matrix[m][n] < 1) isRowed = false;
            }
            if (isRowed) {
                std::cout << "OX-XO-XO";
                break;
            }
        }

        bool isCols;
        if (!isRowed){
            for (int m = 0; m < cols; m++){
                isCols = true;
                for (int n = 0; n < rows; n++){
                    if (matrix[n][m] < 1) isCols = false;
                }
                if (isCols) {
                    std::cout << "AX-XA-XA";
                    break;
                }
            }

        } else break;

        if (isCols) break;
        // TODO 38 считав очередное i, j - увеличьте ячейку в думерном массиве находящуюся в j-ой колонке, в i-ом ряду
        // TODO 39 выведите в консоль текущее состояние двумерного массива
        // TODO 40 добавьте проверку что если пользователь заполнил единицами хотя бы один ряд - то выводится сообщение "OX-XO-XO" и программа завершается
        // TODO 41 добавьте проверку что если пользователь заполнил единицами хотя бы одну колонку - то выводится сообщение "AX-XA-XA" и программа завершается
    }

}


int main() {
    try {
//        task1();
//        task2();
        task3(); // TODO 30 закомментируйте предыдущие две строки и раскоментируйте эту чтобы начать выполнять третье задание
        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}