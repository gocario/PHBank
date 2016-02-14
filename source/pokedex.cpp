#include "pokedex.hpp"
#include "pokemon.hpp"

#include "key.h"

#include <3ds.h>
#include <stdio.h>

namespace Pokedex
{
	/**
	 * @brief Gets a bit of a buffer.
	 * @param buf The buffer to extract the bit from.
	 * @param off The offset (in byte).
	 * @param bit The offset (in bit).
	 */
	static inline bool getOffsetBit(const u8* buf, u32 off, u32 bit)
	{
		return ((buf[off+(bit/8)] >> (bit%8)) & 0x1) == 1;
	}

	/**
	 * @brief Sets a bit of a buffer.
	 * @param buf The buffer to inject the bit to.
	 * @param off The offset (in byte).
	 * @param bit The offset (in bit).
	 * @param v The new value of the bit.
	 */
	static inline void setOffsetBit(u8* buf, u32 off, u32 bit, bool v)
	{
		buf[off+(bit/8)] = (buf[off+(bit/8)] & ~(0x1 << (bit%8))) | ((v?1:0) << (bit%8));
	}

	/**
	 * @brief Gets the bit offset of a Pokémon form of a XY game.
	 * @param species The species of the Pokémon.
	 * @return The bit offset of the form, -1 if none.
	 * @see getFormDexOffsetORAS(u16)
	 */
	static s32 getFormDexOffsetXY(u16 species)
	{
		s32 offset = 0;

		switch (species)
		{
			case 460: offset += 2; // Abomasnow
			case 448: offset += 2; // Lucario
			case 445: offset += 2; // Garchomp
			case 381: offset += 2; // Latios
			case 380: offset += 2; // Latias
			case 359: offset += 2; // Absol
			case 354: offset += 2; // Banette
			case 310: offset += 2; // Manetric
			case 308: offset += 2; // Medicham
			case 306: offset += 2; // Aggron
			case 303: offset += 2; // Mawile
			case 257: offset += 2; // Blaziken
			case 248: offset += 2; // Tyranitar
			case 229: offset += 2; // Houndoom
			case 214: offset += 2; // Heracros
			case 212: offset += 2; // Scizor
			case 181: offset += 3; // Ampharos
			case 150: offset += 2; // Mewtwo
			case 142: offset += 2; // Aerodactyl
			case 130: offset += 2; // Gyarados
			case 127: offset += 2; // Pinsir
			case 115: offset += 2; // Kangaskhan
			case  94: offset += 2; // Gengar
			case  65: offset += 2; // Alakazam
			case   9: offset += 2; // Blastoise
			case   6: offset += 3; // Charizard
			case   3: offset += 2; // Venusaur
			case 716: offset += 2; // Xerneas
			case 681: offset += 4; // Aegislash
			case 711: offset += 4; // Gourgeist
			case 710: offset += 5; // Pumpkaboo
			case 671: offset += 6; // Florges
			case 670: offset += 5; // Floette
			case 669: offset += 20; // Flabébé
			case 666: offset += 2; // Vivillion
			case 645: offset += 2; // Landorus
			case 641: offset += 2; // Tornadus
			case 642: offset += 2; // Thundurus
			case 647: offset += 3; // Keldeo
			case 646: offset += 2; // Kyurem
			case 550: offset += 2; // Basculin
			case 555: offset += 2; // Darmanitan
			case 648: offset += 4; // Meloetta
			case 586: offset += 4; // Sawsbuck
			case 585: offset += 2; // Deerling
			case 421: offset += 4; // Cherrim
			case 351: offset += 3; // Castform
			case 413: offset += 3; // Wormadam
			case 412: offset += 2; // Burmy
			case 423: offset += 2; // Gastrodon
			case 422: offset += 6; // Shellos
			case 479: offset += 2; // Rotom
			case 487: offset += 2; // Giratina
			case 492: offset += 4; // Shaymin
			case 386: offset += 28; // Deoxys
			case 201: offset += 0; // Unown
			break; default: offset = -1;
		}

		return offset;
	}

