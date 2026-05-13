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
#include "Line.h"
#include "Clipping.h"
#include "Fill.h"
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
              case ID_LINE_PARAMETRIC:
              {
                  DrawLineParametric(hdc, points[0].x, points[0].y, points[1].x, points[1].y, drawingColor);
                  Shape S = {currentWmId, points, drawingColor, (int)n};
                  drawnShapes.push_back(S);
                  points.clear();
                  break;
              }
              case ID_LINE_MIDPOINT:
              {
                  DrawLineMidpoint(hdc, points[0].x, points[0].y, points[1].x, points[1].y, drawingColor);
                  Shape S = {currentWmId, points, drawingColor, (int)n};
                  drawnShapes.push_back(S);
                  points.clear();
                  break;
              }
           }
        break;
       }
       case 3:{
         switch(currentWmId)
         {
           case ID_CLIP_RECT_POINT:
           {
             ClipPointRectangle(hdc,
                                points[0],
                                points[1],
                                points[2],
                                drawingColor);

             Shape S = {currentWmId, points, drawingColor, (int)n};
             drawnShapes.push_back(S);

             points.clear();
             break;
           }

           case ID_CLIP_SQUARE_POINT:
           {
             ClipPointSquare(hdc,
                             points[0],
                             points[1],
                             points[2],
                             drawingColor);

             Shape S = {currentWmId, points, drawingColor, (int)n};
             drawnShapes.push_back(S);

             points.clear();
             break;
           }

           case ID_CLIP_CIRCLE_POINT:
           {
             ClipPointCircle(hdc,
                             points[0],
                             points[1],
                             points[2],
                             drawingColor);

             Shape S = {currentWmId, points, drawingColor, (int)n};
             drawnShapes.push_back(S);

             points.clear();
             break;
           }
           case ID_FILL_CIRC_LINE:
            {
                POINT center = points[0];
                POINT edge = points[1];
                POINT click = points[2];

                int R = (int)round(sqrt(pow(edge.x - center.x, 2) + pow(edge.y - center.y, 2)));
                if (R == 0) R = 1;

                int quarter = 0;
                if (click.x >= center.x && click.y <= center.y) quarter = 1;
                else if (click.x < center.x && click.y <= center.y) quarter = 2;
                else if (click.x < center.x && click.y > center.y) quarter = 3;
                else quarter = 4;

                Shape S = {currentWmId, points, drawingColor, (int)points.size()};

                CircleBresenham(hdc, points, R, drawingColor);
                FillCircleQuarterScanLine(hdc, center.x, center.y, R, quarter, drawingColor);


                drawnShapes.push_back(S);

                points.clear();
                break;
            }

           case ID_FILL_SQ_HERMITE:
           {
             POINT p1 = points[0];
             POINT p2 = points[1];

             int left = min(p1.x, p2.x);
             int top = min(p1.y, p2.y);
             int size = min(abs(p2.x - p1.x), abs(p2.y - p1.y));

             Rectangle(hdc, left, top, left + size, top + size);

             FillSquareHermiteVertical(hdc, left, top, size, drawingColor);

             Shape S = {currentWmId, points, drawingColor, (int)points.size()};
             drawnShapes.push_back(S);

             points.clear();
             break;
           }

           case ID_FILL_RECT_BEZIER:
            {
                POINT p1 = points[0];
                POINT p2 = points[1];

                int left = min(p1.x, p2.x);
                int top = min(p1.y, p2.y);
                int width = abs(p2.x - p1.x);
                int height = abs(p2.y - p1.y);

                Rectangle(hdc, left, top, left + width, top + height);
                FillRectangleBezierHorizontal(hdc, left, top, width, height, drawingColor);

                Shape S = {currentWmId, points, drawingColor, (int)points.size()};
                drawnShapes.push_back(S);
                points.clear();
            }
            break;
         }
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
            case ID_CLIP_RECT_LINE:
                {
                  ClipLineRectangle(hdc,
                                    points[0],
                                    points[1],
                                    points[2],
                                    points[3],
                                    drawingColor);

                  Shape S = {CurrentMenuID, points, drawingColor, (int)points.size()};
                  drawnShapes.push_back(S);

                  points.clear();
                  break;
                }

            case ID_CLIP_SQUARE_LINE:
                {
                  ClipLineSquare(hdc,
                                 points[0],
                                 points[1],
                                 points[2],
                                 points[3],
                                 drawingColor);

                  Shape S = {currentWmId, points, drawingColor, (int)n};
                  drawnShapes.push_back(S);

                  points.clear();
                  break;
                }

            case ID_CLIP_CIRCLE_LINE:
                {
                  ClipLineCircle(hdc,
                                 points[0],
                                 points[1],
                                 points[2],
                                 points[3],
                                 drawingColor);

                  Shape S = {currentWmId, points, drawingColor, (int)n};
                  drawnShapes.push_back(S);

                  points.clear();
                  break;
                }
          }
        break;
       }
      case 7:
       {
         switch (currentWmId)
         {
           case ID_CLIP_RECT_POLY:
           {
             if (points.size() != 7)
               return;

             // Rectangle points
             POINT rectP1 = points[0];
             POINT rectP2 = points[1];

             //Polygon points
             POINT polygon[5] =
             {
               points[2],
               points[3],
               points[4],
               points[5],
               points[6]
           };

             ClipPolygonRectangle(
                 hdc,
                 polygon,
                 5,
                 rectP1,
                 rectP2,
                 drawingColor
             );

             Shape S = {currentWmId, points, drawingColor, 7};
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
