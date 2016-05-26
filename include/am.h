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

/// A simple titleid/mediatype struct with a magic smdh.
typedef struct
{
	u64 titleid;
	smdh_s* smdh;
	FS_MediaType mediatype;
} AM_TitleMediaEntry;

/**
 * @brief Gets a list of Pokémon title IDs paired with its mediatype.
 * @param count Pointer to write the title count to.
 * @param titleList Buffer to write retrieved title media entries to. (it's malloc'd inside)
 * @see AM_FreePokemonTitleEntryList(AM_TitleMediaEntry*, u32)
 */
Result AM_GetPokemonTitleEntryList(AM_TitleMediaEntry** titleList, u32* count);

/**
 * @brief Frees the title media entries buffer previously alocated.
 * @param count The title count.
 * @param titleList Buffer to free.
 * @see AM_GetPokemonTitleEntryList(AM_TitleMediaEntry**, u32*)
 */
Result AM_FreePokemonTitleEntryList(AM_TitleMediaEntry* titleList, u32 count);

#ifdef __cplusplus
}
#endif

#endif // AM_H
