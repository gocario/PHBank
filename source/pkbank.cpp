#include "pkbank.hpp"

/*----------------------------------------------------------*\
 |                 Constructor / Destructor                 |
\*----------------------------------------------------------*/


// --------------------------------------------------
PKBank::PKBank()
// --------------------------------------------------
{
	this->savedata = NULL;
	this->bankdata = NULL;
	this->gametype = Game::None;
}


// --------------------------------------------------
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


Result PKBank::readLoad(Result fs, Handle *sdHandle, Handle *saveHandle, FS_archive *sdArchive, FS_archive *saveArchive)
{
	Result ret = 0;

	printf("> Reading...\n");
	ret = read(fs, sdHandle, saveHandle, sdArchive, saveArchive);
	if (ret) return ret;

	printf("> Loading...\n");
	ret = load();
	if (ret) return ret;

	return ret;
}


Result PKBank::writeSave(Result fs, Handle *sdHandle, Handle *saveHandle, FS_archive *sdArchive, FS_archive *saveArchive)
{
	Result ret = 0;

	ret = write(fs, sdHandle, saveHandle, sdArchive, saveArchive);
	if (ret) return ret;

	ret = save();
	if (ret) return ret;

	return ret;
}


	/*--------------------------------------------------*\
	 |              Reading/Writing Section             |
	\*--------------------------------------------------*/


// --------------------------------------------------
/// Read data from cartridge
Result PKBank::read(Result fs, Handle *sdHandle, Handle *saveHandle, FS_archive *sdArchive, FS_archive *saveArchive)
// --------------------------------------------------
{
	Result ret = 0;

	char path[] = "/pkbank";
	createDirectory(path, sdHandle, sdArchive);

	if (fs)
	{
		printf(">Loading Save from SD\n");
		ret = readSave(sdHandle, sdArchive);
		if (ret) return ret;
	}
	else
	{
		printf(">Loading Save from Card\n");
		ret = readSave(saveHandle, saveArchive);
		if (ret) return ret;
	}

	printf(">Loading Bank from SD\n");
	ret = readBank(sdHandle, sdArchive);
	if (ret) return ret;

	hidScanInput();
	if (hidKeysHeld() & KEY_L)
	{
		printf(">Backing up Save to SD\n");
		ret = backupSave(sdHandle, sdArchive);
		if (ret) return ret;
	}
	else
	{
		printf(">Not backing up Save to SD\n");
	}

	return ret;
}

// --------------------------------------------------
/// Write data from cartridge
Result PKBank::write(Result fs, Handle *sdHandle, Handle *saveHandle, FS_archive *sdArchive, FS_archive *saveArchive)
// --------------------------------------------------
{
	Result ret = 0;

	char path[] = "/pkbank";
	createDirectory(path, sdHandle, sdArchive);

	if (fs)
	{
		printf(">Writing Save from SD\n");
		ret = writeSave(sdHandle, sdArchive);
		if (ret) return ret;
	}
	else
	{
		printf(">Writing Save from Card\n");
		ret = writeSave(saveHandle, saveArchive);
		if (ret) return ret;
	}

	printf(">Writing Bank from SD\n");
	ret = writeBank(sdHandle, sdArchive);
	if (ret) return ret;

	hidScanInput();
	if (hidKeysHeld() & KEY_L)
	{
		printf(">Backing up Save to SD\n");
		ret = backupSave(sdHandle, sdArchive);
		if (ret) return ret;
	}
	else
	{
		printf(">Not backing up Save to SD\n");
	}
	return ret;
}

	/*--------------------------------------------------*\
	 |                 Loading Section                  |
	\*--------------------------------------------------*/


// --------------------------------------------------
/// Load data from save and bank buffers
Result PKBank::load()
// --------------------------------------------------
{
	Result ret = 0;
	
	ret = loadSave();
	if (ret) return ret;

	ret = loadBank();
	if (ret) return ret;

	return ret;
}


	/*--------------------------------------------------*\
	 |                  Saving Section                  |
	\*--------------------------------------------------*/


