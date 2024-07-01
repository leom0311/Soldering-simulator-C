#pragma once

#include <windows.h>
#include <gdiplus.h>


using namespace Gdiplus;

#define X(x, w) ((x) + (w) / 2)
#define Y(y, h) (-(y) + (h) / 2)

#define PI (3.141592654)

typedef struct POINT_D {
	double x;
	double y;
} POINT_D;

double distance(POINT p1, POINT p2);
POINT calculateThirdPoint(POINT A, POINT B, double AB, double BC, double CA);
void rotatePoint(double x1, double y1, double x2, double y2, double theta, double* rotatedX, double* rotatedY);
