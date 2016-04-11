#include "save_manager.hpp"
#include "bank_updater.hpp"

#include "fs.h"
#include "utils.h"
#include "pkdir.h"
#include "version.h"

#include "personal.hpp"
#include "pokemon.hpp"
#include "pokedex.hpp"
#include "filter.hpp"

#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dirent.h>

// ------------------------------------
SaveManager::SaveManager(void)
// ------------------------------------
{
	offsetTrainerCard = 0x0;
	offsetPCLayout = 0x0;
	offsetPCBackground = 0x0;
	offsetPC = 0x0;

	sizeSave = SaveConst::ORAS_size; // > SaveConst::XY_size
	sizeBank = SaveConst::BANK_size;

	version = Game::None;

	gspWaitForVBlank();
	hidScanInput();
	dev = !(hidKeysHeld() & KEY_L) && !(hidKeysHeld() & KEY_R) && (hidKeysHeld() & KEY_ZL) && (hidKeysHeld() & KEY_ZR);
	if (dev) printf("Dev mode enabled\n");
}


// ------------------------------------
SaveManager::~SaveManager(void)
// ------------------------------------
{
	printf("Deleting PC Boxes: (%u)", savedata.pc.boxUnlocked);
	for (u16 iB = 0; iB < savedata.pc.boxUnlocked; iB++)
	{
		for (u16 iP = 0; iP < BOX_PKM_COUNT; iP++)
		{
			pkm_s* ppkm = &savedata.pc.box[iB].slot[iP];
			if (ppkm != NULL)
			{
				delete[] ppkm->pk6;
				ppkm->pk6 = NULL;
				ppkm->ek6 = NULL;
			}
		}
	}
	printf(" OK\n");

	printf("Deleting BK Boxes: (%u)", bankdata.bk.boxUnlocked);
	for (u16 iB = 0; iB < bankdata.bk.boxUnlocked; iB++)
	{
		for (u16 iP = 0; iP < BOX_PKM_COUNT; iP++)
		{
			pkm_s* ppkm = &bankdata.bk.box[iB].slot[iP];
			if (ppkm->pk6 != NULL)
			{
				delete[] ppkm->pk6;
				ppkm->pk6 = NULL;
				ppkm->ek6 = NULL;
			}
		}
	}
	printf(" OK\n");

	version = Game::None;
}


// ----------------------------------------------
Result SaveManager::load(void)
// ----------------------------------------------
{
	Result ret;

	printf(">Reading...\n");
	ret = loadFile();
	if (R_FAILED(ret)) return ret;

	printf(">Loading...\n");
	loadData();

	return 0;
}


// ----------------------------------------------
Result SaveManager::save(void)
// ----------------------------------------------
{
	Result ret;

	printf("> Writing...\n");
	saveData();

	printf("> Saving...\n");
	ret = saveFile();
	if (R_FAILED(ret)) return ret;

	return 0;
}



	/*--------------------------------------------------*\
	 |              Reading/Writing Section             |
	\*--------------------------------------------------*/


// ----------------------------------------------
Result SaveManager::loadFile(void)
// ----------------------------------------------
{
	Result ret;

	printf(">Loading Save from save\n");
	ret = loadSaveFile();
	if (R_FAILED(ret)) return ret;

	printf(">Loading Bank from sdmc\n");
	ret = loadBankFile();
	if (R_FAILED(ret)) return ret;

	hidScanInput();
	if (hidKeysHeld() & KEY_L)
	{
		printf(">Backing up\n");
		ret = backupFile();
		// if (R_FAILED(ret)) return ret;
	}
	else
	{
		printf(">Not backing up\n");
	}

	return ret;
}


// ----------------------------------------------
Result SaveManager::saveFile(void)
// ----------------------------------------------
{
	Result ret;

	mkdir(SDMC BASE_FOLDER, 0700);
	mkdir(SDMC BANK_FOLDER, 0700);

	printf(">Saving Save to save\n");
	ret = saveSaveFile();
	// if (R_FAILED(ret)) return ret; // ASK Uncomment this?

	printf(">Saving Bank to sdmc\n");
	ret = saveBankFile();
	// if (R_FAILED(ret)) return ret;

	return ret;
}


// ----------------------------------------------
Result SaveManager::backupFile(void)
// ----------------------------------------------
{
	Result ret;

	mkdir(SDMC BASE_FOLDER, 0700);
	mkdir(SDMC BACKUP_FOLDER, 0700);

	printf(">Backing up Save to sdmc\n");
	ret = backupSaveFile();
	// if (R_FAILED(ret)) return ret;

	printf(">Backing up Bank to sdmc\n");
	ret = backupBankFile();
	// if (R_FAILED(ret)) return ret;

	return ret;
}


// ----------------------------------------------
Result SaveManager::loadSaveFile(void)
// ----------------------------------------------
{
	// memset(savebuffer, 0, sizeof(savebuffer_t));

	Result ret;
	u32 bytesRead = 0;
	u32 size = sizeSave;
	char path[0x30];

	sprintf(path, ROOT SAVE_FILE);

	printf("Loading savefile...");
	ret = FS_ReadFile(path, savebuffer, size, &saveArchive, &bytesRead);

	if (R_FAILED(ret)) printf(" ERROR\n");
	else printf(" OK\n");
	printf("  Read %ld/%ld bytes\n", bytesRead, size);

	if (R_SUCCEEDED(ret))
	{
		setGame(bytesRead);
	}

	return ret;
}


