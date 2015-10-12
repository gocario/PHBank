#include "pkbank.hpp"

#include <time.h>
#include <stdlib.h>

#include "pkdata.hpp"
#include "pokemon.hpp"

/*----------------------------------------------------------*\
 |                 Constructor / Destructor                 |
\*----------------------------------------------------------*/


// ==================================================
PKBank::PKBank()
// --------------------------------------------------
{
	this->savedata = NULL;
	this->bankdata = NULL;
	this->gametype = Game::None;
}


// ==================================================
PKBank::~PKBank()
// --------------------------------------------------
{
	if (this->savedata)
	{
		for (uint16_t iB = 0; iB < PC_BOXCOUNT; iB++)
		{
			printf("Deleting Box %-2u\n", iB);
			for (uint16_t iP = 0; iP < BOX_PKMCOUNT; iP++)
			{
				pkm_t* ppkm = &this->savedata->pc.box[iB].slot[iP];
				ppkm->ek6 = NULL;
				if (ppkm->pk6 != NULL) adelete(ppkm->pk6)
			}
		}
	}

	if (this->bankdata)
	{
		for (uint16_t iB = 0; iB < BANK_BOXCOUNT; iB++)
		{
			printf("Deleting Box %-2u\n", iB);
			for (uint16_t iP = 0; iP < BOX_PKMCOUNT; iP++)
			{
				pkm_t* ppkm = &this->bankdata->bank.box[iB].slot[iP];
				ppkm->ek6 = NULL;
				if (ppkm->pk6 != NULL) adelete(ppkm->pk6)
			}
		}
	}
	cdelete(this->savedata);
	cdelete(this->bankdata);
	this->gametype = Game::None;
}



/*----------------------------------------------------------*\
 |                      Public Section                      |
\*----------------------------------------------------------*/


// ==================================================
Result PKBank::load(Result fs, Handle *sdHandle, Handle *saveHandle, FS_archive *sdArchive, FS_archive *saveArchive)
// --------------------------------------------------
{
	Result ret = 0;
	
	fsData.fs = fs;
	fsData.sdHandle = sdHandle;
	fsData.saveHandle = saveHandle;
	fsData.sdArchive = sdArchive;
	fsData.saveArchive = saveArchive;

	printf("> Reading...\n");
	ret = loadFile(fs, sdHandle, saveHandle, sdArchive, saveArchive);
	if (ret) return ret;

	printf("> Loading...\n");
	ret = loadData();
	if (ret) return ret;

	return ret;
}

// ==================================================
Result PKBank::save(Result fs, Handle *sdHandle, Handle *saveHandle, FS_archive *sdArchive, FS_archive *saveArchive)
// --------------------------------------------------
{
	Result ret = 0;

	fsData.fs = fs;
	fsData.sdHandle = sdHandle;
	fsData.saveHandle = saveHandle;
	fsData.sdArchive = sdArchive;
	fsData.saveArchive = saveArchive;

	printf("> Writing...\n");
	ret = saveData();
	if (ret) return ret;

	printf("> Saving...\n");
	ret = saveFile(fs, sdHandle, saveHandle, sdArchive, saveArchive);
	if (ret) return ret;

	return ret;
}


	/*--------------------------------------------------*\
	 |              Reading/Writing Section             |
	\*--------------------------------------------------*/


// ==================================================
/// Read data from cartridge
Result PKBank::loadFile(Result fs, Handle *sdHandle, Handle *saveHandle, FS_archive *sdArchive, FS_archive *saveArchive)
// --------------------------------------------------
{
	Result ret = 0;

	char path[] = "/pkbank";
	ret = FS_createDirectory(path, sdHandle, sdArchive);
	// if (ret) return ret;

	if (fs)
	{
		printf(">Loading Save from SD\n");
		ret = loadSaveFile(fs, sdHandle, sdArchive);
		if (ret) return ret;
	}
	else
	{
		printf(">Loading Save from Card\n");
		ret = loadSaveFile(fs, saveHandle, saveArchive);
		if (ret) return ret;
	}

	printf(">Loading Bank from SD\n");
	ret = loadBankFile(sdHandle, sdArchive);
	if (ret) return ret;

	hidScanInput();
	if (hidKeysHeld() & KEY_L)
	{
		printf(">Backing up Save to SD\n");
		ret = backupSaveFile(sdHandle, sdArchive);
		if (ret) return ret;
	}
	else
	{
		printf(">Not backing up Save to SD\n");
	}

	return ret;
}

// ==================================================
/// Write data from cartridge
Result PKBank::saveFile(Result fs, Handle *sdHandle, Handle *saveHandle, FS_archive *sdArchive, FS_archive *saveArchive)
// --------------------------------------------------
{
	Result ret = 0;

	char path[] = "/pkbank";
	ret = FS_createDirectory(path, sdHandle, sdArchive);
	// if (ret) return ret;

	// hidScanInput();
	// if (hidKeysHeld() & KEY_L)
	// {
	// 	printf(">Backing up Save to SD\n");
	// 	ret = backupSaveFile(sdHandle, sdArchive);
	// 	// if (ret) return ret;
	// }
	// else
	// {
	// 	printf(">Not backing up Save to SD\n");
	// }

	if (fs)
	{
		printf(">Saving Save to SD\n");
		ret = saveSaveFile(fs, sdHandle, sdArchive);
		// if (ret) return ret;
	}
	else
	{
		printf(">Saving Save to Card\n");
		ret = saveSaveFile(fs, saveHandle, saveArchive);
		// if (ret) return ret;
	}

	printf(">Saving Bank to SD\n");
	ret = saveBankFile(sdHandle, sdArchive);
	// if (ret) return ret;

	return ret;
}


// ==================================================
/// Copy the save to the backup folder
Result PKBank::backupFile()
// --------------------------------------------------
{
	Result ret = 0;

	char path[] = "/pkbank";
	ret = FS_createDirectory(path, fsData.sdHandle, fsData.sdArchive);
	// if (ret) return ret;

	printf(">Backing up Save to SD\n");
	ret = backupSaveFile(fsData.sdHandle, fsData.sdArchive);
	// if (ret) return ret;

	return ret;
}

	/*--------------------------------------------------*\
	 |                 Loading Section                  |
	\*--------------------------------------------------*/


// ==================================================
/// Load data from save and bank buffers
Result PKBank::loadData()
// --------------------------------------------------
{
	Result ret = 0;
	
	ret = loadSaveData();
	if (ret) return ret;

	ret = loadBankData();
	if (ret) return ret;

	return ret;
}


	/*--------------------------------------------------*\
	 |                  Saving Section                  |
	\*--------------------------------------------------*/


// ==================================================
/// Save data to save and bank buffers
Result PKBank::saveData()
// --------------------------------------------------
{
	Result ret = 0;
	
	ret = saveSaveData();
	if (ret) return ret;
	
	ret = saveBankData();
	if (ret) return ret;

	return ret;
}


	/*--------------------------------------------------*\
	 |                   Utils Section                  |
	\*--------------------------------------------------*/


