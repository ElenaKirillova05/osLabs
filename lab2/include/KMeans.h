#ifndef KMEANS_H
#define KMEANS_H

#include <vector>
#include "Point.h"

class KMeans {
public:
    KMeans(int k, int maxThreads);
    void run(std::vector<Point>& points);
    void printResults(const std::vector<Point>& points);
private:
    int k;
    int maxThreads;
    std::vector<Point> centroids;
    
    struct ThreadData {
        std::vector<Point>* points;
        std::vector<Point>* centroids;
        int start;
        int end;
        int k;
    };

    static void* assignClusters(void* arg);
    void initializeCentroids(std::vector<Point>& points);
};

#endif