	/**
	 * @brief Gets the bit offset of a Pokémon form of an ORAS game.
	 * @param species The species of the Pokémon.
	 * @return The bit offset of the form, -1 if none.
	 * @see getFormDexOffsetXY(u16)
	 */
	static s32 getFormDexOffsetORAS(u16 species)
	{
		s32 offset = 0;

		switch (species)
		{
			case 676: offset += 5; // Furfrou
			case 649: offset += 18; // Genesect
			case 493: offset += 2; // Arceus
			case 383: offset += 2; // Groudon
			case 382: offset += 2; // Kyogre
			case 719: offset += 2; // Diancie
			case 531: offset += 2; // Audino
			case 475: offset += 2; // Gallade
			case 428: offset += 2; // Lopunny
			case 384: offset += 2; // Rayquaza
			case 376: offset += 2; // Metagross
			case 373: offset += 2; // Salamence
			case 362: offset += 2; // Glalie
			case 334: offset += 2; // Altaria
			case 323: offset += 2; // Camerupt
			case 319: offset += 2; // Sharpedo
			case 302: offset += 2; // Sableye
			case 360: offset += 2; // Swampert
			case 254: offset += 2; // Sceptile
			case 208: offset += 2; // Steelix
			case  80: offset += 2; // Slowbro
			case  18: offset += 2; // Pidgeot
			case  15: offset += 2; // Beedrill
			case 720: offset += 6; // Hoopa
			case  25: offset += 2; // Pikachu
			// case  25: offset += 1; // Hoopa
			// case  25: offset += 5; // Cosplay Pikachu
			// case  25: offset += 1; // Unused (Cosplay Pikachu)
			// case  25: offset += 2; // Pikachu
			case 460: offset += 2; // Abomasnow
			case 448: offset += 2; // Lucario
			case 445: offset += 2; // Garchomp
			case 381: offset += 2; // Latios
			case 380: offset += 2; // Latias
			case 359: offset += 2; // Absol
			case 354: offset += 2; // Banette
			case 310: offset += 2; // Manetric
			case 308: offset += 2; // Medicham
			case 306: offset += 2; // Aggron
			case 303: offset += 2; // Mawile
			case 257: offset += 2; // Blaziken
			case 248: offset += 2; // Tyranitar
			case 229: offset += 2; // Houndoom
			case 214: offset += 2; // Heracros
			case 212: offset += 2; // Scizor
			case 181: offset += 3; // Ampharos
			case 150: offset += 2; // Mewtwo
			case 142: offset += 2; // Aerodactyl
			case 130: offset += 2; // Gyarados
			case 127: offset += 2; // Pinsir
			case 115: offset += 2; // Kangaskhan
			case  94: offset += 2; // Gengar
			case  65: offset += 2; // Alakazam
			case   9: offset += 2; // Blastoise
			case   6: offset += 3; // Charizard
			case   3: offset += 2; // Venusaur
			case 716: offset += 2; // Xerneas
			case 681: offset += 4; // Aegislash
			case 711: offset += 4; // Gourgeist
			case 710: offset += 5; // Pumpkaboo
			case 671: offset += 6; // Florges
			case 670: offset += 5; // Floette
			case 669: offset += 20; // Flabébé
			case 666: offset += 2; // Vivillion
			case 645: offset += 2; // Landorus
			case 641: offset += 2; // Tornadus
			case 642: offset += 2; // Thundurus
			case 647: offset += 3; // Keldeo
			case 646: offset += 2; // Kyurem
			case 550: offset += 2; // Basculin
			case 555: offset += 2; // Darmanitan
			case 648: offset += 4; // Meloetta
			case 586: offset += 4; // Sawsbuck
			case 585: offset += 2; // Deerling
			case 421: offset += 4; // Cherrim
			case 351: offset += 3; // Castform
			case 413: offset += 3; // Wormadam
			case 412: offset += 2; // Burmy
			case 423: offset += 2; // Gastrodon
			case 422: offset += 6; // Shellos
			case 479: offset += 2; // Rotom
			case 487: offset += 2; // Giratina
			case 492: offset += 4; // Shaymin
			case 386: offset += 28; // Deoxys
			case 201: offset += 0; // Unown
			break; default: offset = -1;
		}

		return offset;
	}

	/// Regex to add printfs:
	/// ([s]et.* /\* (.*) \*/.*;)
	/// { printf("$2\\n"); $1 }
	/// Regex to remove printfs:
	/// \{ (printf.*;) (.*;) \}
	/// $2

