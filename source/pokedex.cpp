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
		switch (species)
		{
			case 460: return 185; // 2 Abomasnow
			case 448: return 183; // 2 Lucario
			case 445: return 181; // 2 Garchomp
			case 381: return 179; // 2 Latios
			case 380: return 177; // 2 Latias
			case 359: return 175; // 2 Absol
			case 354: return 173; // 2 Banette
			case 310: return 171; // 2 Manetric
			case 308: return 169; // 2 Medicham
			case 306: return 167; // 2 Aggron
			case 303: return 165; // 2 Mawile
			case 257: return 163; // 2 Blaziken
			case 248: return 161; // 2 Tyranitar
			case 229: return 159; // 2 Houndoom
			case 214: return 157; // 2 Heracros
			case 212: return 155; // 2 Scizor
			case 181: return 153; // 2 Ampharos
			case 150: return 150; // 3 Mewtwo
			case 142: return 148; // 2 Aerodactyl
			case 130: return 146; // 2 Gyarados
			case 127: return 144; // 2 Pinsir
			case 115: return 142; // 2 Kangaskhan
			case  94: return 140; // 2 Gengar
			case  65: return 138; // 2 Alakazam
			case   9: return 136; // 2 Blastoise
			case   6: return 133; // 3 Charizard
			case   3: return 131; // 2 Venusaur
			case 716: return 129; // 2 Xerneas
			case 681: return 127; // 2 Aegislash
			case 711: return 123; // 4 Gourgeist
			case 710: return 119; // 4 Pumpkaboo
			case 671: return 114; // 5 Florges
			case 670: return 108; // 6 Floette
			case 669: return 103; // 5 Flabébé
			case 666: return  83; // 20 Vivillion
			case 645: return  81; // 2 Landorus
			case 641: return  79; // 2 Tornadus
			case 642: return  77; // 2 Thundurus
			case 647: return  75; // 2 Keldeo
			case 646: return  72; // 3 Kyurem
			case 550: return  70; // 2 Basculin
			case 555: return  68; // 2 Darmanitan
			case 648: return  66; // 2 Meloetta
			case 586: return  62; // 4 Sawsbuck
			case 585: return  58; // 4 Deerling
			case 421: return  56; // 2 Cherrim
			case 351: return  52; // 4 Castform
			case 413: return  49; // 3 Wormadam
			case 412: return  46; // 3 Burmy
			case 423: return  44; // 2 Gastrodon
			case 422: return  42; // 2 Shellos
			case 479: return  36; // 6 Rotom
			case 487: return  34; // 2 Giratina
			case 492: return  32; // 2 Shaymin
			case 386: return  28; // 4 Deoxys
			case 201: return   0; // 28 Unown
			default: return -1;
		}
	}

	/**
	 * @brief Gets the bit offset of a Pokémon form of an ORAS game.
	 * @param species The species of the Pokémon.
	 * @return The bit offset of the form, -1 if none.
	 * @see getFormDexOffsetXY(u16)
	 */
	static s32 getFormDexOffsetORAS(u16 species)
	{
		switch (species)
		{
			case 676: return 259; // 10 Furfrou
			case 649: return 254; // 5 Genesect
			case 493: return 236; // 18 Arceus
			case 383: return 234; // 2 Groudon
			case 382: return 232; // 2 Kyogre
			case 719: return 230; // 2 Diancie
			case 531: return 228; // 2 Audino
			case 475: return 226; // 2 Gallade
			case 428: return 224; // 2 Lopunny
			case 384: return 222; // 2 Rayquaza
			case 376: return 220; // 2 Metagross
			case 373: return 218; // 2 Salamence
			case 362: return 216; // 2 Glalie
			case 334: return 214; // 2 Altaria
			case 323: return 212; // 2 Camerupt
			case 319: return 210; // 2 Sharpedo
			case 302: return 208; // 2 Sableye
			case 360: return 206; // 2 Swampert
			case 254: return 204; // 2 Sceptile
			case 208: return 202; // 2 Steelix
			case  80: return 200; // 2 Slowbro
			case  18: return 198; // 2 Pidgeot
			case  15: return 196; // 2 Beedrill
			case 720: return 194; // 2 Hoopa
			case  25: return 187; // 7 Pikachu
			// case 720: return 194; // 2 Hoopa
			// case  25: return 193; // 1 Cosplay Pikachu
			// case  25: return 188; // 5 Unused (Cosplay Pikachu)
			// case  25: return 187; // 1 Pikachu
			default: return getFormDexOffsetXY(species);
		}
	}

	/// Regex to add printfs:
	/// ([s]et.* /\* (.*)(_OFFSET) \*/.*;)
	/// { printf("$2_FLAG\\n"); $1 }
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
		u8 lang = Pokemon::language(pkm);
		if (lang-- > 5) lang--; // { 0 ,.., 7 }

		printf("\naddDexXY:\n");

		bool isDisplayed = false;

		isDisplayed |= getOffsetBit(sav, SaveConst::XY_offsetDex + 0x8 + 0x60*5 /* MALE_DISPLAY_OFFSET */, speciesID);
		isDisplayed |= getOffsetBit(sav, SaveConst::XY_offsetDex + 0x8 + 0x60*6 /* FEMALE_DISPLAY_OFFSET */, speciesID);
		isDisplayed |= getOffsetBit(sav, SaveConst::XY_offsetDex + 0x8 + 0x60*7 /* MALE_SHINY_DISPLAY_OFFSET */, speciesID);
		isDisplayed |= getOffsetBit(sav, SaveConst::XY_offsetDex + 0x8 + 0x60*8 /* FEMALE_SHINY_DISPLAY_OFFSET */, speciesID);

		// Formdex
		if (formID > 0)
		{
			s32 formdexBit = getFormDexOffsetXY(speciesID + 1);

			if (formdexBit >= 0) // Has an entry in the formdex
			{
				formdexBit += formID;

				isDisplayed |= getOffsetBit(sav, SaveConst::XY_offsetDex + 0x368 + 0x18*2 /* FORM_DISPLAY_OFFSET */, formdexBit);
				isDisplayed |= getOffsetBit(sav, SaveConst::XY_offsetDex + 0x368 + 0x18*3 /* FORM_SHINY_DISPLAY_OFFSET */, formdexBit);

				// Seen
				if (isShiny) { printf("FORM_SHINY_SEEN_FLAG\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x368 + 0x18*1 /* FORM_SHINY_SEEN_OFFSET */, formdexBit, true); }
				else { printf("FORM_SEEN_FLAG\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x368 + 018*0 /* FORM_SEEN_OFFSET */, formdexBit, true); }

				if (!isDisplayed)
				{
					if (isShiny) { printf("FORM_SHINY_DISPLAYED_FLAG\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x368 + 0x18*3 /* FORM_SHINY_DISPLAYED_OFFSET */, formdexBit, true); }
					else { printf("FORM_DISPLAYED_FLAG\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x368 + 0x18*2 /* FORM_DISPLAYED_OFFSET */, formdexBit, true); }
					isDisplayed = true;
				}
			}
		}

		// Seen
		if (isFemale) // Female
		{
			if (isShiny) { printf("FEMALE_SHINY_SEEN_FLAG\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x8 + 0x60 + 0x60*1 + 0x60*2*1 /* FEMALE_SHINY_SEEN_OFFSET */, speciesID, true); }
			else { printf("FEMALE_SEEN_FLAG\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x8 + 0x60 + 0x60*1 + 0x60*2*0 /* FEMALE_SEEN_OFFSET */, speciesID, true); }
		}
		else // Male or Genderless
		{
			if (isShiny) { printf("MALE_SHINY_SEEN_FLAG\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x8 + 0x60 + 0x60*0 + 0x60*2*1 /* MALE_SHINY_SEEN_OFFSET */, speciesID, true); }
			else { printf("MALE_SEEN_FLAG\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x8 + 0x60 + 0x60*0 + 0x60*2*0 /* MALE_SEEN_OFFSET */, speciesID, true); }
		}

		// Displayed
		if (!isDisplayed)
		{
			if (!isFemale) // Female
			{
				if (isShiny) { printf("FEMALE_SHINY_DISPLAYED_FLAG\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x8 + 0x60*5 + 0x60*1 + 0x60*2*1 /* FEMALE_SHINY_DISPLAYED_OFFSET */, speciesID, true); }
				else { printf("FEMALE_DISPLAYED_FLAG\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x8 + 0x60*5 + 0x60*1 + 0x60*2*0 /* FEMALE_DISPLAYED_OFFSET */, speciesID, true); }
			}
			else // Male or Genderless
			{
				if (isShiny) { printf("MALE_SHINY_DISPLAYED_FLAG\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x8 + 0x60*5 + 0x60*0 + 0x60*2*1 /* MALE_SHINY_DISPLAYED_OFFSET */, speciesID, true); }
				else { printf("MALE_DISPLAYED_FLAG\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x8 + 0x60*5 + 0x60*0 + 0x60*2*0 /* MALE_DISPLAYED_OFFSET */, speciesID, true); }
			}
			isDisplayed = true;
		}

		// Lang
		if (lang >= 0 && lang < 7)
		{
			{ printf("LANG_FLAG\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x3C8 /* LANG_OFFSET */, speciesID * 7 + lang, true); }
		}

		// ForeignDex
		if (speciesID < 650 && !isKalosBorn)
		{
			{ printf("FOREIGN_FLAG\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x64C /* FOREIGN_OFFSET */, speciesID, true); }
		}
		// Owned
		else
		{
			{ printf("OWNED_FLAG\n"); setOffsetBit(sav, SaveConst::XY_offsetDex + 0x8 /* OWNED_OFFSET */, speciesID, true); }
		}

		hidScanInput(); if (hidKeysHeld() & KEY_L) waitKey(KEY_ANY);
	}

	void importToORAS(savebuffer_t sav, pkm_s* pkm)
	{
		bool isShiny = Pokemon::isShiny(pkm);
		bool isFemale = Pokemon::gender(pkm) == 1;
		u16 speciesID = Pokemon::speciesID(pkm) - 1;
		u8 formID = Pokemon::formID(pkm);
		u8 lang = Pokemon::language(pkm);
		if (lang-- > 5) lang--; // { 0 ,.., 7 }

		printf("\naddDexORAS:\n");

		bool isDisplayed = false;

		isDisplayed |= getOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x8 + 0x60*5 /* MALE_DISPLAY_OFFSET */, speciesID);
		isDisplayed |= getOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x8 + 0x60*6 /* FEMALE_DISPLAY_OFFSET */, speciesID);
		isDisplayed |= getOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x8 + 0x60*7 /* MALE_SHINY_DISPLAY_OFFSET */, speciesID);
		isDisplayed |= getOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x8 + 0x60*8 /* FEMALE_SHINY_DISPLAY_OFFSET */, speciesID);

		// Formdex
		if (formID > 0)
		{
			s32 formdexBit = getFormDexOffsetORAS(speciesID + 1);

			if (formdexBit >= 0) // Has an entry in the formdex
			{
				formdexBit += formID;

				isDisplayed |= getOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x368 + 0x26*2 /* FORM_DISPLAY_OFFSET */, formdexBit);
				isDisplayed |= getOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x368 + 0x26*3 /* FORM_SHINY_DISPLAY_OFFSET */, formdexBit);

				if (isShiny) { printf("FORM_SHINY_SEEN_FLAG\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x368 + 0x26*1 /* FORM_SHINY_SEEN_OFFSET */, formdexBit, true); }
				else { printf("FORM_SEEN_FLAG\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x368 + 0x26*0 /* FORM_SEEN_OFFSET */, formdexBit, true); }

				if (!isDisplayed)
				{
					if (isShiny) { printf("FORM_SHINY_DISPLAYED_FLAG\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x368 + 0x26*3 /* FORM_SHINY_DISPLAYED_OFFSET */, formdexBit, true); }
					else { printf("FORM_DISPLAYED_FLAG\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x368 + 0x26*2 /* FORM_DISPLAYED_OFFSET */, formdexBit, true); }
					isDisplayed = true;
				}
			}
		}

		// Seen
		if (isFemale) // Female
		{
			if (isShiny) { printf("FEMALE_SHINY_SEEN_FLAG\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x8 + 0x60 + 0x60*1 + 0x60*2*1 /* FEMALE_SHINY_SEEN_OFFSET */, speciesID, true); }
			else { printf("FEMALE_SEEN_FLAG\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x8 + 0x60 + 0x60*1 + 0x60*2*0 /* FEMALE_SEEN_OFFSET */, speciesID, true); }
		}
		else // Male or Genderless
		{
			if (isShiny) { printf("MALE_SHINY_SEEN_FLAG\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x8 + 0x60 + 0x60*0 + 0x60*2*1 /* MALE_SHINY_SEEN_OFFSET */, speciesID, true); }
			else { printf("MALE_SEEN_FLAG\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x8 + 0x60 + 0x60*0 + 0x60*2*0 /* MALE_SEEN_OFFSET */, speciesID, true); }
		}

		// Displayed
		if (!isDisplayed)
		{
			if (isFemale) // Female
			{
				if (isShiny) { printf("FEMALE_SHINY_DISPLAYED_FLAG\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x8 + 0x60*5 + 0x60*1 + 0x60*2*1 /* FEMALE_SHINY_DISPLAYED_OFFSET */, speciesID, true); }
				else { printf("FEMALE_DISPLAYED_FLAG\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x8 + 0x60*5 + 0x60*1 + 0x60*2*0 /* FEMALE_DISPLAYED_OFFSET */, speciesID, true); }
			}
			else // Male or Genderless
			{
				if (isShiny) { printf("MALE_SHINY_DISPLAYED_FLAG\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x8 + 0x60*5 + 0x60*0 + 0x60*2*1 /* MALE_SHINY_DISPLAYED_OFFSET */, speciesID, true); }
				else { printf("MALE_DISPLAYED_FLAG\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x8 + 0x60*5 + 0x60*0 + 0x60*2*0 /* MALE_DISPLAYED_OFFSET */, speciesID, true); }
			}
			isDisplayed = true;
		}

		// Lang
		if (lang >= 0 && lang < 7)
		{
			{ printf("LANG_FLAG\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x400 /* LANG_OFFSET */, speciesID * 7 + lang, true); }
		}

		// DexNav
		u16* dexNav = (u16*)(sav + SaveConst::ORAS_offsetDex + 0x686 /* DEXNAV_OFFSET */ + speciesID * 2);
		if (*dexNav == 0) { printf("DEXNAV_FLAG\n"); *dexNav = 1; }

		// Owned
		{ printf("OWNED_FLAG\n"); setOffsetBit(sav, SaveConst::ORAS_offsetDex + 0x8 /* OWNED_OFFSET */, speciesID, true); }

		hidScanInput(); if (hidKeysHeld() & KEY_L) waitKey(KEY_ANY);
	}
}
