#include "pkfilter.hpp"

#include "pokemon.hpp"


// --------------------------------------------------
// From Bank to XY
bool PKFilter::filterToXY(pkm_t* pkm)
// --------------------------------------------------
{
	bool isFiltered = true;

	isFiltered &= PKFilter::filterItemORASExclusiv(Pokemon::itemID(pkm));
	isFiltered &= PKFilter::filterMoveORASExclusiv(Pokemon::move1(pkm));
	isFiltered &= PKFilter::filterMoveORASExclusiv(Pokemon::move2(pkm));
	isFiltered &= PKFilter::filterMoveORASExclusiv(Pokemon::move3(pkm));
	isFiltered &= PKFilter::filterMoveORASExclusiv(Pokemon::move4(pkm));
	isFiltered &= PKFilter::filterAbilityORASExclusiv(Pokemon::ability(pkm));
	isFiltered &= PKFilter::filterSchoolGirlPikachu(Pokemon::speciesID(pkm), Pokemon::form(pkm));

	return isFiltered;
}


// --------------------------------------------------
// From XY to Bank
bool PKFilter::filterFromXY(pkm_t* pkm)
// --------------------------------------------------
{
	bool isFiltered = true;

	return isFiltered;
}


// --------------------------------------------------
// From Bank to ORAS
bool PKFilter::filterToORAS(pkm_t* pkm)
// --------------------------------------------------
{
	bool isFiltered = true;

	return isFiltered;
}


// --------------------------------------------------
// From Bank to ORAS
bool PKFilter::filterFromORAS(pkm_t* pkm)
// --------------------------------------------------
{
	bool isFiltered = true;

	return isFiltered;
}


// --------------------------------------------------
bool PKFilter::filterItemORASExclusiv(uint16_t itemID)
// --------------------------------------------------
{
	uint16_t itemCount = 64; // + 6;
	uint16_t itemFilterList[itemCount] = {
	//  0x1a9, 0x1c2, 0x1d7, 0x26b, 0x2c9, 0x2cc,
		0x1a9, 0x1c2, 0x1d7, 0x26b, 0x2c9, 0x2cc, 0x2ce, 0x2cf, 0x2d0, 0x2d1, 0x2d2, 0x2d3, 0x2d4, 0x2d5, 0x2d6, 0x2d7, 0x2d8, 0x2d9, 0x2da, 0x2db, 0x2dc, 0x2dd, 0x2de, 0x2df, 0x2e0, 0x2e1, 0x2e2, 0x2e3, 0x2e4, 0x2e5, 0x2e6, 0x2e7, 0x2e8, 0x2e9, 0x2ea, 0x2eb, 0x2ec, 0x2ed, 0x2ee, 0x2ef, 0x2f0, 0x2f1, 0x2f2, 0x2f3, 0x2f4, 0x2f5, 0x2f6, 0x2f7, 0x2f8, 0x2f9, 0x2fa, 0x2fb, 0x2fc, 0x2fd, 0x2fe, 0x2ff, 0x300, 0x301, 0x302, 0x303, 0x304, 0x305, 0x306, 0x307
	};


	bool isFiltered = true;

	for (uint32_t i = 0; i < itemCount && isFiltered; i++)
	{
		if (itemFilterList[i] == itemID)
			isFiltered = false;
	}

	// printf(/*\x1B[1;0H*/" Item[%x] %s\n", itemID, (isFiltered ? "allowed" : "forbidden"));
	return isFiltered;
}


// --------------------------------------------------
bool PKFilter::filterMoveORASExclusiv(uint16_t moveID)
// --------------------------------------------------
{
	uint16_t moveCount = 4;
	uint16_t moveFilterList[moveCount] = {
		0x26a, 0x26b, 0x26c, 0x26d
	};


	bool isFiltered = true;

	for (uint32_t i = 0; i < moveCount && isFiltered; i++)
	{
		if (moveFilterList[i] == moveID)
			isFiltered = false;
	}

	// printf(/*\x1B[1;0H*/" Move[%x] %s\n", moveID, (isFiltered ? "allowed" : "forbidden"));
	return isFiltered;
}


// --------------------------------------------------
bool PKFilter::filterAbilityORASExclusiv(uint16_t abilityID)
// --------------------------------------------------
{
	// printf(/*\x1B[1;0H*/" Ability[%x] %s\n", abilityID, (true ? "allowed" : "forbidden"));
	return true;

	// uint16_t abilityCount = 64;
	// uint16_t abilityFilterList[abilityID] = {
	// 	0xbd, 0xbe, 0xbf
	// };

	// bool isFiltered = true;

	// for (uint32_t i = 0; i < abilityCount && isFiltered; i++)
	// {
	// 	if (abilityFilterList[i] == abilityID)
	// 		isFiltered = false;
	// }

	// return isFiltered;
}


// --------------------------------------------------
bool PKFilter::filterSchoolGirlPikachu(uint16_t speciesID, uint16_t formID)
// --------------------------------------------------
{
	bool isFiltered = true;

	isFiltered = !(speciesID == 25 && formID > 0);
	
	// printf(/*\x1B[1;0H*/" School girl %s\n", (isFiltered ? "allowed" : "forbidden"));
	return isFiltered;
}




// --------------------------------------------------
uint8_t PKFilter::getBaseFriendship(uint16_t speciesID)
// --------------------------------------------------
{
	uint16_t baseFriendshipCount = 721;
	uint8_t baseFriendship[baseFriendshipCount] = { 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 140, 140, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 
		70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 140, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 35, 35, 35, 35, 35, 35, 0, 100, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 140, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 35, 35, 70, 35, 
		70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 35, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 35, 35, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 140, 35, 35, 35, 35, 35, 35, 0, 0, 100, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 35, 35, 35, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 
		70, 35, 70, 35, 35, 35, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 35, 35, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 35, 35, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 35, 35, 35, 35, 70, 70, 35, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 35, 35, 35, 35, 35, 35, 35, 35, 35, 90, 90, 0, 0, 0, 100, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 
		70, 70, 70, 100, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 100, 70, 70, 70, 70, 70, 70, 100, 70, 70, 0, 140, 35, 35, 70, 70, 70, 70, 70, 70, 70, 70, 70, 140, 35, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 100, 70, 70, 70, 70, 70, 70, 70, 35, 70, 70, 70, 70, 70, 70, 70, 70, 35, 35, 70, 70, 70, 35, 70, 35, 70, 70, 140, 140, 140, 0, 0, 100, 0, 0, 100, 70, 70, 0, 100, 0, 100, 70, 70, 70, 70, 70, 70, 
		70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 35, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 
		70, 70, 70, 70, 70, 70, 70, 70, 70, 35, 35, 35, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 35, 35, 70, 70, 70, 35, 35, 70, 70, 35, 35, 35, 70, 70, 35, 35, 35, 90, 90, 0, 0, 90, 0, 35, 100, 0, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 
		70, 70, 70, 35, 35, 35, 70, 70, 70, 70, 70, 70, 70, 70, 70, 0, 0, 0, 70, 100, 100
	};

	return baseFriendship[speciesID];
}
