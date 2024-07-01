#pragma once
#include "types.h"

class Joint {
private:
	
	POINT m_posOrg;
	POINT m_posTarget;
	POINT m_posMiddle;

	int m_nLength0;
	int m_nLength1;

	BOOL m_bFlip;
public:
	Joint();
	Joint(POINT org, int length0, int length1);
	~Joint();

	void SetTarget(POINT target);
	void SetParameters(POINT org, int length0, int length1, BOOL flip);
	void GetPositions(POINT& org, POINT& middle, POINT& target);

	void TestPaint(HDC hdc, int width, int height);
};