#pragma once
/**
 * @file personal.hpp
 * @author Gocario
 */

#include <3ds/types.h>

#define PERSONAL_XY_COUNT (799)
#define PERSONAL_AO_COUNT (826)
#define PERSONAL_INFO_XY_SIZE (0x40)
#define PERSONAL_INFO_AO_SIZE (0x50)

struct __attribute__((packed)) PersonalInfo
{
	uint8_t HP;					// 0x00
	uint8_t ATK;				// 0x01
	uint8_t DEF;				// 0x02
	uint8_t SPE;				// 0x03
	uint8_t SPA;				// 0x04
	uint8_t SPD;				// 0x05
	uint8_t Types[2];			// 0x06-0x07
	uint8_t CatchRate;			// 0x08
	uint8_t EvoStage;			// 0x09
	union {
		uint16_t EVs;			// 0x0A-0x0B
		struct __attribute__((packed)) {
			uint8_t EV_HP : 2;
			uint8_t EV_ATK : 2;
			uint8_t EV_DEF : 2;
			uint8_t EV_SPE : 2;
			uint8_t EV_SPA : 2;
			uint8_t EV_SPD : 2;
			unsigned : 4;
		};
	};
	uint16_t Items[3];			// 0x0C-11
	uint8_t Gender;				// 0x12
	uint8_t HatchCycles;		// 0x13
	uint8_t BaseFriendship;		// 0x14
	uint8_t ExpGrowth;			// 0x15
	uint8_t EggGroups[2];		// 0x16-0x17
	uint8_t Abilities[3];		// 0x18-0x1A
	uint8_t EscapeRate;			// 0x1B
	uint16_t FormStats;			// 0x1C-0x1D
	uint16_t FormSprite;		// 0x1E-0x1F
	uint8_t FormCount;			// 0x20
	uint8_t Color;				// 0x21
	uint16_t BaseEXP;			// 0x22-0x23
	uint16_t Height;			// 0x24-0x25
	uint16_t Width;				// 0x26-0x27

	// TODO: Move TMH/Tutor
};

class PersonalMaster
{
	private:
		u32 personalCount;
		u32 personalSize;
		PersonalInfo* personals;

	public:
		PersonalMaster(void);
		~PersonalMaster(void);
		void import(const u8* buffer, u32 personalCount, u32 personalSize);

		const PersonalInfo& operator()(u16 species) const;
		const PersonalInfo& operator()(u16 species, u8 form) const;
};

extern PersonalMaster Personal;

// PERSONAL_HPP
