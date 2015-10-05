#ifndef PKFILTER_HPP
#define PKFILTER_HPP

#include "main.hpp"

class PKFilter
{
	public:

		static bool filterItemID(uint16_t itemID);
		static bool filterMoveID(uint16_t moveID);
		static bool filterAbilityID(uint16_t abilityID);
		static bool filterSchoolGirlPikachu(uint16_t speciesID, uint16_t formID);

};


#endif // PKFILTER_HPP