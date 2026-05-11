#include <windows.h>
#include <cmath>

#define M_PI 3.14159265358979323846

// Menu IDs for the 5 algorithms
#define ID_ALGO_DIRECT 1
#define ID_ALGO_POLAR 2
#define ID_ALGO_ITER_POLAR 3
#define ID_ALGO_BRESENHAM 4
#define ID_ALGO_FAST_BRESENHAM 5

POINT center;
bool centerSet = false;
int currentAlgorithm = ID_ALGO_DIRECT; // Default algorithm

void Draw8Points(HDC hdc, int xc, int yc, int a, int b, COLORREF color) {
    SetPixel(hdc, xc + a, yc + b, color);
    SetPixel(hdc, xc - a, yc + b, color);
    SetPixel(hdc, xc - a, yc - b, color);
    SetPixel(hdc, xc + a, yc - b, color);
    SetPixel(hdc, xc + b, yc + a, color);
    SetPixel(hdc, xc - b, yc + a, color);
    SetPixel(hdc, xc - b, yc - a, color);
    SetPixel(hdc, xc + b, yc - a, color);
}

void CircleDirect(HDC hdc, int xc, int yc, int R, COLORREF color) {
    int x = 0, y = R;
    int R2 = R * R;
    Draw8Points(hdc, xc, yc, x, y, color);
    while (x < y) {
        x++;
        y = round(sqrt((double)(R2 - x * x)));
        Draw8Points(hdc, xc, yc, x, y, color);
    }
}

void CirclePolar(HDC hdc, int xc, int yc, int R, COLORREF color) {
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

void CircleIterativePolar(HDC hdc, int xc, int yc, int R, COLORREF color) {
    double x = R, y = 0;
    double dtheta = 1.0 / R;
    double cdtheta = cos(dtheta), sdtheta = sin(dtheta);
    Draw8Points(hdc, xc, yc, R, 0, color);
    while (x > y) {
        double x1 = x * cdtheta - y * sdtheta;
        y = x * sdtheta + y * cdtheta;
        x = x1;
        Draw8Points(hdc, xc, yc, round(x), round(y), color);
    }
}

void CircleBresenham(HDC hdc, int xc, int yc, int R, COLORREF color) {
    int x = 0, y = R;
    int d = 1 - R;
    Draw8Points(hdc, xc, yc, x, y, color);
    while (x < y) {
        if (d < 0)
            d += 2 * x + 2;
        else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
        Draw8Points(hdc, xc, yc, x, y, color);
    }
}

void CircleFasterBresenham(HDC hdc, int xc, int yc, int R, COLORREF color) {
    int x = 0, y = R;
    int d = 1 - R;
    int c1 = 3, c2 = 5 - 2 * R;
    Draw8Points(hdc, xc, yc, x, y, color);
    while (x < y) {
        if (d < 0) {
            d += c1;
            c2 += 2;
        } else {
            d += c2;
            c2 += 4;
            y--;
        }
        c1 += 2;
        x++;
        Draw8Points(hdc, xc, yc, x, y, color);
    }
}

LRESULT WINAPI WndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp) {
    switch (mcode) {
        case WM_CREATE: {
            // Create the top menu bar
            HMENU hMenu = CreateMenu();
            HMENU hSubMenu = CreatePopupMenu();

            AppendMenu(hSubMenu, MF_STRING, ID_ALGO_DIRECT, "1. Direct Cartesian");
            AppendMenu(hSubMenu, MF_STRING, ID_ALGO_POLAR, "2. Polar");
            AppendMenu(hSubMenu, MF_STRING, ID_ALGO_ITER_POLAR, "3. Iterative Polar");
            AppendMenu(hSubMenu, MF_STRING, ID_ALGO_BRESENHAM, "4. Bresenham");
            AppendMenu(hSubMenu, MF_STRING, ID_ALGO_FAST_BRESENHAM, "5. Faster Bresenham");

            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Algorithms");
            SetMenu(hwnd, hMenu);
            break;
        }

        case WM_COMMAND:
            // Handle menu selections
            if (LOWORD(wp) >= ID_ALGO_DIRECT && LOWORD(wp) <= ID_ALGO_FAST_BRESENHAM) {
                currentAlgorithm = LOWORD(wp);
            }
            break;

        case WM_LBUTTONDOWN:
            // First Click: Set Center
            center.x = LOWORD(lp);
            center.y = HIWORD(lp);
            centerSet = true;
            break;

        case WM_RBUTTONDOWN:
            // Second Click: Calculate Radius and Draw
            if (centerSet) {
                int rX = LOWORD(lp);
                int rY = HIWORD(lp);

                // Calculate Radius
                int R = round(sqrt(pow(rX - center.x, 2) + pow(rY - center.y, 2)));
                if (R == 0) R = 1; // Prevent invisible circles

                HDC hdc = GetDC(hwnd);

                // Draw based on selected algorithm
                switch (currentAlgorithm) {
                    case ID_ALGO_DIRECT: CircleDirect(hdc, center.x, center.y, R, RGB(255, 0, 0)); break; // Red
                    case ID_ALGO_POLAR: CirclePolar(hdc, center.x, center.y, R, RGB(0, 255, 0)); break; // Green
                    case ID_ALGO_ITER_POLAR: CircleIterativePolar(hdc, center.x, center.y, R, RGB(0, 0, 255)); break; // Blue
                    case ID_ALGO_BRESENHAM: CircleBresenham(hdc, center.x, center.y, R, RGB(255, 255, 0)); break; // Yellow
                    case ID_ALGO_FAST_BRESENHAM: CircleFasterBresenham(hdc, center.x, center.y, R, RGB(255, 0, 255)); break; // Magenta
                }

                GdiFlush(); // CRITICAL FOR WINE: Forces screen update immediately
                ReleaseDC(hwnd, hdc);

                centerSet = false; // Reset for the next pair of clicks
            }
            break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, mcode, wp, lp);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE h, HINSTANCE, LPSTR, int nsh) {
    WNDCLASS wc = {};
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = h;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "CircleAlgorithmsClass";
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
        "CircleAlgorithmsClass",
        "Select Algorithm from Menu -> Left Click (Center) -> Right Click (Radius)",
        WS_OVERLAPPEDWINDOW,
        100, 100, 800, 600,
        NULL, NULL, h, NULL
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