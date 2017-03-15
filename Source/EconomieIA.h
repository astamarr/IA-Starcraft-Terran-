#pragma once

#include "IAManager.h"

class EconomieIA : public IAManager
{
public:
	EconomieIA();
	~EconomieIA();
private:
	Fiche createFiche();
};

