#pragma once
/**
 * @file viwe/box_browser.hpp
 * @author Gocario
 */

#include "view/scene.hpp"
#include "phbank.hpp"

/// Structure containing the data displayed on top screen
struct vPkm_s
{
	Pkm_s* pkm = NULL;
	bool emptySlot : 1;
	bool isKalosBorn : 1;
	bool isInfected : 1;
	bool isCured : 1;
	unsigned : 4;
	uint32_t Nickname[11];
	uint32_t OT_Name[11];
	uint32_t HT_Name[11];
	sf2d_texture* icon = NULL;
	const uint32_t* ability;
	const uint32_t* item;
	const uint32_t* moves[4];
	const uint32_t* nature;
	const uint32_t* species;
	const uint32_t* hpType;
	u16 PSV;
	u8 level;
	u16 stats[Stat::Count];
	u16 ivs[Stat::Count];
	u16 evs[Stat::Count];
	u8 ball;
	u8 gen;
};

struct CursorSlot_s
{
	bool inBank : 1;
	unsigned : 7;
	int8_t box;
	int8_t row;
	int8_t col;
};

struct CursorInbox_s
{
	bool inBank : 1;
	unsigned : 7;
	int8_t box;
	int8_t row;
	int8_t col;
	int8_t slot;
	Box_s* vBox;
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

void computeInbox(CursorInbox_s* inbox);
void extractCursor(CursorSlot_s* slot, const CursorInbox_s* inbox);
void injectCursor(CursorInbox_s* inbox, const CursorSlot_s* slot);

class BoxBrowser : public Scene
{
	public:
		BoxBrowser(Scene* parent = NULL);
		BoxBrowser(SceneType sType, Scene* parent = NULL);
		~BoxBrowser();

		Result initialize() override;
		Result drawTopScreen() override;
		Result drawBotScreen() override;
		Result update() override;

		void selectViewBox(uint16_t boxID, bool inBank);

	private:
		SaveManager* save;
		DataManager* data;
		CursorType cursorType = CursorType::SingleSelect;

		bool isPkmHeld = false;		///< Is a Pokémon held using the buttons?
		bool isPkmDragged = false;	///< Is a Pokémon dragged using the stylus?
		bool isPkmChecking = false;	///< Is there Pokémon being (un)checked?
		bool checkToggle;			///< How to toggle the checked Pokémon?

		CursorSlot_s sSlot;
		CursorInbox_s cPC;
		CursorInbox_s cBK;
		CursorInbox_s* cBox;

		vPkm_s vPkm;
		vPkm_s sPkm;
		Pkm_s* sPkms[BOX_PKM_COUNT] = { 0 };
		bool sPkmBanked[BOX_PKM_COUNT];
		u8 sPkmCount;

		HeldTick_s heldTick;
		float cursorPositionOffY = 0.0f;
		float cursorPositionMaxY = 8.0f;
		float cursorPositionShiftY = 0.5f;
		bool cursorPositionDirY = true;

		/*
		 Draw
		    */
		void drawBox(const Box_s* box, int16_t x, int16_t y, bool cursor);
		void drawPokemon(const Pkm_s* pkm, int16_t x, int16_t y, bool shadow);
		void drawPokemonScale(const Pkm_s* pkm, int16_t x, int16_t y, float scale);
		void drawViewPokemon(vPkm_s* vPkm, int16_t x, int16_t y);
		void drawCursorButton(int16_t x, int16_t y);

		/*
		 Cursor
		      */
		void selectCursorType(CursorType cursorType);
		void switchCursorType();

		/*
		 Update displayed data
		                     */
		void swapViewBox(CursorInbox_s* cInbox);
		void selectViewBox();
		void selectViewPokemon();

		/*
		 Move
		    */
		void selectMovePokemon();
		void selectMultiMovePokemon(bool check);

		/*
		 Paste
		     */
		void selectPastePokemon();
		void selectMultiPastePokemon(bool check);

		void cancelMovePokemon();
		void populateVPkmData(vPkm_s* vPkm, Pkm_s* pkm);
};

// BOX_VIEWER_HPP