// ==================================================
void PKBank::printByte(u8* bytes, u32 key, uint32_t max)
// --------------------------------------------------
{
	printf("  ");
	if (bytes == NULL)
	{
		for (uint32_t i = 0; i < max; i++)
		{
			printf("OO");
			if (i % 0x10 == 0x0f)
				printf("\n ");
			if (i % 0x02 == 0x01)
				printf(" ");
		}
	}
	else
	{
		for (uint32_t i = 0; i < max; i++)
		{
			printf("%02x", bytes[i]);
			if (i % 0x10 == 0x0f)
				printf("\n ");
			if (i % 0x02 == 0x01)
				printf(" ");
		}
	}
	printf("\n");

	if (key)
	{
		while (aptMainLoop())
		{
			hidScanInput();

			if (hidKeysDown() & key) break;

			gfxFlushBuffers();
			// gfxSwapBuffers();
			gspWaitForVBlank();
		}
	}
}


// ==================================================
void PKBank::printPkm(pkm_t* pkm, u32 key, uint32_t max)
// --------------------------------------------------
{
	// printf("Pkm(off): 0x%08x\n", pkm);
	// printByte(pkm->ek6, 0, 0x10);
	printByte(pkm->pk6, key, PK6_SIZE);

	// printByte(pkm->pk6, key, max);
}

// ==================================================
gametype_e PKBank::getGame(uint32_t bytesRead)
// --------------------------------------------------
{
	gametype_e gametype = Game::None;

	if (bytesRead == SAVEDATA_XY_SIZE)
	{
		printf("Found X/Y save\n");
		gametype = Game::XY;
	}
	else if (bytesRead == SAVEDATA_ORAS_SIZE)
	{
		printf("Found OR/AS save\n");
		gametype = Game::ORAS;
	}
	else
	{
		printf("Found no suitable save\n");
	}

	return gametype;
}


// ==================================================
bool PKBank::isPkmEmpty(pkm_t* pkm)
// --------------------------------------------------
{
	return (Pokemon::speciesID(pkm) == 0x0 && Pokemon::PID(pkm) == 0x0);
}


// ==================================================
bool PKBank::isSlotEmpty(uint16_t boxId, uint16_t slotId, bool inBank)
// --------------------------------------------------
{
	pkm_t** pkm = NULL;
	getPkm(boxId, slotId, pkm, inBank);
	return isPkmEmpty(*pkm);
}


// ==================================================
void PKBank::getBox(uint16_t boxID, box_t** box, bool inBank)
// --------------------------------------------------
{
	if (inBank)
	{
		*box = &bankdata->bank.box[boxID];
	}
	else
	{
		*box = &savedata->pc.box[boxID];
	}
}


// ==================================================
void PKBank::getPkm(uint16_t slotId, pkm_t** pkm, bool inBank)
// --------------------------------------------------
{
	getPkm(slotId / BOX_PKMCOUNT, slotId % BOX_PKMCOUNT, pkm, inBank);
}


// ==================================================
void PKBank::getPkm(uint16_t boxId, uint16_t slotId, pkm_t** pkm, bool inBank)
// --------------------------------------------------
{
	if (inBank)
	{
		*pkm = &bankdata->bank.box[boxId].slot[slotId];
	}
	else
	{
		*pkm = &savedata->pc.box[boxId].slot[slotId];
	}
}


// ==================================================
void PKBank::getPkm(uint16_t boxId, uint16_t rowId, uint16_t colId, pkm_t** pkm, bool inBank)
// --------------------------------------------------
{
	getPkm(boxId, rowId * BOX_COL_PKMCOUNT + colId, pkm, inBank);
}


// ==================================================
void PKBank::movePkm(pkm_t* src, pkm_t* dst)
// --------------------------------------------------
{
	pkm_t tmp;
	tmp.pk6 = dst->pk6;
	dst->pk6 = src->pk6;
	src->pk6 = tmp.pk6;

	loadPkmPk6(src);
	loadPkmPk6(dst);
	
	if (!isPkmEmpty(src))
		src->moved = true;
	if (!isPkmEmpty(dst))
		dst->moved = true;
}


// ==================================================
void PKBank::movePkm(pkm_t* src, pkm_t* dst, bool srcBanked, bool dstBanked)
// --------------------------------------------------
{
	if (srcBanked && !isPkmEmpty(dst) && !filterPkm(dst))
		{ printf("No! (dst)"); return; }
	if (dstBanked && !isPkmEmpty(src) && !filterPkm(src))
		{ printf("No! (src)"); return; }

	movePkm(src, dst);

	if (gametype == Game::ORAS)
	{
		if (srcBanked && !dstBanked)
			addDex(src);
		if (!srcBanked && dstBanked)
			addDex(dst);
	}
}


// ==================================================
void PKBank::moveBox(uint16_t boxID_1, bool inBank_1, uint16_t boxID_2, bool inBank_2)
// --------------------------------------------------
{
	pkm_t* pkm_1 = NULL;
	pkm_t* pkm_2 = NULL;

	printf("Transfering Pokémon... [%s]%u [%s]%u\n", (inBank_1 ? "BK" : "PC"), boxID_1, (inBank_2 ? "BK" : "PC"), boxID_2);
	for (uint32_t i = 0; i < BOX_PKMCOUNT; i++)
	{
		printf(" %lu", i);
		pkm_1 = &(inBank_1 ? bankdata->bank.box[boxID_1].slot[i] : savedata->pc.box[boxID_1].slot[i]);
		pkm_2 = &(inBank_2 ? bankdata->bank.box[boxID_2].slot[i] : savedata->pc.box[boxID_2].slot[i]);
		movePkm(pkm_1, pkm_2, inBank_1, inBank_2);
	}
	printf("\n");
}


// ==================================================
bool PKBank::filterPkm(pkm_t* pkm)
// --------------------------------------------------
{
	// consoleClear();
	// printf("\x1B[0;0HFiltering Pokémon\n");
	bool isFiltered = true;
	isFiltered &= PKFilter::filterItemID(Pokemon::itemID(pkm));
	isFiltered &= PKFilter::filterMoveID(Pokemon::move1(pkm));
	isFiltered &= PKFilter::filterMoveID(Pokemon::move2(pkm));
	isFiltered &= PKFilter::filterMoveID(Pokemon::move3(pkm));
	isFiltered &= PKFilter::filterMoveID(Pokemon::move4(pkm));
	isFiltered &= PKFilter::filterAbilityID(Pokemon::ability(pkm));
	isFiltered &= PKFilter::filterSchoolGirlPikachu(Pokemon::speciesID(pkm), Pokemon::form(pkm));

	// printf("Filtering: %s\n", (isFiltered ? "allowed" : "forbidden"));
	// waitKey(KEY_A);
	return isFiltered;
}