	void importToXY(savebuffer_t sav, pkm_s* pkm)
	{
		bool isShiny = Pokemon::isShiny(pkm);
		bool isFemale = Pokemon::gender(pkm) == 1;
		bool isKalosBorn = Pokemon::isKalosBorn(pkm);
		u16 speciesID = Pokemon::speciesID(pkm) - 1;
		u8 formID = Pokemon::formID(pkm);
		u8 lang = Pokemon::language(pkm) - 1;
		if (lang > 5) lang--; // { 0 ,.., 7 }

		printf("\naddDexXY:\n");

		bool isDisplayed = false;

		isDisplayed |= getOffsetBit(sav, SaveConst::XY_offsetDex + 0x1E8 /* MALE_DISPLAY_OFFSET */, speciesID);
		isDisplayed |= getOffsetBit(sav, SaveConst::XY_offsetDex + 0x248 /* FEMALE_DISPLAY_OFFSET */, speciesID);
		isDisplayed |= getOffsetBit(sav, SaveConst::XY_offsetDex + 0x2A8 /* MALE_SHINY_DISPLAY_OFFSET */, speciesID);
		isDisplayed |= getOffsetBit(sav, SaveConst::XY_offsetDex + 0x308 /* FEMALE_SHINY_DISPLAY_OFFSET */, speciesID);

		// Formdex
		if (formID > 0)
		{
			s32 formdexBit = getFormDexOffsetXY(speciesID + 1);

			if (formdexBit >= 0) // Has an entry in the formdex
			{
				formdexBit += formID;

				isDisplayed |= getOffsetBit(sav, SaveConst::XY_offsetDex + 0x398 /* FORM_DISPLAY_OFFSET */, formdexBit);
				isDisplayed |= getOffsetBit(sav, SaveConst::XY_offsetDex + 0x3B0 /* FORM_SHINY_DISPLAY_OFFSET */, formdexBit);

				if (isShiny) { printf("FORM_SHINY_SEEN_OFFSET\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x380 /* FORM_SHINY_SEEN_OFFSET */, formdexBit, true); }
				else { printf("FORM_SEEN_OFFSET\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x368 /* FORM_SEEN_OFFSET */, formdexBit, true); }

				if (!isDisplayed)
				{
					if (isShiny) { printf("FORM_SHINY_DISPLAYED_OFFSET\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x3B0 /* FORM_SHINY_DISPLAYED_OFFSET */, formdexBit, true); }
					else { printf("FORM_DISPLAYED_OFFSET\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x398 /* FORM_DISPLAYED_OFFSET */, formdexBit, true); }
					isDisplayed = true;
				}
			}
		}

		// Seen
		if (isFemale) // Female
		{
			if (isShiny) { printf("FEMALE_SHINY_SEEN_OFFSET\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x188 /* FEMALE_SHINY_SEEN_OFFSET */, speciesID, true); }
			else { printf("FEMALE_SEEN_OFFSET\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x0C8 /* FEMALE_SEEN_OFFSET */, speciesID, true); }
		}
		else // Male or Genderless
		{
			if (isShiny) { printf("MALE_SHINY_SEEN_OFFSET\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x128 /* MALE_SHINY_SEEN_OFFSET */, speciesID, true); }
			else { printf("MALE_SEEN_OFFSET\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x068 /* MALE_SEEN_OFFSET */, speciesID, true); }
		}

		// Displayed
		if (!isDisplayed)
		{
			if (!isFemale) // Female
			{
				if (isShiny) { printf("FEMALE_SHINY_DISPLAYED_OFFSET\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x308 /* FEMALE_SHINY_DISPLAYED_OFFSET */, speciesID, true); }
				else { printf("FEMALE_DISPLAYED_OFFSET\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x248 /* FEMALE_DISPLAYED_OFFSET */, speciesID, true); }
			}
			else // Male or Genderless
			{
				if (isShiny) { printf("MALE_SHINY_DISPLAYED_OFFSET\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x2A8 /* MALE_SHINY_DISPLAYED_OFFSET */, speciesID, true); }
				else { printf("MALE_DISPLAYED_OFFSET\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x1E8 /* MALE_DISPLAYED_OFFSET */, speciesID, true); }
			}
			isDisplayed = true;
		}

		// Lang
		if (lang >= 0 && lang < 7)
		{
			{ printf("LANG_OFFSET\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x3C8 /* LANG_OFFSET */, speciesID * 7 + lang, true); }
		}

		// ForeignDex
		if (speciesID < 650 && !isKalosBorn)
		{
			{ printf("FOREIGN_OFFSET\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x64C /* FOREIGN_OFFSET */, speciesID, true); }
		}
		// Owned
		else
		{
			{ printf("OWNED_OFFSET\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x008 /* OWNED_OFFSET */, speciesID, true); }
		}

		hidScanInput(); if (hidKeysHeld() & KEY_L) waitKey(KEY_ANY);
	}

	void importToORAS(savebuffer_t sav, pkm_s* pkm)
	{
		bool isShiny = Pokemon::isShiny(pkm);
		bool isFemale = Pokemon::gender(pkm) == 1;
		u16 speciesID = Pokemon::speciesID(pkm) - 1;
		u8 formID = Pokemon::formID(pkm);
		u8 lang = Pokemon::language(pkm) - 1;
		if (lang > 5) lang--; // { 0 ,.., 7 }

		printf("\naddDexORAS:\n");

		bool isDisplayed = false;

		isDisplayed |= getOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x1E8 /* MALE_DISPLAY_OFFSET */, speciesID);
		isDisplayed |= getOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x248 /* FEMALE_DISPLAY_OFFSET */, speciesID);
		isDisplayed |= getOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x2A8 /* MALE_SHINY_DISPLAY_OFFSET */, speciesID);
		isDisplayed |= getOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x308 /* FEMALE_SHINY_DISPLAY_OFFSET */, speciesID);

		// Formdex
		if (formID > 0)
		{
			s32 formdexBit = getFormDexOffsetORAS(speciesID + 1);

			if (formdexBit >= 0) // Has an entry in the formdex
			{
				formdexBit += formID;

				isDisplayed |= getOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x3B4 /* FORM_DISPLAY_OFFSET */, formdexBit);
				isDisplayed |= getOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x3DA /* FORM_SHINY_DISPLAY_OFFSET */, formdexBit);

				if (isShiny) { printf("FORM_SHINY_SEEN_OFFSET\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x38E /* FORM_SHINY_SEEN_OFFSET */, formdexBit, true); }
				else { printf("FORM_SEEN_OFFSET\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x368 /* FORM_SEEN_OFFSET */, formdexBit, true); }

				if (!isDisplayed)
				{
					if (isShiny) { printf("FORM_SHINY_DISPLAYED_OFFSET\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x3DA /* FORM_SHINY_DISPLAYED_OFFSET */, formdexBit, true); }
					else { printf("FORM_DISPLAYED_OFFSET\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x3B4 /* FORM_DISPLAYED_OFFSET */, formdexBit, true); }
					isDisplayed = true;
				}
			}
		}

		// Seen
		if (isFemale) // Female
		{
			if (isShiny) { printf("FEMALE_SHINY_SEEN_OFFSET\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x188 /* FEMALE_SHINY_SEEN_OFFSET */, speciesID, true); }
			else { printf("FEMALE_SEEN_OFFSET\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x0C8 /* FEMALE_SEEN_OFFSET */, speciesID, true); }
		}
		else // Male or Genderless
		{
			if (isShiny) { printf("MALE_SHINY_SEEN_OFFSET\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x128 /* MALE_SHINY_SEEN_OFFSET */, speciesID, true); }
			else { printf("MALE_SEEN_OFFSET\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x068 /* MALE_SEEN_OFFSET */, speciesID, true); }
		}

		// Displayed
		if (!isDisplayed)
		{
			if (isFemale) // Female
			{
				if (isShiny) { printf("FEMALE_SHINY_DISPLAYED_OFFSET\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x308 /* FEMALE_SHINY_DISPLAYED_OFFSET */, speciesID, true); }
				else { printf("FEMALE_DISPLAYED_OFFSET\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x248 /* FEMALE_DISPLAYED_OFFSET */, speciesID, true); }
			}
			else // Male or Genderless
			{
				if (isShiny) { printf("MALE_SHINY_DISPLAYED_OFFSET\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x2A8 /* MALE_SHINY_DISPLAYED_OFFSET */, speciesID, true); }
				else { printf("MALE_DISPLAYED_OFFSET\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x1E8 /* MALE_DISPLAYED_OFFSET */, speciesID, true); }
			}
			isDisplayed = true;
		}

		// Lang
		if (lang >= 0 && lang < 7)
		{
			{ printf("LANG_OFFSET\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x400 /* LANG_OFFSET */, speciesID * 7 + lang, true); }
		}

		// DexNav
		u16* dexNav = (u16*)(sav + SaveConst::ORAS_offsetDex + 0x686 /* DEXNAV_OFFSET */ + speciesID * 2);
		if (*dexNav == 0) { printf("DEXNAV_OFFSET\n"); *dexNav = 1; }

		// Owned
		{ printf("OWNED_OFFSET\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x008 /* OWNED_OFFSET */, speciesID, true); }

		hidScanInput(); if (hidKeysHeld() & KEY_L) waitKey(KEY_ANY);
	}
}
