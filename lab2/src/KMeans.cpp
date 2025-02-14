#include "KMeans.h"
#include "Utils.h"
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

KMeans::KMeans(int k, int maxThreads) : k(k), maxThreads(maxThreads) {
    centroids.resize(k);
    std::srand(static_cast<unsigned int>(std::time(0)));
}

void KMeans::initializeCentroids(std::vector<Point>& points) {
    for(int i = 0; i < k; ++i){
        centroids[i] = points[std::rand() % points.size()];
    }
}

void* KMeans::assignClusters(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    for (int i = data->start; i < data->end; ++i) {
        double minDist = distance((*data->points)[i], (*data->centroids)[0]);
        int bestCluster = 0;
        for (int j = 1; j < data->k; ++j) {
            double dist = distance((*data->points)[i], (*data->centroids)[j]);
            if (dist < minDist) {
                minDist = dist;
                bestCluster = j;
            }
        }
        (*data->points)[i].cluster = bestCluster;
    }
    return nullptr;
}

void KMeans::run(std::vector<Point>& points) {
    initializeCentroids(points);

    bool changed;
    int iterations = 0;
    const int maxIterations = 100;

    do {
        changed = false;

        int pointsPerThread = points.size() / maxThreads;
        std::vector<pthread_t> threads(maxThreads);
        std::vector<ThreadData> threadData(maxThreads);

        for (int i = 0; i < maxThreads; ++i) {
            int start = i * pointsPerThread;
            int end;
            if (i == maxThreads - 1) {
                end = points.size();
            } else {
                end = (i + 1) * pointsPerThread;
            }

            threadData[i] = { &points, &centroids, start, end, k };

            if (pthread_create(&threads[i], nullptr, assignClusters, &threadData[i]) != 0) {
                std::cerr << "Ошибка при создании потока\n";
                exit(1);
            }
        }

        for (int i = 0; i < maxThreads; ++i) {
            pthread_join(threads[i], nullptr);
        }

        std::vector<int> count(k, 0);
        std::vector<double> sumX(k, 0.0), sumY(k, 0.0);

        for (const auto &point : points) {
            sumX[point.cluster] += point.x;
            sumY[point.cluster] += point.y;
            count[point.cluster]++;
        }

        for (int i = 0; i < k; ++i) {
            if (count[i] > 0) {
                Point newCentroid(sumX[i] / count[i], sumY[i] / count[i]);

                if (centroids[i].x != newCentroid.x || centroids[i].y != newCentroid.y) {
                    changed = true;
                }
                centroids[i] = newCentroid;
            }
        }

        iterations++;

    } while (changed && iterations < maxIterations);
}

void KMeans::printResults(const std::vector<Point>& points) {
    for (int i = 0; i < k; ++i) {
        std::cout << "Кластер " << i + 1 << " центр: (" << centroids[i].x << ", " << centroids[i].y << ")\n";
    }

    for (size_t i = 0; i < points.size(); ++i) {
        std::cout << "Точка " << i + 1 << " (" << points[i].x << ", " << points[i].y << ") принадлежит кластеру " << points[i].cluster + 1 << "\n";
    }

    //std::cout << iterations << '\n';
}