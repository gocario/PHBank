#include "pkdata.hpp"


// --------------------------------------------------
u8 PKData::_lang = LANG_EN;
u8 PKData::_personal[PERSONAL_COUNT][PERSONAL_LENGTH];
u8 PKData::_abilities[DEX_ABILITIES_COUNT][DEX_ABILITIES_LENGTH];
u8 PKData::_items[DEX_ITEMS_COUNT][DEX_ITEMS_LENGTH];
u8 PKData::_natures[DEX_NATURES_COUNT][DEX_NATURES_LENGTH];
u8 PKData::_moves[DEX_MOVES_COUNT][DEX_MOVES_LENGTH];
u8 PKData::_species[DEX_SPECIES_COUNT][DEX_SPECIES_LENGTH];
// --------------------------------------------------


// --------------------------------------------------
const char* PKData::lang()
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
u8* PKData::personal(u32 species, u32 form)
// --------------------------------------------------
{
	if (species < PERSONAL_COUNT)
	{
		if (form > PKData::_personal[species][0xC])
			form = PKData::_personal[species][0xC];
		return PKData::_personal[species];
	}
	else
		return NULL;
	
}


// --------------------------------------------------
u8* PKData::personal(u32 species)
// --------------------------------------------------
{
	return PKData::personal(species, 0);
}


// --------------------------------------------------
u8* PKData::abilities(u32 ability)
// --------------------------------------------------
{
	if (ability < DEX_ABILITIES_COUNT)
		return PKData::_abilities[ability];
		// return (u8*)(((u8*)&(PKData::_abilities)) + DEX_ABILITIES_LENGTH * ability);
	else
		return NULL;
	
}

// --------------------------------------------------
u8* PKData::items(u32 item)
// --------------------------------------------------
{
	if (item < DEX_ITEMS_COUNT)
		return PKData::_items[item];
		// return (u8*)(((u8*)&(PKData::_items)) + DEX_ITEMS_LENGTH * item);
	else
		return NULL;
}

// --------------------------------------------------
u8* PKData::moves(u32 move)
// --------------------------------------------------
{
	if (move < DEX_MOVES_COUNT)
		return PKData::_moves[move];
		// return (u8*)(((u8*)&(PKData::_moves)) + DEX_MOVES_LENGTH * move);
	else
		return NULL;
}

// --------------------------------------------------
u8* PKData::natures(u32 nature)
// --------------------------------------------------
{
	if (nature < DEX_NATURES_COUNT)
		return PKData::_natures[nature];
		// return (u8*)(((u8*)&(PKData::_natures)) + DEX_NATURES_LENGTH * nature);
	else
		return NULL;
}

// --------------------------------------------------
u8* PKData::species(u32 species)
// --------------------------------------------------
{
	if (species < DEX_SPECIES_COUNT)
		return PKData::_species[species];
		// return (u8*)(((u8*)&(PKData::_species)) + DEX_SPECIES_LENGTH * species);
	else
		return NULL;
}


const u8 hpTypes[16][9] = {
	"Fighting", "Flying", "Poison", "Ground",
	"Rock", "Bug", "Ghost", "Steel", "Fire", "Water",
	"Grass", "Electric", "Psychic", "Ice", "Dragon", "Dark"
};

// --------------------------------------------------
const u8* PKData::HPTypes(u8 hiddenPower)
// --------------------------------------------------
{
	if (hiddenPower < 16)
		return hpTypes[hiddenPower];
	else
		return NULL;
}


