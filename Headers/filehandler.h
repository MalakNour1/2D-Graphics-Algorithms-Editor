#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <windows.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

#include "eventdispatcher.h"
#include "menu_id.h"
#include "structs.h"


void SaveFile(string filename, const vector<Shape>& drawnShapes) {
    ofstream outFile(filename);
    if (!outFile) {
        cout << "Error: Could not open file for saving." << endl;
        return;
    }

    for (const auto& sh : drawnShapes) {
        // 1. Save MenuID and the number of points
        outFile << sh.MenuID << " " << sh.points.size() << " ";
        // 2. Save all points (x and y)
        for (const auto& p : sh.points) {
            cout << p.x << " " << p.y << " " << endl;
            outFile << p.x << " " << p.y << " ";
        }

        // 3. Save Color (extracting R, G, and B)      
        outFile << (int)GetRValue(sh.color) << " "
                << (int)GetGValue(sh.color) << " "
                << (int)GetBValue(sh.color) << endl;
                
    }

    outFile.close();
    cout << "Project saved successfully to " << filename << endl;
}

void LoadFile(string filename, vector<Shape>& drawnShapes) {
    ifstream inFile(filename);
    if (!inFile) {
        cout << "Error: Could not open file for loading." << endl;
        return;
    }

    drawnShapes.clear();
    int menuID, numPoints, r, g, b;

    while (inFile >> menuID >> numPoints) {
        Shape tempShape;
        tempShape.MenuID = menuID;

        for (int i = 0; i < numPoints; i++) {
            POINT p;
            inFile >> p.x >> p.y;
            tempShape.points.push_back(p);
        }

        inFile >> r >> g >> b;
        tempShape.color = RGB(r, g, b);

        drawnShapes.push_back(tempShape);
    }

    inFile.close();
    cout << "Project loaded successfully. Shapes found: " << drawnShapes.size() << endl;
}

void DrawLoadedShapes(HDC hdc, vector<Shape>& drawnShapes) {
    for (auto& sh : drawnShapes) {
        EventDispatcher(hdc, sh.points, sh.MenuID, sh.color, false);
    }
}

#endif // FILEHANDLER_H
