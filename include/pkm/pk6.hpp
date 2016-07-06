#pragma once
/**
 * @file pkm/pk6.hpp
 * @author Gocario
 */

#include <3ds/types.h>

struct __attribute__((packed)) PK6_s // 0xE8
{
	/*********\
	| Block A |
	\*********/
	u32 EncryptionKey; // 0x00-0x03
	u16 Sanity; // 0x04-0x005
	u16 Checksum; // 0x06-0x07
	u16 Species; // 0x08-0x09
	u16 HeldItem; // 0x0A-0x0B
	u16 TID; // 0x0C-0x0D
	u16 SID; // 0x0D-0x0F
	u32 EXP; // 0x10-0x13
	u8 Ability; // 0x14x
	u8 AbilityNumber; // 0x15
	u8 TrainingBagHits; // 0x16
	u8 TrainingBag; // 0x17
	u32 PID; // 0x18-0x1B
	u8 Nature; // 0x1C
	struct __attribute__((packed)) {
		bool FatefulEncounter : 1; // 0x1D
		u8 Gender : 2; // 0x1D
		u8 AltForm : 5; // 0x1D
	};
	u8 EV_HP; // 0x1E
	u8 EV_ATK; // 0x1F
	u8 EV_DEF; // 0x20
	u8 EV_SPE; // 0x21
	u8 EV_SPA; // 0x22
	u8 EV_SPD; // 0x23
	u8 CNT_Cool; // 0x24
	u8 CNT_Beauty; // 0x25
	u8 CNT_Cute; // 0x26
	u8 CNT_Smart; // 0x27
	u8 CNT_Tough; // 0x28
	u8 CNT_Sheen; // 0x29
	union {
		u8 MarkByte; // 0x2A
		struct __attribute__((packed)) {
			bool Circle : 1;
			bool Triangle : 1;
			bool Square : 1;
			bool Heart : 1;
			bool Star : 1;
			bool Diamond : 1;
			bool _0x2A_1 : 1;
			bool _0x2A_0 : 1;
		};
	};
	union {
		u8 PKRS; // 0x2B
		struct __attribute__((packed)) {
			u8 PKRS_Days : 4;
			u8 PKRS_Strain : 4;
		};
	};
	union {
		u8 ST1; // 0x2C
		struct __attribute__((packed)) {
			bool _0x2C_0 : 1;
			bool _0x2C_1 : 1;
			bool ST1_SPA : 1;
			bool ST1_HP : 1;
			bool ST1_ATK : 1;
			bool ST1_SPD : 1;
			bool ST1_SPE : 1;
			bool ST1_DEF : 1;
		};
	};
	union {
		u8 ST2; // 0x2D
		struct __attribute__((packed)) {
			bool ST2_SPA : 1;
			bool ST2_HP : 1;
			bool ST2_ATK : 1;
			bool ST2_SPD : 1;
			bool ST2_SPE : 1;
			bool ST2_DEF : 1;
			bool ST3_SPA : 1;
			bool ST3_HP : 1;
		};
	};
	union {
		u8 ST3; // 0x2E
		struct __attribute__((packed)) {
			bool ST3_ATK : 1;
			bool ST3_SPD : 1;
			bool ST3_SPE : 1;
			bool ST3_DEF : 1;
			bool ST4_1 : 1;
			bool ST5_1 : 1;
			bool ST5_2 : 1;
			bool ST5_3 : 1;
		};
	};
	union {
		u8 ST4; // 0x2F
		struct __attribute__((packed)) {
			bool ST5_4 : 1;
			bool ST6_1 : 1;
			bool ST6_2 : 1;
			bool ST6_3 : 1;
			bool ST7_1 : 1;
			bool ST7_2 : 1;
			bool ST7_3 : 1;
			bool ST8_1 : 1;
		};
	};
	union {
		u8 RIB0; // 0x30
		struct __attribute__((packed)) {
			bool RIB0_0 : 1;
			bool RIB0_1 : 1;
			bool RIB0_2 : 1;
			bool RIB0_3 : 1;
			bool RIB0_4 : 1;
			bool RIB0_5 : 1;
			bool RIB0_6 : 1;
			bool RIB0_7 : 1;
		};
	};
	union {
		u8 RIB1; // 0x31
		struct __attribute__((packed)) {
			bool RIB1_0 : 1;
			bool RIB1_1 : 1;
			bool RIB1_2 : 1;
			bool RIB1_3 : 1;
			bool RIB1_4 : 1;
			bool RIB1_5 : 1;
			bool RIB1_6 : 1;
			bool RIB1_7 : 1;
		};
	};
	union {
		u8 RIB2; // 0x32
		struct __attribute__((packed)) {
			bool RIB2_0 : 1;
			bool RIB2_1 : 1;
			bool RIB2_2 : 1;
			bool RIB2_3 : 1;
			bool RIB2_4 : 1;
			bool RIB2_5 : 1;
			bool RIB2_6 : 1;
			bool RIB2_7 : 1;
		};
	};
	union {
		u8 RIB3; // 0x33
		struct __attribute__((packed)) {
			bool RIB3_0 : 1;
			bool RIB3_1 : 1;
			bool RIB3_2 : 1;
			bool RIB3_3 : 1;
			bool RIB3_4 : 1;
			bool RIB3_5 : 1;
			bool RIB3_6 : 1;
			bool RIB3_7 : 1;
		};
	};
	union {
		u8 RIB4; // 0x34
		struct __attribute__((packed)) {
			bool RIB4_0 : 1;
			bool RIB4_1 : 1;
			bool RIB4_2 : 1;
			bool RIB4_3 : 1;
			bool RIB4_4 : 1;
			bool RIB4_5 : 1;
			bool RIB4_6 : 1;
			bool RIB4_7 : 1;
		};
	};
	union {
		u8 RIB5; // 0x35
		struct __attribute__((packed)) {
			bool RIB5_0 : 1;
			bool RIB5_1 : 1;
			bool RIB5_2 : 1;
			bool RIB5_3 : 1;
			bool RIB5_4 : 1;
			bool RIB5_5 : 1;
			bool RIB5_6 : 1;
			bool RIB5_7 : 1;
		};
	};
	u8 _0x36; // 0x36
	u8 _0x37; // 0x37
	u8 Memory_ContestCount; // 0x38
	u8 Memory_BattleCount; // 0x39
	union {
		u8 Dist; // 0x3A
		struct __attribute__((packed)) {
			bool Dist1 : 1;
			bool Dist2 : 1;
			bool Dist3 : 1;
			bool Dist4 : 1;
			bool Dist5 : 1;
			bool Dist6 : 1;
			bool Dist7 : 1;
			bool Dist8 : 1;
		};
	};
	u8 _0x3B; // 0x3B
	u8 _0x3C; // 0x3C
	u8 _0x3D; // 0x3D
	u8 _0x3E; // 0x3E
	u8 _0x3F; // 0x3F

