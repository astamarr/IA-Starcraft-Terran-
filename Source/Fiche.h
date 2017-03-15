#pragma once
#include "BWAPI.h"
#include <list>

#include "BuildingPlacer.h"

class Fiche
{
public:

	enum Type
	{
		BUILDING,
		RESEARCH,
		UNIT,
		GATHERING,
		ATTACK
	};

	Fiche(const Type& type, const BWAPI::UnitType& unitType, const BWAPI::UnitCommandType& unitCommand);
	~Fiche();

	Type getType() const;

	unsigned int getCountdown() const;
	//check if the unit is a good candidate for the Task and store it's score in the given var
	bool isUnitAGoodCandidate(const BWAPI::Unit& unit, unsigned int& score);

private : 
	//type de fiche
	Type type;
	//type d'unit (building, ressource, research, ...)
	BWAPI::UnitType unitOrder;
	//type d'action (build, gather, reasearch, ...)
	BWAPI::UnitCommandType actionType;

	// time required to complete the task
	unsigned int countdown;

	// registered units to complete the task (Unit, intérêt)
	std::list<std::pair<BWAPI::Unit, int>> registeredUnits;
	unsigned int getScore(const BWAPI::Unit&) const;
};

