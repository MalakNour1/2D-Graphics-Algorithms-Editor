#include <windows.h>
#include <stack> // Required for the Non-Recursive stack

#define ID_ALGO_RECURSIVE 1
#define ID_ALGO_NON_RECURSIVE 2

int currentAlgorithm = ID_ALGO_RECURSIVE;
int clickState = 0;
POINT p1, p2;

void FloodFill(HDC hdc, int x, int y, COLORREF Cb, COLORREF Cf) {
    COLORREF C = GetPixel(hdc, x, y);
    if (C == Cb || C == Cf)
    {
        return;
    }

    SetPixel(hdc, x, y, Cf); // FIXED: Capitalized Cf

    FloodFill(hdc, x + 1, y, Cb, Cf);
    FloodFill(hdc, x - 1, y, Cb, Cf);
    FloodFill(hdc, x, y + 1, Cb, Cf);
    FloodFill(hdc, x, y - 1, Cb, Cf);
}

struct Vertex {
    int x, y;
    Vertex(int x, int y) : x(x), y(y) {}
};

void NRFloodFill(HDC hdc, int x, int y, COLORREF Cb, COLORREF Cf) {
    std::stack<Vertex> S;
    S.push(Vertex(x, y));

    while (!S.empty()) {
        Vertex v = S.top();
        S.pop();

        COLORREF c = GetPixel(hdc, v.x, v.y);

        if (c == Cb || c == Cf) continue;

        SetPixel(hdc, v.x, v.y, Cf);

        S.push(Vertex(v.x + 1, v.y));
        S.push(Vertex(v.x - 1, v.y));
        S.push(Vertex(v.x, v.y + 1));
        S.push(Vertex(v.x, v.y - 1));
    }
}

void DrawBoundaryBox(HDC hdc, int x1, int y1, int x2, int y2) {
    HPEN whitePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    HBRUSH hollowBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    
    SelectObject(hdc, whitePen);
    SelectObject(hdc, hollowBrush);
    
    Rectangle(hdc, x1, y1, x2, y2);
    
    DeleteObject(whitePen);
}

LRESULT WINAPI WndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp) {
    switch (mcode) {
        case WM_CREATE: {
            HMENU hMenu = CreateMenu();
            HMENU hSubMenu = CreatePopupMenu();

            AppendMenu(hSubMenu, MF_STRING, ID_ALGO_RECURSIVE, "1. Recursive (Small shapes only)");
            AppendMenu(hSubMenu, MF_STRING, ID_ALGO_NON_RECURSIVE, "2. Non-Recursive (Stack)");

            AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Algorithms");
            SetMenu(hwnd, hMenu);
            break;
        }

        case WM_COMMAND:
            if (LOWORD(wp) == ID_ALGO_RECURSIVE || LOWORD(wp) == ID_ALGO_NON_RECURSIVE) {
                currentAlgorithm = LOWORD(wp);
            }
            break;

        case WM_LBUTTONDOWN: {
            HDC hdc = GetDC(hwnd);

            if (clickState == 0) {
                // Click 1: Top Left of boundary
                p1.x = LOWORD(lp);
                p1.y = HIWORD(lp);
                clickState = 1;
            } 
            else if (clickState == 1) {
                // Click 2: Bottom Right of boundary, draw the box!
                p2.x = LOWORD(lp);
                p2.y = HIWORD(lp);
                DrawBoundaryBox(hdc, p1.x, p1.y, p2.x, p2.y);
                GdiFlush();
                clickState = 2;
            } 
            else if (clickState == 2) {
                // Click 3: Seed point to fill
                int startX = LOWORD(lp);
                int startY = HIWORD(lp);

                COLORREF boundaryColor = RGB(255, 255, 255); // White outline
                COLORREF fillColor = RGB(0, 255, 0); // Green paint

                if (currentAlgorithm == ID_ALGO_RECURSIVE) {
                    FloodFill(hdc, startX, startY, boundaryColor, fillColor);
                } else {
                    NRFloodFill(hdc, startX, startY, boundaryColor, fillColor);
                }

                GdiFlush();
                clickState = 0; // Reset so you can draw a new box!
            }

            ReleaseDC(hwnd, hdc);
            break;
        }

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
    wc.lpszClassName = "FloodFillClass";
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wc);

    HWND hwnd = CreateWindow("FloodFillClass", "Flood Fill (Click TopLeft -> BottomRight -> Inside to Fill)", WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, h, NULL);
    
    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}