// --------------------------------------------------
/// Save data to save and bank buffers
Result PKBank::save()
// --------------------------------------------------
{
	Result ret = 0;
	
	ret = saveSave();
	if (ret) return ret;
	
	ret = saveBank();
	if (ret) return ret;

	return ret;
}


	/*--------------------------------------------------*\
	 |                   Utils Section                  |
	\*--------------------------------------------------*/


// --------------------------------------------------
void PKBank::printByte(u8* bytes, u32 key, uint32_t max)
// --------------------------------------------------
{
	printf(" ");
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
			if (hidKeysHeld() & (KEY_L | KEY_R | KEY_A)) break;

			gfxFlushBuffers();
			// gfxSwapBuffers();
			gspWaitForVBlank();
		}
	}
}


// --------------------------------------------------
void PKBank::printPkm(pkm_t* pkm, u32 key, uint32_t max)
// --------------------------------------------------
{
	// printf("Pkm(off): 0x%08x\n", pkm);
	// printByte(pkm->ek6, 0, 0x10);
	printByte(pkm->pk6, key, 0x10);

	// printByte(pkm->pk6, key, max);
}

// --------------------------------------------------
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


// --------------------------------------------------
void PKBank::getPkm(uint16_t slotId, pkm_t** pkm, bool inBank)
// --------------------------------------------------
{
	// T O D O !!
}


// --------------------------------------------------
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


// --------------------------------------------------
void PKBank::getPkm(uint16_t boxId, uint16_t rowId, uint16_t colId, pkm_t** pkm, bool inBank)
// --------------------------------------------------
{
	getPkm(boxId, rowId * BOX_COL_PKMCOUNT + colId, pkm, inBank);
}


/*------------------------------------------------------------*\
 |                      Private Section                       |
\*------------------------------------------------------------*/


	/*--------------------------------------------------*\
	 |              Reading/Writing Section             |
	\*--------------------------------------------------*/


// --------------------------------------------------
Result PKBank::readSave(Handle *fsHandle, FS_archive *fsArchive)
// --------------------------------------------------
{
	Result ret = 0;
	uint32_t bytesRead;
	char path[] = "/main";

	printf("Loading savefile... ");

	ret = loadFile(path, this->savebuffer, fsArchive, fsHandle, SAVEDATA_ORAS_SIZE, &bytesRead);
	if (ret) return ret;

	printf(" OK\n  Read %ld bytes\n", bytesRead);

	gametype = PKBank::getGame(bytesRead);

	return ret;
}


// --------------------------------------------------
Result PKBank::readBank(Handle *fsHandle, FS_archive *fsArchive)
// --------------------------------------------------
{
	Result ret = 0;
	uint32_t bytesRead;
	char path[] = "/pkbank/bank";

	printf("Loading bankfile... ");

	ret = loadFile(path, this->bankbuffer, fsArchive, fsHandle, BANKDATA_PKBK_SIZE, &bytesRead);
	if (ret)
	{
		printf(" Creating...");
		for (uint32_t i = 0; i < BANKDATA_PKBK_SIZE; i++)
		{
			bankbuffer[i] = 0x00;
		}
		bytesRead = BANKDATA_PKBK_SIZE;

		ret = 0;
	}

	printf(" OK\n  Read %ld bytes\n", bytesRead);

	return ret;
}


// --------------------------------------------------
Result PKBank::writeSave(Handle *fsHandle, FS_archive *fsArchive)
// --------------------------------------------------
{
	Result ret = 0;

	// T O D O !!

	return ret;
}


// --------------------------------------------------
Result PKBank::writeBank(Handle *fsHandle, FS_archive *fsArchive)
// --------------------------------------------------
{
	Result ret = 0;

	// T O D O !!

	return ret;
}


