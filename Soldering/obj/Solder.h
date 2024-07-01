#pragma once
#include "types.h"
#include "Joint.h"

enum {
	ST_SOLDER_stop = 0,
	ST_SOLDER_moving,
	ST_SOLDER_finished,
};

class Solder : public Joint {
public:
	Solder();
	~Solder();

	POINT m_pos0;
	POINT m_pos1;
	POINT m_pos2;

	int m_nState;
	float m_fGone;
	float m_fPeriod;
	int m_nSoldered;

	void SetMovingPath(POINT p0, POINT p1, POINT p2);
	void Update(DWORD dt, Graphics* graphics, int w, int h);
	int GetState();
	void SetState(int state);
	int GetSoldered();

	void TestPaint(Graphics* graphics, int w, int h);
};