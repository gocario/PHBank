#pragma once
/**
 * @file box_viewer.hpp
 */
#ifndef BOX_VIEWER_HPP
#define BOX_VIEWER_HPP

#include "viewer.hpp"
#include "phbank.hpp"
#include "save.hpp"

/// Structure containing the data displayed on top screen
struct vPkm_s
{
	pkm_s* pkm = NULL;
	bool emptySlot : 1;
	bool isKalosBorn : 1;
	bool isInfected : 1;
	bool isCured : 1;
	unsigned : 4;
	bool circle : 1;
	bool triangle : 1;
	bool square : 1;
	bool heart : 1;
	bool star : 1;
	bool diamond : 1;
	unsigned : 2;
	uint32_t NKName[0xD];
	uint32_t OTName[0xD];
	uint32_t HTName[0xD];
	sf2d_texture* icon;
	const uint32_t* ability;
	const uint32_t* item;
	const uint32_t* moves[4];
	const uint32_t* nature;
	const uint32_t* species;
	const uint32_t* hpType;
	u16 PSV;
	u8 level;
	u16 stats[6];
	u16 ivs[6];
	u16 evs[6];
	u8 ball;
	u8 gen;
};

// TODO: Reformat like PHBankGB
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

// TODO: Reformat like PHBankGB
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

struct HeldTick_s
{
	u64 KEY_RIGHT = 0;
	u64 KEY_LEFT = 0;
	u64 KEY_UP = 0;
	u64 KEY_DOWN = 0;
	u64 KEY_R = 0;
	u64 KEY_L = 0;
};

enum class CursorType : u8
{
	SingleSelect = 0,
	QuickSelect = 1,
	MultiSelect = 2,
	DevSelect = 3,
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

		void selectViewBox(uint16_t boxId, bool inBank);

	private:
		SaveManager* save;
		DataManager* data;
		CursorBox_s cursorBox;
		CursorType cursorType = CursorType::SingleSelect;
		touchPosition touch;
		HeldTick_s heldTick;
		bool isPkmHeld = false;		///< Is a Pokémon held using the buttons?
		bool isPkmDragged = false;	///< Is a Pokémon dragged using the stylus?
		bool isPkmChecking = false;	///< Is there Pokémon being (un)checked?
		bool checkToggle;			///< How to toggle the checked Pokémon?
		BoxSlot_s sSlot;
		pkm_s* sPkm = NULL;
		pkm_s* sPkms[BOX_PKM_COUNT];
		bool sPkmBanked[BOX_PKM_COUNT];
		u8 sPkmCount;
		vPkm_s vPkm;
		box_s* vPCBox = NULL;
		box_s* vBKBox = NULL;

		float cursorPositionOffY = 0.0f;
		float cursorPositionMaxY = 8.0f;
		float cursorPositionShiftY = 0.5f;
		bool cursorPositionDirY = true;

		void drawBox(box_s* box, int16_t x, int16_t y, bool cursor);
		void drawPokemon(pkm_s* pkm, int16_t x, int16_t y, bool shadow);
		void drawPokemonScale(pkm_s* pkm, int16_t x, int16_t y, float scale);
		void drawViewPokemon(vPkm_s* vPkm, int16_t x, int16_t y);
		void drawCursorButton(int16_t x, int16_t y);

		bool isWonderBox(u16 boxId, bool inBank);

		void selectCursorType(CursorType cursorType);
		void switchCursorType();
		void selectViewBox();
		void selectViewPokemon();
		void selectMovePokemon();
		void selectMultiMovePokemon(bool check);
		void selectPastePokemon();
		void selectMultiPastePokemon(bool check);
		void cancelMovePokemon();
		void populateVPkmData(vPkm_s* vPkm, pkm_s* pkm);
};

#endif // BOX_VIEWER_HPP
