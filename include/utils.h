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
 * @brief Converts an unicode string to an ascii string.
 * @param dst The destination (char).
 * @param src The source (u16).
 * @param max The max length of the string.
 */
inline void unicodeToChar(char* dst, const u16* src, s16 max)
{
	if (!src || !dst) return;
	u16 ii;
	for (ii = 0; src[ii] && ii < max-1; ii++)
		dst[ii] = src[ii] & 0xFF;
	dst[ii] = '\0';
}

/**
 * @brief Compare two u16 strings.
 * @param str1 The first u16 string.
 * @param str2 The second u16 string.
 * @param max The max length of the strings.
 * @return Whether the strings are different.
 */
inline s32 u16cmp(const u16* str1, const u16* str2, s16 max)
{
	if (!str1 || !str2) return 0;
	for (u16 ii = 0; ii < max; ii++)
		if (str1[ii] == '\0' || str2[ii] == '\0' || str1[ii] != str2[ii])
			return str2[ii] - str1[ii];
	return 0;
}

#ifdef __cplusplus
}
#endif

#endif // UTILS_H
