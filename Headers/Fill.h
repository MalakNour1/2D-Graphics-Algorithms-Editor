#ifndef Fill_H
#define Fill_H

#include <windows.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <list>
#include <stack>

#include "menu_id.h"
#include "Structs.h"
# include "Line.h"
# include "Circle.h"
#include "Curve.h"


#define MAXENTRIES 600
#define MY_MAXINT 100000

typedef list<EdgeRec> EdgeList;

inline void InitEntries(Entry table[])
{
    for(int i = 0; i < MAXENTRIES; i++)
    {
        table[i].xmin = MY_MAXINT;
        table[i].xmax = -MY_MAXINT;
    }
}

inline void ScanEdge(POINT v1, POINT v2, Entry table[])
{
    if(v1.y == v2.y)
        return;

    if(v1.y > v2.y)
        swap(v1, v2);

    double minv = (double)(v2.x - v1.x) / (v2.y - v1.y);

    double x = v1.x;
    int y = v1.y;

    while(y < v2.y)
    {
        if(y >= 0 && y < MAXENTRIES)
        {
            if(x < table[y].xmin)
                table[y].xmin = (int)ceil(x);

            if(x > table[y].xmax)
                table[y].xmax = (int)floor(x);
        }

        y++;
        x += minv;
    }
}

inline void DrawScanLines(HDC hdc, Entry table[], COLORREF color)
{
    for(int y = 0; y < MAXENTRIES; y++)
    {
        if(table[y].xmin < table[y].xmax)
        {
            for(int x = table[y].xmin; x <= table[y].xmax; x++)
            {
                SetPixel(hdc, x, y, color);
            }
        }
    }
}

inline void ConvexFill(HDC hdc, POINT p[], int n, COLORREF color)
{
    Entry* table = new Entry[MAXENTRIES];

    InitEntries(table);

    POINT v1 = p[n - 1];

    for(int i = 0; i < n; i++)
    {
        POINT v2 = p[i];

        ScanEdge(v1, v2, table);

        v1 = v2;
    }

    DrawScanLines(hdc, table, color);

    delete[] table;
}

// Non-convex
EdgeRec InitEdgeRec(POINT& v1, POINT& v2)
{
    if(v1.y > v2.y)
        swap(v1, v2);

    EdgeRec rec;

    rec.x = v1.x;
    rec.ymax = v2.y;
    rec.minv = (double)(v2.x - v1.x) / (v2.y - v1.y);

    return rec;
}

inline void InitEdgeTable(POINT* polygon, int n, EdgeList table[])
{
    POINT v1 = polygon[n - 1];

    for(int i = 0; i < n; i++)
    {
        POINT v2 = polygon[i];

        if(v1.y == v2.y)
        {
            v1 = v2;
            continue;
        }

        EdgeRec rec = InitEdgeRec(v1, v2);

        table[v1.y].push_back(rec);

        v1 = polygon[i];
    }
}

void NonConvexFill(HDC hdc, POINT* polygon, int n, COLORREF color)
{
    EdgeList* table = new EdgeList[MAXENTRIES];

    InitEdgeTable(polygon, n, table);

    int y = 0;

    while(y < MAXENTRIES && table[y].size() == 0)
        y++;

    if(y == MAXENTRIES)
    {
        delete[] table;
        return;
    }

    EdgeList ActiveList = table[y];

    while(ActiveList.size() > 0)
    {
        ActiveList.sort();

        for(auto it = ActiveList.begin(); it != ActiveList.end(); )
        {
            int x1 = (int)ceil(it->x);

            it++;

            if(it == ActiveList.end())
                break;

            int x2 = (int)floor(it->x);

            for(int x = x1; x <= x2; x++)
            {
                SetPixel(hdc, x, y, color);
            }

            it++;
        }

        y++;

        auto it = ActiveList.begin();

        while(it != ActiveList.end())
        {
            if(y == it->ymax)
                it = ActiveList.erase(it);
            else
                it++;
        }

        for(auto it = ActiveList.begin(); it != ActiveList.end(); it++)
        {
            it->x += it->minv;
        }

        ActiveList.insert(
            ActiveList.end(),
            table[y].begin(),
            table[y].end()
        );
    }

    delete[] table;
}





inline void FloodFill(HDC hdc, int x, int y, COLORREF Cb, COLORREF Cf)
{
    if(x < 0 || y < 0 || x > 2000 || y > 2000)
        return;

    COLORREF C = GetPixel(hdc, x, y);

    if (C == Cb || C == Cf)
        return;

    if (C == CLR_INVALID)
        return;

    SetPixel(hdc, x, y, Cf);

    FloodFill(hdc, x + 1, y, Cb, Cf);
    FloodFill(hdc, x - 1, y, Cb, Cf);
    FloodFill(hdc, x, y + 1, Cb, Cf);
    FloodFill(hdc, x, y - 1, Cb, Cf);
}

