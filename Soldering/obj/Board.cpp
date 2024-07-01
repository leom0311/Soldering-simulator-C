#include "Board.h"
#include <stdio.h>

Board::Board() {
	m_ActiveCircuit = new Circuit();
	m_PendingCircuit = new Circuit();
}

Board::Board(POINT pos, int radius, float period) {
	m_ActiveCircuit = new Circuit();
	m_PendingCircuit = new Circuit();

	SetParameters(pos, radius, period);
}

Board::~Board() {
}

void Board::SetParameters(POINT pos, int radius, float period) {
	m_position = pos;
	m_nRadius = radius;
	m_dAngle = 0.0;
	m_fPeriod = period;

	m_nState = ST_BOARD_stop;
	InitCircuits();
}

void Board::SetAngle(double angle) {
	m_dAngle = angle;

	m_ActiveCircuit->Rotate(m_position, m_dAngle);
	m_PendingCircuit->Rotate(m_position, m_dAngle);
}

void Board::InitCircuits() {
	POINT pos;
	pos.x = m_position.x;
	pos.y = m_position.y + m_nRadius * 2 / 3;
	m_ActiveCircuit->SetParameters(pos, m_nRadius * 3 / 5, m_nRadius / 8);
	m_ActiveCircuit->Rotate(m_position, 0);
	pos.y = m_position.y - m_nRadius * 2 / 3;
	m_PendingCircuit->SetParameters(pos, m_nRadius * 3 / 5, m_nRadius / 8);
	m_PendingCircuit->Rotate(m_position, 0);
}

void Board::GetSolderingPath(POINT& pos0, POINT& pos1, POINT& pos2) {
	POINT lt, lb, rt, rb;
	m_ActiveCircuit->GetSolderingPoint(SOLDER_PNT - 1, lt, lb, rt, rb);
	int h = rt.y - rb.y;
	pos1.x = rt.x;
	pos1.y = rt.y - h / 2;

	m_ActiveCircuit->GetSolderingPoint(0, lt, lb, rt, rb);
	h = rt.y - rb.y;
	pos2.x = rt.x;
	pos2.y = rt.y - h / 2;

	pos0.x = pos1.x;
	pos0.y = pos1.y + 60;
}

int Board::GetState() {
	return m_nState;
}

void Board::Update(DWORD dt, Graphics* graphics, int w, int h) {
	if (m_nState != ST_BOARD_rotating) {
		TestPaint(graphics, w, h);
		return;
	}
	m_dAngle = m_dAngle + (PI / m_fPeriod * dt);
	if (m_dAngle >= PI) {
		m_dAngle = PI;
		m_nState = ST_BOARD_stop;
	}
	m_ActiveCircuit->Rotate(m_position, -m_dAngle);
	m_PendingCircuit->Rotate(m_position, -m_dAngle);
	TestPaint(graphics, w, h);
}

void Board::RemoveCompleted() {
	m_ActiveCircuit->SetVisible(FALSE);
}

void Board::SetSoldered(int n) {
	m_ActiveCircuit->SetSoldered(n);
}

void Board::TestPaint(Graphics* graphics, int w, int h) {
	SolidBrush brush(Color(255, 0xee, 0xec, 0xe1));
	Pen pen(Color(255, 0, 0, 0), 2);
	graphics->FillEllipse(&brush, X(m_position.x, w) - m_nRadius, Y(m_position.y, h) - m_nRadius, m_nRadius * 2, m_nRadius * 2);
	graphics->DrawEllipse(&pen, X(m_position.x, w) - m_nRadius, Y(m_position.y, h) - m_nRadius, m_nRadius * 2, m_nRadius * 2);

	m_ActiveCircuit->TestPaint(graphics, w, h);
	m_PendingCircuit->TestPaint(graphics, w, h);
}