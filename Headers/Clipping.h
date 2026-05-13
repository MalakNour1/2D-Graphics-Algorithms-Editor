#ifndef CLIPPING_H
#define CLIPPING_H

#include <windows.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "Line.h"
#include "Circle.h"

using namespace std;

bool pointIsInside(double x, double y, double xleft, double xright, double ytop, double ybottom)
{
    return x >= xleft && x <= xright && y >= ytop && y <= ybottom;
}

union OutCode
{
    struct
    {
        unsigned L : 1, R : 1, T : 1, B : 1;
    };

    unsigned all : 4;
};

OutCode GetOutCode(double x, double y, double xleft, double xright, double ytop, double ybottom)
{
    OutCode out;
    out.all = 0;

    if (x < xleft) out.L = 1;
    else if (x > xright) out.R = 1;

    if (y < ytop) out.T = 1;
    else if (y > ybottom) out.B = 1;

    return out;
}

void vIntersect(double xedge, double x1, double y1,
                double x2, double y2, double &xi, double &yi)
{
    if (x1 == x2) return;

    yi = y1 + (xedge - x1) * (y2 - y1) / (x2 - x1);
    xi = xedge;
}

void HIntersect(double yedge, double x1, double y1,
                double x2, double y2, double &xi, double &yi)
{
    if (y1 == y2) return;

    xi = x1 + (yedge - y1) * (x2 - x1) / (y2 - y1);
    yi = yedge;
}

bool CohenSuth(double &x1, double &y1, double &x2, double &y2,
               double xleft, double xright, double ytop, double ybottom)
{
    OutCode out1 = GetOutCode(x1, y1, xleft, xright, ytop, ybottom);
    OutCode out2 = GetOutCode(x2, y2, xleft, xright, ytop, ybottom);

    while (true)
    {
        if (out1.all == 0 && out2.all == 0)
            return true;

        if ((out1.all & out2.all) != 0)
            return false;

        double xi = 0, yi = 0;

        bool firstPoint = (out1.all != 0);
        OutCode out = firstPoint ? out1 : out2;

        if (out.L)
            vIntersect(xleft, x1, y1, x2, y2, xi, yi);

        else if (out.R)
            vIntersect(xright, x1, y1, x2, y2, xi, yi);

        else if (out.T)
            HIntersect(ytop, x1, y1, x2, y2, xi, yi);

        else if (out.B)
            HIntersect(ybottom, x1, y1, x2, y2, xi, yi);

        if (firstPoint)
        {
            x1 = xi;
            y1 = yi;
            out1 = GetOutCode(x1, y1, xleft, xright, ytop, ybottom);
        }
        else
        {
            x2 = xi;
            y2 = yi;
            out2 = GetOutCode(x2, y2, xleft, xright, ytop, ybottom);
        }
    }
}

void ClipPointRectangle(HDC hdc, POINT pt, POINT rectP1, POINT rectP2, COLORREF color)
{
    double xleft = min(rectP1.x, rectP2.x);
    double xright = max(rectP1.x, rectP2.x);

    double ytop = min(rectP1.y, rectP2.y);
    double ybottom = max(rectP1.y, rectP2.y);

    Rectangle(hdc, (int)xleft, (int)ytop, (int)xright, (int)ybottom);

    if (pointIsInside(pt.x, pt.y, xleft, xright, ytop, ybottom))
    {
        SetPixel(hdc, pt.x, pt.y, color);
    }
}

void ClipLineRectangle(HDC hdc, POINT p1, POINT p2,
                              POINT rectP1, POINT rectP2, COLORREF color)
{
    double xleft = min(rectP1.x, rectP2.x);
    double xright = max(rectP1.x, rectP2.x);

    double ytop = min(rectP1.y, rectP2.y);
    double ybottom = max(rectP1.y, rectP2.y);

    Rectangle(hdc, (int)xleft, (int)ytop, (int)xright, (int)ybottom);

    double x1 = p1.x, y1 = p1.y;
    double x2 = p2.x, y2 = p2.y;

    if (CohenSuth(x1, y1, x2, y2, xleft, xright, ytop, ybottom))
    {
        DrawLineMidpoint(hdc, (int)round(x1), (int)round(y1),
                         (int)round(x2), (int)round(y2), color);
    }
}

struct Vertex
{
    double x, y;

    Vertex(double x1 = 0, double y1 = 0)
    {
        x = x1;
        y = y1;
    }
};

typedef vector<Vertex> VertexList;
typedef bool (*IsInFunc)(Vertex& v, int edge);
typedef Vertex (*IntersectFunc)(Vertex& v1, Vertex& v2, int edge);

VertexList ClipWithEdge(VertexList p, int edge, IsInFunc In, IntersectFunc Intersect)
{
    VertexList OutList;

    Vertex v1 = p[p.size() - 1];
    bool v1_in = In(v1, edge);

    for (int i = 0; i < (int)p.size(); i++)
    {
        Vertex v2 = p[i];
        bool v2_in = In(v2, edge);

        if (!v1_in && v2_in)
        {
            OutList.push_back(Intersect(v1, v2, edge));
            OutList.push_back(v2);
        }
        else if (v1_in && v2_in)
        {
            OutList.push_back(v2);
        }
        else if (v1_in)
        {
            OutList.push_back(Intersect(v1, v2, edge));
        }

        v1 = v2;
        v1_in = v2_in;
    }

    return OutList;
}

