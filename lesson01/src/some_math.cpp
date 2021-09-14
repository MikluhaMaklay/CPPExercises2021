#include "some_math.h"
#include <cfloat>
#include "cmath"

int fibbonachiRecursive(int n) {
    if (n == 0) return 0;
    else if (n == 1) return 1;
    else return (fibbonachiRecursive(n-1) + fibbonachiRecursive(n-2));
}

int fibbonachiFast(int n) {
    std::vector<int> fib;
    fib.push_back(0);
    fib.push_back(1);

    for (int i = 2; i <= n; i++){
        fib[i % 2] = fib[0] + fib[1];
    }
    return fib[n % 2];
}

double solveLinearAXB(double a, double b) {
    // TODO 10 решите линейное уравнение a*x+b=0 а если решения нет - верните наибольшее значение double - найдите как это сделать в интернете по запросу "so cpp double max value" (so = stackoverflow = сайт где часто можно найти ответы на такие простые запросы), главное НЕ КОПИРУЙТЕ ПРОСТО ЧИСЛО, ПОЖАЛУЙСТААаа
    // если решений сколь угодно много - верните максимальное значение со знаком минус
    if (a == 0){
        if (b == 0) return - DBL_MAX;
        else return DBL_MAX;
    }
    else {
        double x = -b / a;
        return x;
    }
}

std::vector<double> solveSquare(double a, double b, double c) {
    // TODO 20 решите квадратное уравнение вида a*x^2+b*x+c=0
    // если корня два - они должны быть упорядочены по возрастанию
    std::vector<double> results;
    if (a == 0) results.push_back(- c / b);
    else {
        double D = b * b - 4 * a * c;
        if (D == 0) {
            results.push_back(-b / (2 * a));
        } else if (D > 0) {
            results.push_back((-b - sqrt(D)) / (2 * a));
            results.push_back((-b + sqrt(D)) / (2 * a));
        }
    }
    // чтобы добавить в вектор элемент - нужно вызвать у него метод push_back:
    return results;
}
