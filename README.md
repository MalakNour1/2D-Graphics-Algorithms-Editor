# 2D Graphics Algorithms Editor

A complete Windows-based 2D Graphics Drawing Package implemented using Win32 API and C++ for the Computer Graphics course term project at Cairo University.

<img width="767" height="456" alt="image" src="https://github.com/user-attachments/assets/f88aed7d-9773-4ae3-9524-5eafe7d9654d" />

## Overview

This project supports:

- Drawing primitives
- Curves
- Filling algorithms
- Clipping algorithms
- File handling
- User interaction using mouse only
- Console logging

The application is built using:
- C++
- Win32 API
- GDI Graphics Functions

---

# Features

## File Menu

- Clear screen
- Save all drawn shapes to file
- Load shapes from saved files

---

## Preferences Menu

- Change background color
- Change mouse cursor shape
- Select drawing color

---

## Line Algorithms

Implemented line drawing algorithms:

- DDA Line
- Midpoint Line
- Parametric Line

---

## Circle Algorithms

Implemented circle drawing algorithms:

- Direct Circle
- Polar Circle
- Iterative Polar Circle
- Midpoint Circle
- Modified Midpoint Circle

---

## Ellipse Algorithms

Implemented ellipse algorithms:

- Direct Ellipse
- Polar Ellipse
- Midpoint Ellipse

---

## Curves

Implemented curve algorithms:

- Cardinal Spline Curve
- Hermite Curve
- Bezier Curve

---

## Filling Algorithms

Implemented filling techniques:

- Filling Circle using Lines
- Filling Circle using Smaller Circles
- Filling Square using Vertical Hermite Curves
- Filling Rectangle using Horizontal Bezier Curves
- Convex Polygon Filling
- Non-Convex Polygon Filling
- Recursive Flood Fill
- Non-Recursive Flood Fill

---

## Clipping Algorithms

### Rectangle Clipping Window

- Point Clipping
- Line Clipping
- Polygon Clipping

### Square Clipping Window

- Point Clipping
- Line Clipping

### Circle Clipping Window (Bonus)

- Point Clipping
- Line Clipping

---

## Bonus Features

### Smiley Faces

Implemented smiley faces using:
- Circles for face and eyes
- Curves for mouth
- Lines for nose

Includes:
- Happy Face
- Sad Face

---

# User Interaction

The application uses:

- Mouse-only interaction for drawing
- Menus and dropdown lists
- Console logs and information display

---

# Project Structure

```text
2D-Graphics-Algorithms-Editor
│
├── Headers
│   ├── Circle.h
│   ├── Clipping.h
│   ├── Curve.h
│   ├── Ellipse.h
│   ├── eventdispatcher.h
│   ├── Face.h
│   ├── filehandler.h
│   ├── Fill.h
│   ├── Line.h
│   ├── menu_id.h
│   ├── Resources.h
│   └── Structs.h
│
├── DrawingFace.cpp
└── main.cpp
```

---

# Main Components

## `main.cpp`

Responsible for:
- Creating the main application window
- Initializing menus
- Handling Win32 messages
- Dispatching drawing operations

---

## `eventdispatcher.h`

Acts as the core controller between:
- Mouse clicks
- Selected menu algorithm
- Drawing functions

It:
- Collects points from the user
- Determines the selected algorithm
- Calls the appropriate drawing/filling/clipping function

---

# Requirements

- Windows OS
- C++20
- Win32 API
- MinGW / GCC
- CLion or any C++ IDE supporting Win32

---

# Notes

- All drawing operations are implemented manually without graphics libraries.
- All rendering is done using Win32 GDI functions.
- User interaction is fully mouse-based.

---

# License

This project is submitted as part of the Computer Graphics course term project at Cairo University.