bool InLeft(Vertex& v, int edge)
{
    return v.x >= edge;
}

bool InRight(Vertex& v, int edge)
{
    return v.x <= edge;
}

bool InTop(Vertex& v, int edge)
{
    return v.y >= edge;
}

bool InBottom(Vertex& v, int edge)
{
    return v.y <= edge;
}

Vertex VIntersect(Vertex& v1, Vertex& v2, int xedge)
{
    Vertex res;

    res.x = xedge;
    res.y = v1.y + (xedge - v1.x) * (v2.y - v1.y) / (v2.x - v1.x);

    return res;
}

Vertex HIntersect(Vertex& v1, Vertex& v2, int yedge)
{
    Vertex res;

    res.y = yedge;
    res.x = v1.x + (yedge - v1.y) * (v2.x - v1.x) / (v2.y - v1.y);

    return res;
}

void ClipPolygonRectangle(HDC hdc, POINT* p, int n,
                          POINT rectP1, POINT rectP2, COLORREF color)
{
    int xleft = min(rectP1.x, rectP2.x);
    int xright = max(rectP1.x, rectP2.x);

    int ytop = min(rectP1.y, rectP2.y);
    int ybottom = max(rectP1.y, rectP2.y);

    Rectangle(hdc, xleft, ytop, xright, ybottom);

    VertexList vlist;

    for (int i = 0; i < n; i++)
        vlist.push_back(Vertex(p[i].x, p[i].y));

    vlist = ClipWithEdge(vlist, xleft, InLeft, VIntersect);
    vlist = ClipWithEdge(vlist, ytop, InTop, HIntersect);
    vlist = ClipWithEdge(vlist, xright, InRight, VIntersect);
    vlist = ClipWithEdge(vlist, ybottom, InBottom, HIntersect);

    if (vlist.empty()) return;

    Vertex v1 = vlist[vlist.size() - 1];

    for (int i = 0; i < (int)vlist.size(); i++)
    {
        Vertex v2 = vlist[i];

        DrawLineMidpoint(hdc,
                         (int)round(v1.x), (int)round(v1.y),
                         (int)round(v2.x), (int)round(v2.y),
                         color);

        v1 = v2;
    }
}

void ClipPointSquare(HDC hdc, POINT pt, POINT center,
                     POINT corner, COLORREF color)
{
    int side = max(abs(corner.x - center.x), abs(corner.y - center.y));

    double xleft = center.x - side;
    double xright = center.x + side;

    double ytop = center.y - side;
    double ybottom = center.y + side;

    Rectangle(hdc, (int)xleft, (int)ytop, (int)xright, (int)ybottom);

    if (pointIsInside(pt.x, pt.y, xleft, xright, ytop, ybottom))
    {
        SetPixel(hdc, pt.x, pt.y, color);
    }
}

void ClipLineSquare(HDC hdc, POINT p1, POINT p2,
                           POINT center, POINT corner, COLORREF color)
{
    int side = max(abs(corner.x - center.x), abs(corner.y - center.y));

    double xleft = center.x - side;
    double xright = center.x + side;

    double ytop = center.y - side;
    double ybottom = center.y + side;

    Rectangle(hdc, (int)xleft, (int)ytop, (int)xright, (int)ybottom);

    double x1 = p1.x, y1 = p1.y;
    double x2 = p2.x, y2 = p2.y;

    if (CohenSuth(x1, y1, x2, y2, xleft, xright, ytop, ybottom))
    {
        DrawLineMidpoint(hdc, (int)round(x1), (int)round(y1),
                         (int)round(x2), (int)round(y2), color);
    }
}

void ClipPointCircle(HDC hdc, POINT pt, POINT center,
                     POINT p2, COLORREF color)
{
    int radius = (int)sqrt((p2.x - center.x) * (p2.x - center.x) +
                           (p2.y - center.y) * (p2.y - center.y));

    vector<POINT> points;
    points.push_back(center);

    CircleFasterBresenham(hdc, points, radius, color);

    int dx = pt.x - center.x;
    int dy = pt.y - center.y;

    if (dx * dx + dy * dy <= radius * radius)
    {
        SetPixel(hdc, pt.x, pt.y, color);
    }
}

void ClipLineCircle(HDC hdc, POINT p1, POINT p2,
                    POINT center, POINT pOnCircle,
                    COLORREF color)
{
    int radius = (int)sqrt((pOnCircle.x - center.x) * (pOnCircle.x - center.x) +
                           (pOnCircle.y - center.y) * (pOnCircle.y - center.y));

    vector<POINT> points;
    points.push_back(center);

    CircleBresenham(hdc, points, radius, color);

    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;

    int steps = max(abs(dx), abs(dy));

    double xInc = (double)dx / steps;
    double yInc = (double)dy / steps;

    double x = p1.x;
    double y = p1.y;

    for (int i = 0; i <= steps; i++)
    {
        int xx = round(x) - center.x;
        int yy = round(y) - center.y;

        // inside circle
        if (xx * xx + yy * yy <= radius * radius)
        {
            SetPixel(hdc, round(x), round(y), color);
        }

        x += xInc;
        y += yInc;
    }
}



#endif