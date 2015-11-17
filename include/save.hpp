#ifndef SAVE_HPP
#define SAVE_HPP

#include <3ds/types.h>

#define BOX_ROW_PKMCOUNT 5
#define BOX_COL_PKMCOUNT 6
#define BOX_PKMCOUNT BOX_ROW_PKMCOUNT * BOX_COL_PKMCOUNT
#define PC_BOXCOUNT 31 // 0x1f
#define BANK_BOXCOUNT 100 // 0x64
#define PKM_COUNT 721

#define PKM_SIZE 0xe8
#define BOX_SIZE PKM_SIZE * BOX_PKMCOUNT
#define PC_SIZE BOX_SIZE * PC_BOXCOUNT
#define BANK_SIZE BOX_SIZE * BANK_BOXCOUNT
#define PCNAME_SIZE 0x22


#define PC_XY_OFFSET 0x22600
#define PC_ORAS_OFFSET 0x33000
#define PCNAME_XY_OFFSET 0x4400
#define PCNAME_ORAS_OFFSET 0x4400
#define TRAINERCARD_XY_OFFSET 0x14000
#define TRAINERCARD_ORAS_OFFSET 0x14000
#define POKEDEX_XY_OFFSET 0x15000
#define POKEDEX_ORAS_OFFSET 0x15000
#define POKEDEXLANG_XY_OFFSET POKEDEX_XY_OFFSET + 0x3c8
#define POKEDEXLANG_ORAS_OFFSET POKEDEX_ORAS_OFFSET + 0x400
#define ENCOUNTERCOUNT_ORAS_OFFSET POKEDEX_ORAS_OFFSET + 0x686
#define BANK_PKBK_OFFSET 0x100

#define SAVEDATA_XY_SIZE 0x65600 // 0x34ad0 (PC size)
#define SAVEDATA_ORAS_SIZE 0x76000 // 0x34ad0 (PC size)
#define BANKDATA_PKBK_SIZE 0xaa000 // 0xa9ec0 (Bank size)


#define EK6_SIZE PKM_SIZE
#define PK6_SIZE PKM_SIZE

#define PK_SPECIES_SIZE 20
#define PK_ITEM_SIZE 20


typedef u8 ek6_t;
typedef u8 pk6_t;

typedef struct pkm_t
{
	ek6_t* ek6 = NULL; // Pointer to MainBuffer
	pk6_t* pk6 = NULL; // Pointer to OwnBuffer
	bool moved : 1;
	bool modified : 1;
	bool isShiny : 1;
	unsigned : 0;
	
	u8* species;
	u8* item;
	u16 speciesID;
	u16 itemID;
	// T O D O !!

	pkm_t() : moved {false}, modified {false} {}
} pkm_t;


typedef struct box_t
{
	pkm_t slot[BOX_PKMCOUNT];
} box_t;

typedef struct pc_t
{
	box_t box[PC_BOXCOUNT];
} pc_t;

typedef struct bank_t
{
	box_t box[BANK_BOXCOUNT];
} bank_t;


typedef struct PACKED dex_t
{
	bool owned : 1;
	bool ownedMale : 1;
	bool ownedFemale : 1;
	bool ownedShinyMale : 1;
	bool ownedShinyFemale : 1;
	bool encounteredMale : 1;
	bool encounteredFemale : 1;
	bool encounteredShinyMale : 1;
	bool encounteredShinyFemale : 1;
	bool langJapanese : 1;
	bool langEnglish : 1;
	bool langFrench : 1;
	bool langItalian : 1;
	bool langGerman : 1;
	bool langSpanish : 1;
	bool langKorean : 1;
	bool foreignXY : 1;
	uint16_t dexNavCount : 15;
} dex_t;

typedef struct pokedex_t
{
	dex_t dexes[0x60 * 8]; // [PKM_COUNT]
} pokedex_t;


typedef struct savedata_t
{
	pc_t pc;
	pokedex_t pokedex;
	u16 TID; // 0x0
	u16 SID; // 0x2
	u16 TSV;
	u8 OTGender; // 0x05
	u8 OTName[0x1a / 2]; // 0x48
	u8 GEORegion; // 0x26
	u8 GEOCountry; // 0x27

} savedata_t;

typedef struct bankdata_t
{
	bank_t bank;
} bankdata_t;


typedef uint8_t savebuffer_t[SAVEDATA_ORAS_SIZE];
typedef uint8_t bankbuffer_t[BANKDATA_PKBK_SIZE];

namespace Game
{
	typedef enum gametype_e
	{
		None = 0x0,
		XY = 0x1,
		ORAS = 0x2,
	} Gametype_e;
};

typedef Game::gametype_e gametype_e;

#endif // SAVE_HPP
