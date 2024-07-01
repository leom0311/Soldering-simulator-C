#pragma once

#include "types.h"
#include "Joint.h"

class Worker {
private:
	POINT m_posHead;
	POINT m_posBody;

	int m_nHeadRadius;
	int m_nBodyHeight;
	int m_nBodyWidth;

	Joint m_leftArm;
	Joint m_rightArm;

	int m_nArmLength_0;
	int m_nArmLength_1;
public:
	Worker() = delete;
	Worker(POINT posHead, int headRadius);
	~Worker();

	void TestPaint(Graphics* graphics, int w, int h);
};