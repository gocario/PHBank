#ifndef PBANK_HPP
#define PBANK_HPP

/* ---------- Includes ---------- */

#include <stdint.h>
#include <stdio.h>
#include <3ds.h>

#include "main.hpp"
#include "filesystem.hpp"


/* ---------- Defines ---------- */

#define BOX_ROW_PKMCOUNT 5
#define BOX_COL_PKMCOUNT 6
#define BOX_PKMCOUNT BOX_ROW_PKMCOUNT * BOX_COL_PKMCOUNT
#define PC_BOXCOUNT 31 // 0x1f
#define BANK_BOXCOUNT 100 // 0x64

#define PKM_SIZE 0xe8
#define BOX_SIZE PKM_SIZE * BOX_PKMCOUNT
#define PC_SIZE BOX_SIZE * PC_BOXCOUNT
#define BANK_SIZE BOX_SIZE * BANK_BOXCOUNT
#define PCNAME_SIZE 0x22


#define PC_XY_OFFSET 0x22600
#define PC_ORAS_OFFSET 0x33000
#define PCNAME_XY_OFFSET 0x4400
#define PCNAME_ORAS_OFFSET 0x4400
#define POKEDEX_XY_OFFSET 0x15000
#define POKEDEX_ROSA_OFFSET 0x15000
#define BANK_PKBK_OFFSET 0x100

#define SAVEDATA_XY_SIZE 0x65600 // 0x34ad0 (PC size)
#define SAVEDATA_ORAS_SIZE 0x76000 // 0x34ad0 (PC size)
#define BANKDATA_PKBK_SIZE 0xaa000 // 0xa9ec0 (Bank size)


#define EK6_SIZE PKM_SIZE
#define PK6_SIZE PKM_SIZE

/* ---------- Types ---------- */

typedef u8 ek6_t;
typedef u8 pk6_t;

struct pkm_t
{
	ek6_t* ek6 = NULL; // Pointer to MainBuffer
	pk6_t* pk6 = NULL; // Pointer to OwnBuffer

	uint16_t species;
	uint16_t TID;
	uint16_t SID;
	uint32_t PID;

	// T O D O !!
};

struct box_t
{
	pkm_t slot[BOX_PKMCOUNT];
};

struct pc_t
{
	box_t box[PC_BOXCOUNT];
};

struct bank_t
{
	box_t box[BANK_BOXCOUNT];
};

struct savedata_t
{
	pc_t pc;
};

struct bankdata_t
{
	bank_t bank;
};

typedef uint8_t savebuffer_t[SAVEDATA_ORAS_SIZE];
typedef uint8_t bankbuffer_t[BANKDATA_PKBK_SIZE];

namespace Game
{
	typedef enum
	{
		None = 0x0,
		XY = 0x1,
		ORAS = 0x2,
	} gametype_e;
};

typedef Game::gametype_e gametype_e;

namespace CursorType
{
	enum CursorType_e
	{
		None = 0x0,
		SingleSelect = 0x1,
		QuickSelect = 0x2,
		MultipleSelect = 0x3,
	};
}

typedef CursorType::CursorType_e CursorType_e;

struct CursorBox_t
{
	pkm_t* vPkm = NULL;
	pkm_t* sPkm = NULL;

	u16 slot = 0;

	u16 pcSlot = 0;
	s16 pcBox = 0;
	s16 pcRow = 0;
	s16 pcCol = 0;

	u16 bkSlot = 0;
	s16 bkBox = 0;
	s16 bkRow = 0;
	s16 bkCol = 0;

	bool inBank = false;
	CursorType_e cursorType = CursorType::SingleSelect;
};

/* ---------- Functions ---------- */

class PKBank
{
	public:
		PKBank();
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

		void getPkm(uint16_t slotId, pkm_t** pkm, bool inBank = false);
		void getPkm(uint16_t boxId, uint16_t slotId, pkm_t** pkm, bool inBank = false);
		void getPkm(uint16_t boxId, uint16_t rowId, uint16_t colId, pkm_t** pkm, bool inBank = false);
		void movePkm(pkm_t* src, pkm_t* dest);


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

		Result loadSaveData();
		Result loadBankData();
		void loadPkmPC(uint16_t boxId, uint16_t slotId);
		void loadPkmBK(uint16_t boxId, uint16_t slotId);
		void loadEk6PC(pkm_t* pkm, uint32_t offsetSlot);
		void loadEk6BK(pkm_t* pkm, uint32_t offsetSlot);
		void loadPk6Ek6(pkm_t* pkm);
		void loadPkmPk6(pkm_t* pkm);

		Result saveSaveData();
		Result saveBankData();
		void savePkmPC(uint16_t boxId, uint16_t slotId);
		void savePkmBK(uint16_t boxId, uint16_t slotId);
		void saveEk6PC(pkm_t* pkm);
		void saveEk6BK(pkm_t* pkm);
		void savePk6Ek6(pkm_t* pkm);
		void savePkmPk6(pkm_t* pkm);
		void editSaveBuffer(uint32_t pos, uint8_t* ptr, uint32_t size);
		void editBankBuffer(uint32_t pos, uint8_t* ptr, uint32_t size);

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