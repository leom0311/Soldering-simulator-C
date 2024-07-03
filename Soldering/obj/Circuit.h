#pragma once
#include "types.h"

class Circuit {
private:
	POINT m_position;
	int m_nWidth;
	int m_nHeight;

	int m_nSoldered;
	BOOL m_bVisible;
	BOOL m_bItemVisible;

	POINT m_rotOrigin;
	double m_rotAngle;

	int m_nItemNum;
public:
	Circuit();
	Circuit(POINT pos, int width, int height);
	~Circuit();

	void Rotate(POINT pos, double angle);
	void GetSolderingPoint(int idx, POINT& lt, POINT& lb, POINT& rt, POINT& rb);
	void SetParameters(POINT pos, int width, int height);
	void SetVisible(BOOL visible);
	BOOL GetVisible();

	void SetItemVisible(BOOL visible);
	BOOL GetItemVisible();

	int GetItemNum();
	void SetItemNum(int num);
	void IncItemNum();

	void SetSoldered(int soldered);
	int GetSoldered();

	void TestPaint(Graphics* graphics, int w, int h);
};