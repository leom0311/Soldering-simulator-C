#include "Board.h"

Board::Board(POINT pos, int radius) {
	m_position = pos;
	m_nRadius = radius;
	m_dAngle = 0.0;

	InitCircuits();
}

Board::~Board() {
}

void Board::SetAngle(double angle) {
	m_dAngle = angle;

	m_ActiveCircuit.Rotate(m_position, m_dAngle);
	m_PendingCircuit.Rotate(m_position, m_dAngle);
}

void Board::InitCircuits() {
	POINT pos;
	pos.x = m_position.x;
	pos.y = m_position.y + m_nRadius * 2 / 3;
	m_ActiveCircuit.SetParameters(pos, m_nRadius * 3 / 5, m_nRadius / 8);
	m_ActiveCircuit.Rotate(m_position, 0);
	pos.y = m_position.y - m_nRadius * 2 / 3;
	m_PendingCircuit.SetParameters(pos, m_nRadius * 3 / 5, m_nRadius / 8);
	m_PendingCircuit.Rotate(m_position, 0);
}

void Board::TestPaint(Graphics* graphics, int w, int h) {
	SolidBrush brush(Color(255, 0xee, 0xec, 0xe1));
	Pen pen(Color(255, 0, 0, 0), 2);
	graphics->FillEllipse(&brush, X(m_position.x, w) - m_nRadius, Y(m_position.y, h) - m_nRadius, m_nRadius * 2, m_nRadius * 2);
	graphics->DrawEllipse(&pen, X(m_position.x, w) - m_nRadius, Y(m_position.y, h) - m_nRadius, m_nRadius * 2, m_nRadius * 2);

	m_ActiveCircuit.TestPaint(graphics, w, h);
	m_PendingCircuit.TestPaint(graphics, w, h);
}