// --------------------------------------------------
Result PKData::load(Handle *sdHandle, FS_archive *sdArchive)
// --------------------------------------------------
{
	// species_en: 6 074 octets #0x17ba
	// species_fr: 6 635 octets #0x19eb
	// items_en: 8 150 octets #0x1fd6
	// items_fr: 8 634 octets #0x21ba

	Result ret;
	ret = PKData::loadData(sdHandle, sdArchive, DEX_ABILITIES_COUNT * DEX_ABILITIES_LENGTH, (char*)("/pkbank/data/en/abilities_en.txt"), (u8*)&(PKData::_abilities), DEX_ABILITIES_LENGTH, DEX_ABILITIES_COUNT);
	ret = PKData::loadData(sdHandle, sdArchive, DEX_ITEMS_COUNT * DEX_ITEMS_LENGTH, (char*)("/pkbank/data/en/items_en.txt"), (u8*)&(PKData::_items), DEX_ITEMS_LENGTH, DEX_ITEMS_COUNT);
	ret = PKData::loadData(sdHandle, sdArchive, DEX_MOVES_COUNT * DEX_MOVES_LENGTH, (char*)("/pkbank/data/en/moves_en.txt"), (u8*)&(PKData::_moves), DEX_MOVES_LENGTH, DEX_MOVES_COUNT);
	ret = PKData::loadData(sdHandle, sdArchive, DEX_NATURES_COUNT * DEX_NATURES_LENGTH, (char*)("/pkbank/data/en/natures_en.txt"), (u8*)&(PKData::_natures), DEX_NATURES_LENGTH, DEX_NATURES_COUNT);
	ret = PKData::loadData(sdHandle, sdArchive, DEX_SPECIES_COUNT * DEX_SPECIES_LENGTH, (char*)("/pkbank/data/en/species_en.txt"), (u8*)&(PKData::_species), DEX_SPECIES_LENGTH, DEX_SPECIES_COUNT);


	u8 buffer[12000];
	u32 bytesRead;
	char path[] = "/pkbank/data/personal";
	ret = FS_loadFile(path, buffer, sdArchive, sdHandle, 12000, &bytesRead);
	if (ret) { printf(" ERROR loading \"%s\"...\n", path); return ret; }
	memcpy(_personal, buffer, bytesRead);
	printf(" OK!\n");

	return ret;
}


// --------------------------------------------------
Result PKData::loadData(Handle *sdHandle, FS_archive *sdArchive, u32 maxSize, char* path, u8* dest, u32 lineLength, u32 lineCount)
// --------------------------------------------------
{
	u32 bytesRead;
	u8* buffer = new u8[maxSize];

	Result ret;
	ret = FS_loadFile(path, buffer, sdArchive, sdHandle, maxSize, &bytesRead);
	if (ret) { printf(" ERROR loading \"%s\"...\n", path); delete[] buffer; return ret; }
	printf(" OK!\n");

	ret = PKData::loadDataLine(buffer, dest, lineLength, lineCount);

	delete[] buffer;
	return ret;
}


// --------------------------------------------------
Result PKData::loadDataLine(u8* src, u8* dst, u32 lineLength, u32 lineCount)
// --------------------------------------------------
{
	u32 count = 0;
	u32 lineOffset = 0;
	u32 sourceOffset = 3;
	while(count < lineCount)
	{
		lineOffset = 0;
		// printf("> %lu ", count);
		// printf("[@%p] ", (void*)(dst + lineLength * count));
		while (src[sourceOffset] != '\n' && src[sourceOffset] != '\0' && lineOffset < lineLength)
		{
			// printf("%c", *(u8*) (src + sourceOffset));
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

		// dst[lineLength * count + lineOffset] = '\0';

		// if (count < 10)
		// {
		// printf(" -> ");
		// for (u32 i = 0; i < 15; i++)
		// {
		// 	if (*(u8*)(dst + lineLength * count + i) > 0x10)
		// 		printf("%c", *(u8*)(dst + lineLength * count + i));
		// 	else if (*(u8*)(dst + lineLength * count + i) == '\0')
		// 		printf("\\");
		// 	else
		// 		printf("#");
		// }
		// printf("|\n");
		// }
		// if (count == 10) waitKey(KEY_A);
		// if (hidKeysHeld() & KEY_R) return 0;

		count++;

	}
	return 0;
}

