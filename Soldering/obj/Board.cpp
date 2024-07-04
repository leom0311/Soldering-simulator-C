#include "Board.h"
#include <stdio.h>

Board::Board() {
	m_ActiveCircuit = new Circuit();
	m_PendingCircuit = new Circuit();

	m_nCompleted = 0;
	m_nEmptySlot = SLOT_NUM;
	m_nEmptyItem = 40;

	m_bMovingCompleted = FALSE;
	m_bMovingEmpty = FALSE;
	m_bMovingItem = FALSE;
}

Board::Board(POINT pos, int radius, float period) {
	m_ActiveCircuit = new Circuit();
	m_PendingCircuit = new Circuit();

	m_nCompleted = 0;
	m_nEmptySlot = SLOT_NUM;
	m_nEmptyItem = 40;
	SetParameters(pos, radius, period);

	m_bMovingCompleted = FALSE;
	m_bMovingEmpty = FALSE;
	m_bMovingItem = FALSE;
}

Board::~Board() {
}

void Board::SetMovingCompleted(BOOL visible, POINT pos) {
	m_posMovingCompleted = pos;
	m_bMovingCompleted = visible;
}

void Board::SetMovingEmpty(BOOL visible, POINT pos) {
	m_posMovingEmpty = pos;
	m_bMovingEmpty = visible;
}

