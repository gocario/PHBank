#pragma once
#ifndef FILTER_HPP
#define FILTER_HPP

#include "save.hpp"

namespace Filter
{
	// public:
	bool filterToXY(pkm_s* pkm);
	bool filterFromXY(pkm_s* pkm);
	bool filterToORAS(pkm_s* pkm);
	bool filterFromORAS(pkm_s* pkm);

	// private:
	bool filterItemORASExclusiv(u16 itemID);
	bool filterMoveORASExclusiv(u16 moveID);
	bool filterAbilityORASExclusiv(u16 abilityID);
	bool filterSchoolGirlPikachu(u16 speciesID, u16 formID);

	// public:
	// TODO Move to a DataManager
	u8 getBaseFriendship(u16 speciesID);
};

#endif // FILTER_HPP
