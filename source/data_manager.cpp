#include "data_manager.hpp"

#include "pkdir.h"

#include <stdio.h>
#include <string.h>

DataManager::DataManager(void)
{
	_lang = LANG_EN;
}

DataManager::~DataManager(void)
{

}

const char* DataManager::lang(void)
{
	switch (_lang)
	{
		case LANG_FR:
			return "fr";
		case LANG_EN:
		default:
			return "en";
	}
}

const u8* DataManager::personal(u32 species, u32 form)
{
	if (species < PERSONAL_COUNT)
	{
		if (form > _personal[species][0xC])
			form = _personal[species][0xC];
		return _personal[species];
	}
	else
		return NULL;
}

const u8* DataManager::personal(u32 species)
{
	return personal(species, 0);
}

const u8* DataManager::abilities(u32 ability)
{
	if (ability < DEX_ABILITIES_COUNT)
		return _abilities[ability];
	else
		return NULL;
}

const u8* DataManager::items(u32 item)
{
	if (item < DEX_ITEMS_COUNT)
		return _items[item];
	else
		return NULL;
}

const u8* DataManager::moves(u32 move)
{
	if (move < DEX_MOVES_COUNT)
		return _moves[move];
	else
		return NULL;
}

const u8* DataManager::natures(u32 nature)
{
	if (nature < DEX_NATURES_COUNT)
		return _natures[nature];
	else
		return NULL;
}

const u8* DataManager::species(u32 species)
{
	if (species < DEX_SPECIES_COUNT)
		return _species[species];
	else
		return NULL;
}

static const u8 hpTypes[16][9] = {
	"Fighting", "Flying", "Poison", "Ground",
	"Rock", "Bug", "Ghost", "Steel", "Fire", "Water",
	"Grass", "Electric", "Psychic", "Ice", "Dragon", "Dark"
};

const u8* DataManager::HPTypes(u8 hiddenPower)
{
	if (hiddenPower < 16)
		return hpTypes[hiddenPower];
	else
		return NULL;
}

Result DataManager::load()
{
	Result ret = 0;
	ret |= loadDataFile((char*) "abilities", (u8*)&(_abilities), DEX_ABILITIES_LENGTH, DEX_ABILITIES_COUNT);
	ret |= loadDataFile((char*) "items", (u8*)&(_items), DEX_ITEMS_LENGTH, DEX_ITEMS_COUNT);
	ret |= loadDataFile((char*) "moves", (u8*)&(_moves), DEX_MOVES_LENGTH, DEX_MOVES_COUNT);
	ret |= loadDataFile((char*) "natures", (u8*)&(_natures), DEX_NATURES_LENGTH, DEX_NATURES_COUNT);
	ret |= loadDataFile((char*) "species", (u8*)&(_species), DEX_SPECIES_LENGTH, DEX_SPECIES_COUNT);

	char path[32];

	sprintf(path, "%spersonal", pk_dataFolder); // Add it into pkdir.h?
	FILE* fp = fopen(path, "rb");
	if (!fp) return (ret | -1);
	
	fread(_personal, PERSONAL_LENGTH, PERSONAL_COUNT, fp);

	fclose(fp);

	return ret;
}

Result DataManager::loadDataFile(char* file, u8* dest, u32 lineMaxLength, u32 lineCount)
{
	char path[40];
	
	sprintf(path, "%s%s/%s_%s.txt", pk_dataFolder, lang(), file, lang());
	FILE* fp = fopen(path, "r");
	if (!fp) return -1;

	u8 buffer[lineMaxLength * lineCount];
	fread(buffer, 1, lineMaxLength * lineCount, fp);
	loadDataLines(buffer, dest, lineMaxLength, lineCount);

	fclose(fp);

	return 0;
}

Result DataManager::loadDataLines(u8* src, u8* dst, u32 lineMaxLength, u32 lineCount)
{
	u32 count = 0;
	u32 lineOffset = 0;
	u32 sourceOffset = 3;
	while (count < lineCount)
	{
		lineOffset = 0;
		while (src[sourceOffset] != '\n' && src[sourceOffset] != '\0' && lineOffset < lineMaxLength)
		{
			*(u8*)(dst + lineMaxLength * count + lineOffset) = *(u8*)(src + sourceOffset);
			lineOffset++;
			sourceOffset++;
		}
		while (lineOffset < lineMaxLength)
		{
			*(u8*)(dst + lineMaxLength * count + lineOffset) = '\0';
			lineOffset++;
		}
		while (src[sourceOffset] == '\n' || src[sourceOffset] == '\0') sourceOffset++;
		count++;
	}
	return 0;
}