// ----------------------------------------------
Result SaveManager::loadBankFile(void)
// ----------------------------------------------
{
	// memset(bankbuffer, 0, sizeof(bankbuffer_t));

	Result ret;
	u32 bytesRead = 0;
	u32 size = sizeBank;
	char path[0x30];

	sprintf(path, SDMC BANK_FOLDER BANK_FILE);

	printf("Loading bankfile...");
	FILE* fp = fopen(path, "rb");
	if (fp)
	{
		printf(" Reading...");
		bytesRead = fread(bankbuffer, 1, size, fp);

		if (ferror(fp)) printf(" ERROR\n");
		else printf(" OK\n");
		printf("  Read %ld/%ld bytes\n", bytesRead, size);

		if (!ferror(fp))
		{
			memset(bankbuffer + bytesRead, 0, size - bytesRead);
		}

		ret = ferror(fp);
		fclose(fp);
	}
	else
	{
		printf(" Creating...");
		memset(bankbuffer, 0, size);
		bytesRead = size;

		printf(" OK\n  Created %ld bytes\n", size);

		ret = 0;
	}

	if (R_SUCCEEDED(ret))
	{
		BankUpdater::updateBank(bankbuffer, bytesRead);
		setBank(bytesRead);
	}

	return ret;
}


// ----------------------------------------------
Result SaveManager::saveSaveFile(void)
// ----------------------------------------------
{
	Result ret;
	u32 bytesWritten = 0;
	u32 size = sizeSave;
	char path[0x30];

	sprintf(path, ROOT SAVE_FILE);

	printf("Deleting old savefile...");
	ret = FS_DeleteFile(path, &saveArchive);
	if (R_FAILED(ret)) printf(" ERROR\n");
	else printf(" OK\n");

	printf("Writing savefile...");
	ret = FS_WriteFile(path, savebuffer, size, &saveArchive, &bytesWritten);
	if (R_FAILED(ret)) printf(" ERROR\n");
	else printf(" OK\n  Written %ld bytes\n", bytesWritten);

	return ret;
}


// ----------------------------------------------
Result SaveManager::saveBankFile(void)
// ----------------------------------------------
{
	Result ret;
	u32 bytesWritten = 0;
	u32 size = sizeBank;
	char path[0x30];

	sprintf(path, ROOT BANK_FOLDER BANK_FILE);

	printf("Writing bankfile...");
	FILE* fp = fopen(path, "wb");
	if (!fp) return -1;

	bytesWritten = fwrite(bankbuffer, 1, size, fp);

	if (ferror(fp)) printf(" ERROR\n");
	else printf(" OK\n");
	printf("  Written %ld bytes\n", bytesWritten);

	ret = ferror(fp);
	fclose(fp);
	return ret;
}


// ----------------------------------------------
Result SaveManager::backupSaveFile(void)
// ----------------------------------------------
{
	Result ret;
	u32 bytesWritten = 0;
	u32 size = sizeSave;
	char path[0x30];

	sprintf(path, ROOT BACKUP_FOLDER SAVE_FILE "_%lli", osGetTime()/* - 2208988800L*/);

	printf("Backing up savefile...");
	FILE* fp = fopen(path, "wb");
	if (!fp) return -1;

	bytesWritten = fwrite(savebuffer, 1, size, fp);

	if (ferror(fp)) printf(" ERROR\n");
	else printf(" OK\n");
	printf("  Written %ld bytes\n", bytesWritten);

	ret = ferror(fp);
	fclose(fp);
	return ret;
}


// ----------------------------------------------
Result SaveManager::backupBankFile(void)
// ----------------------------------------------
{
	Result ret;
	u32 bytesWritten;
	u32 size = sizeBank;
	char path[0x30];

	sprintf(path, SDMC BACKUP_FOLDER BANK_FILE "_%lli", osGetTime()/* - 2208988800L*/);

	printf("Backing up bankfile...");
	FILE* fp = fopen(path, "wb");
	if (!fp) return -1;

	bytesWritten = fwrite(bankbuffer, 1, size, fp);

	if (ferror(fp)) printf(" ERROR\n");
	else printf(" OK\n");
	printf("  Written %ld bytes\n", bytesWritten);

	ret = ferror(fp);
	fclose(fp);
	return ret;
}


	/*--------------------------------------------------*\
	 |                 Loading Section                  |
	\*--------------------------------------------------*/


// ----------------------------------------------
void SaveManager::loadData(void)
// ----------------------------------------------
{
	loadSaveData();
	loadBankData();
}


