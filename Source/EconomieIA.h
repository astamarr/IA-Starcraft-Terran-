#pragma once

#include "IAManager.h"

class EconomieIA : public IAManager
{
public:
	EconomieIA();
	~EconomieIA();
	void update();
private:
	Fiche createFiche();
};

