#include "filter.hpp"

#include "pokemon.hpp"

namespace Filter
{
	static bool filterItemORASExclusiv(u16 itemID)
	{
		const u16 itemCount = 64; // 6;
		const u16 itemFilterList[itemCount] = {
		//  0x1a9, 0x1c2, 0x1d7, 0x26b, 0x2c9, 0x2cc, // XY
			0x1a9, 0x1c2, 0x1d7, 0x26b, 0x2c9, 0x2cc, 0x2ce, 0x2cf, 0x2d0, 0x2d1, 0x2d2, 0x2d3, 0x2d4, 0x2d5, 0x2d6, 0x2d7, 0x2d8, 0x2d9, 0x2da, 0x2db, 0x2dc, 0x2dd, 0x2de, 0x2df, 0x2e0, 0x2e1, 0x2e2, 0x2e3, 0x2e4, 0x2e5, 0x2e6, 0x2e7, 0x2e8, 0x2e9, 0x2ea, 0x2eb, 0x2ec, 0x2ed, 0x2ee, 0x2ef, 0x2f0, 0x2f1, 0x2f2, 0x2f3, 0x2f4, 0x2f5, 0x2f6, 0x2f7, 0x2f8, 0x2f9, 0x2fa, 0x2fb, 0x2fc, 0x2fd, 0x2fe, 0x2ff, 0x300, 0x301, 0x302, 0x303, 0x304, 0x305, 0x306, 0x307
		};

		bool isFiltered = true;

		for (u32 i = 0; i < itemCount && isFiltered; i++)
		{
			if (itemFilterList[i] == itemID)
				isFiltered = false;
		}

		// printf(" Item[%x] %s\n", itemID, (isFiltered ? "allowed" : "forbidden"));

		return isFiltered;
	}

	static bool filterMoveORASExclusiv(u16 moveID)
	{
		const u16 moveCount = 4;
		const u16 moveFilterList[moveCount] = {
			0x26a, 0x26b, 0x26c, 0x26d
		};

		bool isFiltered = true;

		for (u32 i = 0; i < moveCount && isFiltered; i++)
		{
			if (moveFilterList[i] == moveID)
				isFiltered = false;
		}

		// printf(" Move[%x] %s\n", moveID, (isFiltered ? "allowed" : "forbidden"));

		return isFiltered;
	}

	static bool filterAbilityORASExclusiv(u16 abilityID)
	{
		// const u16 abilityCount = 3;
		// const u16 abilityFilterList[abilityCount] = {
		// 	0xbd, 0xbe, 0xbf
		// };

		bool isFiltered = true;

		// for (u32 i = 0; i < abilityCount && isFiltered; i++)
		// {
		// 	if (abilityFilterList[i] == abilityID)
		// 		isFiltered = false;
		// }

		// printf(" Ability[%x] %s\n", abilityID, (isFiltered ? "allowed" : "forbidden"));

		return isFiltered;
	}


	static bool filterSchoolGirlPikachu(u16 speciesID, u16 formID)
	{
		bool isFiltered = true;

		if (speciesID == 25 && formID > 0)
			isFiltered = false;

		// printf(" School girl %s\n", (isFiltered ? "allowed" : "forbidden"));

		return isFiltered;
	}

	bool filterToXY(pkm_s* pkm)
	{
		bool isFiltered = true;

		isFiltered &= Filter::filterItemORASExclusiv(Pokemon::itemID(pkm));
		isFiltered &= Filter::filterMoveORASExclusiv(Pokemon::move1(pkm));
		isFiltered &= Filter::filterMoveORASExclusiv(Pokemon::move2(pkm));
		isFiltered &= Filter::filterMoveORASExclusiv(Pokemon::move3(pkm));
		isFiltered &= Filter::filterMoveORASExclusiv(Pokemon::move4(pkm));
		isFiltered &= Filter::filterAbilityORASExclusiv(Pokemon::ability(pkm));
		// isFiltered &= Filter::filterSchoolGirlPikachu(Pokemon::speciesID(pkm), Pokemon::formID(pkm));

		// printf("To XY %s\n", (isFiltered ? "allowed" : "forbidden"));

		return isFiltered;
	}

	bool filterFromXY(pkm_s* pkm)
	{
		bool isFiltered = true;

		// printf("From XY %s\n", (isFiltered ? "allowed" : "forbidden"));

		return isFiltered;
	}

	bool filterToORAS(pkm_s* pkm)
	{
		bool isFiltered = true;

		// printf("To ORAS %s\n", (isFiltered ? "allowed" : "forbidden"));

		return isFiltered;
	}

	bool filterFromORAS(pkm_s* pkm)
	{
		bool isFiltered = true;

		isFiltered &= Filter::filterSchoolGirlPikachu(Pokemon::speciesID(pkm), Pokemon::formID(pkm));

		// printf("From ORAS %s\n", (isFiltered ? "allowed" : "forbidden"));

		return isFiltered;
	}
}