// ------------------------------------
void SaveManager::loadSaveData(void)
// ------------------------------------
{
	if (Game::is(version, Game::XY) || Game::is(version, Game::ORAS))
	{
		printf("Loading Trainer Card:");
		savedata.TID = *(u16*)(savebuffer + offsetTrainerCard + 0x00);
		savedata.SID = *(u16*)(savebuffer + offsetTrainerCard + 0x02);
		savedata.TSV = (savedata.TID ^ savedata.SID) >> 4;
		savedata.OTGender = *(u8*)(savebuffer + offsetTrainerCard + 0x05);
		savedata.GEORegion = *(u8*)(savebuffer + offsetTrainerCard + 0x26);
		savedata.GEOCountry = *(u8*)(savebuffer + offsetTrainerCard + 0x27);
		memset(savedata.OTName, 0, 0xD * sizeof(uint32_t));
		utf16_to_utf32(savedata.OTName, (u16*)(savebuffer + offsetTrainerCard + 0x48), 0xD);
		str32nfix(savedata.OTName, 0xD);
		printf(" OK\n");

		printf("Loading PC Boxes:");
		savedata.pc.boxUnlocked = *(u8*)(savebuffer + 0x483E);
		if (savedata.pc.boxUnlocked < 30) savedata.pc.boxUnlocked = 30;
		for (u16 iB = 0; iB < savedata.pc.boxUnlocked; iB++)
		{
			savedata.pc.box[iB].count = 0;

			for (u16 iP = 0; iP < BOX_PKM_COUNT; iP++)
			{
				if (!isPkmEmpty(loadPkmPC(iB, iP)))
					savedata.pc.box[iB].count++;
			}

			memset(savedata.pc.box[iB].title, 0, 0x11 * sizeof(uint32_t));
			utf16_to_utf32(savedata.pc.box[iB].title, (u16*)(savebuffer + offsetPCLayout + 0x22 * iB), 0x10);
			savedata.pc.box[iB].background = *(savebuffer + offsetPCBackground + 0x1 * iB);
			savedata.pc.box[iB].number = iB;
		}
		printf(" OK\n");
	}
}


// ----------------------------------------------
void SaveManager::loadBankData(void)
// ----------------------------------------------
{
	printf("Loading BK Card:");
	bankdata.magic = *bankbuffer;
	bankdata.version = *(u32*)(bankbuffer + 0x4);
	printf(" OK\n");

	printf("Loading BK Boxes:");
	bankdata.bk.boxUnlocked = BK_BOX_COUNT;
	for (u16 iB = 0; iB < bankdata.bk.boxUnlocked; iB++)
	{
		bankdata.bk.box[iB].count = 0;

		for (u16 iP = 0; iP < BOX_PKM_COUNT; iP++)
		{
			if (!isPkmEmpty(loadPkmBK(iB, iP)))
				bankdata.bk.box[iB].count++;
		}

		memset(bankdata.bk.box[iB].title, 0, 0x11 * sizeof(uint32_t));
		utf16_to_utf32(bankdata.bk.box[iB].title, (u16*)(bankbuffer + offsetBKLayout + 0x22 * iB), 0x10);
		bankdata.bk.box[iB].background = *(bankbuffer + offsetBKBackground + 0x1 * iB);
		bankdata.bk.box[iB].number = iB;
	}
	printf(" OK\n");

	printf("Loading Wonder box:");
	bankdata.bk.wboxUnlocked = false;
	for (u16 iP = 0; iP < BOX_PKM_COUNT; iP++)
	{
		pkm_s* pkm = loadPkmWBK(iP);

		// If the wbox contains a Pokémon, unlock it.
		if (!isPkmEmpty(pkm));
		// 	bankdata.bk.wboxUnlocked = true; // TODO: Uncomment when working wbox!
	}
	bankdata.bk.wbox.title[0] = 'W';
	bankdata.bk.wbox.title[1] = 'o';
	bankdata.bk.wbox.title[2] = 'n';
	bankdata.bk.wbox.title[3] = 'd';
	bankdata.bk.wbox.title[4] = 'e';
	bankdata.bk.wbox.title[5] = 'r';
	bankdata.bk.wbox.title[6] = ' ';
	bankdata.bk.wbox.title[7] = 'B';
	bankdata.bk.wbox.title[8] = 'o';
	bankdata.bk.wbox.title[9] = 'x';
	bankdata.bk.wbox.title[10] = '\0';
	bankdata.bk.wbox.background = 15;
	bankdata.bk.wbox.number = 0;
	printf(" OK\n");
}


// ------------------------------------
pkm_s* SaveManager::loadPkmPC(u16 boxId, u16 slotId)
// ------------------------------------
{
	pkm_s* pkm = &savedata.pc.box[boxId].slot[slotId];
	loadEk6PC(pkm, BOX_SIZE * boxId + PKM_SIZE * slotId);
	loadPk6Ek6(pkm); // Pokemon stored as Ek6
	loadPkmPk6(pkm);
	pkm->fromBank = false;
	return pkm;
}


// ----------------------------------------------
pkm_s* SaveManager::loadPkmBK(u16 boxId, u16 slotId)
// ----------------------------------------------
{
	pkm_s* pkm = &bankdata.bk.box[boxId].slot[slotId];
	loadEk6BK(pkm, BOX_SIZE * boxId + PKM_SIZE * slotId);
	// loadPk6Ek6(pkm); // Pokemon stored as Pk6
	loadPkmPk6(pkm);
	pkm->fromBank = true;
	return pkm;
}


// ----------------------------------------------
pkm_s* SaveManager::loadPkmWBK(u16 slotId)
// ----------------------------------------------
{
	pkm_s* pkm = &bankdata.bk.wbox.slot[slotId];
	loadEk6BK(pkm, PKM_SIZE * slotId);
	// loadPk6Ek6(pkm); // Pokemon stored as Pk6
	loadPkmPk6(pkm);
	pkm->fromBank = true;
	return pkm;
}


// ------------------------------------
void SaveManager::loadEk6PC(pkm_s* pkm, u32 offsetSlot)
// ------------------------------------
{
	pkm->ek6 = savebuffer + offsetPC + offsetSlot;
}


// ------------------------------------
void SaveManager::loadEk6BK(pkm_s* pkm, u32 offsetSlot)
// ------------------------------------
{
	pkm->ek6 = bankbuffer + offsetBK + offsetSlot;
	// Pokemon stored as Pk6
	pkm->pk6 = new pk6_t[PK6_SIZE];
	memcpy(pkm->pk6, pkm->ek6, PKM_SIZE);
}


