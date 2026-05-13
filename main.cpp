#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif


#include "resources.h"
using namespace std;

void AddMenus(HWND hwnd) {
    HMENU hMenuBar = CreateMenu();

    // --- 1. File Menu ---
    HMENU hFile = CreatePopupMenu();
    AppendMenu(hFile, MF_STRING, ID_FILE_CLEAR, "Clear Screen");
    AppendMenu(hFile, MF_STRING, ID_FILE_SAVE, "Save Data");
    AppendMenu(hFile, MF_STRING, ID_FILE_LOAD, "Load Data");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFile, "&File");

    // --- 2. Preferences Menu ---
    HMENU hBackground = CreatePopupMenu();
    AppendMenu(hBackground, MF_STRING, ID_PREF_WHITE_BG, "White Background");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hBackground, "&Pref.Background Color");

    HMENU hMouseShape = CreatePopupMenu();
    AppendMenu(hMouseShape, MF_STRING, ID_PREF_CURSOR1, "Arrow");
    AppendMenu(hMouseShape, MF_STRING, ID_PREF_CURSOR2, "Cross");
    AppendMenu(hMouseShape, MF_STRING, ID_PREF_CURSOR3, "Hand");
    AppendMenu(hMouseShape, MF_STRING, ID_PREF_CURSOR4, "IBeam");
    AppendMenu(hMouseShape, MF_STRING, ID_PREF_CURSOR5, "Wait");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hMouseShape, "&Pref.Mouse Shape");

    HMENU hDrawingColor = CreatePopupMenu();
    AppendMenu(hDrawingColor, MF_STRING, ID_PREF_COLOR_RED, "Red");
    AppendMenu(hDrawingColor, MF_STRING, ID_PREF_COLOR_BLACK, "Black");
    AppendMenu(hDrawingColor, MF_STRING, ID_PREF_COLOR_BLUE, "Blue");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hDrawingColor, "&Pref.Select Color");

    // --- 3. Lines Menu (Person 2) ---
    HMENU hLine = CreatePopupMenu();
    AppendMenu(hLine, MF_STRING, ID_LINE_DDA, "DDA");
    AppendMenu(hLine, MF_STRING, ID_LINE_MIDPOINT, "Midpoint");
    AppendMenu(hLine, MF_STRING, ID_LINE_PARAMETRIC, "Parametric");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hLine, "&Lines");

    // --- 4. Circles Menu (Person 3) ---
    HMENU hCircle = CreatePopupMenu();
    AppendMenu(hCircle, MF_STRING, ID_CIRCLE_DIRECT, "Direct");
    AppendMenu(hCircle, MF_STRING, ID_CIRCLE_POLAR, "Polar");
    AppendMenu(hCircle, MF_STRING, ID_CIRCLE_ITER_POLAR, "Iterative Polar");
    AppendMenu(hCircle, MF_STRING, ID_CIRCLE_MIDPOINT, "Midpoint");
    AppendMenu(hCircle, MF_STRING, ID_CIRCLE_MOD_MIDPOINT, "Modified Midpoint");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hCircle, "&Circles");

    // --- 5. Ellipse Menu (Person 3) ---
    HMENU hEllipse = CreatePopupMenu();
    AppendMenu(hEllipse, MF_STRING, ID_ELLIPSE_DIRECT, "Direct");
    AppendMenu(hEllipse, MF_STRING, ID_ELLIPSE_POLAR, "Polar");
    AppendMenu(hEllipse, MF_STRING, ID_ELLIPSE_MIDPOINT, "Midpoint");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hEllipse, "&Ellipse");

    // --- 6. Curves Menu (Person 2) ---
    HMENU hCurve = CreatePopupMenu();
    AppendMenu(hCurve, MF_STRING, ID_CURVE_CARDINAL, "Cardinal Spline");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hCurve, "C&urves");

    // --- 7. Filling Menu (Person 4 & Others) ---
    HMENU hFill = CreatePopupMenu();
    AppendMenu(hFill, MF_STRING, ID_FILL_CIRC_LINE, "Circle with Lines");
    AppendMenu(hFill, MF_STRING, ID_FILL_CIRC_CIRC, "Circle with Circles");
    AppendMenu(hFill, MF_STRING, ID_FILL_SQ_HERMITE, "Square (Hermite)");
    AppendMenu(hFill, MF_STRING, ID_FILL_RECT_BEZIER, "Rectangle (Bezier)");
    AppendMenu(hFill, MF_SEPARATOR, 0, NULL);
    AppendMenu(hFill, MF_STRING, ID_FILL_CONVEX, "Convex");
    AppendMenu(hFill, MF_STRING, ID_FILL_NONCONVEX, "Non-Convex");
    AppendMenu(hFill, MF_STRING, ID_FILL_FLOOD_REC, "Recursive Flood Fill");
    AppendMenu(hFill, MF_STRING, ID_FILL_FLOOD_NONREC, "Non-Recursive Flood Fill");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFill, "&Filling");

    // --- 8. Clipping Menu (Person 5) ---
    HMENU hClip = CreatePopupMenu();
    AppendMenu(hClip, MF_STRING, ID_CLIP_RECT_POINT, "Rectangle - Point");
    AppendMenu(hClip, MF_STRING, ID_CLIP_RECT_LINE, "Rectangle - Line");
    AppendMenu(hClip, MF_STRING, ID_CLIP_RECT_POLY, "Rectangle - Polygon");
    AppendMenu(hClip, MF_STRING, ID_CLIP_SQUARE_POINT, "Square - Point");
    AppendMenu(hClip, MF_STRING, ID_CLIP_SQUARE_LINE, "Square - Line");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hClip, "&Clipping");

    // --- Bonus Menu ---
    HMENU hBonus = CreatePopupMenu();
    AppendMenu(hBonus, MF_STRING, ID_BONUS_CLIP_CIRC, "Circle Clipping");
    AppendMenu(hBonus, MF_STRING, ID_BONUS_FACE_HAPPY, "Happy Face");
    AppendMenu(hBonus, MF_STRING, ID_BONUS_FACE_SAD, "Sad Face");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hBonus, "&Bonus");

    // Attach the Menu Bar to the Window
    SetMenu(hwnd, hMenuBar);
}

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


