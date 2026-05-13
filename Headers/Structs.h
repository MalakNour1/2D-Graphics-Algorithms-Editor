#ifndef STRUCT_H
#define STRUCT_H

// standard system libraries
#include <windows.h>
#include <vector>

using namespace std;

struct Shape {
    int MenuID;
    vector<POINT> points;
    COLORREF color;
    int needed_points;
};
vector<Shape> drawnShapes; // to store drawn shapes for save/load Files
vector<POINT> points;
int x, y;
int CurrentMenuID;
COLORREF drawingColor = RGB(0, 0, 0); // Shape/Fill Color

#endif // STRUCT_H

