#include "Solder.h"
#include <math.h>


// it is same as Joint but paint
Solder::Solder() {
	m_nState = ST_SOLDER_stop;
	m_fGone = 0.0;
	m_fPeriod = 1500.0;
}

Solder::~Solder() {

}

float linePoint(int x1, float t1, int x2, float t2, float t) {
	float m = (x2 - x1) / (t2 - t1);
	float b = x1 - m * t1;
	return m * t + b;
}

void Solder::SetMovingPath(POINT p0, POINT p1, POINT p2) {
	m_pos0 = p0;
	m_pos1 = p1;
	m_pos2 = p2;
}

int Solder::GetSoldered() {
	return m_nSoldered;
}

void Solder::Update(DWORD dt, Graphics* graphics, int w, int h) {
	if (m_nState != ST_SOLDER_moving) {
		TestPaint(graphics, w, h);
		return;
	}

	m_fGone += dt;
	if (m_fGone <= m_fPeriod / 3.0) {
		// moving to start point of soldering according below path
		//			\
		//           \
		//            \
		//
		POINT newTarget;
		newTarget.x = linePoint(m_pos0.x, 0, m_pos1.x, m_fPeriod / 3.0, m_fGone);
		newTarget.y = linePoint(m_pos0.y, 0, m_pos1.y, m_fPeriod / 3.0, m_fGone);
		SetTarget(newTarget);

		m_nSoldered = 0;
	}
	else if (m_fGone <= m_fPeriod * 2.0 / 3.0) {
		if (m_nSoldered == 0) {
			m_nSoldered = 1;
		}
		// soldering
		// ----------------
		POINT newTarget;
		newTarget.x = linePoint(m_pos1.x, m_fPeriod / 3.0, m_pos2.x, m_fPeriod * 2.0 / 3.0, m_fGone);
		newTarget.y = linePoint(m_pos1.y, m_fPeriod / 3.0, m_pos2.y, m_fPeriod * 2.0 / 3.0, m_fGone);
		SetTarget(newTarget);
	}
	else if (m_fGone < m_fPeriod) {
		if (m_nSoldered != SOLDER_PNT) {
			m_nSoldered = SOLDER_PNT;
		}
		// return org pos accroding below path
		//          /
		//         /
		//        /
		POINT newTarget;
		newTarget.x = linePoint(m_pos2.x, m_fPeriod * 2.0 / 3.0, m_pos0.x, m_fPeriod, m_fGone);
		newTarget.y = linePoint(m_pos2.y, m_fPeriod * 2.0 / 3.0, m_pos0.y, m_fPeriod, m_fGone);
		SetTarget(newTarget);
	}
	else {
		m_fGone = 0;
		m_nState = ST_SOLDER_finished;
		SetTarget(m_pos0);
	}

	TestPaint(graphics, w, h);
}

int Solder::GetState() {
	return m_nState;
}

void Solder::SetState(int state) {
	m_nState = state;
}

void Solder::TestPaint(Graphics* graphics, int w, int h) {
	// draw solder
	//    just draw rectangle around arm of solder
	SolidBrush brush(Color(255, 0xff, 0xcc, 0x99));
	Pen blackPen(Color(255, 0, 0, 0), 1);

	POINT org, middle, target;
	GetPositions(org, middle, target);

	graphics->DrawLine(&blackPen, Point(X(org.x, w), Y(org.y, h)), Point(X(middle.x, w), Y(middle.y, h)));
	
	DrawRotatedRectangle(graphics, blackPen, brush, RectF(X(middle.x - 6, w), Y(middle.y + 10, h), 12, m_nLength1 + 10), PointF(X(middle.x, w), Y(middle.y, h)), (90 + atan2(target.y - middle.y, target.x - middle.x) * 180 / PI));
	DrawRotatedRectangle(graphics, blackPen, brush, RectF(X(org.x - 9, w), Y(org.y + 8, h), 18, m_nLength0 + 16), PointF(X(org.x, w), Y(org.y, h)), (90 + atan2(middle.y - org.y, middle.x - org.x) * 180 / PI));

	// draw circle in joint point
	graphics->DrawEllipse(&blackPen, X(org.x - 2, w), Y(org.y + 2, h), 4, 4);
	graphics->DrawEllipse(&blackPen, X(middle.x - 2, w), Y(middle.y + 2, h), 4, 4);
}