	/*********\
	| Block B |
	\*********/
	u16 Nickname[13]; // 0x40-0X59
	u16 Moves[4]; // 0x5A-0X61
	u8 Moves_PP[4]; // 0x62-0x65
	u8 Moves_PPUps[4]; // 0x66-0x69
	u16 RelearnMoves[4]; // 0x6A-0x71
	struct __attribute__((packed)) {
		bool SecretSuperTraining : 1; // 0x72
		u8 _0x72_0 : 7; // 0x72
	};
	u8 _0x73; // 0x73
	union {
		u32 IV32; // 0x74-0x77
		struct __attribute__((packed)) {
			u8 IV_HP : 5; // 0x00-0x04
			u8 IV_ATK : 5; // 0x05-0x09
			u8 IV_DEF : 5; // 0x0A-0x0E
			u8 IV_SPE : 5; // 0x0F-0x13
			u8 IV_SPA : 5; // 0x14-0x18
			u8 IV_SPD : 5; // 0x19-0x1D
			bool IsEgg : 1; // 0x1E
			bool IsNicknamed : 1; // 0xF
		};
	};

	/*********\
	| Block C |
	\*********/
	u16 HT_Name[13]; // 0x78-0x91
	u8 HT_Gender; // 0x92
	u8 CurrentHandler; // 0x93
	struct __attribute__((packed)) {
		u8 Region;
		u8 Country;
	} Geos[5]; // 0x94-0x9D
	u8 _0x9E; // 0x9E
	u8 _0x9F; // 0x9F
	u8 _0xA0; // 0xA0
	u8 _0xA1; // 0xA1
	u8 HT_Friendship; // 0xA2
	u8 HT_Affection; // 0xA3
	u8 HT_Intensity; // 0xA4
	u8 HT_Memory; // 0xA5
	u8 HT_Feeling; // 0xA6
	u8 _0xA7; // 0xA7
	u16 HT_TextVar; // 0xA8-0xA9
	u8 _0xAA; // 0xAA
	u8 _0xAB; // 0xAB
	u8 _0xAC; // 0xAC
	u8 _0xAD; // 0xAD
	u8 Fullness; // 0xAE
	u8 Enjoyement; // 0xAF

