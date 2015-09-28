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

typedef uint8_t pkm_t[PKM_SIZE];

typedef pkm_t box_t[BOX_PKMCOUNT];

typedef box_t pc_t[PC_BOXCOUNT];
typedef box_t bank_t[BANK_BOXCOUNT];

// typdef uint8_t pc_t[PC_BOXCOUNT][BOX_PKMCOUNT][PKM_SIZE];
// typdef uint8_t bank_t[BANK_BOXCOUNT][BOX_PKMCOUNT][PKM_SIZE];

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

/* ---------- Functions ---------- */

class PKBank
{
	public:
		PKBank();
		~PKBank();

		/// Propertie: `save`
		bool saveLoaded();
		/// Propertie: `bank`
		bool bankLoaded();

		/// Load data
		bool load(Result fs, Handle *sdHandle, Handle *saveHandle, FS_archive *sdArchive, FS_archive *saveArchive);
		bool loadSave(Handle *fsHandle, FS_archive *fsArchive);
		bool loadBank(Handle *fsHandle, FS_archive *fsArchive);
		void loadPokemonSave(uint16_t box, uint16_t slot, pkm_t pkm);
		void loadPokemonSave(uint32_t offsetSlot, pkm_t pkm);
		void loadPokemonBank(uint16_t box, uint16_t slot, pkm_t pkm);
		void loadPokemonBank(uint32_t offsetSlot, pkm_t pkm);
		void loadPokemon(ek6_t* ek6, pkm_t pkm);

		/// Save data
		bool save(Result fs, Handle *sdHandle, Handle *saveHandle, FS_archive *sdArchive, FS_archive *saveArchive);
		bool saveSave(Handle *fsHandle, FS_archive *fsArchive);
		bool saveBank(Handle *fsHandle, FS_archive *fsArchive);

		/// Backup data
		bool backupSave(Handle *fsHandle, FS_archive *fsArchive);

		savedata_t* savedata;
		bankdata_t* bankdata;

		Game::gametype_e gametype;

		static Game::gametype_e getGame(uint32_t bytesRead);

		void printPkm(pkm_t pkm, u32 key = 0, u32 max = PKM_SIZE);
		void printPk6(pk6_t* pk6, u32 key = 0, u32 max = PK6_SIZE);

		void convertPkmPk6(pkm_t pkm, pk6_t* pk6);
		void encryptPokemon(ek6_t* ek6, pk6_t* pk6);
	private:

		// void convertPkmPk6(pkm_t pkm, pk6_t* pk6);
		void convertPk6Pkm(pk6_t* pk6, pkm_t pkm);
		void convertPk6Ek6(pk6_t* pk6, ek6_t* ek6);
		void convertEk6Pk6(ek6_t* ek6, pk6_t* pk6);

		// void encryptPokemon(ek6_t* ek6, pk6_t* pk6);
		void decryptPokemon(pk6_t* pk6, ek6_t* ek6);
		void shufflePokemon(pk6_t* pk6, uint8_t sv);

		uint32_t LCRNG(uint32_t seed);

		/// Write buffers
		bool writeBuffers();
		bool writeSaveBuffer();
		bool writeBankBuffer();
		void editSaveBuffer(uint32_t pos, uint8_t* ptr, uint32_t size);
		void editBankBuffer(uint32_t pos, uint8_t* ptr, uint32_t size);

		savebuffer_t savebuffer;
		bankbuffer_t bankbuffer;
};

#endif // PBANK_HPP