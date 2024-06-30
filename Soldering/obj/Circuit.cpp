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

void Circuit::GetSolderingPoint(int idx, POINT& lt, POINT& rb) {
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

		rb.x = x + width;
		rb.y = m_position.y - height / 2;

		x += (width + offset) + 1;
		if (i == idx) {
			break;
		}
	}
}

void Circuit::TestPaint(HDC hDC, int w, int h) {
	POINT leftTop, rightBottom;
	leftTop.x = m_position.x - m_nWidth / 2;
	leftTop.y = m_position.y + m_nHeight / 2;

	rightBottom.x = m_position.x + m_nWidth / 2;
	rightBottom.y = m_position.y - m_nHeight / 2;

	POINT pt;
	MoveToEx(hDC, X(leftTop.x, w), Y(leftTop.y, h), &pt);
	LineTo(hDC, X(rightBottom.x, w), Y(leftTop.y, h));
	LineTo(hDC, X(rightBottom.x, w), Y(rightBottom.y, h));
	LineTo(hDC, X(leftTop.x, w), Y(rightBottom.y, h));
	LineTo(hDC, X(leftTop.x, w), Y(leftTop.y, h));

	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN hOldPen = NULL;
	for (int i = 0; i < SOLDER_PNT; i++) {
		POINT lt, rb;
		GetSolderingPoint(i, lt, rb);
		if (i == (SOLDER_PNT - m_nSoldered)) {
			hOldPen = (HPEN)SelectObject(hDC, hPen);
		}		
		MoveToEx(hDC, X(lt.x, w), Y(lt.y, h), &pt);
		LineTo(hDC, X(rb.x, w), Y(lt.y, h));
		LineTo(hDC, X(rb.x, w), Y(rb.y, h));
		LineTo(hDC, X(lt.x, w), Y(rb.y, h));
		LineTo(hDC, X(lt.x, w), Y(lt.y, h));		
	}
	if (hOldPen) {
		SelectObject(hDC, hOldPen);
	}
	DeleteObject(hPen);	
}