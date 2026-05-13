#ifndef Line_H
#define Line_H

#include <windows.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include "menu_id.h"
#include "Structs.h"

inline void DrawLineParametric(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color)
{
    int dx = xe - xs;
    int dy = ye - ys;

    int steps = max(abs(dx), abs(dy));

    double dt = 1.0 / steps;

    for(double t = 0; t <= 1; t += dt)
    {
        int x = round(xs + dx * t);
        int y = round(ys + dy * t);

        SetPixel(hdc, x, y, color);
    }
}

inline void DrawLineMidpoint(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color)
{
    int dx = xe - xs;
    int dy = ye - ys;

    int x = xs;
    int y = ys;

    int xinc = (dx > 0) ? 1 : -1;
    int yinc = (dy > 0) ? 1 : -1;

    dx = abs(dx);
    dy = abs(dy);

    SetPixel(hdc, x, y, color);

    // slope < 1
    if(dx > dy)
    {
        int d = 2 * dy - dx;

        int change1 = 2 * dy;
        int change2 = 2 * (dy - dx);

        while(x != xe)
        {
            if(d < 0)
            {
                d += change1;
            }
            else
            {
                d += change2;
                y += yinc;
            }

            x += xinc;

            SetPixel(hdc, x, y, color);
        }
    }

    // slope >= 1
    else
    {
        int d = 2 * dx - dy;

        int change1 = 2 * dx;
        int change2 = 2 * (dx - dy);

        while(y != ye)
        {
            if(d < 0)
            {
                d += change1;
            }
            else
            {
                d += change2;
                x += xinc;
            }

            y += yinc;

            SetPixel(hdc, x, y, color);
        }
    }
}

#endif // Line_H