// ==================================================
void PKBank::addDex(uint16_t speciesID)
// --------------------------------------------------
{
	/*

	DEBUG - TESTING

	if (speciesID == 3)
	{
		dex_t &dex = savedata->pokedex.dexes[1];
		dex.owned = 1;
		dex.ownedShinyMale = 1;
		dex.ownedShinyFemale = 1;

		dex.encounteredShinyMale = 1;
		dex.encounteredShinyFemale = 1;

		dex.langJapanese = 1;
		speciesID = 1;
	}

	if (speciesID == 2)
	{
		loadDex();
		speciesID = 1;
	}
	*/

	// {
	// 	for (uint32_t i = 0; i < 10; i++)
	// 	{
	// 		dex_t &dex = savedata->pokedex.dexes[i];

			// printf("%x", dex.owned);
			// printf("%x", dex.ownedMale);
			// printf("%x", dex.ownedFemale);
			// printf("%x", dex.ownedShinyMale);
			// printf("%x", dex.ownedShinyFemale);
			// printf(" ");
			// printf("%x", dex.encounteredMale);
			// printf("%x", dex.encounteredFemale);
			// printf("%x", dex.encounteredShinyMale);
			// printf("%x", dex.encounteredShinyFemale);
			// printf(" ");
			// printf("%x", dex.langJapanese);
			// printf("%x", dex.langEnglish);
			// printf("%x", dex.langFrench);
			// printf("%x", dex.langItalian);
			// printf("%x", dex.langGerman);
			// printf("%x", dex.langSpanish);
			// printf("%x", dex.langKorean);
			// printf("\n");
	// 	}
	// }

	printf("\n");

}


// ==================================================
void PKBank::addDex(pkm_t* pkm)
// --------------------------------------------------
{
	uint32_t speciesID = Pokemon::speciesID(pkm);
	dex_t &dex = savedata->pokedex.dexes[speciesID-1];
	bool male = (Pokemon::gender(pkm) == 1);
	bool shiny = Pokemon::isShiny(pkm);
	uint8_t lang = Pokemon::language(pkm);

	if (shiny)
	{
		if (male)
			dex.ownedShinyMale = true;
		else
			dex.ownedShinyFemale = true;
	}
	else
	{
		if (male)
			dex.ownedMale = true;
		else
			dex.ownedFemale = true;
	}
	
	if (!dex.owned)
	{
		if (shiny)
		{
			if (male)
				dex.encounteredShinyMale = true;
			else
				dex.encounteredShinyFemale = true;
		}
		else
		{
			if (male)
				dex.encounteredMale = true;
			else
				dex.encounteredFemale = true;
		}
		dex.owned = true;
	}

	if (lang == 0)
		dex.langJapanese = true;
	else if (lang == 1)
		dex.langEnglish = true;
	else if (lang == 2)
		dex.langFrench = true;
	else if (lang == 3)
		dex.langItalian = true;
	else if (lang == 4)
		dex.langGerman = true;
	else if (lang == 5)
		dex.langSpanish = true;
	else if (lang == 6)
		dex.langKorean = true;
}


/*------------------------------------------------------------*\
 |                      Private Section                       |
\*------------------------------------------------------------*/


	/*--------------------------------------------------*\
	 |              Reading/Writing Section             |
	\*--------------------------------------------------*/


// ==================================================
Result PKBank::loadSaveFile(Result fs, Handle *fsHandle, FS_archive *fsArchive)
// --------------------------------------------------
{
	Result ret = 0;
	uint32_t bytesRead;
	uint32_t size = (gametype == Game::XY ? SAVEDATA_XY_SIZE : SAVEDATA_ORAS_SIZE);
	char path[] = "/main";

	printf("Loading savefile... ");

	ret = FS_loadFile(path, this->savebuffer, fsArchive, fsHandle, size, &bytesRead);
	if (ret) return ret;

	printf(" OK\n  Read %ld bytes\n", bytesRead);

	gametype = PKBank::getGame(bytesRead);

	return ret;
}


// ==================================================
Result PKBank::loadBankFile(Handle *fsHandle, FS_archive *fsArchive)
// --------------------------------------------------
{
	Result ret = 0;
	uint32_t bytesRead;
	uint32_t size = BANKDATA_PKBK_SIZE;
	char path[] = "/pkbank/bank";

	printf("Loading bankfile... ");

	ret = FS_loadFile(path, this->bankbuffer, fsArchive, fsHandle, size, &bytesRead);
	if (ret)
	{
		printf(" Creating...");
		bytesRead = BANKDATA_PKBK_SIZE;
		for (uint32_t i = 0; i < bytesRead; i++)
		{
			bankbuffer[i] = 0x00;
		}

		ret = 0;
	}

	printf(" OK\n  Read %ld bytes\n", bytesRead);

	return ret;
}


// ==================================================
Result PKBank::saveSaveFile(Result fs, Handle *fsHandle, FS_archive *fsArchive)
// --------------------------------------------------
{
	Result ret = 0;
	uint32_t bytesWritten;
	uint32_t size = (gametype == Game::XY ? SAVEDATA_XY_SIZE : SAVEDATA_ORAS_SIZE);
	char path[] = "/main";

	ret = FS_deleteFile(path, fsHandle, fsArchive);

	printf("Writing savefile... ");
	ret = FS_saveSFile(path, this->savebuffer, size, fsArchive, fsHandle, &bytesWritten);

	if (ret) printf(" ERROR\n");
	else printf(" OK\n  Written %ld bytes\n", bytesWritten);

	return ret;
}


// ==================================================
Result PKBank::saveBankFile(Handle *fsHandle, FS_archive *fsArchive)
// --------------------------------------------------
{
	Result ret = 0;
	uint32_t bytesWritten;
	uint32_t size = BANKDATA_PKBK_SIZE;
	char path[] = "/pkbank/bank";

	printf("Writing bankfile... ");
	ret = FS_saveFile(path, this->bankbuffer, size, fsArchive, fsHandle, &bytesWritten);
	if (ret) printf(" ERROR\n");
	else printf(" OK\n  Written %ld bytes\n", bytesWritten);

	return ret;
}


// ==================================================
Result PKBank::backupSaveFile(Handle *fsHandle, FS_archive *fsArchive)
// --------------------------------------------------
{
	Result ret = 0;
	uint32_t bytesWritten;
	uint32_t size = (gametype == Game::XY ? SAVEDATA_XY_SIZE : SAVEDATA_ORAS_SIZE);
	char path[0x20];

	char pathDir[] = "/pkbank/backup/";
	ret = FS_createDirectory(pathDir, fsHandle, fsArchive);
	// if (ret) return ret;

	sprintf(path, "%smain_%i", pathDir, (int) time(NULL));
	ret = FS_deleteFile(path, fsHandle, fsArchive);
	// if (ret) return ret;

	printf("Backing up savefile...");
	ret = FS_saveFile(path, savebuffer, size, fsArchive, fsHandle, &bytesWritten);
	if (ret) printf(" ERROR\n");
	else printf(" OK\n  Written %ld bytes\n", bytesWritten);

	return ret;
}


	/*--------------------------------------------------*\
	 |                 Loading Section                  |
	\*--------------------------------------------------*/


