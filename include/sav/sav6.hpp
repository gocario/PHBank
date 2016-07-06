#pragma once
/**
 * @file sav/sav6.hpp
 * @author Gocario
 */

#include "pkm/pk6.hpp"

#define BOX_ROW_PKM_COUNT (6) // How much pkm per row in a box
#define BOX_COL_PKM_COUNT (5) // How much pkm per col in a box
#define BOX_PKM_COUNT (30) // How much pkm in a box
#define PC_BOX_COUNT (31) // How much pkm in a pc

#define PK6_SIZE (0xE8)

typedef u8 EK6_t[PK6_SIZE];
typedef u8 PK6_t[PK6_SIZE];

struct __attribute__((packed)) TrainerCard_s // 0x200
{
	u16 TID; // 0x00-0x01
	u16 SID; // 0x02-0x03
	u8 Game; // 0x04
	u8 Gender; // 0x05
	u8 _0x06; // 0x06
	u8 Sprite; // 0x07
	u64 GameSyncID; // 0x08-0x0F
	u8 _0x10[0x16]; // 0x10-0x25
	u8 SubRegion; // 0x26
	u8 Country; // 0x27
	u8 _0x28; // 0x28
	u8 _0x29; // 0x29
	u8 _0x2A; // 0x2A
	u8 _0x2B; // 0xB
	u8 ConsoleRegion; // 0x2C
	u8 Language; // 0x2D
	u8 _0x2E[0x1A]; // 0x2E-0x47
	u16 Name[13]; // 0x48-0x61
	u8 _0x62[0x1A]; // 0x62-0x7B
	u16 Sayings[5][17]; // 0x07C-0x125
	u8 _0x126[0xDA]; // 0x126-0x1FF
};

struct __attribute__((packed)) PCLayout_s // 0x600
{
	u16 Box_Names[PC_BOX_COUNT][17]; // 0x000-0x41D (0x4400-0x481D)
	u8 Box_Backgrounds[PC_BOX_COUNT]; // 0x41E-0x43C (0x481E-0x483C)
	u8 Box_Background_Flags[3]; // 0x43D-0x43F (0x483D-0x483F)
	u8 _0x440[0x1C0]; // 0x440-0x5FF (0x4840-0x49FF)
};

struct __attribute__((packed)) HallOfFamePokemon_s // 0x48
{
	u16 Species; // 0x00-0x01
	u16 HeldItem; // 0x02-0x03
	u16 Moves[4]; // 0x04-0x0B
	u32 EncryptionKey; // 0x0C-0x0F
	u16 TID; // 0x10-0x11
	u16 SID; // 0x12-0x13
	struct __attribute__((packed)) {
		bool IsNicknamed : 1;
		bool IsShiny : 1;
		u8 Level : 7;
		u8 Gender : 2;
		u8 AltForm : 5;
	}; // 0x14-0x15
	u8 _0x16; // 0x16
	u8 _0x17; // 0x17
	u16 Nickname[11]; // 0x18-0x2D
	u16 NicknameEnd; // 0x2E-0x2F
	u16 T_Name[11]; // 0x30-0x45
	u16 T_NameEnd; // 0x48-0x47
};

struct __attribute__((packed)) HallOfFameEntry_s // 0x1B4
{
	HallOfFamePokemon_s first[6]; // 0x000-0x1AF
	u8 NumberOfVictory; // 0x1B0
	struct __attribute__((packed)) {
		u8 _0x1B1_1 : 1;
		u8 Day : 5;
		u8 Month : 4;
		u8 Year : 8;
		u8 _0x1B3_1 : 6;
	}; // 0x1B1-0x1B3
};

struct __attribute__((packed)) HallOfFame_s // 0x1B40
{
	HallOfFameEntry_s first; // 0x000-0x1B3
	HallOfFameEntry_s latest[15]; // 0x01B4-0x1B3F
};

struct __attribute__((packed)) BattleMansion_s // 0x200
{
	struct __attribute__((packed)) {
		u16 CurrentStreak; // 0x00-0x01
		u16 CurrentSuperStreak; // 0x02-0x03
		u16 BestStreak; // 0x04-0x05
		u16 BestSuperStreak; // 0x06-0x07
	}
	Singles, // 0x00-0x07
	Doubles, // 0x08-0x0F
	Triples, // 0x10-0x17
	Rotation, // 0x18-0x1F
	Multi; // 0x20-0x27
	u8 _0x28[0x1D8]; // 0x28-0x1FF
};

struct __attribute__((packed)) PC_s // 0x34AD0
{
	EK6_t Ek6[PC_BOX_COUNT][BOX_PKM_COUNT]; // 0x00000-0x34ACF
};

// SAV6_HPP
