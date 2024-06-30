#pragma once
#include "types.h"

#define SOLDER_PNT (4)

class Circuit {
private:
	POINT m_position;
	int m_nWidth;
	int m_nHeight;

	int m_nSoldered;
	BOOL m_bVisible;
public:
	Circuit();
	Circuit(POINT pos, int width, int height);
	~Circuit();

	void GetSolderingPoint(int idx, POINT& lt, POINT& rb);
	void SetParameters(POINT pos, int width, int height);
	void SetVisible(BOOL visible);
	BOOL GetVisible();
	void SetSoldered(int soldered);
	int GetSoldered();
	void TestPaint(HDC hDC, int w, int h);
};