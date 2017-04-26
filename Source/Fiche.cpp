#include "Fiche.h"


Fiche::Fiche(const Fiche::Type& type_t, const BWAPI::UnitType& unitType,const BWAPI::UnitCommandType& unitCommand) : type(type_t)
{
	unitOrder = unitType;
	actionType = unitCommand;

	maxUnitCount = 0;
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

bool Fiche::registerUnit(const BWAPI::Unit& unit)
{
	unsigned int score = 0;
	bool successfullyAdded = false;
	//check if the unit is better than another one already assigned
	if (isUnitAGoodCandidate(unit, score))
	{
		//check if there is still enough places
		if (registeredUnits.size() < maxUnitCount)
		{
			//remove the worst unit, add our unit
			registeredUnits.erase(worstUnit);
		}
		registeredUnits.emplace_back(std::pair<BWAPI::Unit, int>(unit, score));
		successfullyAdded = true;
	}
	return successfullyAdded;
}

bool Fiche::isUnitAGoodCandidate(const BWAPI::Unit& unit, unsigned int& score)
{
	score = getScore(unit);
	bool goodCandidate = false;
	unsigned int best{ 0 }, worst{std::numeric_limits<unsigned int>::max()};
	//go through every registered units and compare its score with unit
	for (std::list<std::pair<BWAPI::Unit, unsigned int>>::iterator& unit = registeredUnits.begin(); unit != registeredUnits.end(); ++unit)
	{
		//update worst unit
		if (unit->second < worst)
		{
			worst = unit->second;
			worstUnit = unit;
		}
		//update best unit
		if (unit->second > best)
		{
			best = unit->second;
			bestunit = unit;
		}
		//at least one unit is worst than the tested unit
		if (score > unit->second) goodCandidate = true;
	}

	return goodCandidate;
}

unsigned int Fiche::getScore(const BWAPI::Unit& unit)
{
	unsigned int score { 0 };
	BuildingPlacer builder;

	switch (type)
	{
	// The task require Building something
	case BUILDING:
	{
		BWAPI::TilePosition buildPos = builder.getBuildLocationNear(unit->getTilePosition(), unitOrder);
		bool ok = BWAPI::Broodwar->canBuildHere(buildPos, unitOrder);
	}break;
	// The task require reasearching something
	case RESEARCH:
	{

	}break;
	// The task require Making unit
	case UNIT:
	{
		
	}break;
	// The task require Gathering resources
	case GATHERING:
	{
		if (unitOrder == BWAPI::UnitTypes::Resource_Mineral_Field)
			score = unit->getDistance(unit->getClosestUnit(BWAPI::Filter::IsMineralField));
		else
			score = unit->getDistance(unit->getClosestUnit(BWAPI::Filter::IsRefinery));
	}break;
	// The task require Attacking something
	case ATTACK:
	{
		score = unit->getDistance(unit->getClosestUnit(BWAPI::Filter::IsEnemy));
		if (unit->isAttacking()) score = static_cast<unsigned int>(score*1.5);
	}break;
	}
		
	return score;
	}