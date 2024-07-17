#pragma once

#include "types.h"
#include "arm.h"
#include "Board.h"
#include "rob.h"

class Automate {
private:
	Solder m_Solder;
	Board m_Board;
	Worker m_Worker;
public:
	Automate();
	~Automate();

	void Update(DWORD dt, Graphics* graphics, int w, int h);
};
