#include "personal.hpp"

// #include <stdlib.h>

static void PersonalInfo_Import(PersonalInfo* pInfo, const u8* buf)
{
	if (!buf) return;

	pInfo->HP = buf[0x0];
	pInfo->ATK = buf[0x1];
	pInfo->DEF = buf[0x2];
	pInfo->SPE = buf[0x3];
	pInfo->SPA = buf[0x4];
	pInfo->SPD = buf[0x5];

	pInfo->types[0] = buf[0x6];
	pInfo->types[1] = buf[0x7];

	pInfo->items[0] = *(u16*)(buf+0xc);
	pInfo->items[1] = *(u16*)(buf+0xe);
	pInfo->items[2] = *(u16*)(buf+0x10);
	pInfo->gender = buf[0x12];
	pInfo->baseFriendship = buf[0x14];
	pInfo->expGrowth = buf[0x15];

	pInfo->formStats = *(u16*)(buf+0x1c);
	pInfo->formCount = buf[0x20];
	pInfo->color = buf[0x21];

	// memcpy(pInfo, buf, sizeof(PersonalInfo));
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
	personalSize = pSize;
	personals = new PersonalInfo[pCount];
	for (u16 i = 0; i < pCount; i++)
	{
		PersonalInfo_Import(&personals[i], (u8*)(buffer+pSize*i));
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
