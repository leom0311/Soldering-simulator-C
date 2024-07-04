#include "Worker.h"
#include <windows.h>
#include <math.h>

Worker::Worker() {
	m_nState = ST_WORKER_finished;
	m_fPeriod = 3600.0;
	m_nAttachedItemNum = 0;
}

Worker::Worker(POINT posHead, int headRadius) {
	m_nState = ST_WORKER_finished;
	m_fPeriod = 3600.0;
	m_nAttachedItemNum = 0;
	SetParameters(posHead, headRadius);
}

Worker::~Worker() {
}

void Worker::SetBoard(Board* p) {
	m_pBoard = p;
}

int Worker::GetState() {
	return m_nState;
}

void Worker::SetState(int state) {
	m_nState = state;
	if (m_nState == ST_WORKER_pending) {
		m_fGone = 0;
	}
}

void Worker::SetParameters(POINT posHead, int headRadius) {
	m_posHead = posHead;
	m_nHeadRadius = headRadius;
	m_posBody = posHead;
	m_posBody.y -= m_nHeadRadius;
	m_nBodyHeight = headRadius * 1.1;
	m_nBodyWidth = m_nBodyHeight * 2.8;
	m_nArmLength_0 = headRadius * 2.5;
	m_nArmLength_1 = headRadius * 2.8;


	POINT pos;
	pos.x = m_posBody.x - m_nBodyWidth + 10;
	pos.y = m_posBody.y - m_nBodyHeight * 2 + 10;
	m_leftArm.SetParameters(pos, m_nArmLength_0, m_nArmLength_1, FALSE);

	pos.x += 2 * m_nBodyWidth - 20;
	pos.y = m_posBody.y - m_nBodyHeight * 2 + 10;
	m_rightArm.SetParameters(pos, m_nArmLength_0, m_nArmLength_1, TRUE);
}

void DrawRotatedEllipse(Graphics* graphics, SolidBrush& brush, RectF& rect, float angle) {
	PointF center(rect.X, rect.Y + rect.Height / 2);

	GraphicsPath path;
	path.AddEllipse(rect);

	Matrix matrix;
	matrix.RotateAt(-(angle / 3.141592 * 180), center);

	path.Transform(&matrix);
	graphics->FillPath(&brush, &path);
}

int Worker::GetAttachedItemNum() {
	return m_nAttachedItemNum;
}


BOOL movedCompleted = FALSE;
BOOL movedEmptySlot = FALSE;

BOOL movedEmptyItem_0 = FALSE;
BOOL movedEmptyItem_1 = FALSE;
BOOL movedEmptyItem_2 = FALSE;
BOOL movedEmptyItem_3 = FALSE;

