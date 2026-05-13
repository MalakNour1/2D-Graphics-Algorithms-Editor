#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <windows.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include "menu_id.h"
#include "Circle.h"
#include "Ellipse.h"
#include "Structs.h"
#include "Curve.h"

using namespace std;
void EventDispatcher(HWND hwnd, vector<POINT>& points, int currentWmId, COLORREF color) {
    if (currentWmId == 0) return;

    HDC hdc = GetDC(hwnd);
    size_t n = points.size();
/**
    the Logic here is we categorize them by needed points,
    and inside each case we make another switch case for the Event ID.
    Once we are in the Event ID Case, we call the function to draw, then we save:
    {the ID, the points used, the color used, and the needed number of points (n)}
    to a vector called DrawnShapes, to save the data of the drawn shapes in case we need to save them into a file
    then we clear the points vector for the next shape.
**/
    switch (n) { //to categorize by required Points needed to Draw
       case 1:{
        break;
       }
       case 2:{
           switch (currentWmId) {
              case ID_CIRCLE_DIRECT:{
                // Calculate Radius
                POINT center = points[0];
                POINT p2 = points[1];
                int R = round(sqrt(pow(p2.x - center.x, 2) + pow(p2.y - center.y, 2)));
                if (R == 0) R = 1; // Prevent invisible circles
                CircleDirect(hdc, points, R, drawingColor);
                Shape S = {currentWmId, points, drawingColor, n};
                drawnShapes.push_back(S);
                points.clear();
                break;
              }
              case ID_CIRCLE_POLAR:{
                // Calculate Radius
                POINT center = points[0];
                POINT p2 = points[1];
                int R = round(sqrt(pow(p2.x - center.x, 2) + pow(p2.y - center.y, 2)));
                if (R == 0) R = 1; // Prevent invisible circles
                CirclePolar(hdc, points, R, drawingColor);
                Shape S = {currentWmId, points, drawingColor, n};
                drawnShapes.push_back(S);
                points.clear();
                break;
              }
              case ID_CIRCLE_ITER_POLAR:{
                // Calculate Radius
                POINT center = points[0];
                POINT p2 = points[1];
                int R = round(sqrt(pow(p2.x - center.x, 2) + pow(p2.y - center.y, 2)));
                if (R == 0) R = 1; // Prevent invisible circles
                CircleIterativePolar(hdc, points, R, drawingColor);
                Shape S = {currentWmId, points, drawingColor, n};
                drawnShapes.push_back(S);
                points.clear();
                break;
              }
              case ID_CIRCLE_MIDPOINT:{
                // Calculate Radius
                POINT center = points[0];
                POINT p2 = points[1];
                int R = round(sqrt(pow(p2.x - center.x, 2) + pow(p2.y - center.y, 2)));
                if (R == 0) R = 1; // Prevent invisible circles
                CircleBresenham(hdc, points, R, drawingColor);
                Shape S = {currentWmId, points, drawingColor, n};
                drawnShapes.push_back(S);
                points.clear();
                break;
              }
              case ID_CIRCLE_MOD_MIDPOINT:{
                // Calculate Radius
                POINT center = points[0];
                POINT p2 = points[1];
                int R = round(sqrt(pow(p2.x - center.x, 2) + pow(p2.y - center.y, 2)));
                if (R == 0) R = 1; // Prevent invisible circles
                CircleFasterBresenham(hdc, points, R, drawingColor);
                Shape S = {currentWmId, points, drawingColor, n};
                drawnShapes.push_back(S);
                points.clear();
                break;
              }
              case ID_ELLIPSE_DIRECT: {
                POINT center = points[0];
                POINT p2 = points[1];
                int rX = p2.x, rY = p2.y;
                POINT newP2 = {abs(rX - center.x), abs(rY - center.y)};
                points.pop_back();
                points.push_back(newP2);
                EllipseDirect(hdc, points, drawingColor);
                Shape S = {currentWmId, points, drawingColor, n};
                drawnShapes.push_back(S);
                points.clear();
                break;
              }
              case ID_ELLIPSE_POLAR: {
                POINT center = points[0];
                POINT p2 = points[1];
                int rX = p2.x, rY = p2.y;
                POINT newP2 = {abs(rX - center.x), abs(rY - center.y)};
                points.pop_back();
                points.push_back(newP2);
                EllipsePolar(hdc, points, drawingColor);
                Shape S = {currentWmId, points, drawingColor, n};
                drawnShapes.push_back(S);
                points.clear();
                break;
              }
              case ID_ELLIPSE_MIDPOINT: {
                POINT center = points[0];
                POINT p2 = points[1];
                int rX = p2.x, rY = p2.y;
                POINT newP2 = {abs(rX - center.x), abs(rY - center.y)};
                points.pop_back();
                points.push_back(newP2);
                EllipseMidpoint(hdc, points, drawingColor);
                Shape S = {currentWmId, points, drawingColor, n};
                drawnShapes.push_back(S);
                points.clear();
                break;
              }
           }
        break;
       }
       case 3:{
        break;
       }
       case 4:{
          switch (currentWmId) {
                case ID_CURVE_CARDINAL:{
                  double c = 0.5; // Tension parameter
                  DrawCardinalSpline(hdc, points, c, 1000, drawingColor);
                  Shape S = {currentWmId, points, drawingColor, n};
                  drawnShapes.push_back(S);
                  points.clear();
                  break;
                }
          }
        break;
       }

    }

    ReleaseDC(hwnd, hdc);
}
#endif // EVENTDISPATCHER_H