inline void NRFloodFill(HDC hdc, int x, int y, COLORREF Cb, COLORREF Cf)
{
    stack<Vertex> S;
    S.push(Vertex(x, y));

    while (!S.empty())
    {
        Vertex v = S.top();
        S.pop();

        if(v.x < 0 || v.y < 0 || v.x > 2000 || v.y > 2000)
            continue;

        COLORREF c = GetPixel(hdc, v.x, v.y);

        if (c == Cb || c == Cf)
            continue;

        SetPixel(hdc, v.x, v.y, Cf);

        S.push(Vertex(v.x + 1, v.y));
        S.push(Vertex(v.x - 1, v.y));
        S.push(Vertex(v.x, v.y + 1));
        S.push(Vertex(v.x, v.y - 1));
    }
}

void FillCircleQuarterScanLine(HDC hdc, int xc, int yc, int R, int quarter, COLORREF color) {
    for (int y_rel = 0; y_rel <= R; y_rel++) {
        int x_boundary = (int)round(sqrt(R * R - y_rel * y_rel));

        int x_start, x_end, y_pos;

        if (quarter == 1) { // Top-Right
            x_start = xc;
            x_end = xc + x_boundary;
            y_pos = yc - y_rel;
        } else if (quarter == 2) { // Top-Left
            x_start = xc - x_boundary;
            x_end = xc;
            y_pos = yc - y_rel;
        } else if (quarter == 3) { // Bottom-Left
            x_start = xc - x_boundary;
            x_end = xc;
            y_pos = yc + y_rel;
        } else { // Bottom-Right (Quarter 4)
            x_start = xc;
            x_end = xc + x_boundary;
            y_pos = yc + y_rel;
        }

        for (int x = x_start; x <= x_end; x++) {
            SetPixel(hdc, x, y_pos, color);
        }
    }
}


void FillCircleWithCircles(HDC hdc, int xc, int yc, int R, int quarter, COLORREF color) {
    // بنمشي نص قطر بنص قطر
    for (int r = 0; r <= R; r++) {
        // بدل الزوايا، هنستخدم معادلة الدايرة عشان نملأ كل بكسل
        for (int x = -r; x <= r; x++) {
            int y = round(sqrt(r * r - x * x)); // معادلة الدايرة x^2 + y^2 = r^2

            if (quarter == 1 && x >= 0) SetPixel(hdc, xc + x, yc - y, color);
            else if (quarter == 2 && x <= 0) SetPixel(hdc, xc + x, yc - y, color);
            else if (quarter == 3 && x <= 0) SetPixel(hdc, xc + x, yc + y, color);
            else if (quarter == 4 && x >= 0) SetPixel(hdc, xc + x, yc + y, color);
        }
        for (int y = -r; y <= r; y++) {
            int x = round(sqrt(r * r - y * y));
            if (quarter == 1 && y <= 0) SetPixel(hdc, xc + x, yc + y, color);
            else if (quarter == 2 && y <= 0) SetPixel(hdc, xc - x, yc + y, color);
            else if (quarter == 3 && y >= 0) SetPixel(hdc, xc - x, yc + y, color);
            else if (quarter == 4 && y >= 0) SetPixel(hdc, xc + x, yc + y, color);
        }
    }
}

inline void FillSquareHermiteVertical(HDC hdc, int left, int top, int size, COLORREF color)
{
    int right = left + size;
    int bottom = top + size;

    for (int x = left; x <= right; x += 2)
    {
        double distFromEdge = min(x - left, right - x);
        double damping = distFromEdge / (size / 2.0);
        double offset = 25.0 * damping;

        Vector2 p1((double)x, (double)top);
        Vector2 t1(0, 100);
        Vector2 p2((double)x, (double)bottom);
        Vector2 t2(0, 100);

        DrawHermiteCurve(hdc, p1, t1, p2, t2, 100, color);
    }
}
inline void FillRectangleBezierHorizontal(HDC hdc, int left, int top, int width, int height, COLORREF color)
{
    int right = left + width;
    int bottom = top + height;

    for (int y = top; y <= bottom; y += 2)
    {
        double distFromEdge = min(y - top, bottom - y);
        double damping = distFromEdge / (height / 2.0);

        double offset = 20.0 * damping;

        POINT p1 = { left, y };
        POINT p2 = { (int)(left + width / 3), (int)(y + offset) };
        POINT p3 = { (int)(left + 2 * width / 3), (int)(y - offset) };
        POINT p4 = { right, y };

        DrawBezierCurve(hdc, p1, p2, p3, p4, color);
    }
}




#endif // Fill_H