// ==================================================
/// Load data from save
Result PKBank::loadSaveData()
// --------------------------------------------------
{
	Result ret = 0;

	cdelete(savedata);
	savedata = new savedata_t();

	printf("Loading PC Boxes:\n");
	for (uint16_t iB = 0; iB < PC_BOXCOUNT; iB++)
	{
		printf("%-2u ", iB);
		if (iB % 0x10 == 0xf) printf("\n");
		for (uint16_t iP = 0; iP < BOX_PKMCOUNT; iP++)
		{
			loadPkmPC(iB, iP);
		}
	}
	printf(" OK!\n");

	if (gametype == Game::ORAS)
	{
		printf("Loading Dexes:");
		loadDex();
		printf(" OK!\n");
	}
	else
	{
		printf("Not Loading Dexes: X/Y");
	}


	printf("Loading Trainer Card:");
	uint32_t trainerCardOffset = (gametype == Game::XY ? TRAINERCARD_XY_OFFSET : TRAINERCARD_ORAS_OFFSET);

	#pragma GCC diagnostic ignored "-Wstrict-aliasing"
	// printf("[@%p]\n", savebuffer);
	savedata->TID = *(uint16_t*)(savebuffer + trainerCardOffset + 0x0);
	savedata->SID = *(uint16_t*)(savebuffer + trainerCardOffset + 0x2);
	savedata->TSV = computeTSV(savedata->TID, savedata->SID);

	savedata->OTGender = *(uint8_t*) (savebuffer + trainerCardOffset + 0x05);
	for (uint32_t i = 0; i < 0x18; i ++)
		savedata->OTName[i] = *(uint8_t*) (savebuffer + trainerCardOffset + 0x48 + i);


	printf(" OK!\n");
	#pragma GCC diagnostic pop

	return ret;
}

// ==================================================
/// Load data from bank
Result PKBank::loadBankData()
// --------------------------------------------------
{
	Result ret = 0;

	cdelete(bankdata);
	bankdata = new bankdata_t();

	printf("Loading BK Boxes:\n");
	for (uint16_t iB = 0; iB < BANK_BOXCOUNT; iB++)
	{
		printf("%-2u ", iB);
		if (iB % 0x10 == 0xf) printf("\n");
		for (uint16_t iP = 0; iP < BOX_PKMCOUNT; iP++)
		{
			loadPkmBK(iB, iP);
		}
	}
	printf(" OK!\n");

	return ret;
}

// ==================================================
/// Load data from pc to pkm
void PKBank::loadPkmPC(uint16_t boxId, uint16_t slotId)
// --------------------------------------------------
{
	pkm_t* pkm = &savedata->pc.box[boxId].slot[slotId];
	// printf("Loading Pkm... [%-2u][%-2u]\n", boxId, slotId);
	loadEk6PC(pkm, BOX_SIZE * boxId + PKM_SIZE * slotId);
	loadPk6Ek6(pkm); // #Pokemon stored as Pk6
	loadPkmPk6(pkm);
	// printPkm(pkm, KEY_A, 0xe8);
}


// ==================================================
/// Load data from bank to pkm
void PKBank::loadPkmBK(uint16_t boxId, uint16_t slotId)
// --------------------------------------------------
{
	if (!this->bankdata) return;

	pkm_t* pkm = &bankdata->bank.box[boxId].slot[slotId];
	// printf("Loading Pkm... [%-2u][%-2u]\n", boxId, slotId);
	loadEk6BK(pkm, BOX_SIZE * boxId + PKM_SIZE * slotId);
	// loadPk6Ek6(pkm); // #Pokemon stored as Pk6
	loadPkmPk6(pkm);
	// printPkm(pkm, KEY_A, 0xe8);
}


// ==================================================
/// Load data from save buffer to ek6
void PKBank::loadEk6PC(pkm_t* pkm, uint32_t offsetSlot)
// --------------------------------------------------
{
	if (!pkm || !this->savebuffer) return;

	uint32_t offsetPC = (gametype == Game::XY ? PC_XY_OFFSET : PC_ORAS_OFFSET);

	pkm->ek6 = savebuffer + offsetPC + offsetSlot;
}


// ==================================================
/// Load data from bank buffer to ek6
void PKBank::loadEk6BK(pkm_t* pkm, uint32_t offsetSlot)
// --------------------------------------------------
{
	if (!pkm || !this->bankbuffer) return;

	uint32_t offsetBK = BANK_PKBK_OFFSET;

	pkm->ek6 = bankbuffer + offsetBK + offsetSlot;

	// #Pokemon stored as Pk6
	pkm->pk6 = new pk6_t[PK6_SIZE];
	memcpy(pkm->pk6, pkm->ek6, EK6_SIZE);
}


// ==================================================
/// Load data from ek6 to pk6
void PKBank::loadPk6Ek6(pkm_t* pkm)
// --------------------------------------------------
{
	if (!pkm || !pkm->ek6) return;

	pkm->pk6 = new pk6_t[PK6_SIZE];

	decryptEk6(pkm);
}


// ==================================================
/// Load data from pk6 to pkm
void PKBank::loadPkmPk6(pkm_t* pkm)
// --------------------------------------------------
{
	if (!pkm || !pkm->pk6) return;

	pkm->isShiny = Pokemon::isShiny(pkm);
	pkm->speciesID = Pokemon::speciesID(pkm);
	pkm->itemID = Pokemon::itemID(pkm);

	pkm->species = PKData::species(pkm->speciesID);
	pkm->item = PKData::items(pkm->itemID);
	// We use the Pokémon class now!
}


