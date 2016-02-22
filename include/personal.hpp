#pragma once

#include <3ds/types.h>

#define PERSONAL_XY_COUNT (799)
#define PERSONAL_AO_COUNT (826)
#define PERSONAL_INFO_XY_SIZE (0x40)
#define PERSONAL_INFO_AO_SIZE (0x50)

class PersonalInfo
{
	public:
		u8 HP, ATK, DEF, SPE, SPA, SPD;
		u16 BST;
		u8 types[2];
		u16 items[3];
		u8 gender, baseFriendship, expGrowth;
		u16 formStats, formSprite;
		u8 formCount, color;

	public:
		PersonalInfo(void);
		void import(const u8* buffer);
};

class PersonalMaster
{
	private:
		u16 personalCount;
		PersonalInfo* personals;

	public:
		PersonalMaster(void);
		~PersonalMaster(void);
		void import(const u8* buffer, u32 personalCount, u32 personalSize);

		const PersonalInfo& operator()(u16 species) const;
		const PersonalInfo& operator()(u16 species, u8 form) const;
};

extern PersonalMaster Personal;
