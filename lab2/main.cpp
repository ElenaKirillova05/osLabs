#include <iostream>
#include <vector>
#include "Point.h"
#include "KMeans.h"
#include "Timer.h"

int main() {
    int maxThreads;

    std::cout << "Введите максимальное количество потоков:\n";
    std::cin >> maxThreads;

    int n, k;
    std::cout << "Введите количество точек:\n";
    std::cin >> n;

    std::vector<Point> points(n);
    std::cout << "Введите координаты точек (x y):\n";
    for (int i = 0; i < n; ++i) {
        std::cin >> points[i].x >> points[i].y;
    }

    std::cout << "Введите количество кластеров:\n";
    std::cin >> k;

    Timer t;
    KMeans kmeans(k, maxThreads);
    kmeans.run(points);
    //kmeans.printResults(points);

    std::cout << "Используемое количество потоков: " << maxThreads << std::endl;

    return 0;
}