// ==================================================
/// Load data from savebuffer to pokedex
void PKBank::loadDex()
// --------------------------------------------------
{
	uint32_t offset, bitSize, b;
	uint32_t offsetDex = (gametype == Game::XY ? POKEDEX_XY_OFFSET : POKEDEX_ORAS_OFFSET);
	uint32_t offsetDexlg = (gametype == Game::XY ? POKEDEXLANG_XY_OFFSET : POKEDEXLANG_ORAS_OFFSET);


	offset = offsetDex + 0x8;
	bitSize = 0x60 * 9 * 8;
	bool* dexBool = new bool[bitSize];
	for (b = 0; b < bitSize; b++)
		dexBool[b] = ((savebuffer[offset + (b / 8)] >> (b % 8)) & 0x1) == 1;
		// dexBool[i] = (((*(u8*)(savebuffer + offset + (i / 8))) >> (i % 8)) & 0x1) == 1;
	
	b = 0;
	for (; b < 0x60 * 8 * 1; b++)
		savedata->pokedex.dexes[b - 0x60 * 8 * 0].owned = dexBool[b];
	for (; b < 0x60 * 8 * 2; b++)
		savedata->pokedex.dexes[b - 0x60 * 8 * 1].ownedMale = dexBool[b];
	for (; b < 0x60 * 8 * 3; b++)
		savedata->pokedex.dexes[b - 0x60 * 8 * 2].ownedFemale = dexBool[b];
	for (; b < 0x60 * 8 * 4; b++)
		savedata->pokedex.dexes[b - 0x60 * 8 * 3].ownedShinyMale = dexBool[b];
	for (; b < 0x60 * 8 * 5; b++)
		savedata->pokedex.dexes[b - 0x60 * 8 * 4].ownedShinyFemale = dexBool[b];
	for (; b < 0x60 * 8 * 6; b++)
		savedata->pokedex.dexes[b - 0x60 * 8 * 5].encounteredMale = dexBool[b];
	for (; b < 0x60 * 8 * 7; b++)
		savedata->pokedex.dexes[b - 0x60 * 8 * 6].encounteredFemale = dexBool[b];
	for (; b < 0x60 * 8 * 8; b++)
		savedata->pokedex.dexes[b - 0x60 * 8 * 7].encounteredShinyMale = dexBool[b];
	for (; b < 0x60 * 8 * 9; b++)
		savedata->pokedex.dexes[b - 0x60 * 8 * 8].encounteredShinyFemale = dexBool[b];


	offset = offsetDexlg;
	bitSize = 0x27C * 8;
	bool* dexlgBool = new bool[bitSize];
	for (b = 0; b < bitSize; b++)
		dexlgBool[b] = ((savebuffer[offset + (b / 8)] >> (b % 8)) & 0x1) == 1;
		// dexlgBool[i] = (((*(u8*)(savebuffer + offset + (i / 8))) >> (i % 8)) & 0x1) == 1;

	for (b = 0; b < 0x27C; b += 7)
		savedata->pokedex.dexes[(b - 0) / 7].langJapanese = dexlgBool[b];
	for (b = 1; b < 0x27C; b += 7)
		savedata->pokedex.dexes[(b - 1) / 7].langEnglish = dexlgBool[b];
	for (b = 2; b < 0x27C; b += 7)
		savedata->pokedex.dexes[(b - 2) / 7].langFrench = dexlgBool[b];
	for (b = 3; b < 0x27C; b += 7)
		savedata->pokedex.dexes[(b - 3) / 7].langItalian = dexlgBool[b];
	for (b = 4; b < 0x27C; b += 7)
		savedata->pokedex.dexes[(b - 4) / 7].langGerman = dexlgBool[b];
	for (b = 5; b < 0x27C; b += 7)
		savedata->pokedex.dexes[(b - 5) / 7].langSpanish = dexlgBool[b];
	for (b = 6; b < 0x27C; b += 7)
		savedata->pokedex.dexes[(b - 6) / 7].langKorean = dexlgBool[b];


	delete[] dexBool;
	delete[] dexlgBool;
}


	/*--------------------------------------------------*\
	 |                  Saving Section                  |
	\*--------------------------------------------------*/


// ==================================================
/// Save data to save
Result PKBank::saveSaveData()
// --------------------------------------------------
{
	Result ret = 0;

	printf("Saving PC Boxes:\n");
	for (uint16_t iB = 0; iB < PC_BOXCOUNT; iB++)
	{
		printf("%-2u ", iB);
		if (iB % 0x10 == 0xf) printf("\n");
		for (uint16_t iP = 0; iP < BOX_PKMCOUNT; iP++)
		{
			savePkmPC(iB, iP);
		}
	}
	printf("\n");


	if (gametype == Game::ORAS)
	{
		printf("Saving Dexes:");
		saveDex();
		printf(" OK!\n");
	}
	else
	{
		printf("Not Saving Dexes: X/Y");
	}


	printf("Rewriting checksums:");
	rewriteSaveCHK();
	printf(" OK!\n");

	return ret;
}


// ==================================================
/// Save data to bank
Result PKBank::saveBankData()
// --------------------------------------------------
{
	Result ret = 0;

	printf("Saving BK Boxes:\n");
	for (uint16_t iB = 0; iB < BANK_BOXCOUNT; iB++)
	{
		printf("%-2u ", iB);
		if (iB % 0x10 == 0xf) printf("\n");
		for (uint16_t iP = 0; iP < BOX_PKMCOUNT; iP++)
		{
			savePkmBK(iB, iP);
		}
	}
	printf("\n");

	return ret;
}


// ==================================================
void PKBank::savePkmPC(uint16_t boxId, uint16_t slotId)
// --------------------------------------------------
{
	if (!this->savebuffer || !this->savedata) return;

	pkm_t* pkm = &savedata->pc.box[boxId].slot[slotId];
	savePkmPk6(pkm); // #Wololo
	savePk6Ek6(pkm); // #Pokemon stored as Ek6
	// saveEk6PC(pkm); // #Pokemon stored as Ek6
}


// ==================================================
void PKBank::savePkmBK(uint16_t boxId, uint16_t slotId)
// --------------------------------------------------
{
	if (!this->bankbuffer || !this->bankdata) return;

	pkm_t* pkm = &bankdata->bank.box[boxId].slot[slotId];
	// savePkmPk6(pkm); // #if COMMIT_CHEAT_CHANGE
	// savePk6Ek6(pkm); // #Pokemon stored as Pk6
	saveEk6BK(pkm); // #Pokemon stored as Pk6
}


// ==================================================
void PKBank::saveEk6PC(pkm_t* pkm)
// --------------------------------------------------
{
	if (!pkm || !this->savebuffer || !pkm->ek6) return;
	// ~ Useless ~ !!
}


// ==================================================
void PKBank::saveEk6BK(pkm_t* pkm)
// --------------------------------------------------
{
	if (!pkm || !this->bankbuffer || !pkm->ek6) return;
	
	// #Pokemon stored as Pk6
	memcpy(pkm->ek6, pkm->pk6, PK6_SIZE);
}


// ==================================================
void PKBank::savePk6Ek6(pkm_t* pkm)
// --------------------------------------------------
{
	if (!pkm || !pkm->ek6 || !pkm->pk6) return;

	encryptPk6(pkm);
}


// ==================================================
void PKBank::savePkmPk6(pkm_t* pkm)
// --------------------------------------------------
{
	if (!pkm || !pkm->pk6) return;

	if (pkm->moved)
	{
		convertPkmTrainer(pkm);
		Pokemon::computeChecksum(pkm);
	}
}


