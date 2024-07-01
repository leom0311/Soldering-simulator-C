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
			break;
		}
	}
}

void Circuit::Rotate(POINT pos, double angle) {
	m_rotOrigin = pos;
	m_rotAngle = angle;

	
}

void DrawRotatedRectangle(Graphics* graphics, Pen& pen, SolidBrush& brush, RectF rect, PointF rotationPoint, float angle)
{
	Matrix originalTransform;
	graphics->GetTransform(&originalTransform);

	Matrix matrix;
	matrix.Translate(rotationPoint.X, rotationPoint.Y);
	matrix.Rotate(-angle);
	matrix.Translate(-rotationPoint.X, -rotationPoint.Y);

	graphics->SetTransform(&matrix);
	graphics->FillRectangle(&brush, rect);
	graphics->DrawRectangle(&pen, rect);
	graphics->SetTransform(&originalTransform);
}

void Circuit::TestPaint(Graphics * graphics, int w, int h) {
	double x;
	double y;
	rotatePoint(m_rotOrigin.x, m_rotOrigin.y, m_position.x, m_position.y, m_rotAngle, &x, &y);
	
	POINT leftTop;
	leftTop.x		= x - m_nWidth / 2;
	leftTop.y		= y + m_nHeight / 2;

	SolidBrush brush(Color(255, 255, 255, 255));
	Pen blackPen(Color(255, 0, 0, 0), 2);
	Pen redPen(Color(255, 255, 0, 0), 2);

	DrawRotatedRectangle(graphics, blackPen, brush, RectF(X(leftTop.x, w), Y(leftTop.y, h), m_nWidth, m_nHeight), PointF(X(x, w), Y(y, h)), m_rotAngle * 180 / PI);

	for (int i = 0; i < SOLDER_PNT; i++) {
		POINT lt, lb, rt, rb;
		GetSolderingPoint(i, lt, lb, rt, rb);
		int width = rt.x - lt.x;
		int height = rt.y - rb.y;

		rotatePoint(m_rotOrigin.x, m_rotOrigin.y, lt.x, lt.y, m_rotAngle, &x, &y);
		DrawRotatedRectangle(graphics, (i < (SOLDER_PNT - m_nSoldered) ? blackPen : redPen), brush, RectF(X(x, w), Y(y, h), width, height), PointF(X(x, w), Y(y, h)), m_rotAngle * 180 / PI);
	}
}