	/*********\
	| Block D |
	\*********/
	u16 OT_Name[13]; // 0xB0-0xC9
	u8 OT_Friendship; // 0xCA
	u8 OT_Affection; // 0xCB
	u8 OT_Intensity; // 0xCC
	u8 OT_Memory; // 0xCD
	u16 OT_TextVar; // 0xCE-0xCF
	u8 OT_Feeling; // 0xD0
	u8 Egg_Year; // 0xD1
	u8 Egg_Month; // 0xD2
	u8 Egg_Day; // 0xD3
	u8 Met_Year; // 0xD4
	u8 Met_Month; // 0xD5
	u8 Met_Day; // 0xD6
	u8 _0xD7; // 0xD7
	u16 Egg_Location; // 0xD8-0xD9
	u16 Met_Location; // 0xDA-0xDB
	u8 Ball; // 0xDC
	struct __attribute__((packed)) {
		u8 Met_Level : 7; // 0xDD
		u8 OT_Gender : 1; // 0xDD
	};
	u8 EncounterType; // 0xDE
	u8 Version; // 0xDF
	u8 Country; // 0xE0
	u8 Region; // 0xE1
	u8 ConsoleRegion; // 0xE2
	u8 Language; // 0xE3
	u8 _0xE4; // 0xE4
	u8 _0xE5; // 0xE5
	u8 _0xE6; // 0xE6
	u8 _0xE7; // 0xE7
};

struct BattlePK6_s // 0x104
{
	PK6_s pk6;
	
	/**************\
	| Battle Block |
	\**************/
	u8 _0xE8; // 0xE8
	u8 _0xE9; // 0xE9
	u8 _0xEA; // 0xEA
	u8 _0xEB; // 0xEB
	u8 Stat_Level; // 0xEC
	u8 _0xED; // 0xED
	u8 _0xEE; // 0xEE
	u8 _0xEF; // 0xEF
	u16 Stat_HPCurrent; // 0xF0-0xF1
	u16 Stat_HPMax; // 0xF2-0xF3
	u16 Stat_ATK; // 0xF4-0xF5
	u16 Stat_DEF; // 0xF6-0xF7
	u16 Stat_SPE; // 0xF8-0xF9
	u16 Stat_SPA; // 0xFA-0xFB
	u16 Stat_SPD; // 0xFC-0xFD
	u8 _0xFE; // 0xFE
	u8 _0xFF; // 0xFF
	u8 _0x100; // 0x100
	u8 _0x101; // 0x101
	u8 _0x102; // 0x102
	u8 _0x103; // 0x103
};

// PK6_HPP