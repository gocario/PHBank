#include "save_manager.hpp"
#include "filter.hpp"

#include <string.h>

SaveManager::SaveManager() { }
SaveManager::~SaveManager() { }

Box_s* SaveManager::getBox(u16 boxID, bool inBank)
{
	if (inBank)
		if (boxID == BK_BOX_COUNT)
			return &this->bnk.tbox;
		else if (boxID == BK_BOX_COUNT+1)
			return &this->bnk.wbox;
		else return &this->bnk.box[boxID];
	else return &this->sav.box[boxID];
}

Pkm_s* SaveManager::getPkm(u16 boxID, u16 slotID, bool inBank)
{
	return &this->getBox(boxID, inBank)->slot[slotID];
}

bool SaveManager::isPkmEmpty(const Pkm_s* pkm)
{
	return pkm->pk6.Species == 0x00;
}

bool SaveManager::isSlotEmpty(u16 boxID, u16 slotID, bool inBank)
{
	return isPkmEmpty(getPkm(boxID, slotID, inBank));
}

bool SaveManager::filterPkm(const Pkm_s* pkm, bool toBank, bool fromBank)
{
	bool isFiltered = true;
	bool toGame = !toBank;
	bool fromGame = !fromBank;

	// printf("Filtering Pokémon %03u\n", pkm->speciesID);

	if (Game::is(sav.version, Game::XY))
	{
		if (toGame && fromBank) isFiltered &= Filter::filterToXY(&pkm->pk6);
		if (fromGame && toBank) isFiltered &= Filter::filterFromXY(&pkm->pk6);
	}
	else if (Game::is(sav.version, Game::ORAS))
	{
		if (toGame && fromBank) isFiltered &= Filter::filterToORAS(&pkm->pk6);
		if (fromGame && toBank) isFiltered &= Filter::filterFromORAS(&pkm->pk6);
	}
	// else if (Game::is(game.version, Game::SM))
	// {
	// 	if (toGame && fromBank) isFiltered &= Filter::filterToSM(pkm);
	// 	if (fromGame && toBank) isFiltered &= Filter::filterFromSM(pkm);
	// }

	// printf("Filtering: %s\n", (isFiltered ? "allowed" : "forbidden"));

	return isFiltered;
}

bool SaveManager::movePkm(Pkm_s* src, Pkm_s* dst)
{
	Pkm_s tmp;
	memcpy(&tmp, dst, sizeof(Pkm_s));
	memcpy(dst, src, sizeof(Pkm_s));
	memcpy(src, &tmp, sizeof(Pkm_s));

	src->moved = true;
	dst->moved = true;

	return true;
}

bool SaveManager::movePkm(Pkm_s* src, Pkm_s* dst, bool srcBanked, bool dstBanked)
{
	if (!isPkmEmpty(src) && !filterPkm(src, dstBanked, srcBanked)) return false;
	if (!isPkmEmpty(dst) && !filterPkm(dst, srcBanked, dstBanked)) return false;

	return movePkm(src, dst);
}

bool SaveManager::pastePkm(Pkm_s* src, Pkm_s* dst)
{
	memcpy(dst, src, sizeof(Pkm_s));

	dst->moved = true;

	return true;
}

bool SaveManager::pastePkm(Pkm_s* src, Pkm_s* dst, bool srcBanked, bool dstBanked)
{
	if (!isPkmEmpty(src) && !filterPkm(src, dstBanked, srcBanked)) return false;
	if (!isPkmEmpty(dst) && !filterPkm(dst, srcBanked, dstBanked)) return false;

	return pastePkm(src, dst);
}

bool SaveManager::moveBox(Box_s* src, Box_s* dst, bool srcBanked, bool dstBanked)
{
	bool moved = true;

	for (u8 i = 0; i < BOX_PKM_COUNT; i++)
	{
		moved = movePkm(&src->slot[i], &dst->slot[i], srcBanked, dstBanked);
	}

	return moved;
}

bool SaveManager::pasteBox(Box_s* src, Box_s* dst, bool srcBanked, bool dstBanked)
{
	bool moved = true;

	for (u8 i = 0; i < BOX_PKM_COUNT; i++)
	{
		moved = pastePkm(&src->slot[i], &dst->slot[i], srcBanked, dstBanked);
	}

	return moved;
}
