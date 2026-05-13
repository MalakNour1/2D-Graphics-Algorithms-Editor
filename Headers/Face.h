#ifndef FACE_H
#define FACE_H

#include <windows.h>
#include <cmath>
#include <algorithm>
#include "Circle.h"
#include "Line.h"
#include "Curve.h"


inline void DrawHappyFace(HDC hdc, POINT center, int R, COLORREF color)
{
    vector<POINT> facePoints;
    facePoints.push_back(center);

    POINT radiusPoint = {center.x + R, center.y};
    facePoints.push_back(radiusPoint);

    CirclePolar(hdc, facePoints, R, color);

    int eyeRadius = R / 4;
    int eyeDistance = R / 2;

    double rightRadians = 45.0 * (M_PI / 180.0);
    int rightEyeX = round(center.x + eyeDistance * cos(rightRadians));
    int rightEyeY = round(center.y - eyeDistance * sin(rightRadians));
    vector<POINT> rightEye;
    POINT rightCenter = {rightEyeX, rightEyeY};
    POINT rightRadius = {rightEyeX + eyeRadius, rightEyeY};

    rightEye.push_back(rightCenter);
    rightEye.push_back(rightRadius);

    CirclePolar(hdc, rightEye, eyeRadius, color);

    double leftRadians = 135.0 * (M_PI / 180.0);
    int leftEyeX = round(center.x + eyeDistance * cos(leftRadians));
    int leftEyeY = round(center.y - eyeDistance * sin(leftRadians));
    vector<POINT> leftEye;
    POINT leftCenter = {leftEyeX, leftEyeY};
    POINT leftRadius = {leftEyeX + eyeRadius, leftEyeY};

    leftEye.push_back(leftCenter);
    leftEye.push_back(leftRadius);

    CirclePolar(hdc, leftEye, eyeRadius, color);

    DrawLineMidpoint(
        hdc,
        center.x,
        center.y - R/8,
        center.x - R/8,
        center.y + R/8,
        color
    );

    DrawLineMidpoint(
        hdc,
        center.x - R/8,
        center.y + R/8,
        center.x + R/8,
        center.y + R/8,
        color
    );

    Vector2 P0(center.x - R/3, center.y + R/6);
    Vector2 P1(center.x + R/3, center.y + R/6);

    Vector2 T0(R/2, R/2);
    Vector2 T1(R/2, -R/2);

    DrawHermiteCurve(
        hdc,
        P0,
        T0,
        P1,
        T1,
        100,
        color
    );
}

inline void DrawSadFace(HDC hdc, POINT center, int R, COLORREF color)
{
    vector<POINT> facePoints;
    facePoints.push_back(center);

    POINT radiusPoint = {center.x + R, center.y};
    facePoints.push_back(radiusPoint);

    CirclePolar(hdc, facePoints, R, color);

    int eyeRadius = R / 4;
    int eyeDistance = R / 2;

    double rightRadians = 45.0 * (M_PI / 180.0);
    int rightEyeX = round(center.x + eyeDistance * cos(rightRadians));
    int rightEyeY = round(center.y - eyeDistance * sin(rightRadians));
    vector<POINT> rightEye;
    POINT rightCenter = {rightEyeX, rightEyeY};
    POINT rightRadius = {rightEyeX + eyeRadius, rightEyeY};

    rightEye.push_back(rightCenter);
    rightEye.push_back(rightRadius);

    CirclePolar(hdc, rightEye, eyeRadius, color);

    double leftRadians = 135.0 * (M_PI / 180.0);
    int leftEyeX = round(center.x + eyeDistance * cos(leftRadians));
    int leftEyeY = round(center.y - eyeDistance * sin(leftRadians));
    vector<POINT> leftEye;
    POINT leftCenter = {leftEyeX, leftEyeY};
    POINT leftRadius = {leftEyeX + eyeRadius, leftEyeY};

    leftEye.push_back(leftCenter);
    leftEye.push_back(leftRadius);
    CirclePolar(hdc, leftEye, eyeRadius, color);

    DrawLineMidpoint(
        hdc,
        center.x,
        center.y - R/8,
        center.x - R/8,
        center.y + R/8,
        color
    );

    DrawLineMidpoint(
        hdc,
        center.x - R/8,
        center.y + R/8,
        center.x + R/8,
        center.y + R/8,
        color
    );

    Vector2 P0(center.x - R/3, center.y + R/2);
    Vector2 P1(center.x + R/3, center.y + R/2);

    Vector2 T0(R/2, -R/2);
    Vector2 T1(R/2, R/2);

    DrawHermiteCurve(
        hdc,
        P0,
        T0,
        P1,
        T1,
        100,
        color
    );
}

#endif // FACE_H