// ------------------------------------
void SaveManager::loadEk6WBK(pkm_s* pkm, u32 offsetSlot)
// ------------------------------------
{
	pkm->ek6 = bankbuffer + offsetWonderBox + offsetSlot;
	// Pokemon stored as Pk6
	pkm->pk6 = new pk6_t[PK6_SIZE];
	memcpy(pkm->pk6, pkm->ek6, PKM_SIZE);
}


// ------------------------------------
void SaveManager::loadPk6Ek6(pkm_s* pkm)
// ------------------------------------
{
	pkm->pk6 = new pk6_t[PK6_SIZE];

	decryptEk6(pkm);
}


// ------------------------------------
void SaveManager::loadPkmPk6(pkm_s* pkm)
// ------------------------------------
{
	if (!pkm || !pkm->pk6) return;

	pkm->checked = false;
	pkm->moved = false;
	pkm->modified = false;
	pkm->isEggy = Pokemon::isEgg(pkm);
	pkm->isShiny = pkm->isEggy ? Pokemon::isShiny(pkm, savedata.TID, savedata.SID) : Pokemon::isShiny(pkm);
	pkm->speciesID = Pokemon::speciesID(pkm);
	pkm->itemID = Pokemon::itemID(pkm);
	pkm->formID = Pokemon::formID(pkm);
}



	/*--------------------------------------------------*\
	 |                  Saving Section                  |
	\*--------------------------------------------------*/


// ----------------------------------------------
void SaveManager::saveData(void)
// ----------------------------------------------
{
	saveSaveData();
	saveBankData();
}


// ------------------------------------
void SaveManager::saveSaveData(void)
// ------------------------------------
{
	if (Game::is(version, Game::XY) || Game::is(version, Game::ORAS))
	{
		printf("Saving PC Boxes:");
		if (*(u8*)(savebuffer + 0x483E) < 30) *(u8*)(savebuffer + 0x483E) = 30;
		for (u16 iB = 0; iB < savedata.pc.boxUnlocked; iB++)
		{
			for (u16 iP = 0; iP < BOX_PKM_COUNT; iP++)
			{
				savePkmPC(iB, iP);
			}

			memset((u16*)(savebuffer + offsetPCLayout + 0x22 * iB), 0, 0x11 * sizeof(uint16_t));
			utf32_to_utf16((u16*)(savebuffer + offsetPCLayout + 0x22 * iB), savedata.pc.box[iB].title, 0x10);
			*(savebuffer + offsetPCBackground + 0x1 * iB) = savedata.pc.box[iB].background;
		}
		printf(" OK\n");

		printf("Rewriting Checksum:");
		rewriteSaveCHK();
		printf(" OK\n");
	}
}


// ----------------------------------------------
void SaveManager::saveBankData(void)
// ----------------------------------------------
{
	printf("Saving BK Boxes:");
	for (u16 iB = 0; iB < bankdata.bk.boxUnlocked; iB++)
	{
		for (u16 iP = 0; iP < BOX_PKM_COUNT; iP++)
		{
			savePkmBK(iB, iP);
		}

		memset((u16*)(bankbuffer + offsetBKLayout + 0x22 * iB), 0, 0x11 * sizeof(uint16_t));
		utf32_to_utf16((u16*)(bankbuffer + offsetBKLayout + 0x22 * iB), bankdata.bk.box[iB].title, 0x10);
		*(bankbuffer + offsetBKBackground + 0x1 * iB) = bankdata.bk.box[iB].background;
	}
	printf(" OK\n");

	printf("Saving Wonder box:");
	for (u16 iP = 0; iP < BOX_PKM_COUNT; iP++)
	{
		// savePkmWBK(iP);
	}
	printf(" OK\n");
}


// ------------------------------------
void SaveManager::savePkmPC(u16 boxId, u16 slotId)
// ------------------------------------
{
	pkm_s* pkm = &savedata.pc.box[boxId].slot[slotId];

	if (pkm->moved)
	{
		savePkmPk6(pkm); // For trainer convert
		savePk6Ek6(pkm); // Pokemon stored as Ek6
		// saveEk6PC(pkm); // Pokemon stored as Ek6
	}
}


// ----------------------------------------------
void SaveManager::savePkmBK(u16 boxId, u16 slotId)
// ----------------------------------------------
{
	pkm_s* pkm = &bankdata.bk.box[boxId].slot[slotId];

	if (pkm->moved)
	{
		// savePkmPk6(pkm); // To cheat/modify
		// savePk6Ek6(pkm); // Pokemon stored as Pk6
		saveEk6BK(pkm); // Pokemon stored as Pk6
	}
}


// ------------------------------------
void SaveManager::saveEk6PC(pkm_s* pkm)
// ------------------------------------
{
	// if (!pkm || !pkm->ek6 || !pkm->pk6) return;

	// Useless!!
}


// ----------------------------------------------
void SaveManager::saveEk6BK(pkm_s* pkm)
// ----------------------------------------------
{
	if (!pkm || !pkm->ek6 || !pkm->pk6) return;

	// Pokémon stored as Pk6
	memcpy(pkm->ek6, pkm->pk6, PKM_SIZE);
}


