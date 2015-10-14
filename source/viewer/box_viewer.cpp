#include "box_viewer.hpp"

#include "ultra_box_viewer.hpp"
#include "savexit_viewer.hpp"
#include "pkm_resume_background.h"
#include "box_background.h"
#include "box_icons.h"
#include "box_tiles.h"

#include "pokemon.hpp"

#define BOX_HEADER_SELECTED -1
#define SLOT_NO_SELECTION -1

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define BACKGROUND_WIDTH 220
#define BACKGROUND_HEIGHT 210
#define BACKGROUND_SHIFT 40

#define PC_BOX_SHIFT_USED 0
#define BK_BOX_SHIFT_UNUSED PC_BOX_SHIFT_USED + BACKGROUND_WIDTH + BACKGROUND_SHIFT


#define BK_BOX_SHIFT_USED SCREEN_WIDTH - BACKGROUND_WIDTH
#define PC_BOX_SHIFT_UNUSED BK_BOX_SHIFT_USED - BACKGROUND_WIDTH - BACKGROUND_SHIFT


// --------------------------------------------------
int16_t* currentBox(CursorBox_t* cursorBox)
// --------------------------------------------------
{
	cursorBox->box = &(cursorBox->inBank ? cursorBox->boxBK : cursorBox->boxPC);
	return cursorBox->box;
}


// --------------------------------------------------
void computeSlot(CursorBox_t* cursorBox)
// --------------------------------------------------
{
	currentBox(cursorBox);
	cursorBox->inslot = (cursorBox->row == BOX_HEADER_SELECTED ? SLOT_NO_SELECTION : cursorBox->row * BOX_COL_PKMCOUNT + cursorBox->col);
	cursorBox->slot   = (cursorBox->row == BOX_HEADER_SELECTED ? SLOT_NO_SELECTION : *cursorBox->box * BOX_PKMCOUNT + cursorBox->inslot);
}

// --------------------------------------------------
void extractBoxSlot(CursorBox_t* cursorBox, BoxSlot_t* boxSlot)
// --------------------------------------------------
{
	boxSlot->inBank = cursorBox->inBank;
	boxSlot->slot = cursorBox->slot;
	boxSlot->inslot = cursorBox->inslot;
	boxSlot->box = *(cursorBox->box);
	boxSlot->row = cursorBox->row;
	boxSlot->col = cursorBox->col;
}

// --------------------------------------------------
void injectBoxSlot(CursorBox_t* cursorBox, BoxSlot_t* boxSlot)
// --------------------------------------------------
{
	cursorBox->inBank = boxSlot->inBank;
	cursorBox->slot = boxSlot->slot;
	cursorBox->inslot = boxSlot->inslot;
	if (boxSlot->inBank)
		cursorBox->boxBK = boxSlot->box;
	else
		cursorBox->boxPC = boxSlot->box;
	cursorBox->row = boxSlot->row;
	cursorBox->col = boxSlot->col;
}


/*----------------------------------------------------------*\
 |                       Viewer Class                       |
\*----------------------------------------------------------*/


	/*--------------------------------------------------*\
	 |             Constructor / Destructor             |
	\*--------------------------------------------------*/


// --------------------------------------------------
BoxViewer::BoxViewer(Viewer* parent) : Viewer(parent) { }
// --------------------------------------------------


// --------------------------------------------------
BoxViewer::BoxViewer(StateView_e state, Viewer* parent) : Viewer(state, parent) { }
// --------------------------------------------------


// --------------------------------------------------
BoxViewer::~BoxViewer()
// --------------------------------------------------
{
	if (backgroundBox) sf2d_free_texture(backgroundBox);
	if (backgroundResume) sf2d_free_texture(backgroundResume);
	if (icons) sf2d_free_texture(icons);
	if (tiles) sf2d_free_texture(tiles);

	// delete vBox;
}


	/*--------------------------------------------------*\
	 |                  Viewer Methods                  |
	\*--------------------------------------------------*/


		/*------------------------------------------*\
		 |              Public Methods              |
		\*------------------------------------------*/


