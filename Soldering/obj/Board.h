#pragma once
#include "types.h"
#include "Circuit.h"

class Board {
private:
	Circuit m_ActiveCircuit;
	Circuit m_PendingCircuit;

	POINT m_position;
	int m_nRadius;
	double m_dAngle;
public:
	Board() = delete;
	Board(POINT pos, int radius);
	~Board();

	void SetAngle(double angle);
	void InitCircuits();

	void TestPaint(HDC hDC, int w, int h);
};