#pragma once

#include <3ds/types.h>

#define PERSONAL_XY_COUNT (799)
#define PERSONAL_AO_COUNT (826)
#define PERSONAL_INFO_XY_SIZE (0x40)
#define PERSONAL_INFO_AO_SIZE (0x50)

struct PersonalInfo
{
	uint8_t HP;
	uint8_t ATK;
	uint8_t DEF;
	uint8_t SPE;
	uint8_t SPA;
	uint8_t SPD;
	uint8_t types[2];
	uint16_t items[3];
	uint8_t gender;
	uint8_t baseFriendship;
	uint8_t expGrowth;
	uint16_t formStats;
	uint8_t formCount;
	uint8_t color;
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
