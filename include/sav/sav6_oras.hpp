#pragma once
/**
 * @file sav/sav6_oras.hpp
 * @author Gocario
 */

#include "sav/sav6.hpp"

struct __attribute__((packed)) Pokedex_ORAS_s // 0x1C00
{
	u32 MagicConstant; // 0x00-0x03
	u32 Counter; // 0x04-0x07
	u8 Owned[0x60]; // 0x08-0x67
	u8 EncounteredMale[0x60]; // 0x68-0xC7
	u8 EncounteredFemale[0x60]; // 0x0C8-0x127
	u8 EncounteredShinyMale[0x60]; // 0x128-0x187
	u8 EncounteredShinyFemale[0x60]; // 0x188-0x1E7
	u8 DisplayedMale[0x60]; // 0x1E8-0x247
	u8 DisplayedFemale[0x60]; // 0x248-0x2A7
	u8 DisplayedShinyMale[0x60]; // 0x2A8-0x307
	u8 DisplayedShinyFemale[0x60]; // 0x308-0x367
	u8 EncounteredForm[0x26]; // 0x368-0x38D
	u8 EncounteredShinyForm[0x26]; // 0x38E-0x3B3
	u8 DisplayedForm[0x26]; // 0x3B4-0x3D9
	u8 DisplayedShinyForm[0x26]; // 0x3DA-0x3FF
	u8 LanguageFlag[0x27C]; // 0x400-0x67B
	u8 _0x67C[0x4]; // 0x67C-0x67F
	u32 Spinda; // 0x680-0x683
	u8 _0x684[0x2]; // 0x684-0x685
	u16 EncounterCounts[0x300]; // 0x686-0xC85
	u8 _0xC86[0x57A]; // 0xC86-0x11FF
};

struct __attribute__((packed)) DayCare_ORAS_s // 0x200
{
	struct __attribute__((packed)) {
		u32 OccupiedFlag; // 0x00-0x03
		u32 StepsCount; // 0x04-0x07
		EK6_t Ek6; // 0x08-0xEF
	}
	Slot[2]; // 0x000-0x1DF
	u64 EggFlag; // 0x1E0-0x1E7
	u64 RNG_Seed; // 0x1E8-0x1EF
	u8 _0x1F0[0x10]; // 0x1F0-0x1FF
};

struct __attribute__((packed)) SAV6_ORAS_s // 0x76000
{
	u8 _0x00000[0x4400]; // 0x0000-0x43FF

	PCLayout_s PCLayout; // 0x4400-0x49FF
	u8 _0x04A00[0xF600]; // 0x4A00-0x13FFF
	TrainerCard_s TrainerCard; // 0x14000-0x141FF
	u8 _0x14200[0xE00]; // 0x14200-0x15000
	Pokedex_ORAS_s Pokedex; // 0x15000-0x161FF
	u8 _0x16200[0x3C00]; // 0x16200-0x19DFF
	HallOfFame_s HallOfFame; // 0x19E00-0x1B93F
	u8 _0x1B940[0xC0]; // 0x1B940-0x1B9FF
	BattleMansion_s BattleMansion; // 0x1BA00-0x1BBFF
	DayCare_ORAS_s DayCare; // 0x1BC00-0x1BDFF
	u8 _0x1BE00[0x200]; // 0x1BE00-0x1BFFF

	u8 _0x1C000[0x17000]; // 0x1C000-0x32FFF

	PC_s PC; // 0x33000-0x67ACF

	u8 _0x67AD0[0xE530]; // 0x67AD0-0x75FFF
};

// SAV6_ORAS_HPP