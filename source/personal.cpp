#include "personal.hpp"

#include <string.h>

static void PersonalInfo_Import(PersonalInfo* pInfo, const u8* buf)
{
	if (!buf) return;

	memcpy(pInfo, buf, sizeof(PersonalInfo));
	// pInfo->ExpGrowth = buf[0x15];
}

PersonalMaster::PersonalMaster(void) : personals(NULL)
{

}

PersonalMaster::~PersonalMaster(void)
{
	if (personals) delete[] personals;
}

void PersonalMaster::import(const u8* buffer, u32 pCount, u32 pSize)
{
	personalCount = pCount;
	personalSize = pSize;
	personals = new PersonalInfo[pCount];

	for (u32 i = 0; i < pCount; i++)
	{
		PersonalInfo_Import(&personals[i], buffer+pSize*i);
	}
}

const PersonalInfo& PersonalMaster::operator()(u16 species) const
{
	return personals[species];
}

const PersonalInfo& PersonalMaster::operator()(u16 species, u8 form) const
{
	return form == 0 || personals[species].FormStats == 0 ? personals[species] : personals[personals[species].FormStats + form - 1];
}

PersonalMaster Personal;