// ==================================================
/// Save data from pokedex to savebuffer
void PKBank::saveDex()
// --------------------------------------------------
{
	uint32_t offset, bitSize, b;
	uint32_t offsetDex = (gametype == Game::XY ? POKEDEX_XY_OFFSET : POKEDEX_ORAS_OFFSET);
	uint32_t offsetDexlg = (gametype == Game::XY ? POKEDEXLANG_XY_OFFSET : POKEDEXLANG_ORAS_OFFSET);


	offset = offsetDex + 0x8;
	bitSize = 0x60 * 9 * 8;
	bool* dexBool = new bool[bitSize];

	b = 0;
	for (; b < 0x60 * 8 * 1; b++)
		dexBool[b] = savedata->pokedex.dexes[b - 0x60 * 8 * 0].owned;
	for (; b < 0x60 * 8 * 2; b++)
		dexBool[b] = savedata->pokedex.dexes[b - 0x60 * 8 * 1].ownedMale;
	for (; b < 0x60 * 8 * 3; b++)
		dexBool[b] = savedata->pokedex.dexes[b - 0x60 * 8 * 2].ownedFemale;
	for (; b < 0x60 * 8 * 4; b++)
		dexBool[b] = savedata->pokedex.dexes[b - 0x60 * 8 * 3].ownedShinyMale;
	for (; b < 0x60 * 8 * 5; b++)
		dexBool[b] = savedata->pokedex.dexes[b - 0x60 * 8 * 4].ownedShinyFemale;
	for (; b < 0x60 * 8 * 6; b++)
		dexBool[b] = savedata->pokedex.dexes[b - 0x60 * 8 * 5].encounteredMale;
	for (; b < 0x60 * 8 * 7; b++)
		dexBool[b] = savedata->pokedex.dexes[b - 0x60 * 8 * 6].encounteredFemale;
	for (; b < 0x60 * 8 * 8; b++)
		dexBool[b] = savedata->pokedex.dexes[b - 0x60 * 8 * 7].encounteredShinyMale;
	for (; b < 0x60 * 8 * 9; b++)
		dexBool[b] = savedata->pokedex.dexes[b - 0x60 * 8 * 8].encounteredShinyFemale;

	uint8_t* dexByte = new uint8_t[bitSize / 8];
	for (b = 0; b < bitSize; b += 8)
		dexByte[b / 8] = 0;
	for (b = 0; b < bitSize; b++)
		dexByte[b / 8] |= ((dexBool[b] ? 1 : 0) << (b % 8));

	for (b = 0; b < bitSize / 8; b++)
		savebuffer[offset + b] = dexByte[b];


	offset = offsetDexlg;
	bitSize = 0x27C * 8;
	bool* dexlgBool = new bool[bitSize];

	for (b = 0; b < 0x27C; b += 7)
		dexlgBool[b] = savedata->pokedex.dexes[(b - 0) / 7].langJapanese;
	for (b = 1; b < 0x27C; b += 7)
		dexlgBool[b] = savedata->pokedex.dexes[(b - 1) / 7].langEnglish;
	for (b = 2; b < 0x27C; b += 7)
		dexlgBool[b] = savedata->pokedex.dexes[(b - 2) / 7].langFrench;
	for (b = 3; b < 0x27C; b += 7)
		dexlgBool[b] = savedata->pokedex.dexes[(b - 3) / 7].langItalian;
	for (b = 4; b < 0x27C; b += 7)
		dexlgBool[b] = savedata->pokedex.dexes[(b - 4) / 7].langGerman;
	for (b = 5; b < 0x27C; b += 7)
		dexlgBool[b] = savedata->pokedex.dexes[(b - 5) / 7].langSpanish;
	for (b = 6; b < 0x27C; b += 7)
		dexlgBool[b] = savedata->pokedex.dexes[(b - 6) / 7].langKorean;

	uint8_t* dexlgByte = new uint8_t[bitSize / 8];
	for (b = 0; b < bitSize; b += 8)
		dexlgByte[b / 8] = 0;
	for (b = 0; b < bitSize; b++)
		dexlgByte[b / 8] |= ((dexlgBool[b] ? 1 : 0) << (b % 8));

	for (b = 0; b < bitSize / 8; b++)
		savebuffer[offset + b] = dexlgByte[b];


	delete[] dexBool;
	delete[] dexlgBool;
	delete[] dexByte;
	delete[] dexlgByte;
}


// ==================================================
void PKBank::editSaveBuffer(uint32_t pos, uint8_t* ptr, uint32_t size)
// --------------------------------------------------
{
	if (!ptr || !savedata) return;

	uint32_t maxOffset = (gametype == Game::XY ? SAVEDATA_XY_SIZE : SAVEDATA_ORAS_SIZE);

	for (uint32_t i = pos; i < maxOffset && pos >= 0 && i < pos + size; i++)
	{
		savebuffer[i] = ptr[i-pos];
	}
}


// ==================================================
void PKBank::editBankBuffer(uint32_t pos, uint8_t* ptr, uint32_t size)
// --------------------------------------------------
{
	if (!ptr || !bankdata) return;

	uint32_t maxOffset = BANKDATA_PKBK_SIZE;

	for (uint32_t i = pos; i < maxOffset && pos >= 0 && i < pos + size; i++)
	{
		bankbuffer[i] = ptr[i-pos];
	}
}


	/*--------------------------------------------------*\
	 |                 Pokemon Section                  |
	\*--------------------------------------------------*/


// ==================================================
void PKBank::convertPkmTrainer(pkm_t* pkm)
// --------------------------------------------------
{
	// If the Pokémon isn't already modified.
	// Modified it.
	if (!pkm->moved)
	{
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
			if (Pokemon::TID(pkm) == savedata->TID && Pokemon::SID(pkm) == savedata->SID && strcmp((char*)Pokemon::HT_name(pkm), (char*)savedata->OTName) == 0 && Pokemon::OT_gender(pkm) == savedata->OTGender)
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
					if (strcmp((char*)Pokemon::HT_name(pkm), (char*)savedata->OTName) != 0)
					{
						convertPkmHT(pkm);
					}
				}
			}
		}
	}
}


// ==================================================
void PKBank::convertPkmHT(pkm_t* pkm)
// --------------------------------------------------
{
	Pokemon::HT_name(pkm, savedata->OTName);
	Pokemon::HT_gender(pkm, savedata->OTGender);
	Pokemon::currentHandler(pkm, 0x01);
	Pokemon::geo5Region(pkm, Pokemon::geo4Region(pkm));
	Pokemon::geo4Region(pkm, Pokemon::geo3Region(pkm));
	Pokemon::geo3Region(pkm, Pokemon::geo2Region(pkm));
	Pokemon::geo2Region(pkm, Pokemon::geo1Region(pkm));
	Pokemon::geo1Region(pkm, savedata->GEORegion);
	Pokemon::geo5Country(pkm, Pokemon::geo4Country(pkm));
	Pokemon::geo4Country(pkm, Pokemon::geo3Country(pkm));
	Pokemon::geo3Country(pkm, Pokemon::geo2Country(pkm));
	Pokemon::geo2Country(pkm, Pokemon::geo1Country(pkm));
	Pokemon::geo1Country(pkm, savedata->GEOCountry);
	Pokemon::HT_friendship(pkm, PKFilter::getBaseFriendship(Pokemon::speciesID(pkm)));
	Pokemon::HT_affection(pkm, 0x00);
	Pokemon::HT_intensity(pkm, 0x01);
	Pokemon::HT_memory(pkm, 0x04);
	Pokemon::HT_feeling(pkm, rand() % 0xa);
	Pokemon::HT_textVar(pkm, 0x0000);
	// Pokemon::fullness(pkm, pkm->fullness);
	// Pokemon::enjoyment(pkm, pkm->enjoyment);

	pkm->modified = true;
}


