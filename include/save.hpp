#pragma once
/**
 * @file save.hpp
 */
#ifndef SAVE_HPP
#define SAVE_HPP

#include <3ds/types.h>

#define BOX_ROW_PKM_COUNT (5)
#define BOX_COL_PKM_COUNT (6)
#define BOX_PKM_COUNT (BOX_ROW_PKM_COUNT * BOX_COL_PKM_COUNT)
#define PC_BOX_COUNT (31) // 0x1F
#define BANK_BOX_COUNT (100) // 0x64
#define PKM_COUNT (721)

#define PKM_SIZE (0xE8) // 232
#define BOX_SIZE (PKM_SIZE * BOX_PKM_COUNT)
#define PC_SIZE (BOX_SIZE * PC_BOX_COUNT)
#define BANK_SIZE (BOX_SIZE * BANK_BOX_COUNT)

#define SAVEDATA_SIZE (0x76000)
#define BANKDATA_SIZE (0xAA100)
#define EK6_SIZE PKM_SIZE
#define PK6_SIZE PKM_SIZE

typedef u8 ek6_t;	///< Encrypted Pokémon Gen6 type.
typedef u8 pk6_t;	///< Pokémon Gen6 type.

typedef u8 savebuffer_t[SAVEDATA_SIZE];	///< Save buffer type.
typedef u8 bankbuffer_t[BANKDATA_SIZE];	///< Bank buffer type.

struct pkm_s
{
	ek6_t* ek6 = NULL;	///< Pointer to MainBuffer. (don't free)
	pk6_t* pk6 = NULL;	///< Pointer to OwnBuffer. (do free)
	bool fromBank : 1;	///< If the Pokémon was in the bank.
	bool moved : 1;		///< If the Pokémon has moved.
	bool modified : 1;	///< If the Pokémon was modified.
	bool isEggy : 1;	///< If the Pokémon is an egg.
	bool isShiny : 1;	///< If the Pokémon is shiny.
	unsigned: 3;

	u16 speciesID;		///< The species of the Pokémon.
	u16 itemID;			///< The held item of the Pokémon.
	u8 formID;			///< The form of the Pokémon.
	u8 gender;			///< The gender of the Pokémon.

	// u8* species;		///< The name of the species.
	// u8* item;			///< The name of the held item.

	// TODO: Add recurrent attributes!
};

struct box_s
{
	pkm_s slot[BOX_PKM_COUNT];
	char title[0x11];
	u8 background;
	u8 number;
};

struct pc_s
{
	box_s box[PC_BOX_COUNT];
	u8 boxUnlocked;
};

struct bk_s
{
	box_s box[BANK_BOX_COUNT];
	u8 boxUnlocked;
};

struct savedata_s
{
	pc_s pc;
	u16 TID; // 0x0
	u16 SID; // 0x2
	u16 TSV;
	u8 OTGender; // 0x05
	char OTName[0xD]; // 0x48
	u8 GEORegion; // 0x26
	u8 GEOCountry; // 0x27
};

struct bankdata_s
{
	u32 magic;
	u32 version;
	bk_s bk;
};

static inline uint32_t MakeMagic(char a, char b, char c, char d)
{
	return a | b << 8 | c << 16 | d << 24;
}

namespace Game
{
	enum gameVersion_e
	{
		None = 0,
		X = 1 << 0,
		Y = 1 << 1,
		XY = X | Y,
		OR = 1 << 2,
		AS = 1 << 3,
		ORAS = OR | AS,
	};

	inline bool is(gameVersion_e g1, gameVersion_e g2) { return (g1 & g2); }
};

typedef Game::gameVersion_e GameVersion;

typedef u32 saveConst_t;

namespace SaveConst
{
	const saveConst_t XY_size = 0x65600;
	const saveConst_t XY_offsetPCLayout = 0x4400;
	const saveConst_t XY_offsetPCBackground = XY_offsetPCLayout + 0x41E;
	const saveConst_t XY_offsetPC = 0x22600;
	const saveConst_t XY_offsetTrainerCard = 0x14000;
	const saveConst_t XY_offsetDex = 0x1A400 - 0x05400; // 0x15000

	const saveConst_t ORAS_size = 0x76000;
	const saveConst_t ORAS_offsetPCLayout = 0x4400;
	const saveConst_t ORAS_offsetPCBackground = ORAS_offsetPCLayout + 0x41E;
	const saveConst_t ORAS_offsetPC = 0x33000;
	const saveConst_t ORAS_offsetTrainerCard = 0x14000;
	const saveConst_t ORAS_offsetDex = 0x1A400 - 0x05400; // 0x15000

	const saveConst_t BANK_size = 0xAA100;
	const saveConst_t BANK_offsetMagicNumber = 0x0;
	const saveConst_t BANK_offsetVersion = 0x4;
	const saveConst_t BANK_offsetBK = 0x100;
	const saveConst_t BANK_offsetBKBackground = 0xAA000;
}

#endif // SAVE_HPP
