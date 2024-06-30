#pragma once

#include <windows.h>

#define X(x, w) ((x) + (w) / 2)
#define Y(y, h) (-(y) + (h) / 2)


double distance(POINT p1, POINT p2);
POINT calculateThirdPoint(POINT A, POINT B, double AB, double BC, double CA);
void rotatePoint(double x1, double y1, double x2, double y2, double theta, double* rotatedX, double* rotatedY);
