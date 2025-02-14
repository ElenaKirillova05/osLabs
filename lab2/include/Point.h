#ifndef POINT_H
#define POINT_H

struct Point {
    double x, y;
    int cluster = -1;

    Point(double x_val = 0.0, double y_val = 0.0) : x(x_val), y(y_val), cluster(-1) {}
};

#endif  