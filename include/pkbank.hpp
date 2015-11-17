#ifndef PKBANK_HPP
#define PKBANK_HPP

/* ---------- Includes ---------- */

#include <3ds/types.h>
#include <3ds/services/fs.h>
#include <stdio.h>
#include <string.h>

#include "save.hpp"

/* ---------- Types ---------- */

typedef struct fs_t
{
	Result fs = 0;
	Handle* sdHandle = NULL;
	Handle* saveHandle = NULL;
	FS_archive* sdArchive = NULL;
	FS_archive* saveArchive = NULL;
} fs_t;


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
		Result backupFile();
		Result loadData();
		Result saveData();

		static void printByte(u8* bytes, u32 key = 0x0, uint32_t max = 0x0);
		static void printPkm(pkm_t* pkm, u32 key = 0x0, uint32_t max = 0x0);
		static gametype_e getGame(uint32_t bytesRead);

		bool isPkmEmpty(pkm_t* pkm);
		bool isSlotEmpty(uint16_t boxId, uint16_t slotId, bool inBank);
		void getBox(uint16_t boxID, box_t** box, bool inBank = false);
		void getPkm(uint16_t slotId, pkm_t** pkm, bool inBank = false);
		void getPkm(uint16_t boxId, uint16_t slotId, pkm_t** pkm, bool inBank = false);
		void getPkm(uint16_t boxId, uint16_t rowId, uint16_t colId, pkm_t** pkm, bool inBank = false);
		bool movePkm(pkm_t* src, pkm_t* dest);
		bool movePkm(pkm_t* src, pkm_t* dst, bool srcBanked, bool dstBanked);
		void moveBox(uint16_t boxID_1, bool inBank_1, uint16_t boxID_2, bool inBank_2);
		bool filterPkm(pkm_t* pkm, bool toBank = false, bool fromBank = false);
		void addDex(pkm_t* pkm);


		fs_t fsData;
		savedata_t* savedata;
		bankdata_t* bankdata;

	private:

		gametype_e gametype;
		savebuffer_t savebuffer;
		bankbuffer_t bankbuffer;

		Result loadSaveFile(Result fs, Handle *fsHandle, FS_archive *fsArchive);
		Result loadBankFile(Handle *fsHandle, FS_archive *fsArchive);
		Result saveSaveFile(Result fs, Handle *fsHandle, FS_archive *fsArchive);
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
		void convertPkmTrainer(pkm_t* pkm);
		void convertPkmHT(pkm_t* pkm);
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

#endif // PKBANK_HPP