// ==================================================
uint16_t PKBank::computePSV(uint32_t PID)
// --------------------------------------------------
{
	return ((((PID >> 16) ^ (PID & 0xFFFF)) >> 4) & 0xFFFF);
}


// ==================================================
uint16_t PKBank::computeTSV(uint16_t TID, uint16_t SID)
// --------------------------------------------------
{
	return ((TID ^ SID) >> 4);
}


// ==================================================
bool PKBank::isShiny(uint32_t PID, uint16_t TID, uint16_t SID)
// --------------------------------------------------
{
	uint16_t PSV = computePSV(PID);
	uint16_t TSV = computeTSV(TID, SID);
	return (PSV == TSV);
}


	/*--------------------------------------------------*\
	 |                 Encrypt Section                  |
	\*--------------------------------------------------*/


// ==================================================
uint32_t PKBank::LCRNG(uint32_t seed)
// --------------------------------------------------
{
	uint32_t a = 0x41c64e6d;
	uint32_t c = 0x00006073;
	return ((seed * a + c) & 0xffffffff);
}


// ==================================================
void PKBank::shufflePk6(pk6_t* pk6, uint8_t sv)
// --------------------------------------------------
{
	uint8_t aloc[] = { 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 2, 3, 1, 1, 2, 3, 2, 3, 1, 1, 2, 3, 2, 3 };
	uint8_t bloc[] = { 1, 1, 2, 3, 2, 3, 0, 0, 0, 0, 0, 0, 2, 3, 1, 1, 3, 2, 2, 3, 1, 1, 3, 2 };
	uint8_t cloc[] = { 2, 3, 1, 1, 3, 2, 2, 3, 1, 1, 3, 2, 0, 0, 0, 0, 0, 0, 3, 2, 3, 2, 1, 1 };
	uint8_t dloc[] = { 3, 2, 3, 2, 1, 1, 3, 2, 3, 2, 1, 1, 3, 2, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0 };

	uint8_t ord[] = { aloc[sv], bloc[sv], cloc[sv], dloc[sv] };

	pk6_t* cpk6 = new pk6_t[PK6_SIZE];
	uint8_t tmp[56];

	// memcpy(cpkm, pkm, 232);
	for (uint32_t i = 0; i < PK6_SIZE; i++)
		cpk6[i] = pk6[i];

	for (uint32_t j = 0; j < 4; j++)
	{
		// memcpy(tmp, pkm + 8 + 56 * ord[j], 56);
		// memcpy(pkm + 8 + 56 * j, tmp, 56);
		for (uint32_t i = 0; i < 56; i++)
		{
			tmp[i] = cpk6[i + 8 + 56 * ord[j]];
		}
		for (uint32_t i = 0; i < 56; i++)
		{
			pk6[i + 8 + 56 * j] = tmp[i];
		}
	}

	adelete(cpk6);
}

// ==================================================
void PKBank::decryptEk6(pkm_t* pkm)
// --------------------------------------------------
{
	memcpy(pkm->pk6, pkm->ek6, EK6_SIZE);

	// printf("\n");
	// printPk6(pkm->pk6, 0, 0x10);

	uint32_t pv = *(uint32_t*)(pkm->pk6 + 0x0);
	uint8_t sv = (((pv & 0x3e000) >> 0xd) % 24);

	uint32_t seed = pv;
	uint16_t tmp;
	// printf("\x1b[0;0H\x1b[2J");
	// printf("Seed: 0x%08lx Sv: 0x%02x\n", seed, sv);
	// printf("Next: 0x%08lx 0x%08lx\n", LCRNG(seed), LCRNG(LCRNG(seed)));
	for (uint32_t i = 0x8; i < EK6_SIZE; i += 0x2)
	{
		// memcpy(&tmp, ek6 + i, 0x2);
		tmp = *(uint16_t*)(pkm->pk6 + i);

		seed = LCRNG(seed);
		tmp ^= (seed >> 16);

		// memcpy(pk6 + i, &tmp, 0x2);
		*(uint16_t*)(pkm->pk6+i) = tmp;
	}

	shufflePk6(pkm->pk6, sv);

	// printPk6(pkm->pk6, 0, 0x10);
}


// ==================================================
void PKBank::encryptPk6(pkm_t* pkm)
// --------------------------------------------------
{
	memcpy(pkm->ek6, pkm->pk6, PK6_SIZE);

	// printf("\n");
	// printPk6(pkm->ek6, 0, 0x10);

	uint32_t pv = *(uint32_t*)(pkm->ek6 + 0x0);
	uint8_t sv = (((pv & 0x3e000) >> 0xd) % 24);

	for (uint32_t i = 0; i < 11; i++)
		shufflePk6(pkm->ek6, sv);

	uint32_t seed = pv;
	uint16_t tmp;
	// printf("\x1b[0;0H\x1b[2J");
	// printf("Seed: 0x%08lx Sv: 0x%02x\n", seed, sv);
	// printf("Next: 0x%08lx 0x%08lx\n", LCRNG(seed), LCRNG(LCRNG(seed)));
	for (uint32_t i = 0x8; i < PK6_SIZE; i += 0x2)
	{
		// memcpy(&tmp, pk6 + i, 0x2);
		tmp = *(uint16_t*)(pkm->ek6 + i);

		seed = LCRNG(seed);
		tmp ^= (seed >> 16);

		// memcpy(ek6 + i, &tmp, 2);
		*(uint16_t*)(pkm->ek6+i) = tmp;
	}

	// printPk6(pkm->ek6, 0, 0x10);
}


	/*--------------------------------------------------*\
	 |                   Save Checksum                  |
	\*--------------------------------------------------*/