// --------------------------------------------------
Result PKBank::backupSave(Handle *fsHandle, FS_archive *fsArchive)
// --------------------------------------------------
{
	Result ret = 0;

	/*
	if (!savebuffer) return -1;

	char pathDir[] = "/pkbank/backup/";
	createDirectory(pathDir, fsHandle, fsArchive);

	uint32_t byteWritten;
	char path[] = "/pkbank/backup/main";
	deleteFile(path, fsHandle, fsArchive);
	printf("Backing up savefile...");
	saveFile(path, savebuffer, SAVEDATA_ORAS_SIZE, fsArchive, fsHandle, &byteWritten);
	printf(" OK\n  Written %ld bytes\n", byteWritten);

	return true;
	*/

	return ret;
}


	/*--------------------------------------------------*\
	 |                 Loading Section                  |
	\*--------------------------------------------------*/


// --------------------------------------------------
/// Load data from save
Result PKBank::loadSave()
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
	printf("\n");

	return ret;
}

// --------------------------------------------------
/// Load data from bank
Result PKBank::loadBank()
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
	printf("\n");

	return ret;
}

// --------------------------------------------------
/// Load data from pc to pkm
void PKBank::loadPkmPC(uint16_t boxId, uint16_t slotId)
// --------------------------------------------------
{
	pkm_t* pkm = &savedata->pc.box[boxId].slot[slotId];
	// printf("Loading Pkm... [%-2u][%-2u]\n", boxId, slotId);
	loadEk6PC(pkm, BOX_SIZE * boxId + PKM_SIZE * slotId);
	loadPk6Ek6(pkm);
	loadPkmPk6(pkm);
	// printPkm(pkm, KEY_A, 0xe8);
}


// --------------------------------------------------
/// Load data from bank to pkm
void PKBank::loadPkmBK(uint16_t boxId, uint16_t slotId)
// --------------------------------------------------
{
	if (!this->bankdata) return;

	pkm_t* pkm = &bankdata->bank.box[boxId].slot[slotId];
	// printf("Loading Pkm... [%-2u][%-2u]\n", boxId, slotId);
	loadEk6BK(pkm, BOX_SIZE * boxId + PKM_SIZE * slotId);
	loadPk6Ek6(pkm);
	loadPkmPk6(pkm);
	// printPkm(pkm, KEY_A, 0xe8);
}


// --------------------------------------------------
/// Load data from save buffer to ek6
void PKBank::loadEk6PC(pkm_t* pkm, uint32_t offsetSlot)
// --------------------------------------------------
{
	if (!this->savebuffer) return;

	uint32_t offsetPC = (gametype == Game::XY ? PC_XY_OFFSET : PC_ORAS_OFFSET);

	pkm->ek6 = savebuffer + offsetPC + offsetSlot;
}


// --------------------------------------------------
/// Load data from bank buffer to ek6
void PKBank::loadEk6BK(pkm_t* pkm, uint32_t offsetSlot)
// --------------------------------------------------
{
	if (!this->bankbuffer) return;

	uint32_t offsetBK = BANK_PKBK_OFFSET;

	pkm->ek6 = savebuffer + offsetBK + offsetSlot;
}


// --------------------------------------------------
/// Load data from ek6 to pk6
void PKBank::loadPk6Ek6(pkm_t* pkm)
// --------------------------------------------------
{
	if (!pkm->ek6) return;

	pkm->pk6 = new pk6_t[PK6_SIZE];

	decryptEk6(pkm);
}


// --------------------------------------------------
/// Load data from pk6 to pkm
void PKBank::loadPkmPk6(pkm_t* pkm)
// --------------------------------------------------
{
	if (!pkm->pk6) return;

	pkm->species = *(uint16_t*)(pkm->pk6 + 0x08);
	pkm->TID = *(uint16_t*)(pkm->pk6 + 0x0c);
	pkm->SID = *(uint16_t*)(pkm->pk6 + 0x0e);
	pkm->PID = *(uint32_t*)(pkm->pk6 + 0x18);

	// T O D O !!
}


	/*--------------------------------------------------*\
	 |                  Saving Section                  |
	\*--------------------------------------------------*/


