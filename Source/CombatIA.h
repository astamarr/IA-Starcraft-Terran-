#pragma once

#include "IAManager.h"

class CombatIA : public IAManager
{
public:
	CombatIA();
	~CombatIA();
	void update();
private:
	Fiche createFiche();
};

