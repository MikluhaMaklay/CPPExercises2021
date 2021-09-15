#include "helper_function.h"

#include <iostream>


void print2DArray(std::vector<std::vector<int>> array2d) {
    for (int j = 0; j < array2d.size(); ++j) { // пробегаем по каждой строке

        int n = array2d[j].size(); // узнаем число элементов в текущей строке
        for (int i = 0; i < n; ++i){
            std::cout << array2d[j][i] << " ";
        }
        std::cout << std::endl;
    }
}