// ------------------------------------
void SaveManager::savePk6Ek6(pkm_s* pkm)
// ------------------------------------
{
	if (!pkm || !pkm->ek6 || !pkm->pk6) return;

	// Pokémon stored as Ek6
	encryptPk6(pkm);
}


// ------------------------------------
void SaveManager::savePkmPk6(pkm_s* pkm)
// ------------------------------------
{
	if (!pkm || !pkm->pk6) return;

	// If it comes from the bank
	if (pkm->fromBank && !isPkmEmpty(pkm))
	{
		if (!pkm->isEggy) 
			addDex(pkm);
		tradePkm(pkm);
	}
}



	/*--------------------------------------------------*\
	 |                       ???                        |
	\*--------------------------------------------------*/


// ------------------------------------
void SaveManager::setGame(u32 bytesRead)
// ------------------------------------
{
	version = Game::None;

	if (bytesRead == SaveConst::XY_size)
	{
		version = Game::XY;
	}
	else if (bytesRead == SaveConst::ORAS_size)
	{
		version = Game::ORAS;
	}

	setGameOffsets();
}


// ------------------------------------
void SaveManager::setGameOffsets(void)
// ------------------------------------
{
	if (Game::is(version, Game::XY))
	{
		offsetTrainerCard = SaveConst::XY_offsetTrainerCard;
		offsetPCLayout = SaveConst::XY_offsetPCLayout;
		offsetPCBackground = SaveConst::XY_offsetPCBackground;
		offsetPC = SaveConst::XY_offsetPC;

		sizeSave = SaveConst::XY_size;
	}
	else if (Game::is(version, Game::ORAS))
	{
		offsetTrainerCard = SaveConst::ORAS_offsetTrainerCard;
		offsetPCLayout = SaveConst::ORAS_offsetPCLayout;
		offsetPCBackground = SaveConst::ORAS_offsetPCBackground;
		offsetPC = SaveConst::ORAS_offsetPC;

		sizeSave = SaveConst::ORAS_size;
	}
}


// ------------------------------------
void SaveManager::setBank(u32 bytesRead)
// ------------------------------------
{
	setBankOffsets();
}


// ------------------------------------
void SaveManager::setBankOffsets(void)
// ------------------------------------
{
	offsetBK = *(u32*)(bankbuffer + SaveConst::BANK_offsetOffsetBK);
	offsetBKLayout = *(u32*)(bankbuffer + SaveConst::BANK_offsetOffsetBKLayout);
	offsetBKBackground = *(u32*)(bankbuffer + SaveConst::BANK_offsetOffsetBKBackground);
	offsetWonderBox = *(u32*)(bankbuffer + SaveConst::BANK_offsetOffsetWonderBox);

	sizeBank = SaveConst::BANK_size;
}


// ------------------------------------
bool SaveManager::isPkmEmpty(pkm_s* pkm)
// ------------------------------------
{
	return pkm->speciesID == 0x00;
}


// ------------------------------------
bool SaveManager::isSlotEmpty(u16 boxId, u16 slotId, bool inBank)
// ------------------------------------
{
	return isPkmEmpty(getPkm(boxId, slotId, inBank));
}


// ------------------------------------
box_s* SaveManager::countBox(u16 boxId, bool inBank)
// ------------------------------------
{
	box_s* box = inBank ? &bankdata.bk.box[boxId] : &savedata.pc.box[boxId];
	box->count = 0;

	for (u16 i = 0; i < BOX_PKM_COUNT; i++)
		if (!isPkmEmpty(&box->slot[i]))
			box->count++;

	return box;
}


// ------------------------------------
box_s* SaveManager::getWBox(void)
// ------------------------------------
{
	// printf("getWBox(): %p\n", &bankdata.bk.wbox);

	return &bankdata.bk.wbox;
}


// ------------------------------------
box_s* SaveManager::getBox(u16 boxId, bool inBank)
// ------------------------------------
{
	// printf("getBox(%u,true): %p\n", boxId, &bankdata.bk.box[boxId]);
	// printf("getBox(%u,false): %p\n", boxId, &savedata.pc.box[boxId]);

	return inBank ? &bankdata.bk.box[boxId] : &savedata.pc.box[boxId];
}


// ------------------------------------
pkm_s* SaveManager::getWPkm(u16 slotId)
// ------------------------------------
{
	// printf("getWPkm(%u): %p\n", slotId, &bankdata.bk.wbox.slot[slotId]);

	return &bankdata.bk.wbox.slot[slotId];
}


// ------------------------------------
pkm_s* SaveManager::getPkm(u16 slotId, bool inBank)
// ------------------------------------
{
	return getPkm(slotId / BOX_PKM_COUNT, slotId % BOX_PKM_COUNT, inBank);
}


// ------------------------------------
pkm_s* SaveManager::getPkm(u16 boxId, u16 slotId, bool inBank)
// ------------------------------------
{
	// printf("getPkm(%u,%u,true): %p\n", boxId, slotId, &bankdata.bk.box[boxId].slot[slotId]);
	// printf("getPkm(%u,%u,false): %p\n", boxId, slotId, &savedata.pc.box[boxId].slot[slotId]);

	return inBank ? &bankdata.bk.box[boxId].slot[slotId] : &savedata.pc.box[boxId].slot[slotId];
}


