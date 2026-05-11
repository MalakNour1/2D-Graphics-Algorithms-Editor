#include <windows.h>
#include <cmath>
#include <algorithm>

#define M_PI 3.14159265358979323846

#define ID_ALGO_ELLIPSE_DIRECT 1
#define ID_ALGO_ELLIPSE_POLAR 2
#define ID_ALGO_ELLIPSE_MIDPOINT 3

POINT center;
bool centerSet = false;
int currentAlgorithm = ID_ALGO_ELLIPSE_DIRECT;

void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color) {
    SetPixel(hdc, xc + x, yc + y, color); 
    SetPixel(hdc, xc - x, yc + y, color); 
    SetPixel(hdc, xc - x, yc - y, color); 
    SetPixel(hdc, xc + x, yc - y, color); 
}

void EllipseDirect(HDC hdc, int xc, int yc, int Rx, int Ry, COLORREF color) {
    long long Rx2 = Rx * Rx;
    long long Ry2 = Ry * Ry;
    int x = 0;
    double y = Ry;
    Draw4Points(hdc, xc, yc, x, round(y), color);
    while ((Ry2 * x) < (Rx2 * y)) {
        x++;
        y = Ry * sqrt(1.0 - (double)(x * x) / Rx2);
        Draw4Points(hdc, xc, yc, x, round(y), color);
    }
    y = 0;
    double x_val = Rx;
    Draw4Points(hdc, xc, yc, round(x_val), y, color);
    while ((Rx2 * y) <= (Ry2 * x_val)) {
        y++;
        x_val = Rx * sqrt(1.0 - (double)(y * y) / Ry2);
        Draw4Points(hdc, xc, yc, round(x_val), y, color);
    }
}

void EllipsePolar(HDC hdc, int xc, int yc, int Rx, int Ry, COLORREF color) {
    double theta = 0;
    double dtheta = 1.0 / std::max(Rx, Ry); 
    while (theta <= M_PI / 2.0) {
        int x = round(Rx * cos(theta));
        int y = round(Ry * sin(theta));
        Draw4Points(hdc, xc, yc, x, y, color);
        theta += dtheta;
    }
}

void EllipseMidpoint(HDC hdc, int xc, int yc, int Rx, int Ry, COLORREF color) {
    long long Rx2 = Rx * Rx;
    long long Ry2 = Ry * Ry;
    long long twoRx2 = 2 * Rx2;
    long long twoRy2 = 2 * Ry2;
    int x = 0;
    int y = Ry;
    long long px = 0;
    long long py = twoRx2 * y;

    Draw4Points(hdc, xc, yc, x, y, color);
    long long p1 = round(Ry2 - (Rx2 * Ry) + (0.25 * Rx2));
    while (px < py) {
        x++;
        px += twoRy2;
        if (p1 < 0) p1 += Ry2 + px;
        else { y--; py -= twoRx2; p1 += Ry2 + px - py; }
        Draw4Points(hdc, xc, yc, x, y, color);
    }
    long long p2 = round(Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2);
    while (y > 0) {
        y--;
        py -= twoRx2;
        if (p2 > 0) p2 += Rx2 - py;
        else { x++; px += twoRy2; p2 += Rx2 - py + px; }
        Draw4Points(hdc, xc, yc, x, y, color);
    }
}

LRESULT WINAPI WndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp) {
    switch (mcode) {
        case WM_CREATE: {
            HMENU hMenu = CreateMenu();
            HMENU hSubMenu = CreatePopupMenu();
            AppendMenu(hSubMenu, MF_STRING, ID_ALGO_ELLIPSE_DIRECT, "1. Direct");
            AppendMenu(hSubMenu, MF_STRING, ID_ALGO_ELLIPSE_POLAR, "2. Polar");
            AppendMenu(hSubMenu, MF_STRING, ID_ALGO_ELLIPSE_MIDPOINT, "3. Midpoint");
            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Algorithms");
            SetMenu(hwnd, hMenu);
            break;
        }
        case WM_COMMAND:
            if (LOWORD(wp) >= ID_ALGO_ELLIPSE_DIRECT && LOWORD(wp) <= ID_ALGO_ELLIPSE_MIDPOINT)
                currentAlgorithm = LOWORD(wp);
            break;
        case WM_LBUTTONDOWN:
            if (!centerSet) {
                center.x = LOWORD(lp); center.y = HIWORD(lp);
                centerSet = true;
            } else {
                int rX = LOWORD(lp); int rY = HIWORD(lp);
                int Rx = abs(rX - center.x); int Ry = abs(rY - center.y);
                if (Rx == 0) Rx = 1; if (Ry == 0) Ry = 1;

                HDC hdc = GetDC(hwnd);
                switch (currentAlgorithm) {
                    case ID_ALGO_ELLIPSE_DIRECT: EllipseDirect(hdc, center.x, center.y, Rx, Ry, RGB(255, 0, 0)); break;
                    case ID_ALGO_ELLIPSE_POLAR: EllipsePolar(hdc, center.x, center.y, Rx, Ry, RGB(0, 255, 0)); break;
                    case ID_ALGO_ELLIPSE_MIDPOINT: EllipseMidpoint(hdc, center.x, center.y, Rx, Ry, RGB(0, 255, 255)); break;
                }
                GdiFlush();
                ReleaseDC(hwnd, hdc);
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
    wc.lpszClassName = "EllipseClass";
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);

    HWND hwnd = CreateWindow("EllipseClass", "Ellipse Tasks", WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, h, NULL);
    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); }
    return 0;
}