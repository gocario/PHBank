#include "save/sav6.hpp"
#include "personal.hpp"
#include "pokedex.hpp"

#include "utils.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include <3ds/util/utf.h>
}


SAV6::SAV6() : savXY(NULL), savORAS(NULL) { }

SAV6::~SAV6()
{
	if (savXY) delete savXY;
	if (savORAS) delete savORAS;
}

u16 SAV6::getTSV(u16 TID, u16 SID)
{
	return (TID ^ SID) >> 4;
}

void SAV6::loadData()
{
	PC_s* PC;
	PCLayout_s* PCLayout;
	TrainerCard_s* TrainerCard;

	if (Game::is(version, Game::XY))
	{
		PC = &savXY->PC;
		TrainerCard = &savXY->TrainerCard;
		PCLayout = &savXY->PCLayout;
	}
	else if (Game::is(version, Game::ORAS))
	{
		PC = &savORAS->PC;
		TrainerCard = &savORAS->TrainerCard;
		PCLayout = &savORAS->PCLayout;
	}
	else return;

	printf("Trainer Card:\n");

	// Trainer Card
	this->TID = TrainerCard->TID;
	this->SID = TrainerCard->SID;
	this->TSV = SAV6::getTSV(this->TID, this->SID);
	memcpy(this->T_Name, TrainerCard->Name, 13 * sizeof(u16));
	memset(this->wT_Name, 0, 13 * sizeof(uint32_t));
	utf16_to_utf32(this->wT_Name, TrainerCard->Name, 13);
	this->T_Gender = TrainerCard->Gender;
	this->GeoRegion = TrainerCard->SubRegion;
	this->GeoCountry = TrainerCard->Country;

	printf("PC/PCLayout:\n");

	// PC
	for (u8 iB = 0; iB < PC_BOX_COUNT; iB++)
	{
		Box_s* box = &this->box[iB];
		box->number = iB;
		box->background = PCLayout->Box_Backgrounds[iB];
		memset(box->title, 0, 17 * sizeof(uint32_t));
		utf16_to_utf32(box->title, PCLayout->Box_Names[iB], 17);
		for (u8 iP = 0; iP < BOX_PKM_COUNT; iP++)
		{
			PK6::decryptEk6((u8*)&box->slot[iP].pk6, PC->Ek6[iB][iP]);
			this->fillPkm(&box->slot[iP]);
		}
	}
	this->boxUnlocked = 31;
}

void SAV6::saveData()
{
	PC_s* PC;
	PCLayout_s* PCLayout;

	if (Game::is(version, Game::XY))
	{
		PC = &savXY->PC;
		PCLayout = &savXY->PCLayout;
	}
	else if (Game::is(version, Game::ORAS))
	{
		PC = &savORAS->PC;
		PCLayout = &savORAS->PCLayout;
	}
	else return;

	for (u8 iB = 0; iB < PC_BOX_COUNT; iB++)
	{
		Box_s* box = &this->box[iB];
		PCLayout->Box_Backgrounds[iB] = box->background;
		memset(PCLayout->Box_Names[iB], 0, 17 * sizeof(uint32_t));
		utf32_to_utf16(PCLayout->Box_Names[iB], box->title, 17);
		for (u8 iP = 0; iP < BOX_PKM_COUNT; iP++)
		{
			PK6::encryptPk6(PC->Ek6[iB][iP], (u8*)&box->slot[iP].pk6);
		}
	}
}

void SAV6::fillPkm(Pkm_s* pkm)
{
	pkm->fromBank = false;
	pkm->modified = false;
	pkm->moved = false;
	pkm->checked = false;
	pkm->isShiny = PK6::isShiny(&pkm->pk6);
}

Box_s* SAV6::getBox(u16 boxID)
{
	return &this->box[boxID];
}

Pkm_s* SAV6::getPkm(u16 slotID)
{
	return getPkm(slotID / BOX_PKM_COUNT, slotID % BOX_PKM_COUNT);
}

Pkm_s* SAV6::getPkm(u16 boxID, u16 slotID)
{
	return &this->box[boxID].slot[slotID];
}

void SAV6::addDexEntry(const PK6_s* pk6)
{
	if (Game::is(version, Game::XY))
		Pokedex::importToGame(version, (u8*)this->savXY, pk6);
	else if (Game::is(version, Game::ORAS))
		Pokedex::importToGame(version, (u8*)this->savORAS, pk6);
}

