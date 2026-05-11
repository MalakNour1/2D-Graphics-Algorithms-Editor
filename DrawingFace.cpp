#include <windows.h>
#include <cmath>
#include <algorithm>

#define M_PI 3.14159265358979323846

POINT center;
bool centerSet = false;

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color) {
    SetPixel(hdc, xc + x, yc + y, color);
    SetPixel(hdc, xc - x, yc + y, color);
    SetPixel(hdc, xc - x, yc - y, color);
    SetPixel(hdc, xc + x, yc - y, color);
    SetPixel(hdc, xc + y, yc + x, color);
    SetPixel(hdc, xc - y, yc + x, color);
    SetPixel(hdc, xc - y, yc - x, color);
    SetPixel(hdc, xc + y, yc - x, color);
}

void CirclePolar(HDC hdc, int xc, int yc, int R, COLORREF color) {
    if (R == 0) return;
    int x = R, y = 0;
    double theta = 0, dtheta = 1.0 / R;
    Draw8Points(hdc, xc, yc, x, y, color);
    while (x > y) {
        theta += dtheta;
        x = round(R * cos(theta));
        y = round(R * sin(theta));
        Draw8Points(hdc, xc, yc, x, y, color);
    }
}

void DrawFaceAndEyes(HDC hdc, int xc, int yc, int R, COLORREF color) {
    CirclePolar(hdc, xc, yc, R, color);

    int eyeRadius = R / 4;
    int eyeDistance = R / 2;

    double rightRadians = 45.0 * (M_PI / 180.0);
    int rightEyeX = round(xc + eyeDistance * cos(rightRadians));
    int rightEyeY = round(yc - eyeDistance * sin(rightRadians));
    CirclePolar(hdc, rightEyeX, rightEyeY, eyeRadius, color);

    double leftRadians = 135.0 * (M_PI / 180.0);
    int leftEyeX = round(xc + eyeDistance * cos(leftRadians));
    int leftEyeY = round(yc - eyeDistance * sin(leftRadians));
    CirclePolar(hdc, leftEyeX, leftEyeY, eyeRadius, color);
}

LRESULT WINAPI WndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp) {
    switch (mcode) {
        case WM_LBUTTONDOWN:
            if (!centerSet) {
                // Click 1: Set the Face Center
                center.x = LOWORD(lp);
                center.y = HIWORD(lp);
                centerSet = true;
            } 
            else {
                // Click 2: Calculate Face Radius and Draw!
                int rX = LOWORD(lp);
                int rY = HIWORD(lp);

                // Distance formula for radius
                int R = round(sqrt(pow(rX - center.x, 2) + pow(rY - center.y, 2)));

                HDC hdc = GetDC(hwnd);
                
                // Draw everything in Yellow
                DrawFaceAndEyes(hdc, center.x, center.y, R, RGB(255, 255, 0));

                GdiFlush(); // Force screen update for Wine
                ReleaseDC(hwnd, hdc);

                // Reset so you can draw more faces!
                centerSet = false;
            }
            break;

        case WM_CLOSE: DestroyWindow(hwnd); break;
        case WM_DESTROY: PostQuitMessage(0); break;
        default: return DefWindowProc(hwnd, mcode, wp, lp);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE h, HINSTANCE, LPSTR, int nsh) {
    WNDCLASS wc = {};
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = h;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "SmileyFaceClass";
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
        "SmileyFaceClass", 
        "Smiley Face (Click Center -> Click Radius)", 
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, h, NULL
    );

    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}