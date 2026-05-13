#ifndef CIRCLE_H
#define CIRCLE_H

#include <windows.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include "menu_id.h"
#include "Structs.h"

#define M_PI 3.14159265358979323846

inline void Draw8Points(HDC hdc, int xc, int yc, int a, int b, COLORREF color) {
    SetPixel(hdc, xc + a, yc + b, color);
    SetPixel(hdc, xc - a, yc + b, color);
    SetPixel(hdc, xc - a, yc - b, color);
    SetPixel(hdc, xc + a, yc - b, color);
    SetPixel(hdc, xc + b, yc + a, color);
    SetPixel(hdc, xc - b, yc + a, color);
    SetPixel(hdc, xc - b, yc - a, color);
    SetPixel(hdc, xc + b, yc - a, color);
}

inline void CircleDirect(HDC hdc, std::vector<POINT> points, int R, COLORREF color) {
    if (points.empty()) return;
    int xc = points[0].x, yc = points[0].y;
    int x = 0, y = R;
    int R2 = R * R;
    Draw8Points(hdc, xc, yc, x, y, color);
    while (x < y) {
        x++;
        y = round(sqrt((double)(R2 - x * x)));
        Draw8Points(hdc, xc, yc, x, y, color);
    }
}

inline void CirclePolar(HDC hdc, std::vector<POINT> points, int R, COLORREF color) {
    if (points.empty()) return;
    int xc = points[0].x, yc = points[0].y;
    int x = R, y = 0;
    double theta = 0, dtheta = 1.0 / R;
    Draw8Points(hdc, xc, yc, x, y, color);
    while (x > y) {
        theta += dtheta;
        x = round(R * cos(theta));
        y = round(R * sin(theta));
        Draw8Points(hdc, xc, yc, x, y, color);
    }
}

inline void CircleIterativePolar(HDC hdc, std::vector<POINT> points, int R, COLORREF color) {
    if (points.empty()) return;
    int xc = points[0].x, yc = points[0].y;
    double x = R, y = 0;
    double dtheta = 1.0 / R;
    double cdtheta = cos(dtheta), sdtheta = sin(dtheta);
    Draw8Points(hdc, xc, yc, R, 0, color);
    while (x > y) {
        double x1 = x * cdtheta - y * sdtheta;
        y = x * sdtheta + y * cdtheta;
        x = x1;
        Draw8Points(hdc, xc, yc, round(x), round(y), color);
    }
}

inline void CircleBresenham(HDC hdc, std::vector<POINT> points, int R, COLORREF color) {
    if (points.empty()) return;
    int xc = points[0].x, yc = points[0].y;
    int x = 0, y = R;
    int d = 1 - R;
    Draw8Points(hdc, xc, yc, x, y, color);
    while (x < y) {
        if (d < 0)
            d += 2 * x + 2;
        else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
        Draw8Points(hdc, xc, yc, x, y, color);
    }
}

inline void CircleFasterBresenham(HDC hdc, std::vector<POINT> points, int R, COLORREF color) {
    if (points.empty()) return;
    int xc = points[0].x, yc = points[0].y;
    int x = 0, y = R;
    int d = 1 - R;
    int c1 = 3, c2 = 5 - 2 * R;
    Draw8Points(hdc, xc, yc, x, y, color);
    while (x < y) {
        if (d < 0) {
            d += c1;
            c2 += 2;
        } else {
            d += c2;
            c2 += 4;
            y--;
        }
        c1 += 2;
        x++;
        Draw8Points(hdc, xc, yc, x, y, color);
    }
}

#endif // CIRCLE_H