bool SAV6::checkOT(const PK6_s* pk6)
{
	return (pk6->TID == this->TID) &&
			(pk6->SID == this->SID) &&
			(pk6->OT_Gender == this->T_Gender) &&
			(str16ncmp(pk6->OT_Name, this->T_Name, 0x18) == 0);
}

bool SAV6::tradePkm(PK6_s* pk6)
{
	bool modified = false;

	if (pk6->IsEgg)
	{
		// If it's not going back to OT
		if (!checkOT(pk6))
		{
			pk6->Met_Location = 0x7532; // a Link Trade (Egg)
		}

		time_t t_time = time(NULL);
		struct tm* tm_time = localtime(&t_time);

		pk6->Met_Year = tm_time->tm_year-100;
		pk6->Met_Month = tm_time->tm_mon+1;
		pk6->Met_Day = tm_time->tm_mday;

		modified |= true;
	}
	else
	{
		// If it's going back to OT
		if (checkOT(pk6))
		{
			// If it's coming back from a non-OT
			if (pk6->CurrentHandler == 0x01)
			{
				pk6->CurrentHandler = 0x00;
				modified |= true;
			}
		}
		// If it's going to a non-OT
		else
		{
			// If it is from OT
			if (pk6->CurrentHandler == 0x00)
			{
				pk6->CurrentHandler = 0x01;
				modified |= tradePkmHT(pk6);
			}
			// If it is from a non-OT
			else
			{
				// If it is not the "same non-OT"
				if (str16ncmp(pk6->HT_Name, this->T_Name, 0x18) != 0)
				{
					modified |= tradePkmHT(pk6);
				}
			}
		}
	}

	if (modified)
	{
		PK6::computeChecksum((u8*) pk6);
	}

	return modified;
}

bool SAV6::tradePkmHT(PK6_s* pk6)
{
	// Save::OTName to Pkmn::HTName
	memcpy(pk6->HT_Name, this->T_Name, 0x18);
	pk6->HT_Gender = this->T_Gender;
	pk6->CurrentHandler = 0x01;
	pk6->Geos[4] = pk6->Geos[3];
	pk6->Geos[3] = pk6->Geos[2];
	pk6->Geos[2] = pk6->Geos[1];
	pk6->Geos[1] = pk6->Geos[0];
	pk6->Geos[0].Region = this->GeoRegion;
	pk6->Geos[0].Country = this->GeoCountry;
	pk6->HT_Friendship = Personal(pk6->Species).BaseFriendship;
	pk6->HT_Affection = 0x00;
	pk6->HT_Intensity = 0x01;
	pk6->HT_Memory = 0x04;
	pk6->HT_Feeling = rand() % 0xA;
	pk6->HT_TextVar = 0x0000;
	// pk6->fullness = pk6->fullness;
	// pk6->enjoyment = pk6->enjoyment;

	return true;
}

u16 SAV6::ccitt16(const u8* data, u32 len)
{
	u16 crc = 0xFFFF;

	for (u32 i = 0; i < len; i++)
	{
		crc ^= (u16) (data[i] << 8);

		for (u32 j = 0; j < 0x8; j++)
		{
			if ((crc & 0x8000) > 0)
				crc = (u16) ((crc << 1) ^ 0x1021);
			else
				crc <<= 1;
		}
	}

	return crc;
}