// --------------------------------------------------
Result BoxViewer::initialize()
// --------------------------------------------------
{
	if (hasChild()) { if (this->child->initialize() == PARENT_STEP) ; else return CHILD_STEP; }
	// Viewer::initialize();

	// if (!vBox)
	// 	vBox = new box_t();

	// for (uint32_t i = 0; i < 30; i++)
	// 	vBox->slot[i].speciesID = (i / 6 * 9) + (i % 6);

	// for (uint32_t i = 9; i < 12; i++)
	// 	vBox->slot[i].speciesID = (i-3); 

	// vBox->slot[6].speciesID = 25;
	// vBox->slot[12].speciesID = 26;

	cursorBox.inBank = true; selectViewBox();
	cursorBox.inBank = false; selectViewBox();
	selectViewPokemon();


	if (!backgroundBox)
		backgroundBox = sf2d_create_texture_mem_RGBA8(boxBackground23o_img.pixel_data, boxBackground23o_img.width, boxBackground23o_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
	if (!backgroundResume)
		backgroundResume = sf2d_create_texture_mem_RGBA8(resumeBackground_img.pixel_data, resumeBackground_img.width, resumeBackground_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
	if (!icons)
		icons = sf2d_create_texture_mem_RGBA8(boxIcons_img.pixel_data, boxIcons_img.width, boxIcons_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
	if (!tiles)
		tiles = sf2d_create_texture_mem_RGBA8(boxTiles_img.pixel_data, boxTiles_img.width, boxTiles_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);

	sf2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));

	return PARENT_STEP;
}


// --------------------------------------------------
Result BoxViewer::drawTopScreen()
// --------------------------------------------------
{
	if (hasRegularChild()) { if (this->child->drawTopScreen() == PARENT_STEP); else return CHILD_STEP; }
	// Viewer::drawTopScreen();

	sf2d_draw_texture(backgroundResume, 0, 0);

	if (vPkm.pkm && !vPkm.emptySlot)
	{
		uint32_t x, y;

		x = 32;
		y = 16 - 2;
		// sftd_draw_text_white(x, y, "%s", (Pokemon::isNicknamed(vPkm.pkm) ? Pokemon::nickname(vPkm.pkm) : PKData::species(Pokemon::speciesID(vPkm.pkm))));
		if (Pokemon::isEgg(vPkm.pkm))
			sftd_draw_text_white(x, y, "%s", "IT'S AN EGG!!!");
		else
			if (Pokemon::isNicknamed(vPkm.pkm))
				sftd_draw_text_white(x, y, "%s", vPkm.NKName);
			else
				sftd_draw_text_white(x, y, "%s", vPkm.species);

		sftd_draw_text_white(x + 168, y, "Lv.%u", vPkm.level);

		x = 11;
		y = 42 - 2;
		sftd_draw_text_white(x, y, "Game's OT");
		sftd_draw_text_white(x+80, y, "%s", PHBank::pKBank()->savedata->OTName);
		sftd_draw_text_white(x, (y += 15), "Dex No.");
		sftd_draw_text_white(x+50, y, "%u", Pokemon::speciesID(vPkm.pkm));
		sftd_draw_text_white(x+80, y, "%s", vPkm.species);
		sftd_draw_text_white(x, (y += 15), "O.T.");
		sftd_draw_text_white(x+50, y, "%s", vPkm.OTName);
		sftd_draw_text_white(x, (y += 15), "Stat");
		sftd_draw_text_white(x+90, y, "Value");
		sftd_draw_text_white(x+128, y, "IVs");
		sftd_draw_text_white(x+158, y, "EVs");
		sftd_draw_text_white(x, (y+=15), "HP");
		sftd_draw_text_white(x+100, y, "% 3u", vPkm.stats[Stat::HP]);
		sftd_draw_text_white(x+130, y, "% 2u", vPkm.stats[Stat::HP]);
		sftd_draw_text_white(x+160, y, "% 3u", vPkm.stats[Stat::HP]);
		sftd_draw_text_white(x, (y+=15), "Attack");
		sftd_draw_text_white(x+100, y, "% 3u", vPkm.stats[Stat::ATK]);
		sftd_draw_text_white(x+130, y, "% 2u", vPkm.stats[Stat::ATK]);
		sftd_draw_text_white(x+160, y, "% 3u", vPkm.stats[Stat::ATK]);
		sftd_draw_text_white(x, (y+=15), "Defense");
		sftd_draw_text_white(x+100, y, "% 3u", vPkm.stats[Stat::DEF]);
		sftd_draw_text_white(x+130, y, "% 2u", vPkm.stats[Stat::DEF]);
		sftd_draw_text_white(x+160, y, "% 3u", vPkm.stats[Stat::DEF]);
		sftd_draw_text_white(x, (y+=15), "Sp.Attack");
		sftd_draw_text_white(x+100, y, "% 3u", vPkm.stats[Stat::SPA]);
		sftd_draw_text_white(x+130, y, "% 2u", vPkm.stats[Stat::SPA]);
		sftd_draw_text_white(x+160, y, "% 3u", vPkm.stats[Stat::SPA]);
		sftd_draw_text_white(x, (y+=15), "Sp.Defense");
		sftd_draw_text_white(x+100, y, "% 3u", vPkm.stats[Stat::SPD]);
		sftd_draw_text_white(x+130, y, "% 2u", vPkm.stats[Stat::SPD]);
		sftd_draw_text_white(x+160, y, "% 3u", vPkm.stats[Stat::SPD]);
		sftd_draw_text_white(x, (y+=15), "Speed");
		sftd_draw_text_white(x+100, y, "% 3u", vPkm.stats[Stat::SPE]);
		sftd_draw_text_white(x+130, y, "% 2u", vPkm.stats[Stat::SPE]);
		sftd_draw_text_white(x+160, y, "% 3u", vPkm.stats[Stat::SPE]);
		sftd_draw_text_white(x, (y += 15), "Nature");
		sftd_draw_text_white(x+50, y, "%s", vPkm.nature);
		sftd_draw_text_white(x, (y += 15), "Ability");
		sftd_draw_text_white(x+50, y, "%s", vPkm.ability);
		sftd_draw_text_white(x, (y += 15), "Item");
		sftd_draw_text_white(x+50, y, "%s", vPkm.item);

		x = 246;
		y = 147 - 15 - 2;
		sftd_draw_text_white(x, (y += 15), " %s Hidden Power", vPkm.hiddenPower);
		sftd_draw_text_white(x, (y += 15), "Moves");
		sftd_draw_text_white(x, (y += 15), " %s", vPkm.moves[0]);
		sftd_draw_text_white(x, (y += 15), " %s", vPkm.moves[1]);
		sftd_draw_text_white(x, (y += 15), " %s", vPkm.moves[2]);
		sftd_draw_text_white(x, (y += 15), " %s", vPkm.moves[3]);
	}

	if (hasOverlayChild()) { this->child->drawTopScreen(); }
	return SUCCESS_STEP;
}


// --------------------------------------------------
Result BoxViewer::drawBotScreen()
// --------------------------------------------------
{
	if (hasRegularChild()) { if (this->child->drawBotScreen() == PARENT_STEP); else return CHILD_STEP; }
	// Viewer::drawBotScreen();
	
	int16_t boxShift;
	box_t** vBox = NULL;


	/*\* Current Box *\*/
	{
		boxShift = (cursorBox.inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED);
		vBox = &(cursorBox.inBank ? vBKBox : vPCBox);

		// Draw Background
		sf2d_draw_texture(backgroundBox, boxShift, 20);
		char boxTitle[0x18];
		snprintf(boxTitle, 0x18, "Box %i", (cursorBox.inBank ? cursorBox.boxBK : cursorBox.boxPC) + 1);
		int boxTitleWidth = sftd_get_text_width(PHBank::font(), 13, boxTitle);

		sftd_draw_text(PHBank::font(), boxShift + (BACKGROUND_WIDTH - boxTitleWidth) / 2, 25, RGBA8(0x00, 0x00, 0x00, 0xFF), 13, boxTitle);

		
		// Draw Pokémon icons
		if (isPkmDragged || isPkmHeld)
		{
			for (uint32_t i = 0; i < 30; i++)
			{
				if (sPkm != &((*vBox)->slot[i]))
				{
					if (Pokemon::isEgg(&((*vBox)->slot[i])))
					{
						sf2d_draw_texture_part(icons, boxShift + (i % BOX_COL_PKMCOUNT) * 35, (i / BOX_COL_PKMCOUNT) * 35 + 50, ((PKM_COUNT) % 25) * 40, ((PKM_COUNT) / 25) * 30, 40, 30);
					}
					else
					{
						sf2d_draw_texture_part(icons, boxShift + (i % BOX_COL_PKMCOUNT) * 35, (i / BOX_COL_PKMCOUNT) * 35 + 50, (((*vBox)->slot[i].speciesID-1) % 25) * 40, (((*vBox)->slot[i].speciesID-1) / 25) * 30, 40, 30);
					}
				}
			}
		}
		else
		{
			for (uint32_t i = 0; i < 30; i++)
			{
				if (Pokemon::isEgg(&((*vBox)->slot[i])))
				{
					sf2d_draw_texture_part(icons, boxShift + (i % BOX_COL_PKMCOUNT) * 35, (i / BOX_COL_PKMCOUNT) * 35 + 50, ((PKM_COUNT) % 25) * 40, ((PKM_COUNT) / 25) * 30, 40, 30);
				}
				else
				{
					sf2d_draw_texture_part(icons, boxShift + (i % BOX_COL_PKMCOUNT) * 35, (i / BOX_COL_PKMCOUNT) * 35 + 50, (((*vBox)->slot[i].speciesID-1) % 25) * 40, (((*vBox)->slot[i].speciesID-1) / 25) * 30, 40, 30);
				}
			}
		}
		

		// Draw CursorType buttons
		sf2d_draw_texture_part(tiles, boxShift + 21 +   0, 0,   0, 0, 64, 32);
		sf2d_draw_texture_part(tiles, boxShift + 21 +  64, 0,  64, 0, 64, 32);
		sf2d_draw_texture_part(tiles, boxShift + 21 + 128, 0, 128, 0, 64, 32);

		// Draw SwapBox buttons
		sf2d_draw_texture_part(tiles, boxShift + 10 +  0, 20,   0, 64, 16, 24);
		sf2d_draw_texture_part(tiles, boxShift + BACKGROUND_WIDTH - 24, 20,  16, 64, 16, 24);


		if (sPkm)
		{
			if (isPkmDragged)
			{
				// Draw dragged Pokémon
				sf2d_draw_texture_part(icons, touch.px - 16, touch.py - 16, ((sPkm->speciesID-1) % 25) * 40, ((sPkm->speciesID-1) / 25) * 30, 40, 30);
			}
			else // isPkmHeld
			{
				if (cursorBox.inslot == SLOT_NO_SELECTION)
				{
					sf2d_draw_texture_part(icons, boxShift + 105, 8, ((sPkm->speciesID-1) % 25) * 40, ((sPkm->speciesID-1) / 25) * 30, 40, 30);
				}
				else
				{
					sf2d_draw_texture_part(icons, boxShift + 12 + (cursorBox.inslot % 6) * 35, 20 + 13 + (cursorBox.inslot / 6) * 35, ((sPkm->speciesID-1) % 25) * 40, ((sPkm->speciesID-1) / 25) * 30, 40, 30);
				}
			}
		}
		else
		{
			// Draw Cursor
			if (cursorBox.inslot == SLOT_NO_SELECTION)
			{
				sf2d_draw_texture_part(tiles, boxShift + 105, 8, 32 * cursorType, 32, 32, 32);
			}
			else
			{
				sf2d_draw_texture_part(tiles, boxShift + 17 + (cursorBox.inslot % 6) * 35, 20 + 13 + (cursorBox.inslot / 6) * 35, 32 * cursorType, 32, 32, 32);
			}
		}
	}

	/*\* The other Box *\*/
	{
		boxShift = (cursorBox.inBank ? PC_BOX_SHIFT_UNUSED : BK_BOX_SHIFT_UNUSED);
		vBox = &(cursorBox.inBank ? vPCBox : vBKBox);

		// Draw Background
		sf2d_draw_texture(backgroundBox, boxShift, 20);
		

		// Draw SwapBox buttons
		sf2d_draw_texture_part(tiles, boxShift + 10 +  0, 20,   0, 64, 16, 24);
		sf2d_draw_texture_part(tiles, boxShift + BACKGROUND_WIDTH - 24, 20,  16, 64, 16, 24);


		// Draw Pokémon icons
		if (isPkmDragged || isPkmHeld)
		{
			for (uint32_t i = 0; i < 30; i++)
			{
				if (sPkm != &((*vBox)->slot[i]))
				{
					if (Pokemon::isEgg(&((*vBox)->slot[i])))
					{
						sf2d_draw_texture_part(icons, boxShift + (i % BOX_COL_PKMCOUNT) * 35, (i / BOX_COL_PKMCOUNT) * 35 + 50, ((PKM_COUNT) % 25) * 40, ((PKM_COUNT) / 25) * 30, 40, 30);
					}
					else
					{
						sf2d_draw_texture_part(icons, boxShift + (i % BOX_COL_PKMCOUNT) * 35, (i / BOX_COL_PKMCOUNT) * 35 + 50, (((*vBox)->slot[i].speciesID-1) % 25) * 40, (((*vBox)->slot[i].speciesID-1) / 25) * 30, 40, 30);
					}
				}
			}
		}
		else
		{
			for (uint32_t i = 0; i < 30; i++)
			{
				if (Pokemon::isEgg(&((*vBox)->slot[i])))
				{
					sf2d_draw_texture_part(icons, boxShift + (i % BOX_COL_PKMCOUNT) * 35, (i / BOX_COL_PKMCOUNT) * 35 + 50, ((PKM_COUNT) % 25) * 40, ((PKM_COUNT) / 25) * 30, 40, 30);
				}
				else
				{
					sf2d_draw_texture_part(icons, boxShift + (i % BOX_COL_PKMCOUNT) * 35, (i / BOX_COL_PKMCOUNT) * 35 + 50, (((*vBox)->slot[i].speciesID-1) % 25) * 40, (((*vBox)->slot[i].speciesID-1) / 25) * 30, 40, 30);
				}
			}
		}
	}


	if (hasOverlayChild()) { this->child->drawBotScreen(); }
	return SUCCESS_STEP;
}


// --------------------------------------------------
Result BoxViewer::updateControls(const u32& kDown, const u32& kHeld, const u32& kUp, const touchPosition* touch)
// --------------------------------------------------
{
	if (hasRegularChild() || hasOverlayChild()) { if (this->child->updateControls(kDown, kHeld, kUp, touch) == PARENT_STEP); else return CHILD_STEP; }
	
	if (kDown & KEY_START)
	{
		new SavexitViewer(StateView::Overlay, this);
		child->initialize();
		return CHILD_STEP;
		// lStateView = StateView::Exiting;
		// return close(); // DEBUG
	}

	{
		bool boolMod = false;
		int16_t boxMod = 0;
		int16_t rowMod = 0;
		int16_t colMod = 0;


		if (kDown & KEY_L) boxMod--;
		else if (kDown & KEY_R) boxMod++;

		if (kDown & KEY_UP) rowMod--;
		else if (kDown & KEY_DOWN) rowMod++;

		if (kDown & KEY_LEFT) { if (cursorBox.row == BOX_HEADER_SELECTED) boxMod--; else colMod--; }
		else if (kDown & KEY_RIGHT) { if (cursorBox.row == BOX_HEADER_SELECTED) boxMod++; else colMod++; }

		if (kDown & KEY_ZL) { cursorBox.inBank = false; boolMod = true; }
		else if (kDown & KEY_ZR) { cursorBox.inBank = true; boolMod = true; }

		if (kDown & KEY_TOUCH)
		{
			int16_t boxShift = (cursorBox.inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED);
			if (touchWithin(touch->px, touch->py, boxShift + 10, 20, 16, 24)) boxMod--;
			else if (touchWithin(touch->px, touch->py, boxShift + 196, 20, 16, 24)) boxMod++;
			boxShift = (cursorBox.inBank ? PC_BOX_SHIFT_UNUSED : BK_BOX_SHIFT_UNUSED);
			if (touchWithin(touch->px, touch->py, boxShift, 20, BACKGROUND_WIDTH, BACKGROUND_HEIGHT)) { cursorBox.inBank = !cursorBox.inBank; boolMod = true; }
		}
		else if (kHeld & KEY_TOUCH)
		{
			if (isPkmDragged)
			{
				int16_t boxShift = (cursorBox.inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED);
				if (touchWithin(touch->px, touch->py, boxShift + 10, 20, 16, 24) && !touchWithin(this->touch.px, this->touch.py, boxShift + 10, 20, 16, 24)) boxMod--;
				else if (touchWithin(touch->px, touch->py, boxShift + 196, 20, 16, 24) && !touchWithin(this->touch.px, this->touch.py, boxShift + 196, 20, 16, 24)) boxMod++;
				boxShift = (cursorBox.inBank ? PC_BOX_SHIFT_UNUSED : BK_BOX_SHIFT_UNUSED);
				if (touchWithin(touch->px, touch->py, boxShift, 20, BACKGROUND_WIDTH, BACKGROUND_HEIGHT)) { cursorBox.inBank = !cursorBox.inBank; boolMod = true; }
			}
		}

		if (boxMod || rowMod || colMod)
		{
			currentBox(&cursorBox);
			*cursorBox.box += boxMod;
			cursorBox.row += rowMod;
			cursorBox.col += colMod;

			if (*cursorBox.box < 0) *cursorBox.box = (cursorBox.inBank ? BANK_BOXCOUNT : PC_BOXCOUNT)-1;
			else if (*cursorBox.box > (cursorBox.inBank ? BANK_BOXCOUNT : PC_BOXCOUNT)-1) *cursorBox.box = 0;

			if (cursorBox.row < BOX_HEADER_SELECTED) cursorBox.row = BOX_ROW_PKMCOUNT-1;
			else if (cursorBox.row > BOX_ROW_PKMCOUNT-1) cursorBox.row = BOX_HEADER_SELECTED;

			if (cursorBox.col < 0) { cursorBox.col = BOX_COL_PKMCOUNT-1; cursorBox.inBank = !cursorBox.inBank; }
			else if (cursorBox.col > BOX_COL_PKMCOUNT-1) { cursorBox.col = 0; cursorBox.inBank = !cursorBox.inBank; }

			boolMod = true;
		}

		if (boolMod)
		{
			currentBox(&cursorBox);
			selectViewBox();
			selectViewPokemon();
		}
	}

	if (kDown & KEY_SELECT)
	{
		switchCursorType();
	}

	if (cursorType == CursorType::SingleSelect)
	{
		if (kDown & KEY_A && !isPkmDragged)
		{
			selectMovePokemon();
		}

		if (kDown & KEY_B && !isPkmDragged)
		{
			cancelMovePokemon();
		}
	}
	else if (cursorType == CursorType::QuickSelect)
	{
		if (kDown & KEY_A && !isPkmDragged)
		{
			selectMovePokemon();
		}

		if (kDown & KEY_B && !isPkmDragged)
		{
			cancelMovePokemon();
		}

		if (kDown & KEY_Y && !isPkmDragged)
		{
			if (vPCBox && vBKBox)
			{
				PHBank::pKBank()->moveBox(cursorBox.boxPC, false, cursorBox.boxBK, true);
			}
		}
	}
	else if (cursorType == CursorType::MultipleSelect)
	{
		if (kDown & KEY_Y)
		{
			// computeSlot(&cursorBox);
			new UltraBoxViewer(StateView::Regular, this);
			((UltraBoxViewer*) child)->selectViewBox(*cursorBox.box, cursorBox.inBank);
			child->initialize();
			return CHILD_STEP;
		}
	}
	

	{
		if (kDown & KEY_TOUCH)
		{
			int16_t boxShift = (cursorBox.inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED);
			uint16_t px = touch->px;
			uint16_t py = touch->py;
			// printf("\x1B[21;0Hdown{%3u, %3u} ", touch->px, touch->py);
			if (touchWithin(px, py, boxShift + 22, 0, 59, 16))
			{
				selectCursorType(CursorType::SingleSelect);
			}
			else if (touchWithin(px, py, boxShift + 86, 0, 59, 16))
			{
				selectCursorType(CursorType::QuickSelect);
			}
			else if (touchWithin(px, py, boxShift + 150, 0, 59, 16))
			{
				selectCursorType(CursorType::MultipleSelect);
			}

			else if (touchWithin(px, py, boxShift, 50, BACKGROUND_WIDTH, BACKGROUND_HEIGHT - 30))
			{
				// printf("{%3u, %3u}", ((py - 50) / 35), ((px - boxShift) / 35));

				if (!sPkm && !isPkmDragged)
				{
					cursorBox.row = ((py - 50) / 35);
					cursorBox.col = ((px - boxShift) / 35);
					
					selectViewPokemon();
					selectMovePokemon();

					if (!vPkm.emptySlot)
						isPkmDragged = true;
				}
				else
				{
					uint16_t oldRow = cursorBox.row;
					uint16_t oldCol = cursorBox.col;

					cursorBox.row = ((py - 50) / 35);
					cursorBox.col = ((px - boxShift) / 35);

					selectViewPokemon();
					if (isPkmHeld && oldRow == cursorBox.row && oldCol == cursorBox.col)
						selectMovePokemon();
				}
			}
		}
		else if (kHeld & KEY_TOUCH)
		{
			this->touch.px = touch->px;
			this->touch.py = touch->py;
		}
		else if (kUp & KEY_TOUCH)
		{
			touch = &(this->touch);
			// printf("\x1B[22;0Hup{%3u, %3u} ", touch->px, touch->py);

			int16_t boxShift = (cursorBox.inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED);
			uint16_t px = touch->px;
			uint16_t py = touch->py;

			if (sPkm && isPkmDragged)
			{
				if (touchWithin(px, py, boxShift, 50, BACKGROUND_WIDTH, BACKGROUND_HEIGHT))
				{
					// printf("{%3u, %3u}", ((py - 50) / 35), ((px - boxShift) / 35));
					cursorBox.row = ((py - 50) / 35);
					cursorBox.col = ((px - boxShift) / 35);

					selectViewPokemon();
					selectMovePokemon();
				}
				else
				{
					cancelMovePokemon();
					// injectBoxSlot(&cursorBox, &sSlot); // Maybe not... ?
					// selectViewBox(); // ... ?
					selectViewPokemon();
				}

				isPkmDragged = false;
			}
		}
	}

	return SUCCESS_STEP;
}


// --------------------------------------------------
void BoxViewer::selectViewBox(uint16_t boxID, bool inBank)
// --------------------------------------------------
{
	bool BK = cursorBox.inBank;
	cursorBox.inBank = inBank;

	if (inBank)
		cursorBox.boxBK = boxID;
	else
		cursorBox.boxPC = boxID;

	selectViewBox();

	cursorBox.inBank = BK;
	selectViewBox();
}


		/*------------------------------------------*\
		 |             Protected Methods            |
		\*------------------------------------------*/


		/*------------------------------------------*\
		 |              Private Methods             |
		\*------------------------------------------*/



// --------------------------------------------------
void BoxViewer::selectCursorType(CursorType_e cursorType)
// --------------------------------------------------
{
	this->cursorType = cursorType;
}


// --------------------------------------------------
void BoxViewer::switchCursorType()
// --------------------------------------------------
{
	if (cursorType == CursorType::SingleSelect)
		selectCursorType(CursorType::QuickSelect);
	else if (cursorType == CursorType::QuickSelect)
		selectCursorType(CursorType::MultipleSelect);
	else if (cursorType == CursorType::MultipleSelect)
		selectCursorType(CursorType::SingleSelect);
}


// --------------------------------------------------
void BoxViewer::selectViewBox()
// --------------------------------------------------
{
	computeSlot(&cursorBox);

	box_t** vBox = NULL;
	if (cursorBox.inBank)
		vBox = &vBKBox;
	else
		vBox = &vPCBox;

	PHBank::pKBank()->getBox(*cursorBox.box, vBox, cursorBox.inBank);

	// printf("View Box: [@%p][@%p][@%p]\n", *vBox, vPCBox, vBKBox);
}


// --------------------------------------------------
void BoxViewer::selectViewPokemon()
// --------------------------------------------------
{
	computeSlot(&cursorBox);

	if (cursorBox.slot == SLOT_NO_SELECTION)
	{
		vPkm.pkm = NULL;
	}
	else
	{
		PHBank::pKBank()->getPkm(*cursorBox.box, cursorBox.inslot, &vPkm.pkm, cursorBox.inBank);
		populateVPkmData(&vPkm);
	}

	// printf("View Pokemon: [@%p]\n", vPkm.pkm);
}


// --------------------------------------------------
void BoxViewer::selectMovePokemon()
// --------------------------------------------------
{
	computeSlot(&cursorBox);
	// selectViewPokemon();

	if (!sPkm)
	{
		if (!PHBank::pKBank()->isPkmEmpty(vPkm.pkm))
		{
			sPkm = vPkm.pkm;
			// extractBoxSlot(&cursorBox, &sSlot);
			if (!isPkmDragged) isPkmHeld = true;
		}
	}
	else if (vPkm.pkm)
	{
		if (sPkm != vPkm.pkm)
		{
			PHBank::pKBank()->movePkm(sPkm, vPkm.pkm, sSlot.inBank, cursorBox.inBank);
			populateVPkmData(&vPkm);
		}

		cancelMovePokemon();
	}

	// printf("Selected Pokemon: [@%p]\n", vPkm.pkm);
}


// --------------------------------------------------
void BoxViewer::cancelMovePokemon()
// --------------------------------------------------
{
	sPkm = NULL;
	isPkmDragged = false;
	isPkmHeld = false;
}


// --------------------------------------------------
void BoxViewer::populateVPkmData(vPkm_t* vPkm)
// --------------------------------------------------
{
	u16* name;

	name = Pokemon::NK_name(vPkm->pkm);
	if (name)
		for (u8 i = 0; i < 0x18; i += 2)
			vPkm->NKName[i / 2] = name[i / 2] & 0xFF;
	vPkm->NKName[0x1a / 2 - 1] = '\0';

	name = Pokemon::OT_name(vPkm->pkm);
	if (name)
		for (u8 i = 0; i < 0x18; i += 2)
			vPkm->OTName[i / 2] = name[i / 2] & 0xFF;
	vPkm->OTName[0x1a / 2 - 1] = '\0';

	name = Pokemon::HT_name(vPkm->pkm);
	if (name)
		for (u8 i = 0; i < 0x18; i += 2)
			vPkm->HTName[i / 2] = name[i / 2] & 0xFF;
	vPkm->HTName[0x1a / 2 - 1] = '\0';


	vPkm->emptySlot = PHBank::pKBank()->isPkmEmpty(vPkm->pkm);

	vPkm->species = PKData::species(Pokemon::speciesID(vPkm->pkm));
	vPkm->item = PKData::items(Pokemon::itemID(vPkm->pkm));
	vPkm->nature = PKData::natures(Pokemon::nature(vPkm->pkm));
	vPkm->ability = PKData::abilities(Pokemon::ability(vPkm->pkm));
	vPkm->hiddenPower = PKData::HPTypes(Pokemon::HPType(vPkm->pkm));

	vPkm->moves[0] = PKData::moves(Pokemon::move1(vPkm->pkm));
	vPkm->moves[1] = PKData::moves(Pokemon::move2(vPkm->pkm));
	vPkm->moves[2] = PKData::moves(Pokemon::move3(vPkm->pkm));
	vPkm->moves[3] = PKData::moves(Pokemon::move4(vPkm->pkm));

	vPkm->level = Pokemon::level(vPkm->pkm);
	vPkm->stats[Stat::HP] = Pokemon::HP(vPkm->pkm);
	vPkm->stats[Stat::ATK] = Pokemon::ATK(vPkm->pkm);
	vPkm->stats[Stat::DEF] = Pokemon::DEF(vPkm->pkm);
	vPkm->stats[Stat::SPA] = Pokemon::SPA(vPkm->pkm);
	vPkm->stats[Stat::SPD] = Pokemon::SPD(vPkm->pkm);
	vPkm->stats[Stat::SPE] = Pokemon::SPE(vPkm->pkm);
	vPkm->ivs[Stat::ATK] = Pokemon::IV_ATK(vPkm->pkm);
	vPkm->ivs[Stat::DEF] = Pokemon::IV_DEF(vPkm->pkm);
	vPkm->ivs[Stat::SPA] = Pokemon::IV_SPA(vPkm->pkm);
	vPkm->ivs[Stat::SPD] = Pokemon::IV_SPD(vPkm->pkm);
	vPkm->ivs[Stat::SPE] = Pokemon::IV_SPE(vPkm->pkm);
	vPkm->evs[Stat::ATK] = Pokemon::EV_ATK(vPkm->pkm);
	vPkm->evs[Stat::DEF] = Pokemon::EV_DEF(vPkm->pkm);
	vPkm->evs[Stat::SPA] = Pokemon::EV_SPA(vPkm->pkm);
	vPkm->evs[Stat::SPD] = Pokemon::EV_SPD(vPkm->pkm);
	vPkm->evs[Stat::SPE] = Pokemon::EV_SPE(vPkm->pkm);
}