#include "save/bnk6.hpp"

#include <stdio.h>
#include <string.h>

extern "C" {
#include <3ds/util/utf.h>
}

BNK6::BNK6() : bnk(NULL) { }

BNK6::~BNK6()
{
	if (bnk) delete bnk;
}

void BNK6::loadData()
{
	printf("BK/BKLayout:\n");

	for (u8 iB = 0; iB < BK_BOX_COUNT; iB++)
	{
		Box_s* box = &this->box[iB];
		box->number = iB;
		box->background = this->bnk->BKLayout.Box_Backgrounds[iB];
		memset(box->title, 0, 17 * sizeof(uint32_t));
		utf16_to_utf32(box->title, this->bnk->BKLayout.Box_Names[iB], 17);
		for (u8 iP = 0; iP < BOX_PKM_COUNT; iP++)
		{
			// PK6::decryptEk6((u8*)&box->slot[iP].pk6, this->bnk->BK.Ek6[iB][iP]);
			memcpy((u8*)&box->slot[iP].pk6, this->bnk->BK.Ek6[iB][iP], PK6_SIZE);
			this->fillPkm(&box->slot[iP]);
		}
	}
	this->boxUnlocked = 100;

	for (u8 iP = 0; iP < BOX_PKM_COUNT; iP++)
	{
		// PK6::encryptPk6((u8*)&this->wbox.slot[iP].pk6, this->bnk->BKExtra.WonderBox.Ek6[iP]);
		memcpy((u8*)&this->wbox.slot[iP].pk6, this->bnk->BKExtra.WonderBox.Ek6[iP], PK6_SIZE);
		this->fillPkm(&this->wbox.slot[iP]);

		// PK6::encryptPk6((u8*)&this->tbox.slot[iP].pk6, this->bnk->BKExtra.TrashBox.Ek6[iP]);
		memcpy((u8*)&this->tbox.slot[iP].pk6, this->bnk->BKExtra.TrashBox.Ek6[iP], PK6_SIZE);	
		this->fillPkm(&this->tbox.slot[iP]);
	}

	this->wboxUnlocked = true;
	this->tboxUnlocked = true;
}

void BNK6::saveData()
{
	for (u8 iB = 0; iB < BK_BOX_COUNT; iB++)
	{
		Box_s* box = &this->box[iB];
		this->bnk->BKLayout.Box_Backgrounds[iB] = box->background;
		memset(this->bnk->BKLayout.Box_Names[iB], 0, 17 * sizeof(uint32_t));
		utf32_to_utf16(this->bnk->BKLayout.Box_Names[iB], box->title, 17);
		for (u8 iP = 0; iP < BOX_PKM_COUNT; iP++)
		{
			// PK6::encryptPk6(this->bnk->BK.Ek6[iB][iP], (u8*)&box->slot[iP].pk6);
			memcpy(this->bnk->BK.Ek6[iB][iP], (u8*)&box->slot[iP].pk6, PK6_SIZE);
		}
	}

	for (u8 iP = 0; iP < BOX_PKM_COUNT; iP++)
	{
		// PK6::encryptPk6(this->bnk->BKExtra.WonderBox.Ek6[iP], (u8*)&this->wbox.slot[iP].pk6);
		memcpy(this->bnk->BKExtra.WonderBox.Ek6[iP], (u8*)&this->wbox.slot[iP].pk6, PK6_SIZE);
		// PK6::encryptPk6(this->bnk->BKExtra.TrashBox.Ek6[iP], (u8*)&this->tbox.slot[iP].pk6);
		memcpy(this->bnk->BKExtra.TrashBox.Ek6[iP], (u8*)&this->tbox.slot[iP].pk6, PK6_SIZE);
	}
}

void BNK6::fillPkm(Pkm_s* pkm)
{
	pkm->fromBank = true;
	pkm->modified = false;
	pkm->moved = false;
	pkm->checked = false;
	pkm->isShiny = PK6::isShiny(&pkm->pk6);
}

Box_s* BNK6::getBox(u16 boxID)
{
	return &this->box[boxID];
}

Pkm_s* BNK6::getPkm(u16 slotID)
{
	return getPkm(slotID / BOX_PKM_COUNT, slotID % BOX_PKM_COUNT);
}

Pkm_s* BNK6::getPkm(u16 boxID, u16 slotID)
{
	return &this->box[boxID].slot[slotID];
}

Box_s* BNK6::getWBox()
{
	return &this->wbox;
}

Pkm_s* BNK6::getWPkm(u16 slotID)
{
	return &this->wbox.slot[slotID];
}

Box_s* BNK6::getTBox()
{
	return &this->tbox;
}

Pkm_s* BNK6::getTPkm(u16 slotID)
{
	return &this->tbox.slot[slotID];
}
