#pragma once
/**
 * @file save/sav6.hpp
 * @author Gocario
 */

#include "save/sav.hpp"
#include "save/pk6.hpp"
#include "save/box.hpp"
#include "sav/sav6_xy.hpp"
#include "sav/sav6_oras.hpp"

class SAV6 : public SAV
{
	public:
		SAV6();
		~SAV6();

	public:
		Game::Version version;
		SAV6_XY_s* savXY;
		SAV6_ORAS_s* savORAS;

		Box_s box[PC_BOX_COUNT];
		u8 boxUnlocked;

		u16 TID, SID, TSV;
		u16 T_Name[13];
		u8 T_Gender;
		u8 GeoRegion;
		u8 GeoCountry;

		uint32_t wT_Name[13];

		static u16 getTSV(u16 TID, u16 SID);

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
		 Dex
		   */
		void addDexEntry(const PK6_s* pk6);

		/*
		 Trade Pkm
		         */
		bool checkOT(const PK6_s* pk6);
		bool tradePkm(PK6_s* pk6);
		bool tradePkmHT(PK6_s* pk6);

	public:
		/*
		 Save encryption
		               */
		u16 ccitt16(const u8* data, u32 len);
		u32 CHKOffset(u32 i);
		u32 CHKLength(u32 i);
		void rewriteCHK();
};

// SAV6_HPP
