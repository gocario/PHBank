#ifndef PBANK_HPP
#define PBANK_HPP

/* ---------- Includes ---------- */

#include <stdint.h>
#include <stdio.h>
#include <3ds.h>

#include "main.hpp"
#include "filesystem.hpp"
#include "pkfilter.hpp"


/* ---------- Defines ---------- */

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
#define BANK_PKBK_OFFSET 0x100

#define SAVEDATA_XY_SIZE 0x65600 // 0x34ad0 (PC size)
#define SAVEDATA_ORAS_SIZE 0x76000 // 0x34ad0 (PC size)
#define BANKDATA_PKBK_SIZE 0xaa000 // 0xa9ec0 (Bank size)


#define EK6_SIZE PKM_SIZE
#define PK6_SIZE PKM_SIZE

#define PK_SPECIES_SIZE 20
#define PK_ITEM_SIZE 20

/* ---------- Types ---------- */

typedef u8 ek6_t;
typedef u8 pk6_t;

typedef struct pkm_t
{
	ek6_t* ek6 = NULL; // Pointer to MainBuffer
	pk6_t* pk6 = NULL; // Pointer to OwnBuffer

	u8* species;
	u8* item;
	uint16_t speciesID; // 0x08
	uint16_t itemID; // 0x??
	uint16_t TID; // 0x0c
	uint16_t SID; // 0x0e
	uint32_t PID; // 0x18
	uint16_t PSV;
	uint8_t gender; // 0x1d
	uint8_t origin; // 0xDF
	uint8_t lang; // 0xe3
	bool isShiny; 

	// T O D O !!
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
	unsigned : 0;
	uint16_t dexNavLevel : 16;
} dex_t;

typedef struct pokedex_t
{
	dex_t dexes[0x60 * 8]; // [PKM_COUNT]
} pokedex_t;


typedef struct savedata_t
{
	pc_t pc;
	pokedex_t pokedex;
	uint16_t TID; // 0x0
	uint16_t SID; // 0x2
	uint16_t TSV;
	uint8_t OTName[0x1a]; // 0x48

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

namespace CursorType
{
	typedef enum cursorType_e
	{
		None = 0x0,
		SingleSelect = 0x1,
		QuickSelect = 0x2,
		MultipleSelect = 0x4,
	} CursorType_e;
}

typedef CursorType::CursorType_e CursorType_e;

typedef struct cursorPosition_t
{
	uint16_t slot = 0;
	uint16_t inslot = 0;
	int16_t box = 0;
	int16_t row = 0;
	int16_t col = 0;
} CursorPosition_t;

typedef struct cursorBox_t
{
	pkm_t* vPkm = NULL;
	pkm_t* sPkm = NULL;

	uint16_t slot = 0;

	CursorPosition_t cPosPC;
	CursorPosition_t cPosBK;

	bool inBank = false;
	CursorType_e cursorType = CursorType::SingleSelect;
} CursorBox_t;


/* ---------- Functions ---------- */

class PKBank
{
	public:
		explicit PKBank();
		~PKBank();

		Result load(Result fs, Handle* sdHandle, Handle* saveHandle, FS_archive* sdArchive, FS_archive* saveArchive);
		Result save(Result fs, Handle* sdHandle, Handle* saveHandle, FS_archive* sdArchive, FS_archive* saveArchive);
		
		Result loadFile(Result fs, Handle* sdHandle, Handle* saveHandle, FS_archive* sdArchive, FS_archive* saveArchive);
		Result saveFile(Result fs, Handle* sdHandle, Handle* saveHandle, FS_archive* sdArchive, FS_archive* saveArchive);
		Result loadData();
		Result saveData();

		static void printByte(u8* bytes, u32 key = 0x0, uint32_t max = 0x0);
		static void printPkm(pkm_t* pkm, u32 key = 0x0, uint32_t max = 0x0);
		static gametype_e getGame(uint32_t bytesRead);

		bool isPkmEmpty(pkm_t* pkm);
		bool isSlotEmpty(uint16_t boxId, uint16_t slotId, bool inBank);
		void getBox(uint16_t boxID, box_t** box, bool inBank = false);
		void getPokemon(uint16_t slotId, pkm_t** pkm, bool inBank = false);
		void getPokemon(uint16_t boxId, uint16_t slotId, pkm_t** pkm, bool inBank = false);
		void getPokemon(uint16_t boxId, uint16_t rowId, uint16_t colId, pkm_t** pkm, bool inBank = false);
		void movePkm(pkm_t* src, pkm_t* dest);
		void moveBox(uint16_t boxID_1, bool inBank_1, uint16_t boxID_2, bool inBank_2);
		void addDex(uint16_t speciesID);
		void addDex(pkm_t* pkm);


		savedata_t* savedata;
		bankdata_t* bankdata;

	private:

		gametype_e gametype;
		savebuffer_t savebuffer;
		bankbuffer_t bankbuffer;

		Result loadSaveFile(Handle *fsHandle, FS_archive *fsArchive);
		Result loadBankFile(Handle *fsHandle, FS_archive *fsArchive);
		Result saveSaveFile(Handle *fsHandle, FS_archive *fsArchive);
		Result saveBankFile(Handle *fsHandle, FS_archive *fsArchive);
		Result backupSaveFile(Handle *fsHandle, FS_archive *fsArchive);

		// Load Data
		Result loadSaveData();
		Result loadBankData();
		// Load Pokemon (pkm_t)
		void loadPkmPC(uint16_t boxId, uint16_t slotId);
		void loadPkmBK(uint16_t boxId, uint16_t slotId);
		void loadEk6PC(pkm_t* pkm, uint32_t offsetSlot);
		void loadEk6BK(pkm_t* pkm, uint32_t offsetSlot);
		void loadPk6Ek6(pkm_t* pkm);
		void loadPkmPk6(pkm_t* pkm);
		// Load Pokedex (pokedex_t)
		void loadDex();

		// Save Data
		Result saveSaveData();
		Result saveBankData();
		// Save Pokemon (pkm_t)
		void savePkmPC(uint16_t boxId, uint16_t slotId);
		void savePkmBK(uint16_t boxId, uint16_t slotId);
		void saveEk6PC(pkm_t* pkm);
		void saveEk6BK(pkm_t* pkm);
		void savePk6Ek6(pkm_t* pkm);
		void savePkmPk6(pkm_t* pkm);
		// Save Pokedex (pokedex_t)
		void saveDex();
		// Edit Buffer
		void editSaveBuffer(uint32_t pos, uint8_t* ptr, uint32_t size);
		void editBankBuffer(uint32_t pos, uint8_t* ptr, uint32_t size);

		// Pokemon
		void loadDexPkm(pkm_t* pkm);
		uint16_t computePSV(uint32_t PID);
		uint16_t computeTSV(uint16_t TID, uint16_t SID);
		bool isShiny(uint32_t PID, uint16_t TID, uint16_t SID);

		// Encryption
		uint32_t LCRNG(uint32_t seed);
		void shufflePk6(pk6_t* pk6, uint8_t sv);
		void decryptEk6(pkm_t* pkm);
		void encryptPk6(pkm_t* pkm);

		// Save Checksum
		uint32_t CHKOffset(uint32_t i);
		uint32_t CHKLength(uint32_t i);
		uint16_t ccitt16(pk6_t* pk6, uint32_t len);
		void rewriteSaveCHK();
};

#endif // PBANK_HPP