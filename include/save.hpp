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
#define PKM_COUNT (721)

#define PKM_SIZE (0xE8) // 232
#define BOX_SIZE (PKM_SIZE * BOX_PKM_COUNT)
#define PC_SIZE (BOX_SIZE * PC_BOX_COUNT)
#define BANK_SIZE (BOX_SIZE * BANK_BOX_COUNT)

#define SAVEDATA_SIZE (0x76000)
#define BANKDATA_SIZE (0xACA00)
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
	bool checked : 1;	///< If the Pokémon has been checked for multi-select.
	bool moved : 1;		///< If the Pokémon has moved.
	bool modified : 1;	///< If the Pokémon was modified.
	bool isEggy : 1;	///< If the Pokémon is an egg.
	bool isShiny : 1;	///< If the Pokémon is shiny.
	unsigned: 2;

	u16 speciesID;		///< The species of the Pokémon.
	u16 itemID;			///< The held item of the Pokémon.
	u8 formID;			///< The form of the Pokémon.
	u8 gender;			///< The gender of the Pokémon.

	// TODO: Add recurrent attributes!
};

struct box_s
{
	pkm_s slot[BOX_PKM_COUNT];	///< The slots of the box.
	char title[0x11];			///< The title of the box.
	u8 background;				///< The background id of the box.
	u8 number;					///< The number (id) of the box.
	u16 count; 					///< The count of Pokémon (static).
	// TODO: Make that variable runtime dynamic!
};

struct pc_s
{
	box_s box[31];	///< Boxes
	u8 boxUnlocked;	///< Unlocked boxes
};

struct bk_s
{
	box_s wbox;			///< Wonderbox
	box_s box[100];		///< Boxes
	u8 boxUnlocked;		///< Unlocked boxes
	bool wboxUnlocked;	///< Wonder box unlocked
};

struct savedata_s
{
	pc_s pc;
	u16 TID, SID, TSV;	///< The ID of the trainer
	u8 GEORegion;		///< The region of the trainer
	u8 GEOCountry;		///< The country of the trainer
	u8 OTGender;		///< The gender of the trainer
	u16 OT_name[0xD];	///< The name of the trainer /!\ REPLACED
	uint32_t OTName[0xD];
};

struct bankdata_s
{
	u32 magic;
	u32 version;
	bk_s bk;
};

namespace Game
{
	enum gameVersion_e
	{
		None = 0,
		X = 1 << 0,	// 24
		Y = 1 << 1, // 25
		XY = X | Y,
		OR = 1 << 2, // 26
		AS = 1 << 3, // 27
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
	const saveConst_t XY_offsetDex = 0x15000; // 0x1A400 - 0x5400

	const saveConst_t ORAS_size = 0x76000;
	const saveConst_t ORAS_offsetPCLayout = 0x4400;
	const saveConst_t ORAS_offsetPCBackground = ORAS_offsetPCLayout + 0x41E;
	const saveConst_t ORAS_offsetPC = 0x33000;
	const saveConst_t ORAS_offsetTrainerCard = 0x14000;
	const saveConst_t ORAS_offsetDex = 0x15000; // 0x1A400 - 0x5400

	const saveConst_t BANK_size = 0xACA00;
	const saveConst_t BANK_offsetMagicNumber = 0x00;
	const saveConst_t BANK_offsetVersion = 0x04;
	const saveConst_t BANK_offsetOffsetBK = 0x20;
	const saveConst_t BANK_offsetOffsetBKLayout = 0x24;
	const saveConst_t BANK_offsetOffsetBKBackground = 0x28;
	const saveConst_t BANK_offsetOffsetWonderBox = 0x30;
}

#endif // SAVE_HPP
