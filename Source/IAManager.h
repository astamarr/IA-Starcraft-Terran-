#pragma once

#include "Fiche.h"
#include <BWAPI.h>

#include <vector>
#include <memory>

class IAManager
{
public:
	void assignUnits(BWAPI::Unitset units);
	virtual void update() = 0;
protected:
	virtual Fiche createFiche() = 0;
	void findBestPosition(const BWAPI::Unit&);
	std::vector<std::shared_ptr<Fiche>> fiches;
	BWAPI::Unitset assignedUnits;

	Fiche* defaultBehaviour;
};

