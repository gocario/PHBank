#pragma once
/**
 * @file save/pk6.hpp
 * @author Gocario
 */

#include "pkm/pk6.hpp"

namespace Stat
{
	enum
	{
		HP = 0,
		ATK = 1,
		DEF = 2,
		SPE = 3,
		SPA = 4,
		SPD = 5,
		Count = 6,
	};
}

class PK6
{
	public:
		static u16 getPSV(u32 PID);
		static u16 getTSV(u16 TID, u16 SID);
		static bool isShiny(u16 TSV, u16 PSV);
		static bool isShiny(const PK6_s* pk6);

		static u8 level(const PK6_s* pk6);
		static u16 stat(u8 stat, u16 species, u8 form, u8 IV, u8 EV, u8 nature, u8 level);
		static u16 HP(const PK6_s* pk6, u8 level = 0);
		static u16 ATK(const PK6_s* pk6, u8 level = 0);
		static u16 DEF(const PK6_s* pk6, u8 level = 0);
		static u16 SPA(const PK6_s* pk6, u8 level = 0);
		static u16 SPD(const PK6_s* pk6, u8 level = 0);
		static u16 SPE(const PK6_s* pk6, u8 level = 0);
		static void stats(u16* values, const PK6_s* pk6, u8 level = 0);

		static u8 HPType(const PK6_s* pk6);

		static bool isKalosBorn(const PK6_s* pk6);
		static bool isGen6Born(const PK6_s* pk6);
		static bool isGen5Born(const PK6_s* pk6);
		static bool isGen4Born(const PK6_s* pk6);
		static bool isGen3Born(const PK6_s* pk6);
		static bool generation(const PK6_s* pk6);

		static bool isInfected(const PK6_s* pk6);
		static bool isCured(const PK6_s* pk6);

	public:
		static void computeChecksum(u8* pk6);

		static u32 LCRNG(u32 seed);
		static void shufflePk6(u8* pk6, u8 sv);
		static void decryptEk6(u8* pk6, const u8* ek6);
		static void encryptPk6(u8* ek6, const u8* pk6);
};

// PK6_HPP