// ------------------------------------
bool SaveManager::movePkm(pkm_s* src, pkm_s* dst)
// ------------------------------------
{
	// We keep the same ek6 pointer
	ek6_t* srcEk6 = src->ek6;
	ek6_t* dstEk6 = dst->ek6;

	// We copy the pokemon data
	pkm_s tmp = *dst;
	memcpy(&tmp, dst, sizeof(pkm_s));
	memcpy(dst, src, sizeof(pkm_s));
	memcpy(src, &tmp, sizeof(pkm_s));

	// We restore the ek6 pointer
	src->ek6 = srcEk6;
	dst->ek6 = dstEk6;

	// We valid the Pokémon move'd
	src->moved = true;
	dst->moved = true;

	return true;
}


// ------------------------------------
bool SaveManager::movePkm(pkm_s* src, pkm_s* dst, bool srcBanked, bool dstBanked)
// ------------------------------------
{
	if (!isPkmEmpty(src) && !filterPkm(src, dstBanked, srcBanked)) return false;
	if (!isPkmEmpty(dst) && !filterPkm(dst, srcBanked, dstBanked)) return false;

	return movePkm(src, dst);
}


// ------------------------------------
bool SaveManager::pastePkm(pkm_s* src, pkm_s* dst)
// ------------------------------------
{
	// We keep the same ek6 pointer
	ek6_t* dstEk6 = dst->ek6;

	// We copy the pokemon data
	memcpy(dst, src, sizeof(pkm_s));

	// We restore the ek6 pointer
	dst->ek6 = dstEk6;

	// We valid the Pokémon move'd
	dst->moved = true;

	return true;
}


// ------------------------------------
bool SaveManager::pastePkm(pkm_s* src, pkm_s* dst, bool srcBanked, bool dstBanked)
// ------------------------------------
{
	if (!dev || (!isPkmEmpty(src) && !filterPkm(src, dstBanked, srcBanked))) return false;

	return pastePkm(src, dst);
}


// ------------------------------------
void SaveManager::moveBox(u16 boxId_1, bool inBank_1, u16 boxId_2, bool inBank_2)
// ------------------------------------
{
	for (u32 i = 0; i < BOX_PKM_COUNT; i++)
	{
		movePkm(getPkm(boxId_1, i, inBank_1), getPkm(boxId_2, i, inBank_2), inBank_1, inBank_2);
	}
}


// ------------------------------------
bool SaveManager::filterPkm(pkm_s* pkm, bool toBank, bool fromBank)
// ------------------------------------
{
	bool isFiltered = true;
	bool toGame = !toBank;
	bool fromGame = !fromBank;

	// printf("Filtering Pokémon %03u\n", pkm->speciesID);

	if (Game::is(version, Game::XY))
	{
		if (toGame && fromBank) isFiltered &= Filter::filterToXY(pkm);
		if (fromGame && toBank) isFiltered &= Filter::filterFromXY(pkm);
	}
	else if (Game::is(version, Game::ORAS))
	{
		if (toGame && fromBank) isFiltered &= Filter::filterToORAS(pkm);
		if (fromGame && toBank) isFiltered &= Filter::filterFromORAS(pkm);
	}

	// printf("Filtering: %s\n", (isFiltered ? "allowed" : "forbidden"));

	return isFiltered;
}


// ------------------------------------
void SaveManager::addDex(pkm_s* pkm)
// ------------------------------------
{
	Pokedex::importToGame(version, savebuffer, pkm);
}


// ------------------------------------
void SaveManager::tradePkm(pkm_s* pkm)
// ------------------------------------
{
	if (Pokemon::isEgg(pkm))
	{
		if ((Pokemon::TID(pkm) == savedata.TID) &&
			(Pokemon::SID(pkm) == savedata.SID) &&
			(Pokemon::OT_gender(pkm) == savedata.OTGender) &&
			(str16ncmp(Pokemon::OT_name(pkm), (u16*)(savebuffer + offsetTrainerCard + 0x48), 0x18) == 0));
		// If it's not going back to OT
		else
		{
			Pokemon::metLocation(pkm, 0x7532); // a Link Trade (Egg)
		}

		time_t t_time = time(NULL);
		struct tm* tm_time = localtime(&t_time);

		Pokemon::metYear(pkm, tm_time->tm_year-100);
		Pokemon::metMonth(pkm, tm_time->tm_mon+1);
		Pokemon::metDay(pkm, tm_time->tm_mday);

		pkm->modified = true;
	}
	else
	{
		// If it's going back to OT
		if ((Pokemon::TID(pkm) == savedata.TID) &&
			(Pokemon::SID(pkm) == savedata.SID) &&
			(Pokemon::OT_gender(pkm) == savedata.OTGender) &&
			(str16ncmp(Pokemon::OT_name(pkm), (u16*)(savebuffer + offsetTrainerCard + 0x48), 0x18) == 0))
		{
			// If it's coming back from a non-OT
			if (Pokemon::currentHandler(pkm) == 0x01)
			{
				Pokemon::currentHandler(pkm, 0x00);
				pkm->modified = true;
			}
		}
		// If it's going to a non-OT
		else
		{
			// If it is from OT
			if (Pokemon::currentHandler(pkm) == 0x00)
			{
				Pokemon::currentHandler(pkm, 0x01);
				tradePkmHT(pkm);
			}
			// If it is from a non-OT
			else
			{
				// If it is not the "same non-OT"
				if (str16ncmp(Pokemon::HT_name(pkm), (u16*)(savebuffer + offsetTrainerCard + 0x48), 0x18) != 0)
				{
					tradePkmHT(pkm);
				}
			}
		}
	}

	if (pkm->modified)
	{
		Pokemon::computeChecksum(pkm);
	}
}


