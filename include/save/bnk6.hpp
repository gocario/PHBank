#pragma once
/**
 * @file save/bnk6.hpp
 * @author Gocario
 */

#include "save/bnk.hpp"
#include "save/pk6.hpp"
#include "save/box.hpp"
#include "sav/bnk6.hpp"

class BNK6 : public BNK
{
	public:
		BNK6();
		~BNK6();

	public:
		BNK6_s* bnk;

		Box_s box[BK_BOX_COUNT];
		Box_s wbox;
		Box_s tbox;

		u8 boxUnlocked;
		bool wboxUnlocked;
		bool tboxUnlocked;

		/*
		 Data load/save
		              */
		void loadData();
		void saveData();

		void fillPkm(Pkm_s* pkm);

	public:
		/*
		 Get Box/Pkm
		           */
		Box_s* getBox(u16 boxID);
		Pkm_s* getPkm(u16 slotID);
		Pkm_s* getPkm(u16 boxID, u16 slotID);

		/*
		 Get WBox/Pkm
		            */
		Box_s* getWBox();
		Pkm_s* getWPkm(u16 slotID);

		/*
		 Get TBox/Pkm
		            */
		Box_s* getTBox();
		Pkm_s* getTPkm(u16 slotID);
};

// BNK6_HPP
