#include "pkbank.hpp"

#include <3ds.h>

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
	adelete(this->savedata);
	adelete(this->bankdata);
	this->gametype = Game::None;
}


// --------------------------------------------------
bool PKBank::saveLoaded()
// --------------------------------------------------
{
	return this->savedata != NULL;
}

// --------------------------------------------------
bool PKBank::bankLoaded()
// --------------------------------------------------
{
	return this->bankdata != NULL;
}


bool PKBank::load(Result fs, Handle *sdHandle, Handle *saveHandle, FS_archive *sdArchive, FS_archive *saveArchive)
{
	bool ret;

	char path[] = "/pkbank";
	createDirectory(path, sdHandle, sdArchive);

	if (fs)
	{
		printf(">Loading Save from SD\n");
		ret = this->loadSave(sdHandle, sdArchive);
	}
	else
	{
		printf(">Loading Save from Card\n");
		ret = this->loadSave(saveHandle, saveArchive);
	}

	// printf(">Loading Bank from SD\n");
	// ret &= this->loadBank(sdHandle, sdArchive);

	// hidScanInput();
	// if (hidKeysHeld() & KEY_L)
	// {
	// 	printf(">Backing up Save to SD\n");
	// 	ret &= this->backupSave(sdHandle, sdArchive);
	// }
	// else
	// {
	// 	printf(">Not backing up Save to SD\n");
	// }

	return ret;
}

// --------------------------------------------------
bool PKBank::loadSave(Handle *fsHandle, FS_archive *fsArchive)
// --------------------------------------------------
{
	Result ret;
	uint32_t bytesRead;
	char path[] = "/main";

	printf("Loading savefile... ");

	ret = loadFile(path, this->savebuffer, fsArchive, fsHandle, SAVEDATA_ORAS_SIZE, &bytesRead);

	if (ret) return false;
	printf(" OK\n  Read %ld bytes\n", bytesRead);
	
	this->gametype = PKBank::getGame(bytesRead);


	cdelete(savedata);
	savedata = new savedata_t();

	printf("Loading Save Boxes: \n");
	for (uint32_t iB = 0x0; iB < PC_BOXCOUNT; iB++)
	{
		printf(" %-2lu", iB+1);
		if (iB % 0x10 == 0xf) printf("\n");
		for (uint32_t iP = 0x0; iP < BOX_PKMCOUNT; iP++)
		{
			// printf("\nLoading... [%lu][%lu]\n", iB, iP);
			loadPokemonSave(iB, iP, savedata->pc[iB][iP]);
			// printf("\nLoading... OK\n");
		}
	}
	printf("\nDone\n");

	return true;
}

bool PKBank::loadBank(Handle *fsHandle, FS_archive *fsArchive)
{
	Result ret;
	char path[] = "/pkbank/bank";
	uint32_t bytesRead;

	printf("Loading bankfile...");
	ret = loadFile(path, savebuffer, fsArchive, fsHandle, BANKDATA_PKBK_SIZE, &bytesRead);
	if (ret)
	{
		printf(" Creating...");
		for (uint32_t i = 0; i < BANKDATA_PKBK_SIZE; i++)
		{
			bankbuffer[i] = 0x00;
		}
		bytesRead = BANKDATA_PKBK_SIZE;
	}
	printf(" OK\n  Read %ld bytes\n", bytesRead);
	

	cdelete(bankdata);
	bankdata = new bankdata_t();

	printf("Loading Bank Boxes: \n");
	for (uint32_t iB = 0; iB < BANK_BOXCOUNT; iB++)
	{
		printf(" %-2lu", iB+1);
		if ((iB+1) % 16 == 0) printf("\n");
		for (uint32_t iP = 0; iP < BOX_PKMCOUNT; iP++)
		{
			// printf("\nLoading... [%lu][%lu]\n", iB, iP);
			loadPokemonBank(iB, iP, bankdata->bank[iB][iP]);
			// printf("\nLoading... OK\n");

			// for (uint32_t iI = 0; iI < PKM_SIZE; iI++)
			// {
			// 	bankdata->bank[iB][iP][iI] = bankbuffer[PC_ORAS_OFFSET + iB * BOX_SIZE + iP * PKM_SIZE + iI];
			// }
		}
	}
	printf("\nDone\n");

	return true;
}

// --------------------------------------------------
void PKBank::loadPokemonSave(uint16_t box, uint16_t slot, pkm_t pkm)
// --------------------------------------------------
{
	if (!savebuffer) return;

	loadPokemonSave(PKM_SIZE * (box * BOX_PKMCOUNT + slot), pkm);
}

// --------------------------------------------------
void PKBank::loadPokemonSave(uint32_t offsetSlot, pkm_t pkm)
{
	if (!savebuffer) return;

	ek6_t* ek6 = new ek6_t[EK6_SIZE];

	uint32_t offsetPC = (gametype == Game::ORAS ? PC_ORAS_OFFSET : PC_XY_OFFSET);
	
	for (uint32_t i = 0; i < PKM_SIZE; i++)
		ek6[i] = savebuffer[offsetPC + offsetSlot + i];

	loadPokemon(ek6, pkm);

	adelete(ek6);
}
// --------------------------------------------------


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

// --------------------------------------------------
void PKBank::loadPokemon(ek6_t* ek6, pkm_t pkm)
// --------------------------------------------------
{
	pk6_t* pk6 = new pk6_t[PK6_SIZE];

	// printf("\nDecrypting...\n");
	decryptPokemon(ek6, pk6);
	// printf("Decrypting... OK\n");
	// printf("Converting...\n");
	convertPk6Pkm(pk6, pkm);
	// printf("Converting... OK\n");

	adelete(pk6);
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
