#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <windows.h>
#include <vector>
#include <cmath>
#include <iostream>

#define M_PI 3.14159265358979323846


void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color) {
    SetPixel(hdc, xc + x, yc + y, color); 
    SetPixel(hdc, xc - x, yc + y, color); 
    SetPixel(hdc, xc - x, yc - y, color); 
    SetPixel(hdc, xc + x, yc - y, color); 
}

void EllipseDirect(HDC hdc, std::vector<POINT> points, COLORREF color) {
    int xc = points[0].x, yc = points[0].y;
    int Rx = points[1].x, Ry = points[1].y;
    long long Rx2 = Rx * Rx;
    long long Ry2 = Ry * Ry;
    int x = 0;
    double y = Ry;
    Draw4Points(hdc, xc, yc, x, round(y), color);
    while ((Ry2 * x) < (Rx2 * y)) {
        x++;
        y = Ry * sqrt(1.0 - (double)(x * x) / Rx2);
        Draw4Points(hdc, xc, yc, x, round(y), color);
    }
    y = 0;
    double x_val = Rx;
    Draw4Points(hdc, xc, yc, round(x_val), y, color);
    while ((Rx2 * y) <= (Ry2 * x_val)) {
        y++;
        x_val = Rx * sqrt(1.0 - (double)(y * y) / Ry2);
        Draw4Points(hdc, xc, yc, round(x_val), y, color);
    }
}

void EllipsePolar(HDC hdc, std::vector<POINT> points, COLORREF color) {
    int xc = points[0].x, yc = points[0].y;
    int Rx = points[1].x, Ry = points[1].y;
    double theta = 0;
    double dtheta = 1.0 / std::max(Rx, Ry); 
    while (theta <= M_PI / 2.0) {
        int x = round(Rx * cos(theta));
        int y = round(Ry * sin(theta));
        Draw4Points(hdc, xc, yc, x, y, color);
        theta += dtheta;
    }
}

void EllipseMidpoint(HDC hdc, std::vector<POINT> points, COLORREF color) {
    int xc = points[0].x, yc = points[0].y;
    int Rx = points[1].x, Ry = points[1].y;
    long long Rx2 = Rx * Rx;
    long long Ry2 = Ry * Ry;
    long long twoRx2 = 2 * Rx2;
    long long twoRy2 = 2 * Ry2;
    int x = 0;
    int y = Ry;
    long long px = 0;
    long long py = twoRx2 * y;

    Draw4Points(hdc, xc, yc, x, y, color);
    long long p1 = round(Ry2 - (Rx2 * Ry) + (0.25 * Rx2));
    while (px < py) {
        x++;
        px += twoRy2;
        if (p1 < 0) p1 += Ry2 + px;
        else { y--; py -= twoRx2; p1 += Ry2 + px - py; }
        Draw4Points(hdc, xc, yc, x, y, color);
    }
    long long p2 = round(Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2);
    while (y > 0) {
        y--;
        py -= twoRx2;
        if (p2 > 0) p2 += Rx2 - py;
        else { x++; px += twoRy2; p2 += Rx2 - py + px; }
        Draw4Points(hdc, xc, yc, x, y, color);
    }
}

#endif // ELLIPSE_H