HCURSOR hCurrentCursor = LoadCursor(NULL, IDC_ARROW); // Shape of Mouse
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT p;
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:{
            AddMenus(hwnd);
            break;
        }
        case WM_COMMAND:{
            int wmId = LOWORD(wParam);

            // Log interaction to console (Requirement 10)
            cout << "Menu Selection ID: " << wmId << endl;

            switch (wmId) {
                case ID_FILE_CLEAR:
                    InvalidateRect(hwnd, NULL, TRUE);
                    points.clear();
                    drawnShapes.clear();
                    break;

                case ID_FILE_SAVE:
                    // Call your save function
                    break;

                case ID_FILE_LOAD:
                    // Call your Load function
                    break;
                case ID_PREF_WHITE_BG:
                    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)GetStockObject(WHITE_BRUSH));
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;

                case ID_PREF_COLOR_RED: {
                    drawingColor = RGB(255, 0, 0);
                    break;
                }
                case ID_PREF_COLOR_BLACK: {
                    drawingColor = RGB(0, 0, 0);
                    break;
                }
                case ID_PREF_COLOR_BLUE: {
                    drawingColor = RGB(0, 0, 255);
                    break;
                }
                case ID_PREF_CURSOR1:{
                    hCurrentCursor = LoadCursor(NULL, IDC_ARROW);
                    SetCursor(hCurrentCursor);
                    cout << "Preference Changed: Mouse Cursor set to Arrow." << endl;
                    break;
                }
                case ID_PREF_CURSOR2:{
                    hCurrentCursor = LoadCursor(NULL, IDC_CROSS);
                    SetCursor(hCurrentCursor);
                    cout << "Preference Changed: Mouse Cursor set to Cross." << endl;
                    break;
                }
                case ID_PREF_CURSOR3:{
                    hCurrentCursor = LoadCursor(NULL, IDC_HAND);
                    SetCursor(hCurrentCursor);
                    cout << "Preference Changed: Mouse Cursor set to Hand." << endl;
                    break;
                }
                case ID_PREF_CURSOR4:{
                    hCurrentCursor = LoadCursor(NULL, IDC_IBEAM);
                    SetCursor(hCurrentCursor);
                    cout << "Preference Changed: Mouse Cursor set to IBeam." << endl;
                    break;
                }
                case ID_PREF_CURSOR5:{
                    hCurrentCursor = LoadCursor(NULL, IDC_WAIT);
                    SetCursor(hCurrentCursor);
                    cout << "Preference Changed: Mouse Cursor set to Wait." << endl;
                    break;
                }
                // Catch-all for algorithm selection
                default:
                    CurrentMenuID = wmId;
                    points.clear();
                    break;
            }
            break;
        }
        case WM_SETCURSOR:
            if (LOWORD(lParam) == HTCLIENT) {
                SetCursor(hCurrentCursor);
                return TRUE;
            }
            break;
        case WM_PAINT:
            BeginPaint(hwnd, &p);
            EndPaint(hwnd, &p);
            break;
        case WM_LBUTTONDOWN:
            POINT p;
            p.x = LOWORD(lParam);
            p.y = HIWORD(lParam);
            points.push_back(p);
            cout << "Left Mouse Click at: (" << p.x << ", " << p.y << ")" << endl;
            EventDispatcher(hwnd, points, CurrentMenuID, drawingColor);
            InvalidateRect(hwnd, NULL, FALSE);
            break;
        case WM_RBUTTONDOWN:
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
