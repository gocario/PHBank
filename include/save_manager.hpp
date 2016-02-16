#pragma once
/**
 * @file save_manager.hpp
 * @brief Save Manager
 */
#ifndef SAVE_MANAGER_HPP
#define SAVE_MANAGER_HPP

#include <3ds/services/fs.h>

#include "save.hpp"

class SaveManager
{
	public:
		bool loaded;
		GameVersion version;

		savebuffer_t savebuffer;
		bankbuffer_t bankbuffer;
		savedata_s savedata;
		bankdata_s bankdata;

	public:
		saveConst_t offsetTrainerCard;
		saveConst_t offsetPCLayout;
		saveConst_t offsetPCBackground;
		saveConst_t offsetPC;
		saveConst_t offsetBK;
		saveConst_t offsetBKBackground;
		saveConst_t sizeSave;
		saveConst_t sizeBank;

	public:
		SaveManager();
		~SaveManager();

		Result load();
		Result save();
		Result backupFile();

		void setGame(u32 bytesRead);
		void setGameOffsets();

		bool isPkmEmpty(pkm_s* pkm);
		bool isSlotEmpty(u16 boxId, u16 slotId, bool inBank);
		void getBox(u16 boxId, box_s** box, bool inBank);
		void getPkm(u16 slotId, pkm_s** pkm, bool inBank);
		void getPkm(u16 boxId, u16 slotId, pkm_s** pkm, bool inBank);
		bool movePkm(pkm_s* src, pkm_s* dst);
		bool movePkm(pkm_s* src, pkm_s* dst, bool srcBanked, bool dstBanked);
		bool pastePkm(pkm_s* src, pkm_s* dst);
		bool pastePkm(pkm_s* src, pkm_s* dst, bool srcBanked, bool dstBanked);
		void moveBox(u16 boxId_1, bool inBank_1, u16 boxId_2, bool inBank_2);
		bool filterPkm(pkm_s* pkm, bool toBank, bool fromBank);

		void addDex(pkm_s* pkm);
		void tradePkm(pkm_s* pkm);
		void tradePkmHT(pkm_s* pkm);


		void decryptEk6(pkm_s* pkm);
		void encryptPk6(pkm_s* pkm);
		void shufflePk6(pk6_t* pk6, u8 sv);
		void rewriteSaveCHK();

		u16 computeTSV(u16 TID, u16 SID);

		u32 LCRNG(u32 seed);
		u32 CHKOffset(u32 i);
		u32 CHKLength(u32 i);
		u16 ccitt16(u8* data, u32 len);

	private:

		Result loadFile();
		Result saveFile();

		Result loadSaveFile();
		Result loadBankFile();
		Result saveSaveFile();
		Result saveBankFile();
		Result backupSaveFile();
		Result backupBankFile();

		// Load Data
		Result loadData();
		Result loadSaveData();
		Result loadBankData();
		// Load Pokemon (pkm_s)
		//   To <- From
		void loadPkmPC(u16 boxId, u16 slotId);
		void loadPkmBK(u16 boxId, u16 slotId);
		void loadEk6PC(pkm_s* pkm, u32 offsetSlot);
		void loadEk6BK(pkm_s* pkm, u32 offsetSlot);
		void loadPk6Ek6(pkm_s* pkm);
		void loadPkmPk6(pkm_s* pkm);

		// Save Data
		Result saveData();
		Result saveSaveData();
		Result saveBankData();
		// Save Pokemon (pkm_s)
		//   From -> To
		void savePkmPC(u16 boxId, u16 slotId);
		void savePkmBK(u16 boxId, u16 slotId);
		void saveEk6PC(pkm_s* pkm);
		void saveEk6BK(pkm_s* pkm);
		void savePk6Ek6(pkm_s* pkm);
		void savePkmPk6(pkm_s* pkm);

};

#endif // SAVE_MANAGER_HPP
