#pragma once
#ifndef BOX_VIEWER_HPP
#define BOX_VIEWER_HPP

#include "viewer.hpp"
#include "save.hpp"


struct vPkm_s {
	pkm_s* pkm = NULL;
	bool emptySlot : 1;
	bool isShiny : 1;
	bool isKalosBorn : 1;
	bool isInfected : 1;
	bool isCured : 1;
	bool isEgg : 1;
	unsigned : 2;
	u8 NKName[0x1a / 2];
	u8 OTName[0x1a / 2];
	u8 HTName[0x1a / 2];
	const u8* species;
	const u8* item;
	const u8* nature;
	const u8* ability;
	const u8* moves[4];
	const u8* hiddenPower;
	u16 speciesID;
	u8 level;
	u16 stats[6];
	u16 ivs[6];
	u16 evs[6];
};

struct BoxSlot_s {
	bool inBank = false;
	int16_t slot = 0;
	int16_t inslot = 0;
	int16_t box = 0;
	int16_t row = 0;
	int16_t col = 0;
	int16_t rowCount = 0;
	int16_t colCount = 0;
};

struct CursorBox_s {
	bool inBank = false;
	int16_t slot = 0;
	int16_t inslot = 0;
	int16_t boxPC = 0;
	int16_t boxBK = 0;
	int16_t* box = NULL;
	int16_t row = 0;
	int16_t col = 0;
};

namespace CursorType
{
	typedef enum CursorType_e {
		SingleSelect = 0,
		QuickSelect = 1,
		MultipleSelect = 2,
	} CursorType_e;
}
typedef CursorType::CursorType_e CursorType_e;

namespace CursorState
{
	typedef enum CursorState_e {
		SelectingTarget = 0,
		TargetFound = 1,
		DraggingTarget = 2,
	} CursorState_e;
}
typedef CursorState::CursorState_e CursorState_e;


void computeSlot(CursorBox_s* cursorBox);
void extractBoxSlot(CursorBox_s* cursorBox, BoxSlot_s* boxSlot);
void injectBoxSlot(CursorBox_s* cursorBox, BoxSlot_s* boxSlot);


class BoxViewer : public Viewer
{
	public:
		BoxViewer(Viewer* parent = NULL);
		BoxViewer(StateView_e state, Viewer* parent = NULL);
		~BoxViewer();

		Result initialize() override;
		Result drawTopScreen() override;
		Result drawBotScreen() override;
		Result updateControls(const u32& kDown = 0, const u32& kHeld = 0, const u32& kUp = 0, const touchPosition* touch = NULL) override;

		void selectViewBox(uint16_t boxID, bool inBank = false);
	private:
		sf2d_texture* backgroundBox = NULL;
		sf2d_texture* backgroundResume = NULL;
		sf2d_texture* icons = NULL;
		sf2d_texture* tiles = NULL;

		CursorBox_s cursorBox;
		CursorType_e cursorType = CursorType::SingleSelect;
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

		void selectCursorType(CursorType_e cursorType);
		void switchCursorType();
		void selectViewBox();
		void selectViewPokemon();
		void selectMovePokemon();
		void selectMultiMovePokemon();
		void cancelMovePokemon();
		void populateVPkmData(vPkm_s* vPkm);
};

#endif // BOX_VIEWER_HPP