// ------------------------------------
void SaveManager::tradePkmHT(pkm_s* pkm)
// ------------------------------------
{
	// Save::OTName to Pkmn::HTName
	Pokemon::HT_name(pkm, (u16*)(savebuffer + offsetTrainerCard + 0x48));
	Pokemon::HT_gender(pkm, savedata.OTGender);
	Pokemon::currentHandler(pkm, 0x01);
	Pokemon::geo5Region(pkm, Pokemon::geo4Region(pkm));
	Pokemon::geo4Region(pkm, Pokemon::geo3Region(pkm));
	Pokemon::geo3Region(pkm, Pokemon::geo2Region(pkm));
	Pokemon::geo2Region(pkm, Pokemon::geo1Region(pkm));
	Pokemon::geo1Region(pkm, savedata.GEORegion);
	Pokemon::geo5Country(pkm, Pokemon::geo4Country(pkm));
	Pokemon::geo4Country(pkm, Pokemon::geo3Country(pkm));
	Pokemon::geo3Country(pkm, Pokemon::geo2Country(pkm));
	Pokemon::geo2Country(pkm, Pokemon::geo1Country(pkm));
	Pokemon::geo1Country(pkm, savedata.GEOCountry);
	Pokemon::HT_friendship(pkm, Personal(pkm->speciesID).baseFriendship);
	Pokemon::HT_affection(pkm, 0x00);
	Pokemon::HT_intensity(pkm, 0x01);
	Pokemon::HT_memory(pkm, 0x04);
	Pokemon::HT_feeling(pkm, rand() % 0xa);
	Pokemon::HT_textVar(pkm, 0x00);
	// Pokemon::fullness(pkm, pkm->fullness);
	// Pokemon::enjoyment(pkm, pkm->enjoyment);

	pkm->modified = true;
}


// ------------------------------------
void SaveManager::decryptEk6(pkm_s* pkm)
// ------------------------------------
{
	memcpy(pkm->pk6, pkm->ek6, PKM_SIZE);

	u32 pv = *(u32*)(pkm->pk6 + 0x0);
	u8 sv = (((pv & 0x3e000) >> 0xd) % 24);

	u32 seed = pv;
	u16 tmp;
	for (u32 i = 0x8; i < PKM_SIZE; i += 0x2)
	{
		memcpy(&tmp, pkm->pk6 + i, 0x2);

		seed = LCRNG(seed);
		tmp ^= (seed >> 16);

		memcpy(pkm->pk6 + i, &tmp, 0x2);
	}

	shufflePk6(pkm->pk6, sv);
}


// ------------------------------------
void SaveManager::encryptPk6(pkm_s* pkm)
// ------------------------------------
{
	memcpy(pkm->ek6, pkm->pk6, PKM_SIZE);

	u32 pv = *(u32*)(pkm->ek6 + 0x0);
	u8 sv = (((pv & 0x3e000) >> 0xd) % 24);

	for (u32 i = 0; i < 11; i++)
		shufflePk6(pkm->ek6, sv);

	u32 seed = pv;
	u16 tmp;
	for (u32 i = 0x8; i < PKM_SIZE; i += 0x2)
	{
		memcpy(&tmp, pkm->ek6 + i, 0x2);

		seed = LCRNG(seed);
		tmp ^= (seed >> 16);

		memcpy(pkm->ek6 + i, &tmp, 0x2);
	}
}


// ------------------------------------
void SaveManager::shufflePk6(pk6_t* pk6, u8 sv)
// ------------------------------------
{
	u8 aloc[] = { 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 2, 3, 1, 1, 2, 3, 2, 3, 1, 1, 2, 3, 2, 3 };
	u8 bloc[] = { 1, 1, 2, 3, 2, 3, 0, 0, 0, 0, 0, 0, 2, 3, 1, 1, 3, 2, 2, 3, 1, 1, 3, 2 };
	u8 cloc[] = { 2, 3, 1, 1, 3, 2, 2, 3, 1, 1, 3, 2, 0, 0, 0, 0, 0, 0, 3, 2, 3, 2, 1, 1 };
	u8 dloc[] = { 3, 2, 3, 2, 1, 1, 3, 2, 3, 2, 1, 1, 3, 2, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0 };

	u8 ord[] = { aloc[sv], bloc[sv], cloc[sv], dloc[sv] };

	pk6_t* cpk6 = new pk6_t[PK6_SIZE];
	u8 tmp[56];

	memcpy(cpk6, pk6, PK6_SIZE);

	for (u32 j = 0; j < 4; j++)
	{
		memcpy(tmp, cpk6 + 8 + 56 * ord[j], 56);
		memcpy(pk6 + 8 + 56 * j, tmp, 56);
	}

	delete[] cpk6;
}


// ------------------------------------
void SaveManager::rewriteSaveCHK(void)
// ------------------------------------
{
	if (Game::is(version, Game::None)) return;

	u8 blockCount = 0;
	u32 csoff = 0;

	if (Game::is(version, Game::XY))
	{
		blockCount = 55;
		csoff = 0x6A81A - 0x5400;
	}
	else if (Game::is(version, Game::ORAS))
	{
		blockCount = 58;
		csoff = 0x7B21A - 0x5400;
	}

	u8* tmp = new u8[0x35000];
	u16 cs;

	if (!tmp) return;

	for (u32 i = 0; i < blockCount; i++)
	{
		memcpy(tmp, savebuffer + CHKOffset(i), CHKLength(i));
		cs = ccitt16(tmp, CHKLength(i));
		memcpy(savebuffer + csoff + i * 8, &cs, 2);
	}

	delete[] tmp;
}