u32 SAV6::CHKOffset(u32 i)
{
	if (Game::is(version, Game::XY))
	{
		const u32 _xy[] = { 0x05400, 0x05800, 0x06400, 0x06600, 0x06800, 0x06A00, 0x06C00, 0x06E00, 0x07000, 0x07200, 0x07400, 0x09600, 0x09800, 0x09E00, 0x0A400, 0x0F400, 0x14400, 0x19400, 0x19600, 0x19E00, 0x1A400, 0x1AC00, 0x1B400, 0x1B600, 0x1B800, 0x1BE00, 0x1C000, 0x1C400, 0x1CC00, 0x1CE00, 0x1D000, 0x1D200, 0x1D400, 0x1D600, 0x1DE00, 0x1E400, 0x1E800, 0x20400, 0x20600, 0x20800, 0x20C00, 0x21000, 0x22C00, 0x23000, 0x23800, 0x23C00, 0x24600, 0x24A00, 0x25200, 0x26000, 0x26200, 0x26400, 0x27200, 0x27A00, 0x5C600, };

		return _xy[i] - 0x5400;
	}
	else if (Game::is(version, Game::ORAS))
	{
		const u32 _oras[] = { 0x05400, 0x05800, 0x06400, 0x06600, 0x06800, 0x06A00, 0x06C00, 0x06E00, 0x07000, 0x07200, 0x07400, 0x09600, 0x09800, 0x09E00, 0x0A400, 0x0F400, 0x14400, 0x19400, 0x19600, 0x19E00, 0x1A400, 0x1B600, 0x1BE00, 0x1C000, 0x1C200, 0x1C800, 0x1CA00, 0x1CE00, 0x1D600, 0x1D800, 0x1DA00, 0x1DC00, 0x1DE00, 0x1E000, 0x1E800, 0x1EE00, 0x1F200, 0x20E00, 0x21000, 0x21400, 0x21800, 0x22000, 0x23C00, 0x24000, 0x24800, 0x24C00, 0x25600, 0x25A00, 0x26200, 0x27000, 0x27200, 0x27400, 0x28200, 0x28A00, 0x28E00, 0x30A00, 0x38400, 0x6D000, };

		return _oras[i] - 0x5400;
	}
	else
	{
		return 0;
	}
}

u32 SAV6::CHKLength(u32 i)
{
	if (Game::is(version, Game::XY))
	{
		const u32 _xy[] = { 0x002C8, 0x00B88, 0x0002C, 0x00038, 0x00150, 0x00004, 0x00008, 0x001C0, 0x000BE, 0x00024, 0x02100, 0x00140, 0x00440, 0x00574, 0x04E28, 0x04E28, 0x04E28, 0x00170, 0x0061C, 0x00504, 0x006A0, 0x00644, 0x00104, 0x00004, 0x00420, 0x00064, 0x003F0, 0x0070C, 0x00180, 0x00004, 0x0000C, 0x00048, 0x00054, 0x00644, 0x005C8, 0x002F8, 0x01B40, 0x001F4, 0x001F0, 0x00216, 0x00390, 0x01A90, 0x00308, 0x00618, 0x0025C, 0x00834, 0x00318, 0x007D0, 0x00C48, 0x00078, 0x00200, 0x00C84, 0x00628, 0x34AD0, 0x0E058, };

		return _xy[i];
	}
	else if (Game::is(version, Game::ORAS))
	{
		const u32 _oras[] = { 0x002C8, 0x00B90, 0x0002C, 0x00038, 0x00150, 0x00004, 0x00008, 0x001C0, 0x000BE, 0x00024, 0x02100, 0x00130, 0x00440, 0x00574, 0x04E28, 0x04E28, 0x04E28, 0x00170, 0x0061C, 0x00504, 0x011CC, 0x00644, 0x00104, 0x00004, 0x00420, 0x00064, 0x003F0, 0x0070C, 0x00180, 0x00004, 0x0000C, 0x00048, 0x00054, 0x00644, 0x005C8, 0x002F8, 0x01B40, 0x001F4, 0x003E0, 0x00216, 0x00640, 0x01A90, 0x00400, 0x00618, 0x0025C, 0x00834, 0x00318, 0x007D0, 0x00C48, 0x00078, 0x00200, 0x00C84, 0x00628, 0x00400, 0x07AD0, 0x078B0, 0x34AD0, 0x0E058, };

		return _oras[i];
	}
	else
	{
		return 0;
	}
}

void SAV6::rewriteCHK()
{
	if (Game::is(version, Game::Unknown)) return;

	u8* save = NULL;
	u8 blockCount = 0;
	u32 csoff = 0;

	if (Game::is(version, Game::XY))
	{
		save = (u8*)this->savXY;
		blockCount = 55;
		csoff = 0x6A81A - 0x5400;
	}
	else if (Game::is(version, Game::ORAS))
	{
		save = (u8*)this->savORAS;
		blockCount = 58;
		csoff = 0x7B21A - 0x5400;
	}

	u16 cs;
	u8* tmp = new u8[0x35000];
	if (!tmp) return;

	for (u32 i = 0; i < blockCount; i++)
	{
		memcpy(tmp, save + CHKOffset(i), CHKLength(i));
		cs = ccitt16(tmp, CHKLength(i));
		memcpy(save + csoff + i * 8, &cs, 2);
	}

	delete[] tmp;
}
