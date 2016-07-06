#pragma once
/**
 * @file sav/sav6.hpp
 * @author Gocario
 */

#include "pkm/pk6.hpp"
#include "sav/sav6.hpp"

#define BK_BOX_COUNT (100)

struct __attribute__((packed)) BK_s // 0xA9EC0
{
	EK6_t Ek6[BK_BOX_COUNT][BOX_PKM_COUNT]; // 0x00000-0xA9EBF
};

struct __attribute__((packed)) BKLayout_s // 0xE00
{
	u16 Box_Names[BK_BOX_COUNT][17]; // 0x000-0xD47
	u8 Box_Backgrounds[BK_BOX_COUNT]; // 0xD48-0xDAB
	u8 _0xDAC[0x54]; // 0xDAC-0xDFF
};

struct __attribute__((packed)) BKExtra_s // 0x3A00
{
	struct __attribute__((packed)) {
		EK6_t Ek6[BOX_PKM_COUNT]; // 0x0000-0x1B2F
	} WonderBox; // 0x0000-0x1B2F
	struct __attribute__((packed)) {
		EK6_t Ek6[BOX_PKM_COUNT]; // 0x0000-0x1B2F
	} TrashBox; // 0x1B30-0x365F
	u8 _0x3660[0x3A0]; // 0x3660-0x39FF
};

struct __attribute__((packed)) BNK6_s // 0xAEA00
{
	u32 magic; // 0x00-0x03
	u32 version; // 0x04-0x07
	u8 _0x08[0xF8]; // 0x08-0xFF

	BK_s BK; // 0x00100-0xA9FBF
	BKLayout_s BKLayout; // 0xA9FC0-0xAADBF
	u8 _0xAADC0[0x40]; // 0xAADC0-0xAADFF

	BKExtra_s BKExtra; // 0xAAE00-0xAE7FF
	u8 _0xAE800[0x200]; // 0xAE800-0xAE9FF
};

// BNK6_HPP
