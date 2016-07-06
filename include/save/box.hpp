#pragma once
/**
 * @file save/box.hpp
 * @author Gocario
 */

#include "sav/sav6.hpp"

struct __attribute__((packed)) Pkm_s
{
	PK6_s pk6;
	bool fromBank: 1;
	bool modified: 1;
	bool moved : 1;
	bool checked : 1;
	bool isShiny : 1;
	unsigned: 3;
};

struct __attribute__((packed)) Box_s
{
	Pkm_s slot[BOX_PKM_COUNT];
	uint32_t title[17];
	u8 background;
	u8 number;
};

// BOX_HPP