// --------------------------------------------------
/// Save data from save
Result PKBank::saveSave()
// --------------------------------------------------
{
	Result ret = 0;

	for (uint16_t iB = 0; iB < PC_BOXCOUNT; iB++)
	{
		for (uint16_t iP = 0; iP < BOX_PKMCOUNT; iP++)
		{
			// savePkmPC()
		}
	}

	return ret;
}

// --------------------------------------------------
/// Save data from bank
Result PKBank::saveBank()
// --------------------------------------------------
{
	Result ret = 0;

	for (uint16_t iB = 0; iB < BANK_BOXCOUNT; iB++)
	{
		for (uint16_t iP = 0; iP < BOX_PKMCOUNT; iP++)
		{
			// savePkmBK()
		}
	}

	return ret;
}


// --------------------------------------------------
void PKBank::savePkmPC(uint16_t boxId, uint16_t slotId)
// --------------------------------------------------
{
	pkm_t* pkm = &savedata->pc.box[boxId].slot[slotId];
	savePkmPk6(pkm);
	savePk6Ek6(pkm);
	saveEk6PC(pkm); // ~
}


// --------------------------------------------------
void PKBank::savePkmBK(uint16_t boxId, uint16_t slotId)
// --------------------------------------------------
{
	pkm_t* pkm = &savedata->pc.box[boxId].slot[slotId];
	savePkmPk6(pkm);
	savePk6Ek6(pkm);
	saveEk6BK(pkm); // ~
}


// --------------------------------------------------
void PKBank::saveEk6PC(pkm_t* pkm)
// --------------------------------------------------
{
	// ~ Useless ~ !!
}


// --------------------------------------------------
void PKBank::saveEk6BK(pkm_t* pkm)
// --------------------------------------------------
{
	// ~ Useless ~ !!
}


// --------------------------------------------------
void PKBank::savePk6Ek6(pkm_t* pkm)
// --------------------------------------------------
{
	if (!pkm->pk6) return;

	encryptPk6(pkm);
}


// --------------------------------------------------
void PKBank::savePkmPk6(pkm_t* pkm)
// --------------------------------------------------
{
	// T O D O !!
}


	/*--------------------------------------------------*\
	 |                 Encrypt Section                  |
	\*--------------------------------------------------*/


// --------------------------------------------------
uint32_t PKBank::LCRNG(uint32_t seed)
// --------------------------------------------------
{
	uint32_t a = 0x41c64e6d;
	uint32_t c = 0x00006073;
	return ((seed * a + c) & 0xffffffff);
}


// --------------------------------------------------
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

// --------------------------------------------------
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


// --------------------------------------------------
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










































