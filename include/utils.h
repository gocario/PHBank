#pragma once
/**
 * @file utils.h
 */
#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <3ds/types.h>

/**
 * @brief Compares two u16 strings.
 * @param[in] str1 The first u16 string.
 * @param[in] str2 The second u16 string.
 * @return The char16 difference between the strings.
 */
inline s32 str16cmp(const u16* str1, const u16* str2)
{
	if (!str1 || !str2) return 0;
	u16 ii;
	for (ii = 0; str1[ii] && str2[ii] && str1[ii] == str2[ii]; ii++);
	return str1[ii] - str2[ii];
}

/**
 * @brief Compares two u16 strings.
 * @param[in] str1 The first u16 string.
 * @param[in] str2 The second u16 string.
 * @param max The max length of the strings.
 * @return The char16 difference between the strings.
 */
inline s32 str16ncmp(const u16* str1, const u16* str2, s16 max)
{
	if (!str1 || !str2) return 0;
	for (u16 ii = 0; ii < max; ii++)
		if (!str1[ii] || !str2[ii] || str1[ii] != str2[ii])
			return str1[ii] - str2[ii];
	return 0;
}

/**
 * @brief Replaces some character of a string.
 * @param[in/out] str The u16 string.
 * @param max The max length of the string.
 * @return The number of characters replaced.
 */
inline u8 str16nfix(u16* str, s16 max)
{
	if (!str) return 0;
	u8 i = 1;
	for (u16 ii = 0; str[ii] && ii < max; ii++)
	{
		if (str[ii] == 0xE08F) str[ii] = 0x2640; // Nidoran ♂
		else if (str[ii] == 0xE08E) str[ii] = 0x2642; // Nidoran ♀
		else if (str[ii] == 0x2019) str[ii] = 0x0027; // Farfetch'd
		else i--; i++;
	}
	return i-1;
}

/**
 * @brief Replaces some character of a string.
 * @param[in/out] str The u32 string.
 * @param max The max length of the string.
 * @return The number of characters replaced.
 */
inline u8 str32nfix(u32* str, s16 max)
{
	if (!str) return 0;
	u8 i = 1;
	for (u16 ii = 0; str[ii] && ii < max; ii++)
	{
		if (str[ii] == 0xE08F) str[ii] = 0x2640; // Nidoran ♂
		else if (str[ii] == 0xE08E) str[ii] = 0x2642; // Nidoran ♀
		else if (str[ii] == 0x2019) str[ii] = 0x0027; // Farfetch'd
		else i--; i++;
	}
	return i-1;
}

/**
 * @brief Gets the length of a u8 string.
 * @param[in] str The u8 string.
 * @param max The max length of the string.
 * @return The length of the string.
 */
inline u16 str8nlen(const u8* str, s16 max)
{
	if (!str) return 0;
	u16 i = 0;
	for (u16 ii = 0; ii < max && str[ii]; ii++)
		if ((str[ii] & 0xC0) ^ 0x80)
			i++;
	return i;
}

#ifdef __cplusplus
}
#endif

#endif // UTILS_H
