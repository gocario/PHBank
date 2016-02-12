#include "save_manager.hpp"

#include "fs.h"
#include "utils.h"
#include "pkdir.h"
#include "version.h"

#include "pokemon.hpp"
#include "pokedex.hpp"
#include "filter.hpp"

#include <3ds.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dirent.h>

// ------------------------------------
SaveManager::SaveManager()
// ------------------------------------
{
	offsetTrainerCard = 0x0;
	offsetPCLayout = 0x0;
	offsetPCBackground = 0x0;
	offsetPC = 0x0;
	offsetBK = SaveConst::BANK_offsetBK;
	offsetBKBackground = SaveConst::BANK_offsetBKBackground;
	sizeSave = SaveConst::ORAS_size;
	sizeBank = SaveConst::BANK_size;
	// Which is greater than SaveConst::XY_size

	version = Game::None;
	loaded = false;
}

// ------------------------------------
SaveManager::~SaveManager()
// ------------------------------------
{
	printf("Deleting PC Boxes:");
	for (u16 iB = 0; iB < PC_BOX_COUNT; iB++)
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

	printf("Deleting BK Boxes:");
	for (u16 iB = 0; iB < BANK_BOX_COUNT; iB++)
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
Result SaveManager::load()
// ----------------------------------------------
{
	Result ret;

	printf(">Reading...\n");
	ret = loadFile();
	if (R_FAILED(ret)) return ret;

	printf(">Loading...\n");
	ret = loadData();
	if (R_FAILED(ret)) return ret;

	return ret;
}


// ----------------------------------------------
Result SaveManager::save()
// ----------------------------------------------
{
	Result ret;

	printf("> Writing...\n");
	ret = saveData();
	if (R_FAILED(ret)) return ret;

	printf("> Saving...\n");
	ret = saveFile();
	if (R_FAILED(ret)) return ret;

	return ret;
}



	/*--------------------------------------------------*\
	 |              Reading/Writing Section             |
	\*--------------------------------------------------*/


// ----------------------------------------------
Result SaveManager::loadFile()
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
Result SaveManager::saveFile()
// ----------------------------------------------
{
	Result ret;

	mkdir(PK_BASE_FOLDER, 0700);
	mkdir(PK_BANK_FOLDER, 0700);

	printf(">Saving Save to save\n");
	ret = saveSaveFile();
	// if (R_FAILED(ret)) return ret; // ASK Uncomment this?

	printf(">Saving Bank to sdmc\n");
	ret = saveBankFile();
	// if (R_FAILED(ret)) return ret;

	return ret;
}


// ----------------------------------------------
Result SaveManager::backupFile()
// ----------------------------------------------
{
	Result ret;

	mkdir(PK_BASE_FOLDER, 0700);
	mkdir(PK_BACKUP_FOLDER, 0700);

	printf(">Backing up Save to sdmc\n");
	ret = backupSaveFile();
	// if (R_FAILED(ret)) return ret; // ASK Uncomment this?

	printf(">Backing up Bank to sdmc\n");
	ret = backupBankFile();
	// if (R_FAILED(ret)) return ret;

	return ret;
}


// ----------------------------------------------
Result SaveManager::loadSaveFile()
// ----------------------------------------------
{
	memset(savebuffer, 0, sizeof(savebuffer_t));

	Result ret;
	u32 bytesRead = 0;
	u32 size = sizeSave;
	char path[32];

	sprintf(path, PK_ROOT_FOLDER PK_SAVE_FILE);

	printf("Loading savefile...");
	ret = FS_ReadFile(path, savebuffer, &saveArchive, size, &bytesRead);

	if (R_FAILED(ret)) printf(" ERROR\n");
	else printf(" OK\n");
	printf("  Read %ld/%ld bytes\n", bytesRead, size);

	if (R_SUCCEEDED(ret))
		setGame(bytesRead);

	return ret;
}


// ----------------------------------------------
Result SaveManager::loadBankFile()
// ----------------------------------------------
{
	memset(bankbuffer, 0, sizeof(bankbuffer_t));
	
	Result ret;
	u32 bytesRead = 0;
	u32 size = sizeBank;
	char path[32];

	sprintf(path, PK_BANK_FOLDER PK_BANK_FILE);

	printf("Loading bankfile...");
	FILE* fp = fopen(path, "rb");
	if (fp)
	{
		printf(" Reading...");
		bytesRead = fread(bankbuffer, 1, size, fp);

		if (ferror(fp)) printf(" ERROR\n");
		else printf(" OK\n");
		printf("  Read %ld/%ld bytes\n", bytesRead, size);

		ret = ferror(fp);
		fclose(fp);
	}
	else
	{
		printf(" Creating...");
		memset(bankbuffer, 0, size);

		printf(" OK\n  Created %ld bytes\n", size);

		ret = 0;
	}

	return ret;
}


// ----------------------------------------------
Result SaveManager::saveSaveFile()
// ----------------------------------------------
{
	Result ret;
	u32 bytesWritten = 0;
	u32 size = sizeSave;
	char path[32];

	sprintf(path, PK_ROOT_FOLDER PK_SAVE_FILE);

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
Result SaveManager::saveBankFile()
// ----------------------------------------------
{
	Result ret;
	u32 bytesWritten = 0;
	u32 size = sizeBank;
	char path[32];

	sprintf(path, PK_BANK_FOLDER PK_BANK_FILE);

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
Result SaveManager::backupSaveFile()
// ----------------------------------------------
{
	Result ret;
	u32 bytesWritten = 0;
	u32 size = sizeSave;
	char path[40];

	sprintf(path, PK_BACKUP_FOLDER PK_SAVE_FILE "_%lli", osGetTime()/* - 2208988800L*/);

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
Result SaveManager::backupBankFile()
// ----------------------------------------------
{
	Result ret;
	u32 bytesWritten;
	u32 size = sizeBank;
	char path[40];

	sprintf(path, PK_BACKUP_FOLDER PK_BANK_FILE "_%lli", osGetTime()/* - 2208988800L*/);

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
Result SaveManager::loadData()
// ----------------------------------------------
{
	Result ret;
	
	ret = loadSaveData();
	if (R_FAILED(ret)) return ret;

	ret = loadBankData();
	if (R_FAILED(ret)) return ret;

	loaded = true;
	
	return ret;
}

// ------------------------------------
Result SaveManager::loadSaveData()
// ------------------------------------
{
	// memset(savedata, 0, sizeof(savedata)); // ASK Is it needed?

	printf("Loading PC Boxes:");
	for (u16 iB = 0; iB < PC_BOX_COUNT; iB++)
	{
		// printf("%-2u ", iB);
		// if (iB % 0x10 == 0xf) printf("\n");
		for (u16 iP = 0; iP < BOX_PKM_COUNT; iP++)
		{
			loadPkmPC(iB, iP);
		}

		savedata.pc.box[iB].background = 0;
		savedata.pc.box[iB].background = *(savebuffer + offsetPCBackground + 0x1 * iB);
		// TODO: Load the backgrounds of the save
	}
	printf(" OK\n");

	if (Game::is(version, Game::XY) || Game::is(version, Game::ORAS))
	{
		printf("Loading Dex:");
		loadDex();
		printf(" OK\n");

		printf("Loading Trainer Card:");
		savedata.TID = *(u16*) (savebuffer + offsetTrainerCard + 0x00);
		savedata.SID = *(u16*) (savebuffer + offsetTrainerCard + 0x02);
		savedata.TSV = computeTSV(savedata.TID, savedata.SID);
		savedata.OTGender = *(u8*) (savebuffer + offsetTrainerCard + 0x05);
		unicodeToChar(savedata.OTName, (u16*) (savebuffer + offsetTrainerCard + 0x48), 0x1a);
		printf(" OK\n");
	}


	return 0;
}


// ----------------------------------------------
Result SaveManager::loadBankData()
// ----------------------------------------------
{
	// memset(bankdata, 0, sizeof(bankdata)); // ASK Is it needed?

	// bankdata.version = *(u32*) (bankbuffer + 0x00);
	bankdata.magic = (bankbuffer[0x04]) |
		(bankbuffer[0x01] << 24) |
		(bankbuffer[0x02] << 16) |
		(bankbuffer[0x03] << 8);
	bankdata.version = *(u32*) (bankbuffer + 0x08);

	printf("Loading BK Boxes:");
	for (u16 iB = 0; iB < BANK_BOX_COUNT; iB++)
	{
		// printf("%-2u ", iB);
		// if (iB % 0x10 == 0xf) printf("\n");
		for (u16 iP = 0; iP < BOX_PKM_COUNT; iP++)
		{
			loadPkmBK(iB, iP);
		}

		bankdata.bk.box[iB].background = *(bankbuffer + offsetBKBackground + 0x1 * iB);
	}
	printf(" OK\n");

	return 0;
}


// ------------------------------------
void SaveManager::loadPkmPC(u16 boxId, u16 slotId)
// ------------------------------------
{
	// if (!loaded) return;

	pkm_s* pkm = &savedata.pc.box[boxId].slot[slotId];
	loadEk6PC(pkm, BOX_SIZE * boxId + PKM_SIZE * slotId);
	loadPk6Ek6(pkm); // Pokemon stored as Ek6
	loadPkmPk6(pkm);
	pkm->fromBank = false;
}

// ----------------------------------------------
void SaveManager::loadPkmBK(u16 boxId, u16 slotId)
// ----------------------------------------------
{
	// if (!loaded) return;

	pkm_s* pkm = &bankdata.bk.box[boxId].slot[slotId];
	loadEk6BK(pkm, BOX_SIZE * boxId + PKM_SIZE * slotId);
	// loadPk6Ek6(pkm); // Pokemon stored as Pk6
	loadPkmPk6(pkm);
	pkm->fromBank = true;
}


// ------------------------------------
void SaveManager::loadEk6PC(pkm_s* pkm, u32 offsetSlot)
// ------------------------------------
{
	if (!pkm) return;

	pkm->ek6 = savebuffer + offsetPC + offsetSlot;
}


// ------------------------------------
void SaveManager::loadEk6BK(pkm_s* pkm, u32 offsetSlot)
// ------------------------------------
{
	if (!pkm) return;

	pkm->ek6 = bankbuffer + offsetBK + offsetSlot;

	// Pokemon stored as Pk6
	pkm->pk6 = new pk6_t[PK6_SIZE];
	memcpy(pkm->pk6, pkm->ek6, PKM_SIZE);
}


// ------------------------------------
void SaveManager::loadPk6Ek6(pkm_s* pkm)
// ------------------------------------
{
	if (!pkm || !pkm->ek6) return;

	pkm->pk6 = new pk6_t[PK6_SIZE];

	decryptEk6(pkm);
}


// ------------------------------------
void SaveManager::loadPkmPk6(pkm_s* pkm)
// ------------------------------------
{
	if (!pkm || !pkm->pk6) return;

	pkm->moved = false;
	pkm->isShiny = Pokemon::isShiny(pkm);
	pkm->speciesID = Pokemon::speciesID(pkm);
	pkm->itemID = Pokemon::itemID(pkm);
	pkm->formID = Pokemon::formID(pkm);

	//pkm->species = PKData::species(pkm->speciesID);
	//pkm->item = PKData::items(pkm->itemID);
}


// ------------------------------------
void SaveManager::loadDex()
// ------------------------------------
{
	// TODO
}



	/*--------------------------------------------------*\
	 |                  Saving Section                  |
	\*--------------------------------------------------*/


// ----------------------------------------------
Result SaveManager::saveData()
// ----------------------------------------------
{
	Result ret;
	
	ret = saveSaveData();
	if (R_FAILED(ret)) return ret;
	
	ret = saveBankData();
	if (R_FAILED(ret)) return ret;

	return ret;
}


// ------------------------------------
Result SaveManager::saveSaveData()
// ------------------------------------
{
	printf("Saving PC Boxes:");
	for (u16 iB = 0; iB < PC_BOX_COUNT; iB++)
	{
		// printf("%-2u ", iB);
		// if (iB % 0x10 == 0xf) printf("\n");
		for (u16 iP = 0; iP < BOX_PKM_COUNT; iP++)
		{
			savePkmPC(iB, iP);
		}
	}
	printf(" OK\n");

	if (Game::is(version, Game::XY) || Game::is(version, Game::ORAS))
	{
		printf("Saving Dex:");
		saveDex();
		printf(" OK\n");
	}

	printf("Rewriting Checksum:");
	rewriteSaveCHK();
	printf(" OK\n");

	return 0;
}


// ----------------------------------------------
Result SaveManager::saveBankData()
// ----------------------------------------------
{
	Result ret = 0;

	printf("Saving BK Boxes:");
	for (u16 iB = 0; iB < BANK_BOX_COUNT; iB++)
	{
		// printf("%-2u ", iB);
		// if (iB % 0x10 == 0xf) printf("\n");
		for (u16 iP = 0; iP < BOX_PKM_COUNT; iP++)
		{
			savePkmBK(iB, iP);
		}
	}
	printf(" OK\n");

	return ret;
}


// ------------------------------------
void SaveManager::savePkmPC(u16 boxId, u16 slotId)
// ------------------------------------
{
	// if (!loaded) return;

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
	// if (!loaded) return;

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
	// if (!pkm || !pkm->ek6) return;

	// Useless!!
}


// ----------------------------------------------
void SaveManager::saveEk6BK(pkm_s* pkm)
// ----------------------------------------------
{
	if (!pkm || !pkm->ek6) return;
	
	// Pokemon stored as Pk6
	memcpy(pkm->ek6, pkm->pk6, PKM_SIZE);
}


// ------------------------------------
void SaveManager::savePk6Ek6(pkm_s* pkm)
// ------------------------------------
{
	if (!pkm || !pkm->ek6 || !pkm->pk6) return;

	encryptPk6(pkm);
}


// ------------------------------------
void SaveManager::savePkmPk6(pkm_s* pkm)
// ------------------------------------
{
	if (!pkm || !pkm->pk6) return;

	if (pkm->fromBank && !isPkmEmpty(pkm)) // && pkm->moved)
	{
		convertPkmTrainer(pkm);
		addDex(pkm);
	}
}


// ------------------------------------
void SaveManager::saveDex()
// ------------------------------------
{
	// TODO
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
void SaveManager::setGameOffsets()
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
bool SaveManager::isPkmEmpty(pkm_s* pkm)
// ------------------------------------
{
	return (Pokemon::speciesID(pkm) == 0x0);
}


// ------------------------------------
bool SaveManager::isSlotEmpty(u16 boxId, u16 slotId, bool inBank)
// ------------------------------------
{
	pkm_s* pkm = NULL;
	getPkm(boxId, slotId, &pkm, inBank);
	return isPkmEmpty(pkm);
}


// ------------------------------------
void SaveManager::getBox(u16 boxId, box_s** box, bool inBank)
// ------------------------------------
{
	if (inBank)
		*box = &bankdata.bk.box[boxId];
	else
		*box = &savedata.pc.box[boxId];
}


// ------------------------------------
void SaveManager::getPkm(u16 slotId, pkm_s** pkm, bool inBank)
// ------------------------------------
{
	getPkm(slotId / BOX_PKM_COUNT, slotId % BOX_PKM_COUNT, pkm, inBank);
}


// ------------------------------------
void SaveManager::getPkm(u16 boxId, u16 slotId, pkm_s** pkm, bool inBank)
// ------------------------------------
{
	if (inBank)
		*pkm = &bankdata.bk.box[boxId].slot[slotId];
	else
		*pkm = &savedata.pc.box[boxId].slot[slotId];
}


// ------------------------------------
bool SaveManager::movePkm(pkm_s* src, pkm_s* dst)
// ------------------------------------
{
	pkm_s tmp;
	tmp.pk6 = dst->pk6;
	dst->pk6 = src->pk6;
	src->pk6 = tmp.pk6;

	loadPkmPk6(src);
	loadPkmPk6(dst);

	src->moved = true;
	dst->moved = true;

	return true;
}


// ------------------------------------
bool SaveManager::movePkm(pkm_s* src, pkm_s* dst, bool srcBanked, bool dstBanked)
// ------------------------------------
{
	if (!isPkmEmpty(src) && !filterPkm(src, dstBanked, srcBanked))
		return false;
	if (!isPkmEmpty(dst) && !filterPkm(dst, srcBanked, dstBanked))
		return false;

	movePkm(src, dst);

	// src->moved = dstBanked;
	// dst->moved = srcBanked;

	return true;
}

// ------------------------------------
bool SaveManager::pastePkm(pkm_s* src, pkm_s* dst)
// ------------------------------------
{
	dst->pk6 = src->pk6;

	loadPkmPk6(dst);

	dst->moved = true;

	return true;
}


// ------------------------------------
bool SaveManager::pastePkm(pkm_s* src, pkm_s* dst, bool srcBanked, bool dstBanked)
// ------------------------------------
{
	if (!isPkmEmpty(src) && !filterPkm(src, dstBanked, srcBanked))
		return false;

	pastePkm(src, dst);

	// dst->moved = srcBanked;

	return true;
}


// ------------------------------------
void SaveManager::moveBox(u16 boxId_1, bool inBank_1, u16 boxId_2, bool inBank_2)
// ------------------------------------
{
	pkm_s* pkm_1 = NULL;
	pkm_s* pkm_2 = NULL;

	printf("Transfering Pokemon... [%s]%u <-> [%s]%u\n", (inBank_1 ? "BK" : "PC"), boxId_1, (inBank_2 ? "BK" : "PC"), boxId_2);
	for (u32 i = 0; i < BOX_PKM_COUNT; i++)
	{
		getPkm(boxId_1, i, &pkm_1, inBank_1);
		getPkm(boxId_2, i, &pkm_2, inBank_2);
		movePkm(pkm_1, pkm_2);
	}
}


// ------------------------------------
bool SaveManager::filterPkm(pkm_s* pkm, bool toBank, bool fromBank)
// ------------------------------------
{
	bool isFiltered = true;
	bool toGame = !toBank;
	bool fromGame = !fromBank;
	// printf("Filtering Pokémon\n");
	if (Game::is(version, Game::XY))
	{
		if (toGame)
			isFiltered &= Filter::filterToXY(pkm);
		if (fromGame)
			isFiltered &= Filter::filterFromXY(pkm);
	}
	else if (Game::is(version, Game::ORAS))
	{
		if (toGame)
			isFiltered &= Filter::filterToORAS(pkm);
		if (fromGame)
			isFiltered &= Filter::filterFromORAS(pkm);
	}
	// printf("Filtering: %s\n", (isFiltered ? "allowed" : "forbidden"));

	return isFiltered;
}


// ------------------------------------
void SaveManager::addDex(pkm_s* pkm)
// ------------------------------------
{
	if (Game::is(version, Game::XY))
	{
		Pokedex::importToXY(savebuffer, pkm);
	}
	else if (Game::is(version, Game::ORAS))
	{
		Pokedex::importToORAS(savebuffer, pkm);
	}
}


// ------------------------------------
void SaveManager::convertPkmTrainer(pkm_s* pkm)
// ------------------------------------
{
	// if (!pkm->moved)
	// {
		if (Pokemon::isEgg(pkm))
		{
			// TODO: Implement
			//	Met location
			//	Met date
			//	Checksum
		}
		else
		{
			// If it's going back to OT
			if (Pokemon::TID(pkm) == savedata.TID && Pokemon::SID(pkm) == savedata.SID && strcmp((char*)Pokemon::HT_name(pkm), (char*)savedata.OTName) == 0 && Pokemon::OT_gender(pkm) == savedata.OTGender)
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
					convertPkmHT(pkm);
				}
				// If it is from a non-OT
				else
				{
					// If it is not the "same non-OT"
					if (strcmp((char*)Pokemon::HT_name(pkm), (char*)savedata.OTName) != 0)
					{
						convertPkmHT(pkm);
					}
				}
			}
		}
		
		Pokemon::computeChecksum(pkm);
	// }
}


// ----------------------------------------------
void SaveManager::convertPkmHT(pkm_s* pkm)
// ----------------------------------------------
{
	Pokemon::HT_name(pkm, (u16*)(savebuffer + offsetTrainerCard + 0x48)); // Save::OTName to Pkmn::HTName
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
	Pokemon::HT_friendship(pkm, Filter::getBaseFriendship(Pokemon::speciesID(pkm)));
	Pokemon::HT_affection(pkm, 0x00);
	Pokemon::HT_intensity(pkm, 0x01);
	Pokemon::HT_memory(pkm, 0x04);
	Pokemon::HT_feeling(pkm, rand() % 0xa);
	Pokemon::HT_textVar(pkm, 0x0000);
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
void SaveManager::rewriteSaveCHK()
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
u16 SaveManager::computeTSV(u16 TID, u16 SID)
// ------------------------------------
{
	return ((TID ^ SID) >> 4);
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
		u32 _xy[] = { 0x05400, 0x05800, 0x06400, 0x06600, 0x06800, 0x06A00, 0x06C00, 0x06E00, 0x07000, 0x07200, 0x07400, 0x09600, 0x09800, 0x09E00, 0x0A400, 0x0F400, 0x14400, 0x19400, 0x19600, 0x19E00, 0x1A400, 0x1AC00, 0x1B400, 0x1B600, 0x1B800, 0x1BE00, 0x1C000, 0x1C400, 0x1CC00, 0x1CE00, 0x1D000, 0x1D200, 0x1D400, 0x1D600, 0x1DE00, 0x1E400, 0x1E800, 0x20400, 0x20600, 0x20800, 0x20C00, 0x21000, 0x22C00, 0x23000, 0x23800, 0x23C00, 0x24600, 0x24A00, 0x25200, 0x26000, 0x26200, 0x26400, 0x27200, 0x27A00, 0x5C600, };

		return _xy[i] - 0x5400;
	}
	else if (Game::is(version, Game::ORAS))
	{
		u32 _oras[] = { 0x05400, 0x05800, 0x06400, 0x06600, 0x06800, 0x06A00, 0x06C00, 0x06E00, 0x07000, 0x07200, 0x07400, 0x09600, 0x09800, 0x09E00, 0x0A400, 0x0F400, 0x14400, 0x19400, 0x19600, 0x19E00, 0x1A400, 0x1B600, 0x1BE00, 0x1C000, 0x1C200, 0x1C800, 0x1CA00, 0x1CE00, 0x1D600, 0x1D800, 0x1DA00, 0x1DC00, 0x1DE00, 0x1E000, 0x1E800, 0x1EE00, 0x1F200, 0x20E00, 0x21000, 0x21400, 0x21800, 0x22000, 0x23C00, 0x24000, 0x24800, 0x24C00, 0x25600, 0x25A00, 0x26200, 0x27000, 0x27200, 0x27400, 0x28200, 0x28A00, 0x28E00, 0x30A00, 0x38400, 0x6D000, };
	
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
		u32 _xy[] = { 0x000002C8, 0x00000B88, 0x0000002C, 0x00000038, 0x00000150, 0x00000004, 0x00000008, 0x000001C0, 0x000000BE, 0x00000024, 0x00002100, 0x00000140, 0x00000440, 0x00000574, 0x00004E28, 0x00004E28, 0x00004E28, 0x00000170, 0x0000061C, 0x00000504, 0x000006A0, 0x00000644, 0x00000104, 0x00000004, 0x00000420, 0x00000064, 0x000003F0, 0x0000070C, 0x00000180, 0x00000004, 0x0000000C, 0x00000048, 0x00000054, 0x00000644, 0x000005C8, 0x000002F8, 0x00001B40, 0x000001F4, 0x000001F0, 0x00000216, 0x00000390, 0x00001A90, 0x00000308, 0x00000618, 0x0000025C, 0x00000834, 0x00000318, 0x000007D0, 0x00000C48, 0x00000078, 0x00000200, 0x00000C84, 0x00000628, 0x00034AD0, 0x0000E058, };
	
		return _xy[i];
	}
	else if (Game::is(version, Game::ORAS))
	{
		u32 _oras[] = { 0x000002C8, 0x00000B90, 0x0000002C, 0x00000038, 0x00000150, 0x00000004, 0x00000008, 0x000001C0, 0x000000BE, 0x00000024, 0x00002100, 0x00000130, 0x00000440, 0x00000574, 0x00004E28, 0x00004E28, 0x00004E28, 0x00000170, 0x0000061C, 0x00000504, 0x000011CC, 0x00000644, 0x00000104, 0x00000004, 0x00000420, 0x00000064, 0x000003F0, 0x0000070C, 0x00000180, 0x00000004, 0x0000000C, 0x00000048, 0x00000054, 0x00000644, 0x000005C8, 0x000002F8, 0x00001B40, 0x000001F4, 0x000003E0, 0x00000216, 0x00000640, 0x00001A90, 0x00000400, 0x00000618, 0x0000025C, 0x00000834, 0x00000318, 0x000007D0, 0x00000C48, 0x00000078, 0x00000200, 0x00000C84, 0x00000628, 0x00000400, 0x00007AD0, 0x000078B0, 0x00034AD0, 0x0000E058, };
	
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
