#pragma once

#include "IAManager.h"

class CombatIA : public IAManager
{
public:
	CombatIA();
	~CombatIA();
private:
	Fiche createFiche();
};