// ==================================================
uint32_t PKBank::CHKOffset(uint32_t i)
// --------------------------------------------------
{
	if (!gametype) return 0;

	if (gametype == Game::XY)
	{
		uint32_t _xy[] = { 0x05400, 0x05800, 0x06400, 0x06600, 0x06800, 0x06A00, 0x06C00, 0x06E00, 0x07000, 0x07200, 0x07400, 0x09600, 0x09800, 0x09E00, 0x0A400, 0x0F400, 0x14400, 0x19400, 0x19600, 0x19E00, 0x1A400, 0x1AC00, 0x1B400, 0x1B600, 0x1B800, 0x1BE00, 0x1C000, 0x1C400, 0x1CC00, 0x1CE00, 0x1D000, 0x1D200, 0x1D400, 0x1D600, 0x1DE00, 0x1E400, 0x1E800, 0x20400, 0x20600, 0x20800, 0x20C00, 0x21000, 0x22C00, 0x23000, 0x23800, 0x23C00, 0x24600, 0x24A00, 0x25200, 0x26000, 0x26200, 0x26400, 0x27200, 0x27A00, 0x5C600, };

		return _xy[i] - 0x5400;
	}
	else if (gametype == Game::ORAS)
	{
		uint32_t _oras[] = { 0x05400, 0x05800, 0x06400, 0x06600, 0x06800, 0x06A00, 0x06C00, 0x06E00, 0x07000, 0x07200, 0x07400, 0x09600, 0x09800, 0x09E00, 0x0A400, 0x0F400, 0x14400, 0x19400, 0x19600, 0x19E00, 0x1A400, 0x1B600, 0x1BE00, 0x1C000, 0x1C200, 0x1C800, 0x1CA00, 0x1CE00, 0x1D600, 0x1D800, 0x1DA00, 0x1DC00, 0x1DE00, 0x1E000, 0x1E800, 0x1EE00, 0x1F200, 0x20E00, 0x21000, 0x21400, 0x21800, 0x22000, 0x23C00, 0x24000, 0x24800, 0x24C00, 0x25600, 0x25A00, 0x26200, 0x27000, 0x27200, 0x27400, 0x28200, 0x28A00, 0x28E00, 0x30A00, 0x38400, 0x6D000, };
	
		return _oras[i] - 0x5400;
	}
	else
	{
		return 0;
	}
}


// ==================================================
uint32_t PKBank::CHKLength(uint32_t i)
// --------------------------------------------------
{
	if (!gametype) return 0;

	if (gametype == Game::XY)
	{
		uint32_t _xy[] = { 0x000002C8, 0x00000B88, 0x0000002C, 0x00000038, 0x00000150, 0x00000004, 0x00000008, 0x000001C0, 0x000000BE, 0x00000024, 0x00002100, 0x00000140, 0x00000440, 0x00000574, 0x00004E28, 0x00004E28, 0x00004E28, 0x00000170, 0x0000061C, 0x00000504, 0x000006A0, 0x00000644, 0x00000104, 0x00000004, 0x00000420, 0x00000064, 0x000003F0, 0x0000070C, 0x00000180, 0x00000004, 0x0000000C, 0x00000048, 0x00000054, 0x00000644, 0x000005C8, 0x000002F8, 0x00001B40, 0x000001F4, 0x000001F0, 0x00000216, 0x00000390, 0x00001A90, 0x00000308, 0x00000618, 0x0000025C, 0x00000834, 0x00000318, 0x000007D0, 0x00000C48, 0x00000078, 0x00000200, 0x00000C84, 0x00000628, 0x00034AD0, 0x0000E058, };
	
		return _xy[i];
	}
	else if (gametype == Game::ORAS)
	{
		uint32_t _oras[] = { 0x000002C8, 0x00000B90, 0x0000002C, 0x00000038, 0x00000150, 0x00000004, 0x00000008, 0x000001C0, 0x000000BE, 0x00000024, 0x00002100, 0x00000130, 0x00000440, 0x00000574, 0x00004E28, 0x00004E28, 0x00004E28, 0x00000170, 0x0000061C, 0x00000504, 0x000011CC, 0x00000644, 0x00000104, 0x00000004, 0x00000420, 0x00000064, 0x000003F0, 0x0000070C, 0x00000180, 0x00000004, 0x0000000C, 0x00000048, 0x00000054, 0x00000644, 0x000005C8, 0x000002F8, 0x00001B40, 0x000001F4, 0x000003E0, 0x00000216, 0x00000640, 0x00001A90, 0x00000400, 0x00000618, 0x0000025C, 0x00000834, 0x00000318, 0x000007D0, 0x00000C48, 0x00000078, 0x00000200, 0x00000C84, 0x00000628, 0x00000400, 0x00007AD0, 0x000078B0, 0x00034AD0, 0x0000E058, };
	
		return _oras[i];
	}
	else
	{
		return 0;
	}
}


// ==================================================
uint16_t PKBank::ccitt16(uint8_t* data, uint32_t len)
// --------------------------------------------------
{
	uint16_t crc = 0xFFFF;

	for (uint32_t i = 0; i < len; i++)
	{
		crc ^= ((uint16_t)(data[i] << 8));

		for (uint32_t j = 0; j < 0x8; j++)
		{
			if ((crc & 0x8000) > 0)
				crc = (uint16_t((crc << 1) ^ 0x1021));
			else
				crc <<= 1;
		}
	}

	return crc;
}


// ==================================================
void PKBank::rewriteSaveCHK()
// --------------------------------------------------
{
	uint8_t blockCount = (gametype == Game::XY ? 55 : 58);
	uint32_t csoff = (gametype == Game::XY ? 0x6A81A : 0x7B21A) - 0x5400;
	uint8_t* tmp = new uint8_t[0x35000];
	uint16_t cs;

	if (!tmp) return;

	for (uint32_t i = 0; i < blockCount; i++)
	{
		memcpy(tmp, savebuffer + CHKOffset(i), CHKLength(i));
		cs = ccitt16(tmp, CHKLength(i));
		memcpy(savebuffer + csoff + i * 8, &cs, 2);
	}

	adelete(tmp);
}

// {
// 	uint32_t saveFileSize = (gametype == Game::XY ? SAVEDATA_XY_SIZE : SAVEDATA_ORAS_SIZE);
// 	uint32_t verificationOffset = saveFileSize - 0x200 + 0x10;

// 	// if ((*(uint32_t*)(savebuffer + verificationOffset)) != 0x42454546)
// 	// 	verificationOffset -= 0x200;

// 	uint32_t count = (saveFileSize - verificationOffset - 0x8) / 8;
// 	verificationOffset += 4;

// 	uint32_t* starts = new uint32_t[count];
// 	uint32_t* lengths = new uint32_t[count];
// 	uint16_t* blockIDs = new uint16_t[count];
// 	uint16_t* checksums = new uint16_t[count];
// 	uint32_t currentPosition = 0;

// 	for (uint32_t i = 0; i < count; i++)
// 	{
// 		starts[i] = currentPosition;
// 		lengths[i] = *(uint32_t*)(savebuffer + verificationOffset + 0 + 8 * i);
// 		blockIDs[i] = *(uint16_t*)(savebuffer + verificationOffset + 4 + 8 * i);
// 		checksums[i] = *(uint16_t*)(savebuffer + verificationOffset + 6 + 8 * i);

// 		currentPosition += (lengths[i] % 0x200 == 0 ? lengths[i] : (0x200 - lengths[i] % 0x200 + lengths[i]));

// 		if ((blockIDs[i] != 0) || (i == 0)) continue;
// 		count = i;
// 		break;
// 	}

// 	for (uint32_t i = 0; i < count; i++)
// 	{
// 		uint16_t ccikit = ccitt16((uint8_t*) (savebuffer + starts[i]), lengths[i]);

// 		*(uint8_t*) (savebuffer + verificationOffset + 6 + 8 * i) = ccikit;
// 	}


// 	adelete(starts);
// 	adelete(lengths);
// 	adelete(blockIDs);
// 	adelete(checksums);
// }

