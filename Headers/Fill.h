#ifndef Fill_H
#define Fill_H

#include <windows.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <list>

#include "menu_id.h"
#include "Structs.h"

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



#endif // Fill_H
