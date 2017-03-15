#include "Fiche.h"


Fiche::Fiche(const Fiche::Type& type_t, const BWAPI::UnitType& unitType,const BWAPI::UnitCommandType& unitCommand) : type(type_t)
{
	unitOrder = unitType;
	actionType = unitCommand;
}


Fiche::~Fiche()
{
}

Fiche::Type Fiche::getType() const
{
	return type;
}

unsigned int Fiche::getCountdown() const
{
	return countdown;
}

bool Fiche::isUnitAGoodCandidate(const BWAPI::Unit& unit, unsigned int& score)
{
	score = getScore(unit);
	bool goodCandidate = false;
	for (auto& unit : registeredUnits)
	{
		if (score > unit.second) goodCandidate = true;
	}

	return goodCandidate;
}

unsigned int Fiche::getScore(const BWAPI::Unit& unit) const
{
	unsigned int score { 0 };
	BuildingPlacer builder;

	switch (type)
	{
	case BUILDING:
	{
		BWAPI::TilePosition buildPos = builder.getBuildLocationNear(unit->getTilePosition(), unitOrder);
		bool ok = BWAPI::Broodwar->canBuildHere(buildPos, unitOrder);
	}break;
	case RESEARCH:
	{

	}break;
	case UNIT:
	{
		
	}break;
	case GATHERING:
	{
		score = unit->getDistance(unit->getClosestUnit(BWAPI::Filter::IsMineralField));
	}break;
	case ATTACK:
	{

	}break;
	}

	return score;
}