#include "save_manager.hpp"

#include "pkdir.h"
#include "fs.h"

#include "bank_updater.hpp"

#include <stdio.h>
#include <string.h>

// #define r(fmt, args ...) printf(fmt, ##args)
#define r(fmt, args ...)

Result SaveManager::load()
{
	Result ret;

	ret = loadGameFile();
	if (R_FAILED(ret)) return ret;
	this->sav.loadData();

	ret = loadBankFile();
	if (R_FAILED(ret)) return ret;
	this->bnk.loadData();

	return ret;
}

Result SaveManager::save()
{
	Result ret;

	this->sav.saveData();
	ret = saveGameFile();
	if (R_FAILED(ret)) return ret;

	this->bnk.saveData();
	ret = saveBankFile();
	if (R_FAILED(ret)) return ret;

	return ret;
}

Result SaveManager::backup()
{
	Result ret;

	ret = backupGameFile();
	if (R_FAILED(ret)) return ret;

	ret = backupBankFile();
	if (R_FAILED(ret)) return ret;

	return ret;
}

Result SaveManager::loadGameFile()
{
	Result ret;
	u64 size;
	u8* dst;
	u32 bytesRead = 0;
	Handle fileHandle;

	ret = FSUSER_OpenFile(&fileHandle, saveArchive, fsMakePath(PATH_ASCII, ROOT SAVE_FILE), FS_OPEN_READ, FS_ATTRIBUTE_NONE);
	r(" > FSUSER_OpenFile: %lx\n", ret);
	if (R_FAILED(ret)) return ret;

	ret = FSFILE_GetSize(fileHandle, &size);
	r(" > FSFILE_GetSize: %lx\n", ret);
	if (R_FAILED(ret)) ret = -2;

	this->sav.version = SAV::getGameVersion(size);
	if (Game::is(this->sav.version, Game::XY))
	{
		if (!this->sav.savXY) this->sav.savXY = new SAV6_XY_s();
		dst = (u8*)this->sav.savXY;
	}
	else if (Game::is(this->sav.version, Game::ORAS))
	{
		if (!this->sav.savORAS) this->sav.savORAS = new SAV6_ORAS_s();
		dst = (u8*)this->sav.savORAS;
	}
	else ret = -8;

	if (R_SUCCEEDED(ret))
	{
		ret = FSFILE_Read(fileHandle, &bytesRead, 0L, dst, size);
		r(" > FSFILE_Read: %lx\n", ret);
		if (R_FAILED(ret) || bytesRead < size) ret = -3;
	}

	FSFILE_Close(fileHandle);
	r(" > FSFILE_Close\n");

	return ret;
}

Result SaveManager::saveGameFile()
{
	Result ret;
	u64 size;
	u8* src;
	u32 bytesWritten = 0;
	Handle fileHandle;

	ret = FSUSER_OpenFile(&fileHandle, saveArchive, fsMakePath(PATH_ASCII, ROOT SAVE_FILE), FS_OPEN_WRITE | FS_OPEN_CREATE, FS_ATTRIBUTE_NONE);
	r(" > FSUSER_OpenFile: %lx\n", ret);
	if (R_FAILED(ret)) return ret;

	size = SAV::getGameSize(this->sav.version);
	if (Game::is(this->sav.version, Game::XY))
		src = (u8*)this->sav.savXY;
	else if (Game::is(this->sav.version, Game::ORAS))
		src = (u8*)this->sav.savORAS;
	else ret = -8;

	if (R_SUCCEEDED(ret))
	{
		ret = FSFILE_Write(fileHandle, &bytesWritten, 0L, src, size, FS_WRITE_FLUSH);
		r(" > FSFILE_Write: %lx\n", ret);
		if (R_FAILED(ret) || bytesWritten != size) ret = -2;
	}

	FSFILE_Close(fileHandle);
	r(" > FSFILE_Close\n");

	return ret;
}

Result SaveManager::backupGameFile()
{
	Result ret;
	u32 bytesWritten = 0;
	u32 size;
	u8* src;
	char path[0x30];

	sprintf(path, ROOT BACKUP_FOLDER SAVE_FILE "_%lli", osGetTime()/* - 2208988800L*/);

	size = SAV::getGameSize(this->sav.version);
	if (Game::is(this->sav.version, Game::XY))
		src = (u8*)this->sav.savXY;
	else if (Game::is(this->sav.version, Game::ORAS))
		src = (u8*)this->sav.savORAS;
	else return -8;

	printf("Backing up savefile...");
	FILE* fp = fopen(path, "wb");
	if (!fp) return -1;

	bytesWritten = fwrite(src, 1, size, fp);

	if (ferror(fp)) printf(" ERROR\n");
	else printf(" OK\n");
	printf("  Written %ld bytes\n", bytesWritten);

	ret = ferror(fp);
	fclose(fp);

	return ret;
}

Result SaveManager::loadBankFile()
{
	Result ret;
	u32 bytesRead = 0;
	u32 size = Game::Const::BANK_size;

	if (!this->bnk.bnk) this->bnk.bnk = new BNK6_s();
	u8* dst = (u8*)this->bnk.bnk;
	if (!dst) return -1;

	printf("Loading bankfile...");
	FILE* fp = fopen(SDMC BANK_FOLDER BANK_FILE, "rb");
	if (fp)
	{
		printf(" Reading...");
		bytesRead = fread(dst, 1, size /* sizeof(BNK6_s) */, fp);
		if (ferror(fp)) printf(" ERROR\n");
		else printf(" OK\n");
		printf("  Read %ld/%ld bytes\n", bytesRead, size);

		if (!ferror(fp))
		{
			memset(dst + bytesRead, 0, size - bytesRead);
		}

		ret = ferror(fp);
		fclose(fp);
	}
	else
	{
		printf(" Creating...");
		memset(dst, 0, size);
		bytesRead = size;

		printf(" OK\n  Created %ld bytes\n", size);

		ret = 0;
	}

	if (R_SUCCEEDED(ret))
	{
		BankUpdater::updateBank(dst, bytesRead);
	}

	return ret;
}

Result SaveManager::saveBankFile()
{
	Result ret;
	u32 bytesWritten = 0;
	u32 size = Game::Const::BANK_size;

	u8* src = (u8*)this->bnk.bnk;
	if (!src) return -1;

	printf("Writing bankfile...");
	FILE* fp = fopen(ROOT BANK_FOLDER BANK_FILE, "wb");
	if (!fp) return -1;

	bytesWritten = fwrite(src, 1, size /* sizeof(BNK6_s) */, fp);

	if (ferror(fp)) printf(" ERROR\n");
	else printf(" OK\n");
	printf("  Written %ld bytes\n", bytesWritten);

	ret = ferror(fp);
	fclose(fp);

	return ret;
}

Result SaveManager::backupBankFile()
{
	Result ret;
	u32 bytesWritten = 0;
	u32 size = Game::Const::BANK_size;

	u8* src = (u8*)this->bnk.bnk;
	if (!src) return -1;

	char path[0x30];
	sprintf(path, ROOT BACKUP_FOLDER BANK_FILE "_%lli", osGetTime()/* - 2208988800L*/);

	printf("Backing up bankfile...");
	FILE* fp = fopen(path, "wb");
	if (!fp) return -1;

	bytesWritten = fwrite(src, 1, size /* sizeof(BNK6_s) */, fp);

	if (ferror(fp)) printf(" ERROR\n");
	else printf(" OK\n");
	printf("  Written %ld bytes\n", bytesWritten);

	ret = ferror(fp);
	fclose(fp);

	return ret;
}
