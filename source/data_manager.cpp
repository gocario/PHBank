#include "data_manager.hpp"

#include "lang.h"
#include "utils.h"
#include "pkdir.h"
#include "personal.hpp"

extern "C" {
#include <3ds/util/utf.h>
}

#include <stdio.h>
#include <string.h>

// wstring for "(None)", just a safe case
const uint32_t wNone[] = { 0x00000028, 0x0000004E, 0x0000004F, 0x0000004E, 0x00000045, 0x00000029, 0x00000000 };

DataManager::DataManager(void)
{

}

DataManager::~DataManager(void)
{
	freeDataLines(wText, BANK_TEXT_COUNT);
	freeDataLines(wAbilities, DEX_ABILITIES_COUNT);
	freeDataLines(wItems, DEX_ITEMS_COUNT);
	freeDataLines(wMoves, DEX_MOVES_COUNT);
	freeDataLines(wNatures, DEX_NATURES_COUNT);
	freeDataLines(wSpecies, DEX_SPECIES_COUNT);
	freeDataLines(wTypes, DEX_TYPES_COUNT);
}

const char* DataManager::lang(void)
{
	switch (userlang)
	{
		// case LANGUAGE_JP: return "jp";	///< Japan
		case LANGUAGE_FR: return "fr";	///< French
		case LANGUAGE_IT: return "it";	///< Italian
		case LANGUAGE_DE: return "de";	///< German
		case LANGUAGE_ES: return "es";	///< Spanish
		// case LANGUAGE_KR: return "kr";	///< Korean
		default:      return "en";	///< English
	}
}

const uint32_t* DataManager::text(BankText text)
{
	if ((u8) text < BANK_TEXT_COUNT)
		return wText[(u8) text];
	else
		return wText[0];
}

const uint32_t* DataManager::abilities(u32 ability)
{
	if (ability < DEX_ABILITIES_COUNT)
		return wAbilities[ability];
	else
		return wAbilities[0];
}

const uint32_t* DataManager::items(u32 item)
{
	if (item < DEX_ITEMS_COUNT)
		return wItems[item];
	else
		return wItems[0];
}

const uint32_t* DataManager::moves(u32 move)
{
	if (move < DEX_MOVES_COUNT)
		return wMoves[move];
	else
		return wMoves[0];
}

const uint32_t* DataManager::natures(u32 nature)
{
	if (nature < DEX_NATURES_COUNT)
		return wNatures[nature];
	else
		return wNone;
}

const uint32_t* DataManager::species(u32 species)
{
	if (species < DEX_SPECIES_COUNT)
		return wSpecies[species];
	else
		return wNone;
}

const uint32_t* DataManager::types(u8 type)
{
	if (type < DEX_TYPES_COUNT)
		return wTypes[type];
	else
		return wNone;
}

Result DataManager::load()
{
	updateSystemLanguage();

	Result ret = 0;

	ret |= loadDataFile("bank_text", wText, BANK_TEXT_COUNT);
	ret |= loadDataFile("abilities", wAbilities, DEX_ABILITIES_COUNT);
	ret |= loadDataFile("items", wItems, DEX_ITEMS_COUNT);
	ret |= loadDataFile("moves", wMoves, DEX_MOVES_COUNT);
	ret |= loadDataFile("natures", wNatures, DEX_NATURES_COUNT);
	ret |= loadDataFile("species", wSpecies, DEX_SPECIES_COUNT);
	ret |= loadDataFile("types", wTypes, DEX_TYPES_COUNT);

	ret |= loadPersonal("personal_ao", true, PERSONAL_AO_COUNT, PERSONAL_INFO_AO_SIZE);

	return ret;
}

Result DataManager::loadPersonal(const char* file, bool ao, u32 personalCount, u32 personalInfoSize)
{
	char path[32];
	sprintf(path, PK_DATA_FOLDER "%s", file);

	FILE* fp = fopen(path, "rb");
	if (!fp) return -1;

	u8* personal_ao = new u8[personalCount * personalInfoSize];

	size_t personalInfoRead = fread(personal_ao, personalInfoSize, personalCount, fp);
	fclose(fp);

	if (personalInfoRead == personalCount)
	{
		Personal.import(personal_ao, personalInfoRead, personalInfoSize);
	}

	delete[] personal_ao;

	return (personalInfoRead == personalCount ? 0 : -3);
}

Result DataManager::loadDataFile(const char* file, uint32_t** data, u32 lineCount)
{
	char path[40];
	sprintf(path, PK_DATA_FOLDER "%s/%s_%s.txt", lang(), file, lang());

	FILE* fp = fopen(path, "r");
	if (!fp)
	{
		sprintf(path, PK_DATA_FOLDER "en/%s_en.txt", file);
		fp = fopen(path, "r");
		if (!fp) return -1;
	}

	fseek(fp, 0L, SEEK_END);
	size_t size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	u8* buffer = new u8[size];

	fread(buffer, 1, size, fp);
	fclose(fp);
	
	loadDataLines(buffer, data, size, lineCount);

	delete[] buffer;

	return 0;
}

void DataManager::loadDataLines(const u8* src, uint32_t** data, u32 srcSize, u32 lineCount)
{
	u32 count = 0;
	u32 lineLength;
	u32 sourceOffset = 0; // 0: No BOM | 3: With BOM
	while (count < lineCount)
	{
		// Calculate the length of the string (in UTF-8)
		for (lineLength = 0; src[sourceOffset + lineLength] != '\n' && sourceOffset + lineLength < srcSize; lineLength++);

		// Generate the wstring (+1 for null terminator)
		data[count] = new uint32_t[lineLength+1];
		memset(data[count], 0, (lineLength+1) * sizeof(uint32_t));

		// Convert the UTF-8 string to a UTF-32 string for wstring
		utf8_to_utf32(data[count], src + sourceOffset, lineLength);

		// Clear the rest of the wstring.
		u16 len8 = u8len(src + sourceOffset, lineLength);
		data[count][len8] = '\0';

		sourceOffset += lineLength + 1;
		count++;
	}
}

void DataManager::freeDataLines(uint32_t** data, u32 lineCount)
{
	for (u32 i = 0; i < lineCount; i++)
		delete data[i];
	// delete[] data;
}
