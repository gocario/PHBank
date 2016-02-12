#pragma once
/**
 * @file smdh.h
 * @brief From 3ds_hb_menu, modified a bit
 */
#ifndef SMDH_H
#define SMDH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <3ds/types.h>

#define SMDH_HEADER_MAGIC (0x48444D53)

typedef struct
{
	u32 magic;
	u16 version;
	u16 reserved;
} smdhHeader_s;

typedef struct
{
	u16 shortDescription[0x40];
	u16 longDescription[0x80];
	u16 publisher[0x40];
} smdhTitle_s;

typedef struct
{
	u8 gameRatings[0x10];
	u32 regionLock;
	u8 matchMakerId[0xC];
	u32 flags;
	u16 eulaVersion;
	u16 reserved;
	u32 defaultFrame;
	u32 cecId;
} smdhSettings_s;

typedef struct
{
	smdhHeader_s header;
	smdhTitle_s applicationTitles[16];
	smdhSettings_s settings;
	u8 reserved[0x8];
	u8 smallIconData[0x480];
	u16 bigIconData[0x900];
} smdh_s;

/**
 * @brief Extracts some data from a smdh.
 * @param smdh The smdh where data shall be extracted.
 * @param name [out] The english name of the smdh.
 * @param desc [out] The english description of the smdh.
 * @param auth [out] The author of the smdh.
 * @param iconData [out] The big icon data as RGB8.
 */
Result smdhExtractData(smdh_s* smdh, char* name, char* desc, char* auth, u8* iconData);

#ifdef __cplusplus
}
#endif

#endif // SMDH_H
