#include "Automate.h"

Automate::Automate() {
    POINT pos;
    pos.x = 0;
    pos.y = 50;
    m_Board.SetParameters(pos, 150, 1000.0);
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

void Automate::Update(DWORD dt, Graphics* graphics, int w, int h) {
    m_Board.Update(dt, graphics, w, h);
    // m_Board.TestPaint(graphics, w, h);
    m_Worker.TestPaint(graphics, w, h);

    m_Solder.Update(dt, graphics, w, h);
    // m_Solder.TestPaint(graphics, w, h);
}