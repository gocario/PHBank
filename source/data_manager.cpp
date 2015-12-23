#include "data_manager.hpp"

#include "fs.h"

#include <stdio.h>
#include <string.h>


// ----------------------------------------------
DataManager::DataManager()
// ----------------------------------------------
{
	_lang = LANG_EN;
}


// ----------------------------------------------
DataManager::~DataManager()
// ----------------------------------------------
{

}


// --------------------------------------------------
const char* DataManager::lang()
// --------------------------------------------------
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


// --------------------------------------------------
const u8* DataManager::personal(u32 species, u32 form)
// --------------------------------------------------
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


// --------------------------------------------------
const u8* DataManager::personal(u32 species)
// --------------------------------------------------
{
	return personal(species, 0);
}


// --------------------------------------------------
const u8* DataManager::abilities(u32 ability)
// --------------------------------------------------
{
	if (ability < DEX_ABILITIES_COUNT)
		return _abilities[ability];
	else
		return NULL;
	
}


// --------------------------------------------------
const u8* DataManager::items(u32 item)
// --------------------------------------------------
{
	if (item < DEX_ITEMS_COUNT)
		return _items[item];
	else
		return NULL;
}


// --------------------------------------------------
const u8* DataManager::moves(u32 move)
// --------------------------------------------------
{
	if (move < DEX_MOVES_COUNT)
		return _moves[move];
	else
		return NULL;
}

// --------------------------------------------------
const u8* DataManager::natures(u32 nature)
// --------------------------------------------------
{
	if (nature < DEX_NATURES_COUNT)
		return _natures[nature];
	else
		return NULL;
}


// --------------------------------------------------
const u8* DataManager::species(u32 species)
// --------------------------------------------------
{
	if (species < DEX_SPECIES_COUNT)
		return _species[species];
	else
		return NULL;
}


const u8 hpTypes[16][9] = {
	"Fighting", "Flying", "Poison", "Ground",
	"Rock", "Bug", "Ghost", "Steel", "Fire", "Water",
	"Grass", "Electric", "Psychic", "Ice", "Dragon", "Dark"
};


// --------------------------------------------------
const u8* DataManager::HPTypes(u8 hiddenPower)
// --------------------------------------------------
{
	if (hiddenPower < 16)
		return hpTypes[hiddenPower];
	else
		return NULL;
}

// ----------------------------------------------
Result DataManager::load()
// ----------------------------------------------
{
	Result ret;
	ret = loadData((char*)("abilities"), (u8*)&(_abilities), DEX_ABILITIES_LENGTH, DEX_ABILITIES_COUNT);
	ret = loadData((char*)("items"), (u8*)&(_items), DEX_ITEMS_LENGTH, DEX_ITEMS_COUNT);
	ret = loadData((char*)("moves"), (u8*)&(_moves), DEX_MOVES_LENGTH, DEX_MOVES_COUNT);
	ret = loadData((char*)("natures"), (u8*)&(_natures), DEX_NATURES_LENGTH, DEX_NATURES_COUNT);
	ret = loadData((char*)("species"), (u8*)&(_species), DEX_SPECIES_LENGTH, DEX_SPECIES_COUNT);

	u8 buffer[12000];
	u32 bytesRead;
	char path[64];
	char pkmFolder[] = "/pkbank/";
	char dataFolder[] = "data/";
	sprintf(path, "%s%spersonal", pkmFolder, dataFolder);
	ret = FS_ReadFile(path, buffer, &sdmcArchive, 12000, &bytesRead);
	if (ret) { printf(" ERROR loading \"%s\"...\n", path); return ret; }
	memcpy(_personal, buffer, bytesRead);

	return 0;
}



// ----------------------------------------------
Result DataManager::loadData(char* file, u8* dest, u32 lineLength, u32 lineCount)
// ----------------------------------------------
{
	u32 bytesRead;
	u8* buffer = new u8[lineLength * lineCount];

	Result ret;

	char path[64];
	char pkmFolder[] = "/pkbank/";
	char dataFolder[] = "data/";
	sprintf(path, "%s%s%s/%s_%s.txt", pkmFolder, dataFolder, lang(), file, lang());
	ret = FS_ReadFile(path, buffer, &sdmcArchive, lineLength * lineCount, &bytesRead);
	if (ret) printf(" ERROR loading \"%s\"...\n", path);

	if (!ret)
	{
		ret = loadDataLine(buffer, dest, lineLength, lineCount);
	}

	delete[] buffer;
	return ret;
}


// ----------------------------------------------
Result DataManager::loadDataLine(u8* src, u8* dst, u32 lineLength, u32 lineCount)
// ----------------------------------------------
{
	u32 count = 0;
	u32 lineOffset = 0;
	u32 sourceOffset = 3;
	while(count < lineCount)
	{
		lineOffset = 0;
		while (src[sourceOffset] != '\n' && src[sourceOffset] != '\0' && lineOffset < lineLength)
		{
			*(u8*)(dst + lineLength * count + lineOffset) = *(u8*)(src + sourceOffset);
			lineOffset++;
			sourceOffset++;
		}
		while (lineOffset < lineLength)
		{
			*(u8*)(dst + lineLength * count + lineOffset) = '\0';
			lineOffset++;
		}
		while (src[sourceOffset] == '\n' || src[sourceOffset] == '\0') sourceOffset++;
		count++;

	}

	return 0;
}
