#ifndef Curve_H
#define Curve_H

#include <windows.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include "menu_id.h"
#include "Structs.h"

inline Vector4 operator*(Matrix4 M, Vector4& b)
{
    Vector4 res;

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            res[i] += M[i][j] * b[j];
        }
    }

    return res;
}

inline double DotProduct(Vector4& a, Vector4& b)
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3];
}

inline Vector4 GetHermiteCoeff(double x0,double s0, double x1,double s1) 
{
    static double H[16] =
    {
         2,  1, -2,  1,
        -3, -2,  3, -1,
         0,  1,  0,  0,
         1,  0,  0,  0
    };

    static Matrix4 basis(H);

    Vector4 v(x0,s0,x1,s1);

    return basis * v;
}

inline void DrawHermiteCurve(HDC hdc, Vector2 P0, Vector2 T0, Vector2 P1, Vector2 T1, int numpoints, COLORREF color)
{
    Vector4 xcoeff = GetHermiteCoeff(P0.x,T0.x,P1.x,T1.x);

    Vector4 ycoeff = GetHermiteCoeff(P0.y,T0.y,P1.y,T1.y);

    if(numpoints < 2)
        return;

    double dt = 1.0 / (numpoints - 1);

    bool first = true;

    for(double t=0; t<=1; t+=dt)
    {
        Vector4 vt;

        vt[3] = 1;

        for(int i=2;i>=0;i--)
        {
            vt[i] = vt[i+1] * t;
        }

        int x = round(DotProduct(xcoeff, vt));
        int y = round(DotProduct(ycoeff, vt));

        if(first)
        {
            MoveToEx(hdc, x, y, NULL);
            first = false;
        }
        else
        {
            LineTo(hdc, x, y);
        }
    }
}

inline void DrawCardinalSpline(HDC hdc, std::vector<POINT> points, double c, int numpoints, COLORREF color)
{
    int n = points.size();

    if(n < 4)
        return;

    double c1 = 1 - c;

    Vector2 T0(
        c1 * (points[2].x - points[0].x),
        c1 * (points[2].y - points[0].y)
    );

    for(int i=2; i<n-1; i++)
    {
        Vector2 T1(
            c1 * (points[i+1].x - points[i-1].x),
            c1 * (points[i+1].y - points[i-1].y)
        );

        Vector2 P0(points[i-1].x, points[i-1].y);
        Vector2 P1(points[i].x, points[i].y);

        DrawHermiteCurve(hdc, P0, T0, P1, T1, numpoints, color);

        T0 = T1;
    }
}

#endif // Curve_H