// ------------------------------------
u32 SaveManager::LCRNG(u32 seed)
// ------------------------------------
{
	u32 a = 0x41c64e6d;
	u32 c = 0x00006073;
	return ((seed * a + c) & 0xffffffff);
}


// ------------------------------------
u32 SaveManager::CHKOffset(u32 i)
// ------------------------------------
{
	if (Game::is(version, Game::XY))
	{
		const u32 _xy[] = { 0x05400, 0x05800, 0x06400, 0x06600, 0x06800, 0x06A00, 0x06C00, 0x06E00, 0x07000, 0x07200, 0x07400, 0x09600, 0x09800, 0x09E00, 0x0A400, 0x0F400, 0x14400, 0x19400, 0x19600, 0x19E00, 0x1A400, 0x1AC00, 0x1B400, 0x1B600, 0x1B800, 0x1BE00, 0x1C000, 0x1C400, 0x1CC00, 0x1CE00, 0x1D000, 0x1D200, 0x1D400, 0x1D600, 0x1DE00, 0x1E400, 0x1E800, 0x20400, 0x20600, 0x20800, 0x20C00, 0x21000, 0x22C00, 0x23000, 0x23800, 0x23C00, 0x24600, 0x24A00, 0x25200, 0x26000, 0x26200, 0x26400, 0x27200, 0x27A00, 0x5C600, };

		return _xy[i] - 0x5400;
	}
	else if (Game::is(version, Game::ORAS))
	{
		const u32 _oras[] = { 0x05400, 0x05800, 0x06400, 0x06600, 0x06800, 0x06A00, 0x06C00, 0x06E00, 0x07000, 0x07200, 0x07400, 0x09600, 0x09800, 0x09E00, 0x0A400, 0x0F400, 0x14400, 0x19400, 0x19600, 0x19E00, 0x1A400, 0x1B600, 0x1BE00, 0x1C000, 0x1C200, 0x1C800, 0x1CA00, 0x1CE00, 0x1D600, 0x1D800, 0x1DA00, 0x1DC00, 0x1DE00, 0x1E000, 0x1E800, 0x1EE00, 0x1F200, 0x20E00, 0x21000, 0x21400, 0x21800, 0x22000, 0x23C00, 0x24000, 0x24800, 0x24C00, 0x25600, 0x25A00, 0x26200, 0x27000, 0x27200, 0x27400, 0x28200, 0x28A00, 0x28E00, 0x30A00, 0x38400, 0x6D000, };

		return _oras[i] - 0x5400;
	}
	else
	{
		return 0;
	}
}


// ------------------------------------
u32 SaveManager::CHKLength(u32 i)
// ------------------------------------
{
	if (Game::is(version, Game::XY))
	{
		const u32 _xy[] = { 0x002C8, 0x00B88, 0x0002C, 0x00038, 0x00150, 0x00004, 0x00008, 0x001C0, 0x000BE, 0x00024, 0x02100, 0x00140, 0x00440, 0x00574, 0x04E28, 0x04E28, 0x04E28, 0x00170, 0x0061C, 0x00504, 0x006A0, 0x00644, 0x00104, 0x00004, 0x00420, 0x00064, 0x003F0, 0x0070C, 0x00180, 0x00004, 0x0000C, 0x00048, 0x00054, 0x00644, 0x005C8, 0x002F8, 0x01B40, 0x001F4, 0x001F0, 0x00216, 0x00390, 0x01A90, 0x00308, 0x00618, 0x0025C, 0x00834, 0x00318, 0x007D0, 0x00C48, 0x00078, 0x00200, 0x00C84, 0x00628, 0x34AD0, 0x0E058, };

		return _xy[i];
	}
	else if (Game::is(version, Game::ORAS))
	{
		const u32 _oras[] = { 0x002C8, 0x00B90, 0x0002C, 0x00038, 0x00150, 0x00004, 0x00008, 0x001C0, 0x000BE, 0x00024, 0x02100, 0x00130, 0x00440, 0x00574, 0x04E28, 0x04E28, 0x04E28, 0x00170, 0x0061C, 0x00504, 0x011CC, 0x00644, 0x00104, 0x00004, 0x00420, 0x00064, 0x003F0, 0x0070C, 0x00180, 0x00004, 0x0000C, 0x00048, 0x00054, 0x00644, 0x005C8, 0x002F8, 0x01B40, 0x001F4, 0x003E0, 0x00216, 0x00640, 0x01A90, 0x00400, 0x00618, 0x0025C, 0x00834, 0x00318, 0x007D0, 0x00C48, 0x00078, 0x00200, 0x00C84, 0x00628, 0x00400, 0x07AD0, 0x078B0, 0x34AD0, 0x0E058, };

		return _oras[i];
	}
	else
	{
		return 0;
	}
}


// ------------------------------------
u16 SaveManager::ccitt16(u8* data, u32 len)
// ------------------------------------
{
	u16 crc = 0xFFFF;

	for (u32 i = 0; i < len; i++)
	{
		crc ^= (u16) (data[i] << 8);

		for (u32 j = 0; j < 0x8; j++)
		{
			if ((crc & 0x8000) > 0)
				crc = (u16) ((crc << 1) ^ 0x1021);
			else
				crc <<= 1;
		}
	}

	return crc;
}