/*
// --------------------------------------------------
void PKBank::loadPokemonBank(uint16_t box, uint16_t slot, pkm_t pkm)
// --------------------------------------------------
{
	if (!bankbuffer) return;

	loadPokemonBank(PKM_SIZE * (box * BOX_PKMCOUNT + slot), pkm);
}


// --------------------------------------------------
void PKBank::loadPokemonBank(uint32_t offsetSlot, pkm_t pkm)
// --------------------------------------------------
{
	if (!bankbuffer) return;

	ek6_t* ek6 = new ek6_t[EK6_SIZE];

	uint32_t offsetBank = BANK_PKBK_OFFSET;
	
	for (uint32_t i = 0; i < PKM_SIZE; i++)
		ek6[i] = savebuffer[offsetBank + offsetSlot + i];

	loadPokemon(ek6, pkm);

	adelete(ek6);
}



bool PKBank::save(Result fs, Handle *sdHandle, Handle *saveHandle, FS_archive *sdArchive, FS_archive *saveArchive)
{
	bool ret;

	char path[] = "/pkbank/";
	createDirectory(path, sdHandle, sdArchive);

	if (fs)
	{
		printf(">Saving Save to SD\n");
		ret = this->saveSave(sdHandle, sdArchive);
	}
	else
	{
		printf(">Saving Save to Card\n");
		ret = this->saveSave(saveHandle, saveArchive);
	}

	printf(">Saving Bank to SD\n");
	ret &= this->saveBank(sdHandle, sdArchive);

	return ret;
}

bool PKBank::saveSave(Handle *fsHandle, FS_archive *fsArchive)
{
	if (!writeSaveBuffer()) return false;

	uint32_t byteWritten;
	char path[] = "/main";
	deleteFile(path, fsHandle, fsArchive);
	printf("Saving savefile...");
	saveFile(path, savebuffer, SAVEDATA_ORAS_SIZE, fsArchive, fsHandle, &byteWritten);
	printf(" OK\n  Written %ld bytes\n", byteWritten);

	return true;
}

bool PKBank::saveBank(Handle *fsHandle, FS_archive *fsArchive)
{
	if (!writeBankBuffer()) return false;

	uint32_t byteWritten;
	char path[] = "/pkbank/bank.pkbk";
	deleteFile(path, fsHandle, fsArchive);
	printf("Saving bankfile...");
	saveFile(path, bankbuffer, BANKDATA_PKBK_SIZE, fsArchive, fsHandle, &byteWritten);
	printf(" OK\n  Written %ld bytes\n", byteWritten);

	return true;
}



bool PKBank::backupSave(Handle *fsHandle, FS_archive *fsArchive)
{
	if (!savebuffer) return false;

	char pathDir[] = "/pkbank/backup/";
	createDirectory(pathDir, fsHandle, fsArchive);

	uint32_t byteWritten;
	char path[] = "/pkbank/backup/main";
	deleteFile(path, fsHandle, fsArchive);
	printf("Backing up savefile...");
	saveFile(path, savebuffer, SAVEDATA_ORAS_SIZE, fsArchive, fsHandle, &byteWritten);
	printf(" OK\n  Written %ld bytes\n", byteWritten);

	return true;
}


// --------------------------------------------------
void PKBank::getPokemon(CursorBox_t* cursor)
// --------------------------------------------------
{
	if (cursor->isInBank)
	{
		getPokemonBK(cursor);
	}
	else
	{
		getPokemonPC(cursor);
	}
}

// --------------------------------------------------
void PKBank::getPokemonPC(CursorBox_t* cursor)
// --------------------------------------------------
{
	getPokemonPC(cursor->box * BOX_PKMCOUNT, PKM_SIZE * (cursor->row * BOX_COL_PKMCOUNT + cursor->col), cursor->sPkm);
}

// --------------------------------------------------
void PKBank::getPokemonPC(uint16_t box, uint16_t slot, pkm_t* ppkm)
// --------------------------------------------------
{
	ppkm = &(savedata->pc[box][slot]);
}

// --------------------------------------------------
void PKBank::getPokemonBK(CursorBox_t* cursor)
// --------------------------------------------------
{
	getPokemonBK(cursor->bBox * BOX_PKMCOUNT, PKM_SIZE * (cursor->bRow * BOX_COL_PKMCOUNT + cursor->bCol), cursor->sPkm);
}

// --------------------------------------------------
void PKBank::getPokemonBK(uint16_t box, uint16_t slot, pkm_t* ppkm)
// --------------------------------------------------
{
	ppkm = &(bankdata->bank[box][slot]);
}


// --------------------------------------------------
Game::gametype_e PKBank::getGame(uint32_t bytesRead)
// --------------------------------------------------
{
	Game::gametype_e gametype = Game::None;

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


void PKBank::transferPokemon(pkm_t src, pkm_t dest)
{

}

// --------------------------------------------------
void PKBank::copyPokemon(pkm_t src, pkm_t dest)
// --------------------------------------------------
{
	for (uint32_t i = 0; i < PKM_SIZE; i++)
		dest[i] = src[i];
}

// --------------------------------------------------
void PKBank::deletePokemon(pkm_t pkm)
// --------------------------------------------------
{
	for (uint32_t i = 0; i < PKM_SIZE; i++)
		pkm[i] = 0x00;
}

// --------------------------------------------------
void PKBank::printPkm(pkm_t pkm, u32 key, u32 max)
// --------------------------------------------------
{
	printf(" ");
	for (u32 i = 0; i < max; i++)
	{
		if (i % 0x10 == 0 && i > 0)
			printf("\n ");
		if (i % 0x02 == 0)
			printf(" ");
		printf("%02x", pkm[i]);
	}
	printf("\n");

	if (key != 0)
	{
		while (aptMainLoop())
		{
			hidScanInput();

			if (hidKeysDown() & key) break;

			gfxFlushBuffers();
			gspWaitForVBlank();
		}
	}
}

// --------------------------------------------------
void PKBank::printPk6(pk6_t* pk6, u32 key, u32 max)
// --------------------------------------------------
{
	printf(" ");
	for (u32 i = 0; i < max; i++)
	{
		if (i % 0x10 == 0 && i > 0)
			printf("\n ");
		if (i % 0x02 == 0)
			printf(" ");
		printf("%02x", pk6[i]);
	}
	printf("\n");


	if (key != 0)
	{
		while (aptMainLoop())
		{
			hidScanInput();

			if (hidKeysDown() & key) break;

			gfxFlushBuffers();
			gspWaitForVBlank();
		}
	}
}


// --------------------------------------------------
void PKBank::convertPkmPk6(pkm_t pkm, pk6_t* pk6)
// --------------------------------------------------
{
	for (u32 i = 0; i < PKM_SIZE; i++)
		pk6[i] = pkm[i];
}

// --------------------------------------------------
void PKBank::convertPk6Pkm(pk6_t* pk6, pkm_t pkm)
// --------------------------------------------------
{
	for (u32 i = 0; i < PK6_SIZE; i++)
		pkm[i] = pk6[i];
}

// --------------------------------------------------
void PKBank::convertPk6Ek6(pk6_t* pk6, ek6_t* ek6)
// --------------------------------------------------
{
	for (u32 i = 0; i < PK6_SIZE; i++)
		ek6[i] = pk6[i];
}

// --------------------------------------------------
void PKBank::convertEk6Pk6(ek6_t* ek6, pk6_t* pk6)
// --------------------------------------------------
{
	for (u32 i = 0; i < EK6_SIZE; i++)
		pk6[i] = ek6[i];
}


// --------------------------------------------------
void PKBank::decryptPokemon(ek6_t* ek6, pk6_t* pk6)
// --------------------------------------------------
{
	// memcpy(pk6, ek6, EK6_SIZE);
	for (uint32_t i = 0; i < EK6_SIZE; i++)
		pk6[i] = ek6[i];

	// printf("\n");
	// printPk6(pk6, 0, 0x10);

	uint32_t pv = *(uint32_t*)(pk6 + 0x0);
	uint8_t sv = (((pv & 0x3e000) >> 0xd) % 24);

	uint32_t seed = pv;
	uint16_t tmp;
	// printf("\x1b[0;0H\x1b[2J");
	// printf("Seed: 0x%08lx Sv: 0x%02x\n", seed, sv);
	// printf("Next: 0x%08lx 0x%08lx\n", LCRNG(seed), LCRNG(LCRNG(seed)));
	for (uint32_t i = 0x8; i < EK6_SIZE; i += 0x2)
	{
		// memcpy(&tmp, ek6 + i, 0x2);
		tmp = *(uint16_t*)(pk6 + i);

		seed = LCRNG(seed);
		tmp ^= (seed >> 16);
		// printf("%08lx ", seed);

		// memcpy(pk6 + i, &tmp, 0x2);
		// pk6[i] = *((uint8_t*)&(tmp));
		// pk6[i+1] = *((uint8_t*)&(tmp)+1);
		*(uint16_t*)(pk6+i) = tmp;
		// printf("0x%-4x -> 0x%-2x 0x%-2x\n", tmp, *pk6[i], *pk6[i+1]);
	}

	// printf("Shuffling...\n");
	shufflePokemon(pk6, sv);
	// printf("Shuffling... OK\n");

	// printPk6(pk6, 0, 0x10);
}

// --------------------------------------------------
void PKBank::encryptPokemon(pk6_t* pk6, ek6_t* ek6)
// --------------------------------------------------
{
	// memcpy(ek6, pk6, PK6_SIZE);
	for (uint32_t i = 0; i < PK6_SIZE; i++)
		ek6[i] = pk6[i];

	// printf("\n");
	// printPk6(ek6, 0, 0x10);

	uint32_t pv = *(uint32_t*)(ek6 + 0x0);
	uint8_t sv = (((pv & 0x3e000) >> 0xd) % 24);

	// printf("Shuffling...\n");
	for (uint32_t i = 0; i < 11; i++)
		shufflePokemon(ek6, sv);
	// printf("Shuffling... OK\n");

	uint32_t seed = pv;
	uint16_t tmp;
	// printf("\x1b[0;0H\x1b[2J");
	// printf("Seed: 0x%08lx Sv: 0x%02x\n", seed, sv);
	// printf("Next: 0x%08lx 0x%08lx\n", LCRNG(seed), LCRNG(LCRNG(seed)));
	for (uint32_t i = 0x8; i < PK6_SIZE; i += 0x2)
	{
		// memcpy(&tmp, pk6 + i, 0x2);
		tmp = *(uint16_t*)(ek6 + i);

		seed = LCRNG(seed);
		tmp ^= (seed >> 16);
		// printf("%08lx ", seed);

		// memcpy(ek6 + i, &tmp, 2);
		// ek6[i] = *((uint8_t*)&(tmp));
		// ek6[i+1] = *((uint8_t*)&(tmp)+1);
		*(uint16_t*)(ek6+i) = tmp;
		// printf("0x%-4x -> 0x%-2x 0x%-2x\n", tmp, ek6[i], ek6[i+1]);
	}

	// printPk6(ek6, 0, 0x10);
}

// --------------------------------------------------
void PKBank::shufflePokemon(pk6_t* pk6, uint8_t sv)
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

// --------------------------------------------------
uint32_t PKBank::LCRNG(uint32_t seed)
// --------------------------------------------------
{
	uint32_t a = 0x41c64e6d;
	uint32_t c = 0x00006073;
	return ((seed * a + c) & 0xffffffff);
}


bool PKBank::writeBuffers()
{
	if (!this->writeSaveBuffer()) return false;
	if (!this->writeBankBuffer()) return false;

	return true;
}

bool PKBank::writeSaveBuffer()
{
	if (!savedata) return false;

	for (uint32_t iB = 0; iB < PC_BOXCOUNT; iB++)
	{
		for (uint32_t iP = 0; iP < BOX_PKMCOUNT; iP++)
		{
			editBankBuffer(PC_XY_OFFSET + iB * BOX_SIZE + iP * PKM_SIZE, (uint8_t*)(this->savedata->pc[iB][iP]), PKM_SIZE);
		}
	}

	// TODO ?
	
	return true;
}

bool PKBank::writeBankBuffer()
{
	if (!bankdata) return false;

	for (uint32_t iB = 0; iB < BANK_BOXCOUNT; iB++)
	{
		for (uint32_t iP = 0; iP < BOX_PKMCOUNT; iP++)
		{
			editBankBuffer(BANK_PKBK_OFFSET + iB * BOX_SIZE + iP * PKM_SIZE, (uint8_t*)(this->bankdata->bank[iB][iP]), PKM_SIZE);
		}
	}
	
	// TODO ?

	return true;
}

void PKBank::editSaveBuffer(uint32_t pos, uint8_t* ptr, uint32_t size)
{
	if (!savedata) return;

	for (uint32_t i = pos; i < SAVEDATA_XY_SIZE && pos >= 0 && i < pos + size; i++)
	{
		savebuffer[i] = ptr[i-pos];
	}
}

void PKBank::editBankBuffer(uint32_t pos, uint8_t* ptr, uint32_t size)
{
	if (!bankdata) return;

	for (uint32_t i = pos; i < BANKDATA_PKBK_SIZE && pos >= 0 && i < pos + size; i++)
	{
		bankbuffer[i] = ptr[i-pos];
	}
}
*/