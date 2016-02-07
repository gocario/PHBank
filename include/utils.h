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
 * @param dst The destination (u8).
 * @param src The source (u16).
 * @param max The max length of the string.
 */
inline void unicodeToChar(char* dst, u16* src, s16 max)
{
	if (!src || !dst) return;
	u16 ii;
	if (max > 0) *dst = 0x00;
	for (ii = 0; *src && ii < max-1; ii++)
		*(dst + ii) = (*(src + ii)) & 0xFF;
	*(dst + ii) = 0x00;
}

#ifdef __cplusplus
}
#endif

#endif // UTILS_H
