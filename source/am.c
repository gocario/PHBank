#include "am.h"
#include "fs.h"

#include <3ds/result.h>

#include <stdio.h>
#include <stdlib.h>

// #define AM_DEBUG

// #define debug_print(fmt, args ...) printf(fmt, ##args)
// #define r(fmt, args ...) printf(fmt, ##args)
#define debug_print(fmt, args ...)
#define r(fmt, args ...)

static const u32 pokemonTitleCount = 4;
static const u64 pokemonTitleIDs[] = {
	0x0004000000055D00, ///< Pokémon X
	0x0004000000055E00, ///< Pokémon Y
	0x000400000011C400, ///< Pokémon Omega Ruby
	0x000400000011C500, ///< Pokémon Alpha Sapphire
	0x0000000000000000,	///< Not Pokémon
};

static const char* pokemonTitleName[] = {
	"Pokemon X",
	"Pokemon Y",
	"Pokemon Omega Ruby",
	"Pokemon Alpha Sapphire",
	"Not Pokemon",
};

static const Result AM_GetSmdh(AM_TitleMediaEntry* title)
{
	if (!title || title->smdh) return -1;

	Result ret;
	u32 bytesRead;
	Handle fileHandle;

	debug_print("AM_GetSmdh:\n");

	u32 archivePath[] = { title->titleid & 0xFFFFFFFF, (title->titleid >> 32) & 0xFFFFFFFF, title->mediatype, 0x00000000 };
	u32 filePath[] = { 0x00000000, 0x00000000, 0x00000002, 0x6E6F6369, 0x00000000 };

	ret = FSUSER_OpenFileDirectly(&fileHandle, (FS_Archive) { ARCHIVE_SAVEDATA_AND_CONTENT , (FS_Path) { PATH_BINARY, 16, archivePath } }, (FS_Path) { PATH_BINARY, 20, filePath }, FS_OPEN_READ, FS_ATTRIBUTE_NONE);
	r(" > FSUSER_OpenFileDirectly: %lx\n", ret);

	if (R_SUCCEEDED(ret))
	{
		title->smdh = (smdh_s*) malloc(sizeof(smdh_s));

		ret = FSFILE_Read(fileHandle, &bytesRead, 0LL, title->smdh, sizeof(smdh_s));
		r(" > FSFILE_Read: %lx\n", ret);
	}

	if (R_FAILED(ret))
	{
		free(title->smdh);
		title->smdh = NULL;
	}

	ret = FSFILE_Close(fileHandle);
	r(" > FSFILE_Close: %lx\n", ret);

	return ret;
}

const u64 AM_GetPokemonTitleID(AM_PokemonTitle pkmTitle)
{
	if (pkmTitle >= POKEMON_X && pkmTitle <= POKEMON_AS)
		return pokemonTitleIDs[pkmTitle];
	return pokemonTitleIDs[pokemonTitleCount];
}

const char* AM_GetPokemonTitleName(u64 titleID)
{
	for (u32 ii = 0; ii < pokemonTitleCount; ii++)
		if	(titleID == pokemonTitleIDs[ii])
			return pokemonTitleName[ii];
	return pokemonTitleName[pokemonTitleCount];
}

Result AM_GetPokemonTitleEntryList(AM_TitleMediaEntry** titleList, u32* count)
{
	if (!titleList || !titleList) return -1;

	Result ret;
	u32 ccount = 0;
	u32 count_SD = 0;
	u32 count_Card = 0;
	u32 pkmCount_SD = 0;
	u32 pkmCount_Card = 0;
	u32 pkmCount_Total = 0;
	u64* titleIDs_SD = NULL;
	u64* titleIDs_Card = NULL;

	debug_print("AM_GetPokemonTitleEntryList:\n");

	ret = AM_GetTitleCount(MEDIATYPE_SD, &count_SD);
	r(" > AM_GetTitleCount: %lx\n", ret);

	ret = AM_GetTitleCount(MEDIATYPE_GAME_CARD, &count_Card);
	r(" > AM_GetTitleCount: %lx\n", ret);

	titleIDs_SD = (u64*) malloc(count_SD * sizeof(u64));
	titleIDs_Card = (u64*) malloc(count_Card * sizeof(u64));

	ret = AM_GetTitleIdList(MEDIATYPE_SD, count_SD, titleIDs_SD);
	r(" > AM_GetTitleIdList: %lx\n", ret);

	ret = AM_GetTitleIdList(MEDIATYPE_GAME_CARD, count_Card, titleIDs_Card);
	r(" > AM_GetTitleIdList: %lx\n", ret);

	debug_print("Count SD  : %lu\n", count_SD);
	debug_print("Count Card: %lu\n", count_Card);

	for (u32 i = 0; i < count_SD; i++)
		for (u32 ii = 0; ii < pokemonTitleCount; ii++)
			if (titleIDs_SD[i] == pokemonTitleIDs[ii])
				pkmCount_SD++;

	for (u32 i = 0; i < count_Card; i++)
		for (u32 ii = 0; ii < pokemonTitleCount; ii++)
			if (titleIDs_Card[i] == pokemonTitleIDs[ii])
				pkmCount_Card++;

	pkmCount_Total = pkmCount_SD + pkmCount_Card;
	if (count) *count = pkmCount_Total;

	debug_print("PkmCount SD  : %lu\n", pkmCount_SD);
	debug_print("PkmCount Card: %lu\n", pkmCount_Card);
	debug_print("PkmCount Total: %lu\n", pkmCount_Total);

	AM_TitleMediaEntry* titleIDs = (AM_TitleMediaEntry*) malloc(pkmCount_Total * sizeof(AM_TitleMediaEntry));
	*titleList = titleIDs;

	debug_print("Caring about card! (%lu/%lu done)\n", ccount, pkmCount_Total);
	// Card first for the final list.
	for (u32 i = 0; i < count_Card; i++)
		for (u32 ii = 0; ii < pokemonTitleCount; ii++)
			if (titleIDs_Card[i] == pokemonTitleIDs[ii])
			{
				debug_print("Adding title: %016llx (%lu/%lu)", pokemonTitleIDs[ii], ccount+1, pkmCount_Total);
				titleIDs[ccount] = (AM_TitleMediaEntry) { pokemonTitleIDs[ii], NULL, MEDIATYPE_GAME_CARD };
				ret = AM_GetSmdh(&(titleIDs[ccount]));
				r(" > AM_GetSmdh: %lx\n", ret);
				ccount++;
			}

	debug_print("Caring about SD! (%lu/%lu done)\n", ccount , pkmCount_Total);
	// SD second for the final list.
	for (u32 i = 0; i < count_SD; i++)
		for (u32 ii = 0; ii < pokemonTitleCount; ii++)
			if (titleIDs_SD[i] == pokemonTitleIDs[ii])
			{
				debug_print("Adding title: %016llx (%lu/%lu)", pokemonTitleIDs[ii], ccount+1, pkmCount_Total);
				titleIDs[ccount] = (AM_TitleMediaEntry) { pokemonTitleIDs[ii], NULL, MEDIATYPE_SD };
				ret = AM_GetSmdh(&(titleIDs[ccount]));
				r(" > AM_GetSmdh: %lx\n", ret);
				ccount++;
			}

	free(titleIDs_SD);
	free(titleIDs_Card);

	return ret;
}

Result AM_FreePokemonTitleEntryList(AM_TitleMediaEntry* titleList, u32 count)
{
	if (!titleList) return -1;

	debug_print("AM_FreePokemonTitleEntryList:\n");

	for (u32 i = 0; i < count; i++)
		free(titleList[i].smdh);
	free(titleList);

	return 0;
}
