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
struct Vector2 {
    double x, y;

    Vector2(double a = 0, double b = 0)
    {
        x = a;
        y = b;
    }
};
class Vector4 {
    double v[4];

public:

    Vector4(double a=0,double b=0,double c=0,double d=0)
    {
        v[0]=a;
        v[1]=b;
        v[2]=c;
        v[3]=d;
    }

    double& operator[](int i)
    {
        return v[i];
    }
};
class Matrix4 {
    Vector4 M[4];

public:

    Matrix4(double A[16])
    {
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
                M[i][j] = A[i*4+j];
    }

    Vector4& operator[](int i)
    {
        return M[i];
    }
};

struct Entry
{
    int xmin;
    int xmax;
};

struct EdgeRec
{
    double x;
    double minv;
    int ymax;

    bool operator<(EdgeRec r)
    {
        return x < r.x;
    }
};

#endif // STRUCT_H

