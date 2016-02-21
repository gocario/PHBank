#pragma once
/**
 * @file box_viewer.hpp
 */
#ifndef BOX_VIEWER_HPP
#define BOX_VIEWER_HPP

#include "viewer.hpp"
#include "phbank.hpp"
#include "save.hpp"

struct vPkm_s
{
	pkm_s* pkm = NULL;
	bool emptySlot : 1;
	bool isKalosBorn : 1;
	bool isInfected : 1;
	bool isCured : 1;
	unsigned : 4;
	u8 NKName[0xD];
	u8 OTName[0xD];
	u8 HTName[0xD];
	const u8* species;
	const u8* item;
	const u8* nature;
	const u8* ability;
	const u8* moves[4];
	const u8* hiddenPower;
	u8 level;
	u16 stats[6];
	u16 ivs[6];
	u16 evs[6];
	u8 ball;
	u8 gen;
};

struct BoxSlot_s
{
	bool inBank = false;
	s16 slot = 0;
	s16 inslot = 0;
	s16 box = 0;
	s16 row = 0;
	s16 col = 0;
	s16 rowCount = 0;
	s16 colCount = 0;
};

struct CursorBox_s
{
	bool inBank = false;
	s16 slot = 0;
	s16 inslot = 0;
	s16 boxPC = 0;
	s16 boxBK = 0;
	s16* box = NULL;
	s16 row = 0;
	s16 col = 0;
};

enum class CursorType : u8
{
	SingleSelect = 0,
	QuickSelect = 1,
	MultipleSelect = 2,
};

enum class CursorState : u8
{
	SelectingTarget = 0,
	TargetFound = 1,
	DraggingTarget = 2,
};


void computeSlot(CursorBox_s* cursorBox);
void extractBoxSlot(CursorBox_s* cursorBox, BoxSlot_s* boxSlot);
void injectBoxSlot(CursorBox_s* cursorBox, BoxSlot_s* boxSlot);


class BoxViewer : public Viewer
{
	public:
		BoxViewer(Viewer* parent = NULL);
		BoxViewer(ViewType vType, Viewer* parent = NULL);
		~BoxViewer();

		Result initialize() override;
		Result drawTopScreen() override;
		Result drawBotScreen() override;
		Result updateControls(const u32& kDown = 0, const u32& kHeld = 0, const u32& kUp = 0, const touchPosition* touch = NULL) override;

		void selectViewBox(uint16_t boxID, bool inBank);

	private:
		SaveManager* save;
		CursorBox_s cursorBox;
		CursorType cursorType = CursorType::SingleSelect;
		touchPosition touch;
		bool isPkmHeld = false;
		bool isPkmDragged = false;
		bool isPkmMDragged = false;
		bool isPkmMSelecting = false;
		BoxSlot_s sSlot;
		pkm_s* sPkm = NULL;
		vPkm_s vPkm;
		box_s* vPCBox = NULL;
		box_s* vBKBox = NULL;

		float cursorPositionOffY = 0.0f;
		float cursorPositionMaxY = 8.0f;
		float cursorPositionShiftY = 0.5f;
		bool cursorPositionDirY = true;

		void drawBox(box_s* box, int16_t x, int16_t y);
		void drawPokemon(pkm_s* pkm, int16_t x, int16_t y);
		void drawPokemonScale(pkm_s* pkm, int16_t x, int16_t y, float scale);

		void selectCursorType(CursorType cursorType);
		void switchCursorType();
		void selectViewBox();
		void selectViewPokemon();
		void selectMovePokemon();
		void selectMultiMovePokemon();
		void cancelMovePokemon();
		void populateVPkmData(vPkm_s* vPkm);
};

#endif // BOX_VIEWER_HPP
