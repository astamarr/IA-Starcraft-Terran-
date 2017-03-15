#pragma once

#include "Fiche.h"
#include <BWAPI.h>

#include <vector>
#include <memory>

class IAManager
{
public:
	IAManager();
	~IAManager();
protected:
	virtual Fiche createFiche() = 0;
	std::vector<std::shared_ptr<Fiche>> fiches;
};

