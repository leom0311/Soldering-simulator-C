#pragma once
#include "types.h"
#include "Circuit.h"

enum {
	ST_BOARD_stop = 0,
	ST_BOARD_rotating
};

class Board {
private:
	BOOL m_nState;

	Circuit *m_ActiveCircuit;
	Circuit* m_PendingCircuit;

	POINT m_position;
	int m_nRadius;
	double m_dAngle;

	float m_fPeriod;

	int m_nCompleted;
	int m_nEmptySlot;

	int m_nEmptyItem;

	BOOL m_bMovingCompleted;
	POINT m_posMovingCompleted;

	BOOL m_bMovingEmpty;
	POINT m_posMovingEmpty;

	BOOL m_bMovingItem;
	POINT m_posMovingItem;

public:
	Board();
	Board(POINT pos, int radius, float period);
	~Board();

	void SetAngle(double angle);
	void SetParameters(POINT pos, int radius, float period);
	void InitCircuits();
	void Update(DWORD dw, Graphics* graphics, int w, int h);
	int GetState();
	void SetState(int state);
	BOOL SolderingFinished();
	void RemoveCompleted();
	void SetSoldered(int n);
	void GetSolderingPath(POINT& pos0, POINT& pos1, POINT& pos2);

	void CircuitAttach();
	void CircuitItemAttach();
	void SetAttachedItemNum(int n);

	int GetCompletedSlot();
	void SetCompletedSlot(int n);
	int GetEmptySlot();
	void SetEmptySlot(int n);
	int GetEmptyItem();
	void SetEmptyItem(int n);

	void SetMovingCompleted(BOOL visible, POINT pos);
	void SetMovingEmpty(BOOL visible, POINT pos);
	void SetMovingItem(BOOL visible, POINT pos);

	void TestPaint(Graphics* graphics, int w, int h);
};