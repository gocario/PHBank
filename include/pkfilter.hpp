#ifndef PKFILTER_HPP
#define PKFILTER_HPP

#include <3ds/types.h>

#include "save.hpp"

class PKFilter
{
	public:

		static bool filterToXY(pkm_t* pkm);
		static bool filterFromXY(pkm_t* pkm);
		static bool filterToORAS(pkm_t* pkm);
		static bool filterFromORAS(pkm_t* pkm);

		static bool filterItemORASExclusiv(uint16_t itemID);
		static bool filterMoveORASExclusiv(uint16_t moveID);
		static bool filterAbilityORASExclusiv(uint16_t abilityID);
		static bool filterSchoolGirlPikachu(uint16_t speciesID, uint16_t formID);

		static uint8_t getBaseFriendship(uint16_t speciesID);
};

#endif // PKFILTER_HPP
