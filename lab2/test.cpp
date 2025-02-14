#include <gtest/gtest.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <filesystem>
#include "KMeans.h"
#include "Point.h"

std::vector<Point> loadPoints(const std::string& relativePath) {
    std::string fullPath = std::filesystem::current_path().string() + "/" + relativePath;
    std::ifstream file(fullPath);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + fullPath);
    }

    std::vector<Point> points;
    int n;
    file >> n;
    for (int i = 0; i < n; ++i) {
        double x, y;
        file >> x >> y;
        points.emplace_back(x, y);
    }

    return points;
}

TEST(Lab2Test, SingleThreadedRun) {
    std::vector<Point> points = loadPoints("files_for_lab2/input_single.txt");

    int k = 2;
    int maxThreads = 1;
    KMeans kmeans(k, maxThreads);

    auto start = std::chrono::high_resolution_clock::now();
    kmeans.run(points);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << "Время выполнения (однопоточный режим): " << duration.count() << " секунд\n";

    for (const auto& point : points) {
        EXPECT_GE(point.cluster, 0) << "Точка не имеет назначенного кластера";
    }
}

TEST(Lab2Test, MultiThreadedRun) {
    std::vector<Point> points = loadPoints("files_for_lab2/speed.txt");

    int k = 3;
    int maxThreadsSingle = 1;
    KMeans kmeansSingle(k, maxThreadsSingle);

    auto startSingle = std::chrono::high_resolution_clock::now();
    kmeansSingle.run(points);
    auto endSingle = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> durationSingle = endSingle - startSingle;
    std::cout << "Время выполнения (однопоточный режим): " << durationSingle.count() << " секунд\n";

    int maxThreadsMulti = 4;
    KMeans kmeansMulti(k, maxThreadsMulti);

    points = loadPoints("files_for_lab2/speed.txt");

    auto startMulti = std::chrono::high_resolution_clock::now();
    kmeansMulti.run(points);
    auto endMulti = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> durationMulti = endMulti - startMulti;
    std::cout << "Время выполнения (четырехпоточный режим): " << durationMulti.count() << " секунд\n";

    EXPECT_LT(durationMulti.count(), durationSingle.count()) << "Многопоточный режим работает медленнее однопоточного";

    for (const auto& point : points) {
        EXPECT_GE(point.cluster, 0) << "Точка не имеет назначенного кластера";
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
