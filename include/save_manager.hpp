#pragma once
/**
 * @file save_manager.hpp
 * @author Gocario
 */

#include <3ds.h>

#include "save/sav6.hpp"
#include "save/bnk6.hpp"

class SaveManager
{
	public:
		SaveManager();
		~SaveManager();

	public:
		SAV6 sav;
		BNK6 bnk;

	public:
		/*
		 Load/Save
		         */
		Result load();
		Result save();
		Result backup();

		/*
		 Load/Save Game File
		                   */
		Result loadGameFile();
		Result saveGameFile();
		Result backupGameFile();

		/*
		 Load/Save Bank File
		                   */
		Result loadBankFile();
		Result saveBankFile();
		Result backupBankFile();

	public:
		/*
		 Get Box/Pkm
		           */
		Box_s* getBox(u16 boxID, bool inBank);
		Pkm_s* getPkm(u16 boxID, u16 slotID, bool inBank);

		/*
		 Validity
		        */
		bool isPkmEmpty(const Pkm_s* pkm);
		bool isSlotEmpty(u16 boxID, u16 slotID, bool inBank);
		bool filterPkm(const Pkm_s* pkm, bool toBank, bool fromBank);

		/*
		 Move/Paste
		          */
		bool movePkm(Pkm_s* src, Pkm_s* dst);
		bool movePkm(Pkm_s* src, Pkm_s* dst, bool srcBanked, bool dstBanked);
		bool pastePkm(Pkm_s* src, Pkm_s* dst);
		bool pastePkm(Pkm_s* src, Pkm_s* dst, bool srcBanked, bool dstBanked);
		bool moveBox(Box_s* src, Box_s* dst, bool srcBanked, bool dstBanked);
		bool pasteBox(Box_s* src, Box_s* dst, bool srcBanked, bool dstBanked);

};

// SM_HPP