void Worker::Update(DWORD dt, Graphics *graphics, int w, int h) {
	SolidBrush brushBody(Color(255, 0x4f, 0x81, 0xbd));
	graphics->FillEllipse(&brushBody, X(m_posBody.x, w) - m_nBodyWidth, Y(m_posBody.y, h) + m_nBodyHeight, m_nBodyWidth * 2, m_nBodyHeight * 2);

	SolidBrush brush(Color(255, 0, 0, 0));
	graphics->FillEllipse(&brush, X(m_posHead.x, w) - m_nHeadRadius * 0.9, Y(m_posHead.y, h) + m_nHeadRadius, m_nHeadRadius * 0.9 * 2, m_nHeadRadius * 2);
	Pen pen(Color(255, 0x38, 0x5d, 0x8a), 3);
	graphics->DrawEllipse(&pen, X(m_posHead.x, w) - m_nHeadRadius * 0.9, Y(m_posHead.y, h) + m_nHeadRadius, m_nHeadRadius * 0.9 * 2, m_nHeadRadius * 2);

	POINT org, middle, target;
	POINT orgLeftTarget;
	POINT orgRightTarget;

	m_leftArm.GetPositions(org, middle, target);
	orgLeftTarget.x = org.x + m_nHeadRadius * 2;
	orgLeftTarget.y = org.y + m_nHeadRadius * 4.5;

	m_rightArm.GetPositions(org, middle, target);
	orgRightTarget.x = org.x - m_nHeadRadius * 2;
	orgRightTarget.y = org.y + m_nHeadRadius * 4.5;

	POINT rightTarget, leftTarget;
	rightTarget.x = orgRightTarget.x;
	rightTarget.y = orgRightTarget.y;

	leftTarget.x = orgLeftTarget.x;
	leftTarget.y = orgLeftTarget.y;

	POINT socketPos;
	socketPos.x = 0;
	socketPos.y = 0;
	if (m_nState > ST_WORKER_pending) {
		m_fGone += dt;
		if (m_fGone <= m_fPeriod / 6.0) {
			SetState(ST_WORKER_removing);
			if (m_fGone <= m_fPeriod / 12.0) {
				rightTarget.x = linePoint(orgRightTarget.x, 0, orgRightTarget.x + m_nHeadRadius * 2, m_fPeriod / 12.0, m_fGone);
				rightTarget.y = linePoint(orgRightTarget.y, 0, orgRightTarget.y + m_nHeadRadius * 0.5, m_fPeriod / 12.0, m_fGone);

				socketPos.x = rightTarget.x;
				socketPos.y = rightTarget.y;
				m_pBoard->SetMovingCompleted(TRUE, socketPos);
			}
			else {
				m_pBoard->SetMovingCompleted(FALSE, socketPos);
				if (movedCompleted == FALSE) {
					m_pBoard->SetCompletedSlot(m_pBoard->GetCompletedSlot() + 1);
					movedCompleted = TRUE;
				}
				rightTarget.x = linePoint(orgRightTarget.x + m_nHeadRadius * 2, m_fPeriod / 12.0, orgRightTarget.x, m_fPeriod / 6.0, m_fGone);
				rightTarget.y = linePoint(orgRightTarget.y + m_nHeadRadius * 0.5, m_fPeriod / 12.0, orgRightTarget.y, m_fPeriod / 6.0, m_fGone);
			}
		}
		else if (m_fGone <= m_fPeriod * 2.0 / 6.0) {
			if (m_fGone <= m_fPeriod * 3.0 / 12.0) {
				leftTarget.x = linePoint(orgLeftTarget.x, m_fPeriod * 1.0 / 6.0, orgLeftTarget.x - m_nHeadRadius * 2, m_fPeriod * 3.0 / 12.0, m_fGone);
				leftTarget.y = linePoint(orgLeftTarget.y, m_fPeriod * 1.0 / 6.0, orgLeftTarget.y + m_nHeadRadius * 0.5, m_fPeriod * 3.0 / 12.0, m_fGone);
			}
			else {
				if (movedEmptySlot == FALSE) {
					m_pBoard->SetEmptySlot(m_pBoard->GetEmptySlot() - 1);
					movedEmptySlot = TRUE;
				}
				
				leftTarget.x = linePoint(orgLeftTarget.x - m_nHeadRadius * 2, m_fPeriod * 3.0 / 12.0, orgLeftTarget.x, m_fPeriod * 2.0 / 6.0, m_fGone);
				leftTarget.y = linePoint(orgLeftTarget.y + m_nHeadRadius * 0.5, m_fPeriod * 3.0 / 12.0, orgLeftTarget.y, m_fPeriod * 2.0 / 6.0, m_fGone);

				socketPos.x = leftTarget.x;
				socketPos.y = leftTarget.y;
				m_pBoard->SetMovingEmpty(TRUE, socketPos);
			}
		}
		else if (m_fGone <= m_fPeriod * 3.0 / 6.0) {
			m_pBoard->SetMovingEmpty(FALSE, socketPos);
			m_nAttachedItemNum = 0;
			if (GetState() != ST_WORKER_set_circuit) {
				SetState(ST_WORKER_set_circuit);
			}
			if (m_fGone <= m_fPeriod * 5.0 / 12.0) {
				leftTarget.x = linePoint(orgLeftTarget.x, m_fPeriod * 2.0 / 6.0, orgLeftTarget.x - m_nHeadRadius * 2, m_fPeriod * 5.0 / 12.0, m_fGone);
				leftTarget.y = linePoint(orgLeftTarget.y, m_fPeriod * 2.0 / 6.0, orgLeftTarget.y - m_nHeadRadius * 1.2, m_fPeriod * 5.0 / 12.0, m_fGone);
			}
			else {
				if (movedEmptyItem_0 == FALSE) {
					m_pBoard->SetEmptyItem(m_pBoard->GetEmptyItem() - 1);
					movedEmptyItem_0 = TRUE;
				}
				leftTarget.x = linePoint(orgLeftTarget.x - m_nHeadRadius * 2, m_fPeriod * 5.0 / 12.0, orgLeftTarget.x, m_fPeriod * 3.0 / 6.0, m_fGone);
				leftTarget.y = linePoint(orgLeftTarget.y - m_nHeadRadius * 1.2, m_fPeriod * 5.0 / 12.0, orgLeftTarget.y, m_fPeriod * 3.0 / 6.0, m_fGone);
				
				// move item 0
				socketPos.x = leftTarget.x;
				socketPos.y = leftTarget.y;
				m_pBoard->SetMovingItem(TRUE, socketPos);

			}
		}
		else if (m_fGone <= m_fPeriod * 4.0 / 6.0) {
			m_nAttachedItemNum = 1;
			if (m_fGone <= m_fPeriod * 7.0 / 12.0) {
				leftTarget.x = linePoint(orgLeftTarget.x, m_fPeriod * 3.0 / 6.0, orgLeftTarget.x - m_nHeadRadius * 2, m_fPeriod * 7.0 / 12.0, m_fGone);
				leftTarget.y = linePoint(orgLeftTarget.y, m_fPeriod * 3.0 / 6.0, orgLeftTarget.y - m_nHeadRadius * 1.2, m_fPeriod * 7.0 / 12.0, m_fGone);

				m_pBoard->SetMovingItem(FALSE, socketPos);
			}
			else {
				if (movedEmptyItem_1 == FALSE) {
					m_pBoard->SetEmptyItem(m_pBoard->GetEmptyItem() - 1);
					movedEmptyItem_1 = TRUE;
				}
				leftTarget.x = linePoint(orgLeftTarget.x - m_nHeadRadius * 2, m_fPeriod * 7.0 / 12.0, orgLeftTarget.x, m_fPeriod * 4.0 / 6.0, m_fGone);
				leftTarget.y = linePoint(orgLeftTarget.y - m_nHeadRadius * 1.2, m_fPeriod * 7.0 / 12.0, orgLeftTarget.y, m_fPeriod * 4.0 / 6.0, m_fGone);

				// move item 1
				socketPos.x = leftTarget.x;
				socketPos.y = leftTarget.y;
				m_pBoard->SetMovingItem(TRUE, socketPos);
			}
		}
		else if (m_fGone <= m_fPeriod * 5.0 / 6.0) {
			m_nAttachedItemNum = 2;
			if (m_fGone <= m_fPeriod * 9.0 / 12.0) {
				leftTarget.x = linePoint(orgLeftTarget.x, m_fPeriod * 4.0 / 6.0, orgLeftTarget.x - m_nHeadRadius * 2, m_fPeriod * 9.0 / 12.0, m_fGone);
				leftTarget.y = linePoint(orgLeftTarget.y, m_fPeriod * 4.0 / 6.0, orgLeftTarget.y - m_nHeadRadius * 1.2, m_fPeriod * 9.0 / 12.0, m_fGone);

				m_pBoard->SetMovingItem(FALSE, socketPos);
			}
			else {
				if (movedEmptyItem_2 == FALSE) {
					m_pBoard->SetEmptyItem(m_pBoard->GetEmptyItem() - 1);
					movedEmptyItem_2 = TRUE;
				}
				leftTarget.x = linePoint(orgLeftTarget.x - m_nHeadRadius * 2, m_fPeriod * 9.0 / 12.0, orgLeftTarget.x, m_fPeriod * 5.0 / 6.0, m_fGone);
				leftTarget.y = linePoint(orgLeftTarget.y - m_nHeadRadius * 1.2, m_fPeriod * 9.0 / 12.0, orgLeftTarget.y, m_fPeriod * 5.0 / 6.0, m_fGone);
				// move item 2
				socketPos.x = leftTarget.x;
				socketPos.y = leftTarget.y;
				m_pBoard->SetMovingItem(TRUE, socketPos);
			}
		}
		else if (m_fGone < m_fPeriod) {
			m_nAttachedItemNum = 3;
			if (m_fGone <= m_fPeriod * 11.0 / 12.0) {
				leftTarget.x = linePoint(orgLeftTarget.x, m_fPeriod * 5.0 / 6.0, orgLeftTarget.x - m_nHeadRadius * 2, m_fPeriod * 11.0 / 12.0, m_fGone);
				leftTarget.y = linePoint(orgLeftTarget.y, m_fPeriod * 5.0 / 6.0, orgLeftTarget.y - m_nHeadRadius * 1.2, m_fPeriod * 11.0 / 12.0, m_fGone);

				m_pBoard->SetMovingItem(FALSE, socketPos);
			}
			else {
				if (movedEmptyItem_3 == FALSE) {
					m_pBoard->SetEmptyItem(m_pBoard->GetEmptyItem() - 1);
					movedEmptyItem_3 = TRUE;
				}
				leftTarget.x = linePoint(orgLeftTarget.x - m_nHeadRadius * 2, m_fPeriod * 11.0 / 12.0, orgLeftTarget.x, m_fPeriod * 6.0 / 6.0, m_fGone);
				leftTarget.y = linePoint(orgLeftTarget.y - m_nHeadRadius * 1.2, m_fPeriod * 11.0 / 12.0, orgLeftTarget.y, m_fPeriod * 6.0 / 6.0, m_fGone);
			
				// move item 3
				socketPos.x = leftTarget.x;
				socketPos.y = leftTarget.y;
				m_pBoard->SetMovingItem(TRUE, socketPos);
			}
			m_fFinishPending = 0.0;
		}
		else {
			POINT pos;
			pos.x = 0;
			pos.y = 0;
			m_pBoard->SetMovingCompleted(FALSE, pos);
			m_pBoard->SetMovingEmpty(FALSE, pos);
			m_pBoard->SetMovingItem(FALSE, pos);

			movedEmptySlot = FALSE;
			movedCompleted = FALSE;
			movedEmptyItem_0 = FALSE;
			movedEmptyItem_1 = FALSE;
			movedEmptyItem_2 = FALSE;
			movedEmptyItem_3 = FALSE;

			m_nAttachedItemNum = 4;
			SetState(ST_WORKER_fill_circuit);
			m_fFinishPending += dt;
			if (m_fFinishPending >= 500) {
				if (m_pBoard->GetCompletedSlot() >= SLOT_NUM) {
					m_pBoard->SetCompletedSlot(0);
				}
				if (m_pBoard->GetEmptySlot() <= 0) {
					m_pBoard->SetEmptySlot(SLOT_NUM);
				}
				if (m_pBoard->GetEmptyItem() <= 0) {
					m_pBoard->SetEmptyItem(40);
				}
				SetState(ST_WORKER_finished);
			}
		}
	}

	m_rightArm.SetTarget(rightTarget);
	m_rightArm.GetPositions(org, middle, target);
	RectF rect2(X(org.x, w), Y(org.y, h), distance(middle, org), 32);
	DrawRotatedEllipse(graphics, brushBody, rect2, atan2(middle.y - org.y, middle.x - org.x));
	RectF rect3(X(middle.x, w), Y(middle.y, h), distance(middle, target), 28);
	DrawRotatedEllipse(graphics, brushBody, rect3, atan2(target.y - middle.y, target.x - middle.x));

	m_leftArm.SetTarget(leftTarget);
	m_leftArm.GetPositions(org, middle, target);
	RectF rect(X(org.x, w), Y(org.y, h), distance(middle, org), 32);
	DrawRotatedEllipse(graphics, brushBody, rect, atan2(middle.y - org.y, middle.x - org.x));
	RectF rect1(X(middle.x, w), Y(middle.y, h), distance(middle, target), 28);
	DrawRotatedEllipse(graphics, brushBody, rect1, atan2(target.y - middle.y, target.x - middle.x));
}