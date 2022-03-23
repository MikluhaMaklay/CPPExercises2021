#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <set>

#include <libutils/rasserts.h>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>
#include <filesystem>

typedef long long ll;

struct Edge {
    int u, v; // номера вершин которые это ребро соединяет
    ll w; // длина ребра (т.е. насколько длинный путь предстоит преодолеть переходя по этому ребру между вершинами)

    Edge(ll u, ll v, ll w) : u(u), v(v), w(w)
    {}
};

// Эта биективная функция по координате пикселя (строчка и столбик) + размерам картинки = выдает номер вершины
ll encodeVertex(ll row, ll column, ll nrows, ll ncolumns) {
    rassert(row < nrows, 348723894723980017);
    rassert(column < ncolumns, 347823974239870018);
    ll vertexId = row * ncolumns + column;
    return vertexId;
}

// Эта биективная функция по номеру вершины говорит какой пиксель этой вершине соовтетствует (эта функция должна быть симметрична предыдущей!)
cv::Point2i decodeVertex(ll vertexId, ll nrows, ll ncolumns) {
    ll row = vertexId/ncolumns;
    ll column = vertexId%ncolumns;

    // сверим что функция симметрично сработала:
    rassert(encodeVertex(row, column, nrows, ncolumns) == vertexId, 34782974923035);

    rassert(row < nrows, 34723894720027);
    rassert(column < ncolumns, 3824598237592030);
    return cv::Point2i(column, row);
}

ll distance(unsigned char r1, unsigned char g1, unsigned char b1, unsigned char r2, unsigned char g2, unsigned char b2){
    return round(sqrt(pow(r2-r1,2)+pow(g2-g1,2)+pow(b2-b1,2)));
}

void run(ll mazeNumber) {
    cv::Mat maze = cv::imread("lesson15/data/mazesImages/maze" + std::to_string(mazeNumber) + ".png");
    rassert(!maze.empty(), 324783479230019);
    rassert(maze.type() == CV_8UC3, 3447928472389020);
    std::cout << "Maze resolution: " << maze.cols << "x" << maze.rows << "\n";

    ll nvertices = maze.cols*maze.rows;

    std::vector<std::vector<Edge>> edges_by_vertex(nvertices);
    ll start, finish;
    if (mazeNumber >= 1 && mazeNumber <= 3) { // Первые три лабиринта очень похожи но кое чем отличаются...
        start = encodeVertex(300, 300, maze.rows, maze.cols);
        finish = encodeVertex(0, 305, maze.rows, maze.cols);
    } else if (mazeNumber == 4) {
        start = encodeVertex(154, 312, maze.rows, maze.cols);
        finish = encodeVertex(477, 312, maze.rows, maze.cols);
    } else if (mazeNumber == 5) { // Лабиринт в большом разрешении, добровольный (на случай если вы реализовали быструю Дейкстру с приоритетной очередью)
        start = encodeVertex(1200, 1200, maze.rows, maze.cols);
        finish = encodeVertex(1200, 1200, maze.rows, maze.cols);
    } else {
        rassert(false, 324289347238920081);
    }
    cv::Vec3b s = maze.at<cv::Vec3b>(decodeVertex(start, maze.rows, maze.cols));

    for (int i = 0; i < maze.rows; ++i) {
        for (int j = 0; j < maze.cols; ++j) {
            cv::Vec3b color = maze.at<cv::Vec3b>(i,j);
            unsigned char blue = color[0];
            unsigned char green = color[1];
            unsigned char red = color[2];

            auto ai = encodeVertex(i,j,maze.rows,maze.cols);
            if(i > 0) {
                cv::Vec3b color2 = maze.at<cv::Vec3b>(i-1,j);
                edges_by_vertex[ai].emplace_back(ai, encodeVertex(i-1,j, maze.rows, maze.cols),
                                                 distance(color2[0], color2[1], color2[2], red, green, blue)+1);
            }
            if(i < maze.rows-1) {
                cv::Vec3b color2 = maze.at<cv::Vec3b>(i+1,j);
                edges_by_vertex[ai].emplace_back(ai, encodeVertex(i+1,j, maze.rows, maze.cols),
                                                 distance(color2[0], color2[1], color2[2], red, green, blue)+1);
            }
            if(j > 0) {
                cv::Vec3b color2 = maze.at<cv::Vec3b>(i,j-1);
                edges_by_vertex[ai].emplace_back(ai, encodeVertex(i,j-1, maze.rows, maze.cols),
                                                 distance(color2[0], color2[1], color2[2], red, green, blue)+1);
            }
            if(j < maze.cols-1) {
                cv::Vec3b color2 = maze.at<cv::Vec3b>(i,j+1);
                edges_by_vertex[ai].emplace_back(ai, encodeVertex(i,j+1, maze.rows, maze.cols),
                                                 distance(color2[0], color2[1], color2[2], red, green, blue)+1);
            }
        }
    }

    const ll INF = std::numeric_limits<ll>::max();

    cv::Mat window = maze.clone(); // на этой картинке будем визуализировать до куда сейчас дошла прокладка маршрута
    cv::Mat window2 = maze.clone(); // на этой картинке будем визуализировать до куда сейчас дошла прокладка маршрута


    // TODO обозначьте найденный маршрут красными пикселями

    // TODO сохраните картинку window на диск

    std::vector<ll> distances(nvertices, INF);
    std::vector<int> prev(nvertices, -1);
    std::set<std::pair<ll, int>> q;        //w, u
    distances[start] = 0;
    prev[start] = start;
    q.emplace(0, start);
    while (!q.empty()){
        auto p = *q.begin();
        q.erase(*q.begin());
        if(distances[p.second] != p.first)
            continue;
        for(auto n : edges_by_vertex[p.second]){
            if(distances[n.v] > p.first+n.w){
                distances[n.v] = p.first+n.w;
                q.emplace(distances[n.v], n.v);
                prev[n.v] = p.second;
            }
        }
    }

    if (distances[finish] != INF) {
        std::vector<int> path;
        for (int i = finish; prev[i] != i; i = prev[i]) {
            path.emplace_back(i);
        }
        path.emplace_back(start);
        for(auto it = path.rbegin(); it != path.rend(); it++) {
            cv::Point2i p = decodeVertex(*it, maze.rows, maze.cols);
            window.at<cv::Vec3b>(p.y, p.x) = cv::Vec3b(0, 255, 0);
            window2.at<cv::Vec3b>(p.y, p.x) = cv::Vec3b(0, 0,255);
            cv::imshow("Maze", window);
            cv::waitKey(1);
        }
        std::cout << "\n";
    } else {
        std::cout << -1 << "\n";
    }

    std::cout << "Finished!" << "\n";

    std::string resultsDir = "lesson15/resultsData/";
    if (!std::filesystem::exists(resultsDir)) { // если папка еще не создана
        std::filesystem::create_directory(resultsDir); // то создаем ее
    }

    std::string filename = resultsDir + "maze" + std::to_string(mazeNumber) + ".jpg"; // удобно в начале файла писать число, чтобы файлы были в том порядке в котором мы их создали
    cv::imwrite(filename, window2);

    // Показываем результат пока пользователь не насладиться до конца и не нажмет Escape
    while (cv::waitKey(10) != 27) {
        cv::imshow("Maze", window);
    }


}

int main() {
    try {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie();
        ll mazeNumber = 4;
        run(mazeNumber);
        std::cout.flush();
        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << "\n";
        return 1;
    }
}