#include "Automate.h"

Automate::Automate() {
    POINT pos;
    pos.x = 0;
    pos.y = 50;
    m_Board.SetParameters(pos, 150, 1500.0);
    m_Board.SetAngle(0 * PI / 180);

    POINT pos0, pos1, pos2;
    m_Board.GetSolderingPath(pos0, pos1, pos2);

    pos.x = 0;
    pos.y = -100;
    Worker worker(pos, 40);
    m_Worker.SetParameters(pos, 40);

    pos.x = -200;
    pos.y = 150;
    Solder solder;
    m_Solder.SetParameters(pos, 140, 120, FALSE);
    pos.x = -40;
    pos.y = 150;
    
    m_Solder.SetMovingPath(pos0, pos1, pos2);
    m_Solder.SetTarget(pos0);
}

Automate::~Automate() {
}

DWORD g_fSolderingFinishPending = 0;
DWORD g_fBoardRotatePending = 0;

void Automate::Update(DWORD dt, Graphics* graphics, int w, int h) {
    m_Board.Update(dt, graphics, w, h);
    m_Worker.Update(dt, graphics, w, h);
    m_Solder.Update(dt, graphics, w, h);

    if (m_Board.GetState() == ST_BOARD_stop) {
        if (g_fBoardRotatePending <= 500) {
            g_fBoardRotatePending += dt;
            return;
        }
        if (m_Worker.GetState() == ST_WORKER_finished) {
            if (m_Solder.GetState() == ST_SOLDER_finished) {
                if (g_fSolderingFinishPending <= 500) {
                    g_fSolderingFinishPending += dt;
                    return;
                }
                g_fSolderingFinishPending = 0;
                g_fBoardRotatePending = 0;

                m_Board.SetState(ST_BOARD_rotating);
                m_Solder.SetState(ST_SOLDER_stop);
                m_Worker.SetState(ST_WORKER_pending);
            }
            else {
                if (m_Solder.GetState() == ST_SOLDER_moving) {
                    m_Board.SetSoldered(m_Solder.GetSoldered());
                }
                else {
                    m_Solder.SetState(ST_SOLDER_moving);
                }
            }
        }
        else {
            if (m_Solder.GetState() != ST_SOLDER_finished) {
                if (m_Solder.GetState() == ST_SOLDER_moving) {
                    m_Board.SetSoldered(m_Solder.GetSoldered());
                }
                else {
                    m_Solder.SetState(ST_SOLDER_moving);
                }
            }
            if (m_Worker.GetState() == ST_WORKER_pending) {
                m_Worker.SetState(ST_WORKER_removing);
            }
            else if (m_Worker.GetState() == ST_WORKER_removing) {
                m_Board.RemoveCompleted();
            }
            else if (m_Worker.GetState() == ST_WORKER_set_circuit) {
                m_Board.CircuitAttach();
                m_Board.SetAttachedItemNum(m_Worker.GetAttachedItemNum());
            }
            else if (m_Worker.GetState() == ST_WORKER_fill_circuit) {
                m_Board.CircuitItemAttach();
                m_Board.SetAttachedItemNum(m_Worker.GetAttachedItemNum());
            }
        }
    }
}