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
 * @brief Converts an UTF-16 string to an ascii string.
 * @param dst Destination (u8).
 * @param src Source (u16).
 * @param length Length of dst.
 */
void readUTF16(void* dst, void* src, u16 length);

/**
 * @brief Converts an unicode string to an ascii string.
 * @param dst The destination (u8).
 * @param src The source (u16).
 * @param length Length of dst.
 */
void readUnicode(void* dst, void* src, u16 length);

#ifdef __cplusplus
}
#endif

#endif // UTILS_H
