#pragma once
/**
 * @file am.h
 * @brief Application Manager Services for Pokémon titles
 */
#ifndef AM_H
#define AM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <3ds/types.h>
#include <3ds/services/fs.h>
#include <3ds/services/am.h>

#include "smdh.h"

/// Pokémon title enumeration.
typedef enum
{
	POKEMON_X = 0,
	POKEMON_Y = 1,
	POKEMON_OR = 2,
	POKEMON_AS = 3,
} AM_PokemonTitle;

/// A simple titleid/mediatype struct with a magic smdh.
typedef struct
{
	u64 titleid;
	smdh_s* smdh;
	FS_MediaType mediatype;
} AM_TitleMediaEntry;

/**
 * @brief Retrieves the Pokémon title ID.
 * @param pkmTitle The Pokémon title.
 * @return The id of the Pokémon title.
 */
const u64 AM_GetPokemonTitleID(AM_PokemonTitle pkmTitle);

/**
 * @brief Retrieves the name of a title.
 * @param titleID The id of the title.
 * @return The name of the title.
 */
const char* AM_GetPokemonTitleName(u64 titleID);

/**
 * @brief Gets a list of Pokémon title IDs paired with its mediatype.
 * @param count Pointer to write the title count to.
 * @param titleList Buffer to write retrieved title media entries to. (it's malloc'd inside)
 * @see AM_FreePokemonTitleEntryList(AM_TitleMediaEntry*)
 */
Result AM_GetPokemonTitleEntryList(AM_TitleMediaEntry** titleList, u32* count);

/**
 * @brief Frees the title media entries buffer previously alocated.
 * @param count The title count.
 * @param titleList Buffer to free.
 * @see AM_GetPokemonTitleEntryList(u32, AM_TitleMediaEntry**)
 */
Result AM_FreePokemonTitleEntryList(AM_TitleMediaEntry* titleList, u32 count);

#ifdef __cplusplus
}
#endif

#endif // AM_H
