#include "Circuit.h"

Circuit::Circuit() {
	m_nSoldered = 0;
	m_bVisible = TRUE;
}

Circuit::Circuit(POINT pos, int width, int height) {
	m_position = pos;
	m_nWidth = width;
	m_nHeight = height;

	m_nSoldered = 0;
	m_bVisible = TRUE;
}

Circuit::~Circuit() {
}

void Circuit::SetParameters(POINT pos, int width, int height) {
	m_position = pos;
	m_nWidth = width;
	m_nHeight = height;
}

void Circuit::SetVisible(BOOL visible) {
	m_bVisible = visible;
}

BOOL Circuit::GetVisible() {
	return m_bVisible;
}

void Circuit::SetSoldered(int soldered) {
	m_nSoldered = soldered;
}

int Circuit::GetSoldered() {
	return m_nSoldered;
}

void Circuit::GetSolderingPoint(int idx, POINT& lt, POINT& lb, POINT& rt, POINT& rb) {
	POINT leftTop;
	leftTop.x = m_position.x - m_nWidth / 2;
	leftTop.y = m_position.y + m_nHeight / 2;

	int offset = m_nWidth / 8;
	int width = (m_nWidth - 5 * offset) / 4;
	int height = m_nHeight * 3 / 6;

	int x = leftTop.x + offset;

	for (int i = 0; i < SOLDER_PNT; i++) {
		lt.x = x;
		lt.y = m_position.y + height / 2;
		lb.x = x;
		lb.y = m_position.y - height / 2;
		rt.x = x + width;
		rt.y = m_position.y + height / 2;
		rb.x = x + width;
		rb.y = m_position.y - height / 2;

		x += (width + offset) + 1;
		if (i == idx) {
			POINT_D ltRot, lbRot, rtRot, rbRot;

			rotatePoint(m_rotOrigin.x, m_rotOrigin.y, lt.x, lt.y, m_rotAngle, &ltRot.x, &ltRot.y);
			rotatePoint(m_rotOrigin.x, m_rotOrigin.y, lb.x, lb.y, m_rotAngle, &lbRot.x, &lbRot.y);
			rotatePoint(m_rotOrigin.x, m_rotOrigin.y, rt.x, rt.y, m_rotAngle, &rtRot.x, &rtRot.y);
			rotatePoint(m_rotOrigin.x, m_rotOrigin.y, rb.x, rb.y, m_rotAngle, &rbRot.x, &rbRot.y);

			lt.x = ltRot.x;
			lt.y = ltRot.y;
			lb.x = lbRot.x;
			lb.y = lbRot.y;
			rb.x = rbRot.x;
			rb.y = rbRot.y;
			rt.x = rtRot.x;
			rt.y = rtRot.y;

			break;
		}
	}
}

void Circuit::Rotate(POINT pos, double angle) {
	m_rotOrigin = pos;
	m_rotAngle = angle;
}

void Circuit::TestPaint(HDC hDC, int w, int h) {
	POINT leftTop, leftBottom, rightTop, rightBottom;
	POINT_D leftTopRot, leftBottomRot, rightTopRot, rightBottomRot;
	leftTop.x		= m_position.x - m_nWidth / 2;
	leftTop.y		= m_position.y + m_nHeight / 2;
	leftBottom.x	= m_position.x - m_nWidth / 2;
	leftBottom.y	= m_position.y - m_nHeight / 2;
	rightTop.x		= m_position.x + m_nWidth / 2;
	rightTop.y		= m_position.y + m_nHeight / 2;
	rightBottom.x	= m_position.x + m_nWidth / 2;
	rightBottom.y	= m_position.y - m_nHeight / 2;

	rotatePoint(m_rotOrigin.x, m_rotOrigin.y, leftTop.x,		leftTop.y,		m_rotAngle, &leftTopRot.x,		&leftTopRot.y);
	rotatePoint(m_rotOrigin.x, m_rotOrigin.y, leftBottom.x,		leftBottom.y,	m_rotAngle, &leftBottomRot.x,	&leftBottomRot.y);
	rotatePoint(m_rotOrigin.x, m_rotOrigin.y, rightTop.x,		rightTop.y,		m_rotAngle, &rightTopRot.x,		&rightTopRot.y);
	rotatePoint(m_rotOrigin.x, m_rotOrigin.y, rightBottom.x,	rightBottom.y,	m_rotAngle, &rightBottomRot.x,	&rightBottomRot.y);

	POINT pt;
	MoveToEx(hDC, X(leftBottomRot.x, w),	Y(leftBottomRot.y, h), &pt);
	LineTo(hDC, X(leftTopRot.x, w),			Y(leftTopRot.y, h));
	LineTo(hDC, X(rightTopRot.x, w),		Y(rightTopRot.y, h));
	LineTo(hDC, X(rightBottomRot.x, w),		Y(rightBottomRot.y, h));
	LineTo(hDC, X(leftBottomRot.x, w),		Y(leftBottomRot.y, h));
	
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN hOldPen = NULL;
	for (int i = 0; i < SOLDER_PNT; i++) {
		POINT lt, lb, rt, rb;
		GetSolderingPoint(i, lt, lb, rt, rb);
		if (i == (SOLDER_PNT - m_nSoldered)) {
			hOldPen = (HPEN)SelectObject(hDC, hPen);
		}		
		MoveToEx(hDC, X(lt.x, w), Y(lt.y, h), &pt);
		LineTo(hDC, X(lb.x, w), Y(lb.y, h));
		LineTo(hDC, X(rb.x, w), Y(rb.y, h));
		LineTo(hDC, X(rt.x, w), Y(rt.y, h));
		LineTo(hDC, X(lt.x, w), Y(lt.y, h));
	}
	if (hOldPen) {
		SelectObject(hDC, hOldPen);
	}
	DeleteObject(hPen);	
}