void Board::SetMovingItem(BOOL visible, POINT pos) {
	m_posMovingItem = pos;
	m_bMovingItem = visible;
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
#define CIRCUIT_W(w) ((w) * 3 / 5)
#define CIRCUIT_H(h) ((h) / 8)

void Board::InitCircuits() {
	POINT pos;
	pos.x = m_position.x;
	pos.y = m_position.y + m_nRadius * 2 / 3;
	m_ActiveCircuit->SetParameters(pos, CIRCUIT_W(m_nRadius), CIRCUIT_H(m_nRadius));
	m_ActiveCircuit->Rotate(m_position, 0);
	pos.y = m_position.y - m_nRadius * 2 / 3;
	m_PendingCircuit->SetParameters(pos, CIRCUIT_W(m_nRadius), CIRCUIT_H(m_nRadius));
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

	pos0.x = (pos1.x + pos2.x) / 2;
	pos0.y = pos1.y + 60;
}

BOOL Board::SolderingFinished() {
	return m_ActiveCircuit->GetSoldered() == SOLDER_PNT;
}

int Board::GetState() {
	return m_nState;
}

void Board::SetState(int state) {
	m_nState = state;
}

void Board::Update(DWORD dt, Graphics* graphics, int w, int h) {
	if (m_nState != ST_BOARD_rotating) {
		TestPaint(graphics, w, h);
		return;
	}
	m_dAngle = m_dAngle + (PI / m_fPeriod * dt);
	if (m_dAngle >= PI) {
		m_dAngle = 0;
		m_nState = ST_BOARD_stop;

		m_ActiveCircuit->SetSoldered(0);
		m_PendingCircuit->SetSoldered(SOLDER_PNT);
	}
	m_ActiveCircuit->Rotate(m_position, -m_dAngle);
	m_PendingCircuit->Rotate(m_position, -m_dAngle);
	TestPaint(graphics, w, h);
}

void Board::RemoveCompleted() {
	m_PendingCircuit->SetVisible(FALSE);
	m_PendingCircuit->SetItemVisible(FALSE);
	m_PendingCircuit->SetSoldered(0);
}

void Board::CircuitAttach() {
	m_PendingCircuit->SetVisible(TRUE);
}

void Board::CircuitItemAttach() {
	m_PendingCircuit->SetItemVisible(TRUE);
}

void Board::SetSoldered(int n) {
	m_ActiveCircuit->SetSoldered(n);
}

int Board::GetCompletedSlot() {
	return m_nCompleted;
}

void Board::SetCompletedSlot(int n) {
	m_nCompleted = n;
}

int Board::GetEmptySlot() {
	return m_nEmptySlot;
}

void Board::SetEmptySlot(int n) {
	m_nEmptySlot = n;
}

int Board::GetEmptyItem() {
	return m_nEmptyItem;
}

void Board::SetEmptyItem(int n) {
	m_nEmptyItem = n;
}

void Board::SetAttachedItemNum(int n) {
	m_PendingCircuit->SetItemNum(n);
}

void Board::TestPaint(Graphics* graphics, int w, int h) {
	SolidBrush brush(Color(255, 0xee, 0xec, 0xe1));
	Pen pen1(Color(255, 0, 0, 0), 1);
	Pen pen2(Color(255, 0, 0, 0), 2);
	Pen redPen(Color(255, 255, 0, 0), 2);
	graphics->FillEllipse(&brush, X(m_position.x, w) - m_nRadius, Y(m_position.y, h) - m_nRadius, m_nRadius * 2, m_nRadius * 2);
	graphics->DrawEllipse(&pen2, X(m_position.x, w) - m_nRadius, Y(m_position.y, h) - m_nRadius, m_nRadius * 2, m_nRadius * 2);

	m_ActiveCircuit->TestPaint(graphics, w, h);
	m_PendingCircuit->TestPaint(graphics, w, h);

	SolidBrush lackBrush(Color(255, 0x8e, 0xb4, 0xe3));
	graphics->FillRectangle(&lackBrush, X(m_position.x, w) - m_nRadius * 2, Y(m_position.y - m_nRadius * 0.5, h), CIRCUIT_H(m_nRadius) * 5 + 10 * 6, CIRCUIT_W(m_nRadius) + 2 * 10);
	graphics->DrawRectangle(&pen1, X(m_position.x, w) - m_nRadius * 2, Y(m_position.y - m_nRadius * 0.5, h), CIRCUIT_H(m_nRadius) * 5 + 10 * 6, CIRCUIT_W(m_nRadius) + 2 * 10);

	SolidBrush whiteBrush(Color(255, 255, 255, 255));
	for (int i = 0; i < m_nEmptySlot; i++) {
		int j = SLOT_NUM - i - 1;
		graphics->FillRectangle(&whiteBrush, X(m_position.x - m_nRadius * 2 + 10 * (j + 1) + j * CIRCUIT_H(m_nRadius), w), Y(m_position.y - m_nRadius * 0.5 - 10, h), CIRCUIT_H(m_nRadius), CIRCUIT_W(m_nRadius));
		graphics->DrawRectangle(&pen2, X(m_position.x - m_nRadius * 2 + 10 * (j + 1) + j * CIRCUIT_H(m_nRadius), w), Y(m_position.y - m_nRadius * 0.5 - 10, h), CIRCUIT_H(m_nRadius), CIRCUIT_W(m_nRadius));
	}

	int offset = CIRCUIT_W(m_nRadius) / 8;
	int width = (CIRCUIT_W(m_nRadius) - 5 * offset) / 4;
	int height = CIRCUIT_H(m_nRadius) * 3 / 6;

	graphics->FillRectangle(&lackBrush, X(m_position.x, w) - m_nRadius * 2, Y(m_position.y - m_nRadius * 0.5, h) + CIRCUIT_W(m_nRadius) + 2 * 10 + 10, CIRCUIT_H(m_nRadius) * 5 + 10 * 6, 8 * height + 9 * 5);
	graphics->DrawRectangle(&pen1, X(m_position.x, w) - m_nRadius * 2, Y(m_position.y - m_nRadius * 0.5, h) + CIRCUIT_W(m_nRadius) + 2 * 10 + 10, CIRCUIT_H(m_nRadius) * 5 + 10 * 6, 8 * height + 9 * 5);

	int nn = 0;
	for (int ii = 0; ii < 5; ii++) {
		int i = 5 - ii - 1;
		for (int j = 0; j < 8; j++) {
			graphics->FillRectangle(&whiteBrush, X(m_position.x - m_nRadius * 2 + 10 * (i + 1) + i * CIRCUIT_H(m_nRadius) + (CIRCUIT_H(m_nRadius) - width) / 2, w), Y(m_position.y - m_nRadius * 0.5, h) + CIRCUIT_W(m_nRadius) + 2 * 10 + 10 + (j + 1) * 5 + j * height, width, height);
			graphics->DrawRectangle(&pen2, X(m_position.x - m_nRadius * 2 + 10 * (i + 1) + i * CIRCUIT_H(m_nRadius) + (CIRCUIT_H(m_nRadius) - width) / 2, w), Y(m_position.y - m_nRadius * 0.5, h) + CIRCUIT_W(m_nRadius) + 2 * 10 + 10 + (j + 1) * 5 + j * height, width, height);
			nn++;
			if (m_nEmptyItem == nn) {
				goto CONT_;
			}
		}
	}

	CONT_:
	
	graphics->FillRectangle(&lackBrush, X(m_position.x, w) + m_nRadius, Y(m_position.y - m_nRadius * 0.5, h), CIRCUIT_H(m_nRadius) * 5 + 10 * 6, CIRCUIT_W(m_nRadius) + 2 * 10);
	graphics->DrawRectangle(&pen1, X(m_position.x, w) + m_nRadius, Y(m_position.y - m_nRadius * 0.5, h), CIRCUIT_H(m_nRadius) * 5 + 10 * 6, CIRCUIT_W(m_nRadius) + 2 * 10);

	for (int i = 0; i < m_nCompleted; i++) {
		int x = m_position.x + m_nRadius + 10 * (i + 1) + i * CIRCUIT_H(m_nRadius);
		int y = m_position.y - m_nRadius * 0.5 - 10;
		graphics->FillRectangle(&whiteBrush, X(x, w), Y(y, h), CIRCUIT_H(m_nRadius), CIRCUIT_W(m_nRadius));
		graphics->DrawRectangle(&pen2, X(x, w), Y(y, h), CIRCUIT_H(m_nRadius), CIRCUIT_W(m_nRadius));

		int offset_y = -offset;
		for (int j = 0; j < SOLDER_PNT; j++) {
			graphics->DrawRectangle(&redPen, X(x, w) + (CIRCUIT_H(m_nRadius) - width) / 2, Y(y + offset_y, h), width, height);
			offset_y -= (height + offset);
		}
	}


	// moving completed circuit
	if (m_bMovingCompleted) {
		int x = m_posMovingCompleted.x - CIRCUIT_W(m_nRadius) + 10;
		int y = m_posMovingCompleted.y;
		graphics->FillRectangle(&whiteBrush, X(x, w), Y(y, h), CIRCUIT_W(m_nRadius), CIRCUIT_H(m_nRadius));
		graphics->DrawRectangle(&pen2, X(x, w), Y(y, h), CIRCUIT_W(m_nRadius), CIRCUIT_H(m_nRadius));

		int offset_x = offset;
		for (int j = 0; j < SOLDER_PNT; j++) {
			graphics->DrawRectangle(&redPen, X(x, w) + offset_x, Y(y, h) + +(CIRCUIT_H(m_nRadius) - height) / 2, width, height);
			offset_x += (width + offset);
		}
	}

	// moving empty circuit
	if (m_bMovingEmpty) {
		int x = m_posMovingEmpty.x;
		int y = m_posMovingEmpty.y;
		graphics->FillRectangle(&whiteBrush, X(x, w), Y(y, h), CIRCUIT_W(m_nRadius), CIRCUIT_H(m_nRadius));
		graphics->DrawRectangle(&pen2, X(x, w), Y(y, h), CIRCUIT_W(m_nRadius), CIRCUIT_H(m_nRadius));
	}

	// moving item
	if (m_bMovingItem) {
		int x = m_posMovingItem.x + width;
		int y = m_posMovingItem.y - height;
		graphics->FillRectangle(&whiteBrush, X(x, w), Y(y, h), width, height);
		graphics->DrawRectangle(&pen2, X(x, w), Y(y, h), width, height);
	}
}