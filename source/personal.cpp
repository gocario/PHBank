#include "personal.hpp"

#include <stdlib.h>

PersonalInfo::PersonalInfo(void)
{

}

void PersonalInfo::import(const u8* buf)
{
	if (!buf) return;

	HP = buf[0x0];
	ATK = buf[0x1];
	DEF = buf[0x2];
	SPE = buf[0x3];
	SPA = buf[0x4];
	SPD = buf[0x5];
	BST = HP + ATK + DEF + SPE + SPA + SPD;

	types[0] = buf[0x6];
	types[1] = buf[0x7];

	items[0] = *(u16*)(buf+0xc);
	items[1] = *(u16*)(buf+0xe);
	items[2] = *(u16*)(buf+0x10);
	gender = buf[0x12];
	baseFriendship = buf[0x14];
	expGrowth = buf[0x15];

	formStats = *(u16*)(buf+0x1c);
	formSprite = *(u16*)(buf+0x1e);
	formCount = buf[0x20];
	color = buf[0x21];
}


PersonalMaster::PersonalMaster(void) : personals(NULL)
{

}

PersonalMaster::~PersonalMaster(void)
{
	delete[] personals;
}

void PersonalMaster::import(const u8* buffer, u32 pCount, u32 pSize)
{
	personalCount = pCount;
	personals = new PersonalInfo[personalCount];
	for (u16 i = 0; i < personalCount; i++)
	{
		personals[i].import((u8*)(buffer+pSize*i));
	}
}

const PersonalInfo& PersonalMaster::operator()(u16 species) const
{
	return personals[species];
}

const PersonalInfo& PersonalMaster::operator()(u16 species, u8 form) const
{
	return form == 0 || personals[species].formStats == 0 ? personals[species] : personals[personals[species].formStats + form - 1];
}

PersonalMaster Personal;
