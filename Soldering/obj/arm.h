#pragma once

#include "types.h"
#include "Joint.h"
#include "Board.h"

enum {
	ST_WORKER_finished = 0,
	ST_WORKER_pending,
	ST_WORKER_removing,
	ST_WORKER_set_circuit,
	ST_WORKER_fill_circuit,
};

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

	int m_nState;

	float m_fGone;
	float m_fPeriod;

	float m_fFinishPending;

	int m_nAttachedItemNum;

	Board* m_pBoard;
public:
	Worker();
	Worker(POINT posHead, int headRadius);
	~Worker();

	void SetBoard(Board* p);
	int GetAttachedItemNum();
	int GetState();
	void SetState(int state);
	void SetParameters(POINT posHead, int headRadius);
	void Update(DWORD dt, Graphics* graphics, int w, int h);
};