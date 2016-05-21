#include "box_viewer.hpp"

#include "lang.h"
#include "text.h"
#include "utils.h"
#include "pkdir.h"
#include "pokemon.hpp"
#include "personal.hpp"

#include "ultra_box_viewer.hpp"
#include "savexit_viewer.hpp"

#include <sfil.h>
#include <sftd.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#define BOX_HEADER_SELECTED (-1)
#define SLOT_NO_SELECTION (-1)

#define SCREEN_WIDTH (320)
#define SCREEN_HEIGHT (240)

#define PKM_WIDTH (35) // Real: 40
#define PKM_HEIGHT (33) // Real: 30
#define PKM_BOX_MARGIN_TOP (45)
#define PKM_BOX_MARGIN_RIGHT (11) // Real: 10
#define BOX_HEADER_WIDTH (220)
#define BOX_HEADER_HEIGHT (32)
#define BACKGROUND_WIDTH (220)
#define BACKGROUND_HEIGHT (210)
#define BACKGROUND_SHIFT (40)

#define PC_BOX_SHIFT_USED (0)
#define BK_BOX_SHIFT_USED (SCREEN_WIDTH - BACKGROUND_WIDTH)
#define BK_BOX_SHIFT_UNUSED (PC_BOX_SHIFT_USED + BACKGROUND_WIDTH + BACKGROUND_SHIFT)
#define PC_BOX_SHIFT_UNUSED (BK_BOX_SHIFT_USED - BACKGROUND_WIDTH - BACKGROUND_SHIFT)

#define UNKNOWN_ID (0) // 0
#define EGG_ID (722) // PKM_COUNT + 1

#define BALL_ROW_COUNT (5)
#define BALL_SIZE (32)

#define HELD_TICK (32000000)

// --------------------------------------------------
/// Compute the current box pointer of the cursor
int16_t* currentBox(CursorBox_s* cursorBox)
// --------------------------------------------------
{
	cursorBox->box = cursorBox->inBank ? &cursorBox->boxBK : &cursorBox->boxPC;
	return cursorBox->box;
}


// --------------------------------------------------
/// Compute the slot and the inslot of the cursor
void computeSlot(CursorBox_s* cursorBox)
// --------------------------------------------------
{
	currentBox(cursorBox);
	cursorBox->inslot = cursorBox->row == BOX_HEADER_SELECTED ? SLOT_NO_SELECTION : cursorBox->row * BOX_COL_PKM_COUNT + cursorBox->col;
	cursorBox->slot   = cursorBox->row == BOX_HEADER_SELECTED ? SLOT_NO_SELECTION : *cursorBox->box * BOX_PKM_COUNT + cursorBox->inslot;
}


// --------------------------------------------------
void computeBoxSlot(BoxSlot_s* boxSlot, CursorBox_s* cursorBox)
// --------------------------------------------------
{
	// extractBoxSlot shall already be called before.

	boxSlot->rowCount = cursorBox->row - boxSlot->row;
	boxSlot->colCount = cursorBox->col - boxSlot->col;

	if (boxSlot->rowCount < 0)
	{
		boxSlot->row = boxSlot->row + boxSlot->rowCount;
		boxSlot->row *= -1;
	}
	if (boxSlot->colCount < 0)
	{
		boxSlot->col = boxSlot->col + boxSlot->colCount;
		boxSlot->col *= -1;
	}

	boxSlot->rowCount++;
	boxSlot->colCount++;
}


// --------------------------------------------------
/// Convert CursorBox position to BoxSlot position
void extractBoxSlot(BoxSlot_s* boxSlot, CursorBox_s* cursorBox)
// --------------------------------------------------
{
	boxSlot->inBank = cursorBox->inBank;
	boxSlot->slot = cursorBox->slot;
	boxSlot->inslot = cursorBox->inslot;
	boxSlot->box = *cursorBox->box;
	boxSlot->row = cursorBox->row;
	boxSlot->col = cursorBox->col;
}


// --------------------------------------------------
/// Convert BoxSlot position to CursorBox position
void injectBoxSlot(BoxSlot_s* boxSlot, CursorBox_s* cursorBox)
// --------------------------------------------------
{
	cursorBox->inBank = boxSlot->inBank;
	cursorBox->slot = boxSlot->slot;
	cursorBox->inslot = boxSlot->inslot;
	(boxSlot->inBank ? cursorBox->boxBK : cursorBox->boxPC) = boxSlot->box;
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
BoxViewer::BoxViewer(ViewType vType, Viewer* parent) : Viewer(vType, parent) { }
// --------------------------------------------------


// --------------------------------------------------
BoxViewer::~BoxViewer()
// --------------------------------------------------
{
	if (vPkm.icon) sf2d_free_texture(vPkm.icon);
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
	if (hasChild()) { if (this->child->initialize() == PARENT_STEP); else return CHILD_STEP; }

	// Use the Save&Data Manager of PHBanku
	save = PHBanku::save;
	data = PHBanku::data;

	// Initialize the 2 boxes content
	cursorBox.inBank = true; selectViewBox();
	cursorBox.inBank = false; selectViewBox();

	// Initialize the current Pokémon
	selectViewPokemon();
	cancelMovePokemon();

	// Change the clear color of sf2d (not useful)
	sf2d_set_clear_color(RGBA8(0x00,0x00,0x00,0xFF));

	return PARENT_STEP;
}


// --------------------------------------------------
Result BoxViewer::drawTopScreen()
// --------------------------------------------------
{
	if (hasRegularChild()) { if (this->child->drawTopScreen() == PARENT_STEP); else return CHILD_STEP; }

	// Draw the resume background
	sf2d_draw_texture(PHBanku::texture->resumeBackground, 0, 0);

	sftd_draw_wtext_white(11, 40, data->text(BankText::GameTrainer));

	// If there is a current Pokémon
	if ((vPkm.pkm && !vPkm.emptySlot) || sPkm)
	{
		pkm_s* pkm = vPkm.pkm && !vPkm.emptySlot ? vPkm.pkm : sPkm;

		sftd_draw_wtextf_white(91, 40, L"%S (%lu/%lu)", save->savedata.OTName, save->savedata.TSV, vPkm.PSV);

		uint32_t x, y;

		sf2d_draw_texture_part(PHBanku::texture->ballIcons, 5, 5, (vPkm.ball % BALL_ROW_COUNT) * BALL_SIZE, (vPkm.ball / BALL_ROW_COUNT) * BALL_SIZE, BALL_SIZE, BALL_SIZE);

		x = 32;
		y = 16 - 2;
		// Is the Pokémon an egg?
		if (pkm->isEggy)
		{
			sftd_draw_wtext_white(x, y, data->species(0));
		}
		else
		{
			// Is the Pokémon nicknamed?
			if (Pokemon::isNicknamed(pkm))
			{
				sftd_draw_wtext_white(x, y, vPkm.NKName);
			}
			else
			{
				sftd_draw_wtext_white(x, y, vPkm.species);
			}
		}

		sftd_draw_wtextf_white(x + 168, y, L"%S%u", data->text(BankText::Level), vPkm.level);

		x = 11;
		y = 42 - 2;
		sftd_draw_wtext_white(x, (y += 15), data->text(BankText::DexNo));
		sftd_draw_text_white(x+50, y, "%03u", pkm->speciesID);
		sftd_draw_wtext_white(x+80, y, vPkm.species);
		sftd_draw_wtext_white(x, (y += 15), data->text(BankText::OriginalTrainer));
		sftd_draw_wtext_white(x+50, y, vPkm.OTName);
		y += 15; // sftd_draw_text_white(x, (y += 15), "Stat");
		sftd_draw_wtext_white(x+90, y, data->text(BankText::Value));
		sftd_draw_wtext_white(x+128, y, data->text(BankText::IV));
		sftd_draw_wtext_white(x+158, y, data->text(BankText::EV));
		sftd_draw_wtext_white(x, (y+=15), data->text(BankText::HitPoint));
		sftd_draw_text_white(x+98, y, "% 3u", vPkm.stats[Stat::HP]);
		sftd_draw_text_white(x+130, y, "% 2u", vPkm.ivs[Stat::HP]);
		sftd_draw_text_white(x+160, y, "% 3u", vPkm.evs[Stat::HP]);
		sftd_draw_wtext_white(x, (y+=15), data->text(BankText::Attack));
		sftd_draw_text_white(x+98, y, "% 3u", vPkm.stats[Stat::ATK]);
		sftd_draw_text_white(x+130, y, "% 2u", vPkm.ivs[Stat::ATK]);
		sftd_draw_text_white(x+160, y, "% 3u", vPkm.evs[Stat::ATK]);
		sftd_draw_wtext_white(x, (y+=15), data->text(BankText::Defense));
		sftd_draw_text_white(x+98, y, "% 3u", vPkm.stats[Stat::DEF]);
		sftd_draw_text_white(x+130, y, "% 2u", vPkm.ivs[Stat::DEF]);
		sftd_draw_text_white(x+160, y, "% 3u", vPkm.evs[Stat::DEF]);
		sftd_draw_wtext_white(x, (y+=15), data->text(BankText::SpAttack));
		sftd_draw_text_white(x+98, y, "% 3u", vPkm.stats[Stat::SPA]);
		sftd_draw_text_white(x+130, y, "% 2u", vPkm.ivs[Stat::SPA]);
		sftd_draw_text_white(x+160, y, "% 3u", vPkm.evs[Stat::SPA]);
		sftd_draw_wtext_white(x, (y+=15), data->text(BankText::SpDefense));
		sftd_draw_text_white(x+98, y, "% 3u", vPkm.stats[Stat::SPD]);
		sftd_draw_text_white(x+130, y, "% 2u", vPkm.ivs[Stat::SPD]);
		sftd_draw_text_white(x+160, y, "% 3u", vPkm.evs[Stat::SPD]);
		sftd_draw_wtext_white(x, (y+=15), data->text(BankText::Speed));
		sftd_draw_text_white(x+98, y, "% 3u", vPkm.stats[Stat::SPE]);
		sftd_draw_text_white(x+130, y, "% 2u", vPkm.ivs[Stat::SPE]);
		sftd_draw_text_white(x+160, y, "% 3u", vPkm.evs[Stat::SPE]);
		sftd_draw_wtext_white(x, (y += 15), data->text(BankText::Nature));
		sftd_draw_wtext_white(x+70, y, vPkm.nature);
		sftd_draw_wtext_white(x, (y += 15), data->text(BankText::Ability));
		sftd_draw_wtext_white(x+70, y, vPkm.ability);
		sftd_draw_wtext_white(x, (y += 15), data->text(BankText::Item));
		sftd_draw_wtext_white(x+70, y, vPkm.item);

		x = 246;
		y = 147 - 15 - 2;
		sftd_draw_wtextf_white(x, (y += 15), L"%S   %S", data->text(BankText::HiddenPower), vPkm.hpType);
		sftd_draw_wtext_white(x, (y += 15), data->text(BankText::Moves));
		sftd_draw_wtext_white(x+10, (y += 15), vPkm.moves[0]);
		sftd_draw_wtext_white(x+10, (y += 15), vPkm.moves[1]);
		sftd_draw_wtext_white(x+10, (y += 15), vPkm.moves[2]);
		sftd_draw_wtext_white(x+10, (y += 15), vPkm.moves[3]);

		// Draw the Pokémon icon and the egg icon if so
		drawViewPokemon(&vPkm, 256, 36);

		// Show Wurmple's final evolution indicator
		if (pkm->speciesID == 265)
		{
			sf2d_texture* pkmIcons = (pkm->isShiny ? PHBanku::texture->pkmShinyIcons : PHBanku::texture->pkmIcons);

			// If < 5 then Beautifly else Dustox
			u16 species = ((Pokemon::encryptionKey(pkm) >> 16) % 10 < 5 ? 267 : 269);

			sf2d_draw_texture_part(pkmIcons, 365, 27, (species % 25) * 40, (species / 25) * 30, 40, 30);
		}

		if (pkm->isShiny)
		{
			sf2d_draw_texture_part(PHBanku::texture->boxTiles, 240, 135, 54, 64, 9, 9);
		}

		if (vPkm.isCured)
		{
			sf2d_draw_texture_part(PHBanku::texture->boxTiles, 260, 135, 72, 64, 9, 9);
		}
		else if (vPkm.isInfected)
		{
			sf2d_draw_texture_part(PHBanku::texture->boxTiles, 260, 135, 81, 64, 27, 9);
		}

		if (vPkm.gen > 2)
		{
			if (vPkm.gen == 6) // Kalos Born (Gen VI)
				sf2d_draw_texture_part(PHBanku::texture->boxTiles, 250, 135, 63, 64, 9, 9);
			else if (vPkm.gen == 5) // Gen V
				sf2d_draw_texture_part(PHBanku::texture->boxTiles, 250, 135, 72, 73, 9, 9);
			else if (vPkm.gen == 4) // Gen IV
				sf2d_draw_texture_part(PHBanku::texture->boxTiles, 250, 135, 63, 73, 9, 9);
			else if (vPkm.gen == 3) // Gen III
				sf2d_draw_texture_part(PHBanku::texture->boxTiles, 250, 135, 54, 73, 9, 9);
		}

		sf2d_draw_texture_part(PHBanku::texture->boxTiles, 290 ,135, 9*0, 64 + 9*vPkm.circle, 9, 9);
		sf2d_draw_texture_part(PHBanku::texture->boxTiles, 300 ,135, 9*1, 64 + 9*vPkm.triangle, 9, 9);
		sf2d_draw_texture_part(PHBanku::texture->boxTiles, 310 ,135, 9*2, 64 + 9*vPkm.square, 9, 9);
		sf2d_draw_texture_part(PHBanku::texture->boxTiles, 320 ,135, 9*3, 64 + 9*vPkm.heart, 9, 9);
		sf2d_draw_texture_part(PHBanku::texture->boxTiles, 330 ,135, 9*4, 64 + 9*vPkm.star, 9, 9);
		sf2d_draw_texture_part(PHBanku::texture->boxTiles, 340 ,135, 9*5, 64 + 9*vPkm.diamond, 9, 9);

		switch (Pokemon::gender(pkm))
		{
			case 0: // Male
				sf2d_draw_texture_part(PHBanku::texture->boxTiles, 385, 130, 60, 82, 12, 12);
				break;
			case 1: // Female
				sf2d_draw_texture_part(PHBanku::texture->boxTiles, 388, 130, 72, 82, 12, 12);
				break;
		}
	}
	else
	{
		sftd_draw_wtextf_white(91, 40, L"%S (%lu)", save->savedata.OTName, save->savedata.TSV);
	}

	if (hasOverlayChild()) { this->child->drawTopScreen(); }
	return SUCCESS_STEP;
}


// --------------------------------------------------
Result BoxViewer::drawBotScreen()
// --------------------------------------------------
{
	if (hasRegularChild()) { if (this->child->drawBotScreen() == PARENT_STEP); else return CHILD_STEP; }

	{
		// Retrieve the current box, and the drawing offset.
		s16 boxShift = cursorBox.inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED;
		s16 middleBoxShift = (cursorBox.inBank ? PC_BOX_SHIFT_UNUSED : PC_BOX_SHIFT_USED) + BACKGROUND_WIDTH;

		// Draw the current box: the background and the icons.
		drawBox((cursorBox.inBank ? vBKBox : vPCBox), boxShift, 0, true);

		// Draw the other box: the background and the icons.
		drawBox((cursorBox.inBank ? vPCBox : vBKBox), (cursorBox.inBank ? PC_BOX_SHIFT_UNUSED : BK_BOX_SHIFT_UNUSED), 0, false);

		// Draw bottom bar
		sf2d_draw_texture_part(PHBanku::texture->boxBackgrounds, (cursorBox.inBank ? PC_BOX_SHIFT_UNUSED : PC_BOX_SHIFT_USED), BACKGROUND_HEIGHT, 440, 968, 480, 30);
		sf2d_draw_texture_part(PHBanku::texture->boxBackgrounds, middleBoxShift, 0, 880, 758, 40, BACKGROUND_HEIGHT);

		// Draw SwapBox button
		sf2d_draw_texture_part(PHBanku::texture->boxTiles, middleBoxShift + 10, 215, 128, 32, 20, 20);

		// If a Pokémon is currently selected
		if (sPkm)
		{
			u16 pkm_x, pkm_y;

			// If the selected Pokémon is dragged
			if (isPkmDragged)
			{
				// Draw dragged Pokémon icon under the stylus
				pkm_x = touch.px - 20; // 40/2
				pkm_y = touch.py - 15; // 30/2
			}
			// If the selected Pokémon is held
			else // if (isPkmHeld)
			{
				if (cursorBox.inslot == SLOT_NO_SELECTION)
				{
					// Draw the Pokémon icon on the box title
					pkm_x = boxShift + (BACKGROUND_WIDTH - PKM_WIDTH) / 2;
					pkm_y = 10;
				}
				else
				{
					// Draw the Pokémon icon on the current slot a bit shifted
					pkm_x = boxShift + cursorBox.col * PKM_WIDTH;
					pkm_y = PKM_BOX_MARGIN_TOP - 12 + cursorBox.row * PKM_HEIGHT;
				}
			}

			drawPokemon(sPkm, pkm_x, pkm_y, true);

			if (!isPkmDragged)
			{
				sf2d_draw_texture_part(PHBanku::texture->boxTiles, pkm_x + 20, pkm_y - 11, (u8) cursorType * 32, 32, 32, 32);
			}
		}
		else
		{
			// Draw the cursor
			if (cursorBox.inslot == SLOT_NO_SELECTION)
			{
				// Draw the cursor icon on the box title
				sf2d_draw_texture_part(PHBanku::texture->boxTiles, boxShift + 105, 3 - cursorPositionOffY, (u8) cursorType * 32, 32, 32, 32);
			}
			else
			{
				// Draw the cursor icon on the current slot a bit shifted
				sf2d_draw_texture_part(PHBanku::texture->boxTiles, boxShift + 16 + (cursorBox.inslot % 6) * PKM_WIDTH + cursorPositionOffY / 2, PKM_BOX_MARGIN_TOP - 7 + (cursorBox.inslot / 6) * PKM_HEIGHT - cursorPositionOffY, (u8) cursorType * 32, 32, 32, 32);
			}
		}

		if (sPkmCount > 0)
		{
			// Draw the selected Pokémon
			for (u8 i = 0; i < 19 && i < sPkmCount; i++)
			{
				drawPokemon(sPkms[i], middleBoxShift, 10 * i, false);
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

	// TODO: Detect if mediatype and the cartridge has been removed!
	// Result FSUSER_CardSlotIsInserted(bool* inserted)

	if (kDown & KEY_START)
	{
		// Open the Savexit popup
		new SavexitViewer(ViewType::Overlay, this);
		child->initialize();
		return CHILD_STEP;
	}

	bool bboxMod = false;

	{
		bool boolMod = false;
		s16 boxMod = 0;
		s16 rowMod = 0;
		s16 colMod = 0;

		if (kDown & KEY_L)
		{
			boxMod--;
			heldTick.KEY_L = svcGetSystemTick() + HELD_TICK * 2;
		}
		else if (kHeld & KEY_L && heldTick.KEY_L + HELD_TICK < svcGetSystemTick())
		{
			boxMod--;
			heldTick.KEY_L = svcGetSystemTick();
		}
		if (kDown & KEY_R)
		{
			boxMod++;
			heldTick.KEY_R = svcGetSystemTick() + HELD_TICK * 2;
		}
		else if (kHeld & KEY_R && heldTick.KEY_R + HELD_TICK < svcGetSystemTick())
		{
			boxMod++;
			heldTick.KEY_R = svcGetSystemTick();
		}

		if (kDown & KEY_UP)
		{
			rowMod--;
			heldTick.KEY_UP = svcGetSystemTick() + HELD_TICK * 2;
		}
		else if (kHeld & KEY_UP && heldTick.KEY_UP + HELD_TICK < svcGetSystemTick())
		{
			rowMod--;
			heldTick.KEY_UP = svcGetSystemTick();
		}
		if (kDown & KEY_DOWN)
		{
			rowMod++;
			heldTick.KEY_DOWN = svcGetSystemTick() + HELD_TICK * 2;
		}
		else if (kHeld & KEY_DOWN && heldTick.KEY_DOWN + HELD_TICK < svcGetSystemTick())
		{
			rowMod++;
			heldTick.KEY_DOWN = svcGetSystemTick();
		}

		if (kDown & KEY_LEFT)
		{
			if (cursorBox.row == BOX_HEADER_SELECTED)
				boxMod--;
			else
				colMod--;
			heldTick.KEY_LEFT = svcGetSystemTick() + HELD_TICK * 2;
		}
		else if (kHeld & KEY_LEFT && heldTick.KEY_LEFT + HELD_TICK < svcGetSystemTick())
		{
			if (cursorBox.row == BOX_HEADER_SELECTED)
				boxMod--;
			else
				colMod--;
			heldTick.KEY_LEFT = svcGetSystemTick();
		}
		if (kDown & KEY_RIGHT)
		{
			if (cursorBox.row == BOX_HEADER_SELECTED)
				boxMod++;
			else
				colMod++;
			heldTick.KEY_RIGHT = svcGetSystemTick() + HELD_TICK * 2;
		}
		else if (kHeld & KEY_RIGHT && heldTick.KEY_RIGHT + HELD_TICK < svcGetSystemTick())
		{
			if (cursorBox.row == BOX_HEADER_SELECTED)
				boxMod++;
			else
				colMod++;
			heldTick.KEY_RIGHT = svcGetSystemTick();
		}

		if (kDown & KEY_ZL) { cursorBox.inBank = false; boolMod = true; }
		else if (kDown & KEY_ZR) { cursorBox.inBank = true; boolMod = true; }

		if (kDown & KEY_TOUCH)
		{
			s16 boxShift = cursorBox.inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED;

			// If the box arrows are touched down
			if (touchWithin(touch->px, touch->py, boxShift + 4, 18, 16, 24)) boxMod--;
			else if (touchWithin(touch->px, touch->py, boxShift + 200, 18, 16, 24)) boxMod++;

			boxShift = cursorBox.inBank ? PC_BOX_SHIFT_UNUSED : BK_BOX_SHIFT_UNUSED;

			// If the other box (PC|BK) is touched down
			if (touchWithin(touch->px, touch->py, boxShift, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT)) { cursorBox.inBank = !cursorBox.inBank; boolMod = bboxMod = true; }
		}
		else if (kHeld & KEY_TOUCH)
		{
			if (sPkm || isPkmChecking)
			{
				s16 boxShift = cursorBox.inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED;

				// If the box arrows are touched held once
				if (touchWithin(touch->px, touch->py, boxShift + 4, 18, 16, 24) && !touchWithin(this->touch.px, this->touch.py, boxShift + 4, 18, 16, 24)) boxMod--;
				else if (touchWithin(touch->px, touch->py, boxShift + 200, 18, 16, 24) && !touchWithin(this->touch.px, this->touch.py, boxShift + 200, 18, 16, 24)) boxMod++;

				boxShift = cursorBox.inBank ? PC_BOX_SHIFT_UNUSED : BK_BOX_SHIFT_UNUSED;

				// If the other box (PC|BK) is touched held
				if (touchWithin(touch->px, touch->py, boxShift, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT)) { cursorBox.inBank = !cursorBox.inBank; boolMod = bboxMod = true; }
			}
		}

		if (boxMod || rowMod || colMod)
		{
			currentBox(&cursorBox);
			*cursorBox.box += boxMod;
			cursorBox.row += rowMod;
			cursorBox.col += colMod;

			if (save->bankdata.bk.wboxUnlocked)
			{
				if (*cursorBox.box < 0) *cursorBox.box = (cursorBox.inBank ? save->bankdata.bk.boxUnlocked+1 : save->savedata.pc.boxUnlocked)-1;
				else if (*cursorBox.box > (cursorBox.inBank ? save->bankdata.bk.boxUnlocked+1 : save->savedata.pc.boxUnlocked)-1) *cursorBox.box = 0;
			}
			else
			{
				if (*cursorBox.box < 0) *cursorBox.box = (cursorBox.inBank ? save->bankdata.bk.boxUnlocked : save->savedata.pc.boxUnlocked)-1;
				else if (*cursorBox.box > (cursorBox.inBank ? save->bankdata.bk.boxUnlocked : save->savedata.pc.boxUnlocked)-1) *cursorBox.box = 0;
			}

			if (cursorBox.row < BOX_HEADER_SELECTED) cursorBox.row = BOX_ROW_PKM_COUNT-1;
			else if (cursorBox.row > BOX_ROW_PKM_COUNT-1) cursorBox.row = BOX_HEADER_SELECTED;

			if (cursorBox.col < 0) { cursorBox.col = BOX_COL_PKM_COUNT-1; cursorBox.inBank = !cursorBox.inBank; }
			else if (cursorBox.col > BOX_COL_PKM_COUNT-1) { cursorBox.col = 0; cursorBox.inBank = !cursorBox.inBank; }

			boolMod = true;
		}

		if (boolMod)
		{
			// Update the current Box/Pokémon displayed
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
			// Move a Pokémon (grab and drop)
			selectMovePokemon();
		}

		if (kDown & KEY_B && !isPkmDragged)
		{
			// Drop a Pokémon
			cancelMovePokemon();
		}

		if (kDown & KEY_Y)
		{
			// Open the UltraBox viewer
			UltraBoxViewer* ultraBoxViewer = new UltraBoxViewer(ViewType::Regular, this);
			ultraBoxViewer->selectViewBox(*cursorBox.box, cursorBox.inBank);
			child->initialize();
			return CHILD_STEP;
		}
	}
	else if (cursorType == CursorType::QuickSelect)
	{
		if (kDown & KEY_A && !isPkmDragged)
		{
			// Move a Pokémon (grab and drop)
			selectMovePokemon();
		}

		if (kDown & KEY_B && !isPkmDragged)
		{
			// Drop a Pokémon
			cancelMovePokemon();
		}

		if (kDown & KEY_Y && !sPkm)
		{
			// Swap the two boxes (PC|BK)
			save->moveBox(cursorBox.boxPC, false, cursorBox.boxBK, true);
		}
	}
	else if (cursorType == CursorType::MultiSelect)
	{
		if (kDown & KEY_A)
		{
			// Toggle a Pokémon (check & move)
			selectMultiMovePokemon(true);

			if (vPkm.pkm)
			{
				checkToggle = vPkm.pkm->checked;
				isPkmChecking = true;
			}
		}
		else if (kHeld & KEY_A)
		{
			if (vPkm.pkm && vPkm.pkm->checked != checkToggle)
			{
				// Toggle a Pokémon (check & move)
				selectMultiMovePokemon(true);
			}
		}

		if (kDown & KEY_B)
		{
			// Cancel the checked
			cancelMovePokemon();
		}

		if (kDown & KEY_X)
		{
			// Move the checked Pokémon to the current box
			selectMultiMovePokemon(false);
		}

		if (kDown & KEY_Y)
		{
			// Open the UltraBox viewer
			UltraBoxViewer* ultraBoxViewer = new UltraBoxViewer(ViewType::Regular, this);
			ultraBoxViewer->selectViewBox(*cursorBox.box, cursorBox.inBank);
			child->initialize();
			return CHILD_STEP;
		}
	}
	else if (cursorType == CursorType::DevSelect)
	{
		if (kDown & KEY_A)
		{
			// Paste a Pokémon (copy & paste)
			selectPastePokemon();
		}

		if (kDown & KEY_B)
		{
			// Drop a Pokémon
			cancelMovePokemon();
		}

		if (kDown & KEY_X)
		{
			// Paste the checked Pokémon to the current box
			selectMultiPastePokemon(false);
		}

		if (kDown & KEY_Y)
		{
			// Toggle a Pokémon (check & move)
			selectMultiPastePokemon(true);

			if (vPkm.pkm)
			{
				checkToggle = vPkm.pkm->checked;
				isPkmChecking = true;
			}
		}
		else if (kHeld & KEY_Y)
		{
			if (vPkm.pkm && vPkm.pkm->checked != checkToggle)
			{
				// Toggle a Pokémon (check & move)
				selectMultiPastePokemon(true);
			}
		}
	}

	{
		if (bboxMod); // Cancel selection if the (PC|BK) changed
		else if (kDown & KEY_TOUCH)
		{
			s16 boxShift = cursorBox.inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED;
			s16 middleBoxShift = (cursorBox.inBank ? PC_BOX_SHIFT_UNUSED : PC_BOX_SHIFT_USED) + BACKGROUND_WIDTH;
			u16 px = touch->px;
			u16 py = touch->py;
			this->touch = *touch;

			if (!sPkm && sPkmCount == 0)
			{
				// If the TouchArea is within the SingleSelect CursorType button area
				if (touchWithin(px, py, boxShift + 22, 0, 59, 16))
				{
					selectCursorType(CursorType::SingleSelect);
				}
				// If the TouchArea is within the QuickSelect CursorType button area
				else if (touchWithin(px, py, boxShift + 86, 0, 59, 16))
				{
					selectCursorType(CursorType::QuickSelect);
				}
				// If the TouchArea is within the MultiSelect CursorType button area
				else if (touchWithin(px, py, boxShift + 150, 0, 59, 16))
				{
					selectCursorType(CursorType::MultiSelect);
				}
				// If the TouchArea is within the SwapBox button area
				else if (touchWithin(px, py, middleBoxShift + 10, 215, 20, 20))
				{
					// Swap the two boxes (PC|BK)
					save->moveBox(cursorBox.boxPC, false, cursorBox.boxBK, true);
				}
			}
			else
			{
				if (cursorType == CursorType::MultiSelect)
				{
					// If the TouchArea is within the MultiSelect CursorType button area
					if (touchWithin(px, py, boxShift + 150, 0, 59, 16))
					{
						cancelMovePokemon();
					}
					// If the TouchArea is within the MiddleArea
					else if (touchWithin(px, py, middleBoxShift, 0, BACKGROUND_SHIFT, BACKGROUND_HEIGHT))
					{
						// Trigger the end of the selection
						selectMultiMovePokemon(false);
					}
				}
			}

			// If the TouchArea is within the Pokémon icons area of the box
			if (touchWithin(px, py, boxShift, PKM_BOX_MARGIN_TOP, BACKGROUND_WIDTH - PKM_BOX_MARGIN_RIGHT, BACKGROUND_HEIGHT - PKM_BOX_MARGIN_TOP))
			{
				// If no Pokémon is currently selected or dragged
				if (!sPkm && !isPkmDragged)
				{
					// Move the cursor to the new slot
					cursorBox.row = (py - PKM_BOX_MARGIN_TOP) / PKM_HEIGHT;
					cursorBox.col = (px - boxShift) / PKM_WIDTH;

					// Update the current Pokémon
					selectViewPokemon();

					if (cursorType == CursorType::MultiSelect)
					{
						// Select the current Pokémon (check)
						selectMultiMovePokemon(true);

						if (vPkm.pkm)
						{
							checkToggle = vPkm.pkm->checked;
							isPkmChecking = true;
						}
					}
					else
					{
						// Move the current Pokémon (grab)
						selectMovePokemon();

						if (!vPkm.emptySlot)
						{
							isPkmDragged = true;
						}
					}
				}
				// If a Pokémon is currently selected and dragged
				else
				{
					u16 oldRow = cursorBox.row;
					u16 oldCol = cursorBox.col;

					// Move the cursor to the new slot
					cursorBox.row = (py - PKM_BOX_MARGIN_TOP) / PKM_HEIGHT;
					cursorBox.col = (px - boxShift) / PKM_WIDTH;

					// Update the current Pokémon
					selectViewPokemon();

					// And drop the Pokémon if one is held and it is the same current slot (mean double tap to move a held Pokémon)
					if (isPkmHeld && oldRow == cursorBox.row && oldCol == cursorBox.col)
					{
						selectMovePokemon();
					}
				}
			}
		}
		else if (kHeld & KEY_TOUCH)
		{
			if (cursorType == CursorType::MultiSelect)
			{
				s16 boxShift = cursorBox.inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED;
				u16 px = touch->px;
				u16 py = touch->py;

				// If the TouchArea is within the Pokémon icons area of the box
				if (touchWithin(px, py, boxShift, PKM_BOX_MARGIN_TOP, BACKGROUND_WIDTH - PKM_BOX_MARGIN_RIGHT, BACKGROUND_HEIGHT - PKM_BOX_MARGIN_TOP))
				{
					s16 row = (py - PKM_BOX_MARGIN_TOP) / PKM_HEIGHT;
					s16 col = (px - boxShift) / PKM_WIDTH;

					// If the touch position is not the same slot as before.
					if (cursorBox.row != row || cursorBox.col != col)
					{
						// Move the cursor to the new slot
						cursorBox.row = row;
						cursorBox.col = col;

						// Update the current Pokémon
						selectViewPokemon();

						// If it has to be toggled.
						if (vPkm.pkm->checked != checkToggle)
						{
							// Select the current Pokémon (check)
							selectMultiMovePokemon(true);
						}
					}
				}
			}

			// Keep the last touched position
			this->touch = *touch;
		}
		else if (kUp & KEY_TOUCH)
		{
			touch = &(this->touch);

			s16 boxShift = cursorBox.inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED;
			u16 px = touch->px;
			u16 py = touch->py;

			// If a Pokémon is currently dragged
			if (sPkm && isPkmDragged)
			{
				// If the TouchArea is within the Pokémon icons area of the box
				if (touchWithin(px, py, boxShift, PKM_BOX_MARGIN_TOP, BACKGROUND_WIDTH - PKM_BOX_MARGIN_RIGHT, BACKGROUND_HEIGHT - PKM_BOX_MARGIN_TOP))
				{
					// Move the cursor to the new slot
					cursorBox.row = (py - PKM_BOX_MARGIN_TOP) / PKM_HEIGHT;
					cursorBox.col = (px - boxShift) / PKM_WIDTH;

					// Update the current Pokémon
					selectViewPokemon();

					// Move the dragged Pokémon (drop)
					selectMovePokemon();
				}
				else
				{
					// Cancel the Pokémon dragging
					cancelMovePokemon();

					// Return to the old dragged Pokémon slot?
					// injectBoxSlot(&sSlot, &cursorBox);
					// selectViewBox();

					// Update the current Pokémon
					selectViewPokemon();
				}

				isPkmDragged = false;
			}

			isPkmChecking = false;
		}
	}

	cursorPositionOffY += cursorPositionShiftY;
	if (cursorPositionOffY < 0 || cursorPositionOffY > cursorPositionMaxY)
		cursorPositionShiftY *= -1;

	return SUCCESS_STEP;
}


// --------------------------------------------------
void BoxViewer::selectViewBox(uint16_t boxId, bool inBank)
// --------------------------------------------------
{
	// Change a box ID and update both displayed boxes (PC|BK)

	bool BK = cursorBox.inBank;
	cursorBox.inBank = inBank;

	(inBank ? cursorBox.boxBK : cursorBox.boxPC) = boxId;
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
void BoxViewer::drawBox(box_s* box, int16_t x, int16_t y, bool cursor)
// --------------------------------------------------
{
	// Draw the box background
	sf2d_draw_texture_part(PHBanku::texture->boxBackgrounds, x, y, ((box->background % 16) % 4) * BACKGROUND_WIDTH, ((box->background % 16) / 4) * BACKGROUND_HEIGHT, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);

	if (cursor)
	{
		// Draw CursorType buttons (Red|Blue|Green)
		drawCursorButton(x, y);
	}

	// Draw the box header
	sf2d_draw_texture_part(PHBanku::texture->boxBackgrounds, x, y + 15, ((box->background % 16) % 4) * BOX_HEADER_WIDTH, 840 + ((box->background % 16) / 4) * BOX_HEADER_HEIGHT, BOX_HEADER_WIDTH, BOX_HEADER_HEIGHT);

	// Draw the box arrows
	sf2d_draw_texture_part(PHBanku::texture->boxTiles, x + 4, y + 18, 96, 32, 16, 24);
	sf2d_draw_texture_part(PHBanku::texture->boxTiles, x + 200, y + 18, 112, 32, 16, 24);

	// Draw the box title
	wchar_t boxTitle[0x1a];
	// TODO: Remove that if statement when complete box->title.
	// TODO: Wait for sftd_get_wtext_wdith.
	if (box->title[0] != '\0') swprintf(boxTitle, 0x1a, (wchar_t*) box->title);
	else swprintf(boxTitle, 0x1a, L"Box %i", box->number+1);
	int boxTitleWidth = sftd_get_wtext_width(font, 12, boxTitle);
	sftd_draw_wtext(font, x + (BACKGROUND_WIDTH - boxTitleWidth) / 2, y + 21, RGBA8(0x22,0x22,0x22,0xFF), 12, boxTitle);

	// TODO: v DRY v

	// If there is a Pokémon currently selected
	if (sPkm)
	{
		// Draw Pokémon icons
		for (u8 i = 0; i < BOX_PKM_COUNT; i++)
		{
			// If the Pokémon isn't the selected Pokémon
			if (sPkm != &(box->slot[i]))
			{
				drawPokemon(&box->slot[i], x + (i % BOX_COL_PKM_COUNT) * PKM_WIDTH, y + PKM_BOX_MARGIN_TOP + (i / BOX_COL_PKM_COUNT) * PKM_HEIGHT, false);
			}
		}
	}
	// If there is no Pokémon currently selected
	else
	{
		// Draw Pokémon icons
		for (u8 i = 0; i < BOX_PKM_COUNT; i++)
		{
			drawPokemon(&box->slot[i], x + (i % BOX_COL_PKM_COUNT) * PKM_WIDTH, y + PKM_BOX_MARGIN_TOP + (i / BOX_COL_PKM_COUNT) * PKM_HEIGHT, false);
		}
	}
}


// --------------------------------------------------
void BoxViewer::drawPokemon(pkm_s* pkm, int16_t x, int16_t y, bool shadow)
// --------------------------------------------------
{
	if (pkm->speciesID == 0) return;

	// sf2d_draw_rectangle(x, y, PKM_WIDTH, PKM_HEIGHT, RGBA8(0x00,0x00,0x00,0x22));

	u16 widthCount = 25;
	u16 sprite = pkm->speciesID;
	sf2d_texture* pkmIcons = PHBanku::texture->pkmIcons;

	// If a particular form and the species isn't Scatterbug/Spewpa (that is ugly tho)
	if (pkm->formID > 0 && pkm->speciesID != 664 && pkm->speciesID != 665)
	{
		if (pkm->isShiny) pkmIcons = PHBanku::texture->pkmShinyFormIcons;
		else pkmIcons = PHBanku::texture->pkmFormIcons;

		widthCount = 12;
		sprite = Personal(pkm->speciesID).formSprite + pkm->formID - 1;
	}
	// If female form with Pyroar/Frillish/Jellicent/Unfezant
	else if (pkm->gender == 1 && (pkm->speciesID == 521 || pkm->speciesID == 592 || pkm->speciesID == 593 || pkm->speciesID == 668))
	{
		if (pkm->isShiny) pkmIcons = PHBanku::texture->pkmShinyFormIcons;
		else pkmIcons = PHBanku::texture->pkmFormIcons;

		widthCount = 12;
		sprite = 173 +
			(pkm->speciesID == 521 ? 1 :
			(pkm->speciesID == 592 ? 2 :
			(pkm->speciesID == 593 ? 3 :
			(pkm->speciesID == 668 ? 4 :
			0))));
	}
	else if (pkm->isShiny) pkmIcons = PHBanku::texture->pkmShinyIcons;

	// Draw the shadow
	if (shadow)
	{
		sf2d_draw_texture_part_blend(pkmIcons, x + 4, y + 4, (sprite % widthCount) * 40, (sprite / widthCount) * 30, 40, 30, RGBA8(0x00,0x00,0x00,0x55));
	}

	if (pkm->isEggy)
	{
		// Draw the egg+Pokémon icon
		sf2d_draw_texture_part_blend(pkmIcons, x, y, (sprite % widthCount) * 40, (sprite / widthCount) * 30, 40, 30, RGBA8(0xFF,0xFF,0xFF,0xAA));
		sf2d_draw_texture_part_blend(pkmIcons, x, y, (EGG_ID % widthCount) * 40, (EGG_ID / widthCount) * 30, 40, 30, RGBA8(0xFF,0xFF,0xFF,0x88));
	}
	else
	{
		// Draw the Pokémon icon
		sf2d_draw_texture_part(pkmIcons, x, y, (sprite % widthCount) * 40, (sprite / widthCount) * 30, 40, 30);

		// Draw the item
		if (pkm->itemID > 0)
		{
			sf2d_draw_texture_part(PHBanku::texture->boxTiles, x + 23, y + 21, 48, 82, 12, 12);
		}
	}

	// Draw the checkbox
	if (pkm->checked)
	{
		sf2d_draw_texture_part(PHBanku::texture->boxTiles, x + 3, y + 22, 96, 56, 8, 8);
	}
}


// --------------------------------------------------
void BoxViewer::drawPokemonScale(pkm_s* pkm, int16_t x, int16_t y, float scale)
// --------------------------------------------------
{
	if (pkm->speciesID == 0) return;

	u16 widthCount = 25;
	u16 sprite = pkm->speciesID;
	sf2d_texture* pkmIcons = PHBanku::texture->pkmIcons;

	// If a particular form and the species isn't Scatterbug/Spewpa (that is ugly tho)
	if (pkm->formID > 0 && pkm->speciesID != 664 && pkm->speciesID != 665)
	{
		if (pkm->isShiny) pkmIcons = PHBanku::texture->pkmShinyFormIcons;
		else pkmIcons = PHBanku::texture->pkmFormIcons;

		widthCount = 12;
		sprite = Personal(pkm->speciesID).formSprite + pkm->formID - 1;
	}
	// If female form with Pyroar/Frillish/Jellicent/Unfezant
	else if (pkm->gender == 1 && (pkm->speciesID == 521 || pkm->speciesID == 592 || pkm->speciesID == 593 || pkm->speciesID == 668))
	{
		if (pkm->isShiny) pkmIcons = PHBanku::texture->pkmShinyFormIcons;
		else pkmIcons = PHBanku::texture->pkmFormIcons;

		widthCount = 12;
		sprite = 173 +
			(pkm->speciesID == 521 ? 1 :
			(pkm->speciesID == 592 ? 2 :
			(pkm->speciesID == 593 ? 3 :
			(pkm->speciesID == 668 ? 4 :
			0))));
	}
	else if (pkm->isShiny) pkmIcons = PHBanku::texture->pkmShinyIcons;

	// Draw the shadow
	{
		sf2d_draw_texture_part_scale_blend(pkmIcons, x + 2 * scale, y + 2 * scale, (sprite % widthCount) * 40, (sprite / widthCount) * 30, 40, 30, scale, scale, RGBA8(0x00,0x00,0x00,0x55));
	}

	if (pkm->isEggy)
	{
		// Draw the egg+Pokémon icon
		// TODO: Enhance the egg display
		sf2d_draw_texture_part_scale(pkmIcons, x, y, (sprite % widthCount) * 40, (sprite / widthCount) * 30, 40, 30, scale, scale);
		sf2d_draw_texture_part(pkmIcons, x, y + 30 * (scale - 0.5f), (EGG_ID % widthCount) * 40, (EGG_ID / widthCount) * 30, 40, 30);
	}
	else
	{
		// Draw the Pokémon icon
		sf2d_draw_texture_part_scale(pkmIcons, x, y, (sprite % widthCount) * 40, (sprite / widthCount) * 30, 40, 30, scale, scale);

		// Draw the item
		if (pkm->itemID > 0)
		{
			// TODO: Enhance the item display
			sf2d_draw_texture_part(PHBanku::texture->itemIcons, x + 5, y + 30 * (scale - 0.5f), (pkm->itemID % 34) * 30, (pkm->itemID / 34) * 30, 30, 30);
		}
	}
}

// --------------------------------------------------
void BoxViewer::drawViewPokemon(vPkm_s* vPkm, int16_t x, int16_t y)
// --------------------------------------------------
{
	if (!vPkm->pkm || vPkm->pkm->speciesID == 0) return;

	// If the big icon is loaded
	if (vPkm->icon)
	{
		// TODO: Draw the Pokémon's form

		sf2d_draw_texture_blend(vPkm->icon, x + 18, y + 2, RGBA8(0x00,0x00,0x00,0x55));
		sf2d_draw_texture(vPkm->icon, x + 16, y);

		if (vPkm->pkm->isEggy)
		{
			sf2d_draw_texture_part(vPkm->pkm->isShiny ? PHBanku::texture->pkmShinyIcons : PHBanku::texture->pkmIcons, x, y + 75, (EGG_ID % 25) * 40, (EGG_ID / 25) * 30, 40, 30);
		}
		else if (vPkm->pkm->itemID > 0)
		{
			sf2d_draw_texture_part(PHBanku::texture->itemIcons, x + 5, y + 75, (vPkm->pkm->itemID % 34) * 30, (vPkm->pkm->itemID / 34) * 30, 30, 30);
		}
	}
	else
	{
		drawPokemonScale(vPkm->pkm, x, y, 3.0f);
	}
}

// --------------------------------------------------
void BoxViewer::drawCursorButton(int16_t x, int16_t y)
// --------------------------------------------------
{
	if (!sPkm && sPkmCount == 0)
	{
		sf2d_draw_texture_part(PHBanku::texture->boxTiles, x + 21 +   0, y,   0, 0, 50, 32);
		sf2d_draw_texture_part(PHBanku::texture->boxTiles, x + 21 +  64, y,  64, 0, 50, 32);
		sf2d_draw_texture_part(PHBanku::texture->boxTiles, x + 21 + 128, y, 128, 0, 50, 32);
	}
	else
	{
		sf2d_draw_texture_part_blend(PHBanku::texture->boxTiles, x + 21 +   0, y,   0, 0, 50, 32, RGBA8(0xCC,0xCC,0xCC,0xAA));
		sf2d_draw_texture_part_blend(PHBanku::texture->boxTiles, x + 21 +  64, y,  64, 0, 50, 32, RGBA8(0xCC,0xCC,0xCC,0xAA));

		if (cursorType == CursorType::MultiSelect)
			sf2d_draw_texture_part(PHBanku::texture->boxTiles, x + 21 + 128, y, 128, 0, 64, 32);
		else
			sf2d_draw_texture_part_blend(PHBanku::texture->boxTiles, x + 21 + 128, y, 128, 0, 50, 32, RGBA8(0xCC,0xCC,0xCC,0xAA));
	}
}


// --------------------------------------------------
bool BoxViewer::isWonderBox(u16 boxId, bool inBank)
// --------------------------------------------------
{
	return save->bankdata.bk.wboxUnlocked && inBank ? boxId == save->bankdata.bk.boxUnlocked : false;
}


// --------------------------------------------------
void BoxViewer::selectCursorType(CursorType cursorType)
// --------------------------------------------------
{
	this->cursorType = cursorType;
	cancelMovePokemon();
}


// --------------------------------------------------
void BoxViewer::switchCursorType()
// --------------------------------------------------
{
	// ... -> Single -> Quick -> Multi -> ... //
	if (cursorType == CursorType::SingleSelect)
		selectCursorType(CursorType::QuickSelect);
	else if (cursorType == CursorType::QuickSelect)
		selectCursorType(CursorType::MultiSelect);
	else if (cursorType == CursorType::MultiSelect)
		selectCursorType(save->dev ? CursorType::DevSelect : CursorType::SingleSelect);
	else if (cursorType == CursorType::DevSelect)
		selectCursorType(CursorType::SingleSelect);
}


// --------------------------------------------------
void BoxViewer::selectViewBox()
// --------------------------------------------------
{
	// Compute the current cursor slot
	computeSlot(&cursorBox);

	// Wonder box
	if (isWonderBox(*cursorBox.box, cursorBox.inBank))
	{
		vBKBox = save->getWBox();
	}
	else
	{
		(cursorBox.inBank ? vBKBox : vPCBox) = save->getBox(*cursorBox.box, cursorBox.inBank);
	}
}


// --------------------------------------------------
void BoxViewer::selectViewPokemon()
// --------------------------------------------------
{
	// Compute the current cursor slot
	computeSlot(&cursorBox);

	// If the cursor isn't in a box slot
	if (cursorBox.slot == SLOT_NO_SELECTION)
	{
		vPkm.pkm = NULL;
	}
	// If the cursor is in a box slot
	else
	{
		// Wonder box
		if (isWonderBox(*cursorBox.box, cursorBox.inBank))
		{
			vPkm.pkm = save->getWPkm(cursorBox.inslot);
		}
		else
		{
			vPkm.pkm = save->getPkm(*cursorBox.box, cursorBox.inslot, cursorBox.inBank);
		}

		populateVPkmData(&vPkm, NULL);
	}
}


// --------------------------------------------------
void BoxViewer::selectMovePokemon()
// --------------------------------------------------
{
	// Compute the current cursor slot
	computeSlot(&cursorBox);

	// If no Pokémon is currently selected
	if (!sPkm)
	{
		// If the current Pokémon slot isn't empty (to avoid empty slot move)
		if (!vPkm.emptySlot)
		{
			// Select the current Pokémon
			sPkm = vPkm.pkm;
			extractBoxSlot(&sSlot, &cursorBox);

			// If the buttons are used.
			if (!isPkmDragged)
			{
				isPkmHeld = true;
			}
		}
	}
	// Else if there is a current Pokémon
	else if (vPkm.pkm)
	{
		// If the selected Pokémon isn't the current Pokémon
		if (sPkm != vPkm.pkm)
		{
			// Swap the selected Pokémon and the current Pokémon, and keep the return value (true: had moved, false: hadn't)
			bool moved = save->movePkm(sPkm, vPkm.pkm, sSlot.inBank, cursorBox.inBank);

			// If the Pokémon had moved
			if (moved)
			{
				// Cancel the selection
				cancelMovePokemon();

				// And populate the Pokémon data
				populateVPkmData(&vPkm, NULL);
			}
		}
		else
		{
			// Cancel the selection, since it's moved to the same slot
			cancelMovePokemon();
		}
	}
}


// --------------------------------------------------
void BoxViewer::selectMultiMovePokemon(bool check)
// --------------------------------------------------
{
	// Compute the current cursor slot
	computeSlot(&cursorBox);

	// We check
	if (check)
	{
		// If there is a real Pokémon to check
		if (vPkm.pkm && !vPkm.emptySlot)
		{
			// If the Pokémon is already checked, uncheck it
			if (vPkm.pkm->checked)
			{
				u8 i;
				sPkmCount--;
				vPkm.pkm->checked = false;
				for (i = 0; sPkms[i] != vPkm.pkm; i++);
				for (; i < sPkmCount; i++) sPkms[i] = sPkms[i+1];
			}
			// If there is enough place in the checked list
			else if (sPkmCount < BOX_PKM_COUNT)
			{
				vPkm.pkm->checked = true;
				sPkms[sPkmCount] = vPkm.pkm;
				sPkmBanked[sPkmCount] = cursorBox.inBank;
				sPkmCount++;
			}
		}
	}
	// Else we move
	else
	{
		u8 vPkmCount = 0;
		box_s* box = (cursorBox.inBank ? vBKBox : vPCBox);

		// Count the free slots
		for (u8 i = 0; i < BOX_PKM_COUNT; i++)
		{
			if (save->isPkmEmpty(&box->slot[i]))
			{
				vPkmCount++;
			}
		}

		// If there is enough free slot
		if (vPkmCount >= sPkmCount)
		{
			// Move the checked Pokémon to the current box
			for (u8 i = 0, ii = 0; ii < sPkmCount; ii++)
			{
				// Find the next empty slot
				for (; !save->isPkmEmpty(&box->slot[i]) && i < BOX_PKM_COUNT; i++);
				save->movePkm(sPkms[ii], &box->slot[i], sPkmBanked[ii], cursorBox.inBank);
				box->slot[i].checked = false;
				i++;
			}

			cancelMovePokemon();
		}
	}
}


// --------------------------------------------------
void BoxViewer::selectPastePokemon()
// --------------------------------------------------
{
	// Compute the current cursor slot
	computeSlot(&cursorBox);

	// If no Pokémon is currently selected
	if (!sPkm)
	{
		// If the current Pokémon slot isn't empty (to avoid empty slot paste)
		if (!vPkm.emptySlot)
		{
			// Select the current Pokémon
			sPkm = vPkm.pkm;
			extractBoxSlot(&sSlot, &cursorBox);

			// If the buttons are used.
			if (!isPkmDragged)
			{
				isPkmHeld = true;
			}
		}
	}
	// Else if there is no real current Pokémon
	else if (vPkm.pkm && vPkm.emptySlot)
	{
		// Paste the selected Pokémon to the current slot, and keep the return value (true: was pasted, false: wasn't)
		bool pasted = save->pastePkm(sPkm, vPkm.pkm, sSlot.inBank, cursorBox.inBank);

		// If the Pokémon was pasted
		if (pasted)
		{
			// Cancel the selection..?
			// cancelMovePokemon();

			// And populate the Pokémon data
			populateVPkmData(&vPkm, NULL);
		}
	}
}


// --------------------------------------------------
void BoxViewer::selectMultiPastePokemon(bool check)
// --------------------------------------------------
{
	// Compute the current cursor slot
	computeSlot(&cursorBox);

	// We check
	if (check)
	{
		// If there is a real Pokémon to check
		if (vPkm.pkm && !vPkm.emptySlot)
		{
			// If the Pokémon is already checked, uncheck it
			if (vPkm.pkm->checked)
			{
				u8 i;
				sPkmCount--;
				vPkm.pkm->checked = false;
				for (i = 0; sPkms[i] != vPkm.pkm; i++);
				for (; i < sPkmCount; i++) sPkms[i] = sPkms[i+1];
			}
			// If there is enough place in the checked list
			else if (sPkmCount < BOX_PKM_COUNT)
			{
				vPkm.pkm->checked = true;
				sPkms[sPkmCount] = vPkm.pkm;
				sPkmBanked[sPkmCount] = cursorBox.inBank;
				sPkmCount++;
			}
		}
	}
	// Else we paste
	else
	{
		u8 vPkmCount = 0;
		box_s* box = (cursorBox.inBank ? vBKBox : vPCBox);

		// Count the free slots
		for (u8 i = 0; i < BOX_PKM_COUNT; i++)
		{
			if (save->isPkmEmpty(&box->slot[i]))
			{
				vPkmCount++;
			}
		}

		// If there is enough free slot
		if (vPkmCount >= sPkmCount)
		{
			// Paste the checked Pokémon to the current box
			for (u8 i = 0, ii = 0; ii < sPkmCount; ii++)
			{
				// Find the next empty slot
				for (; !save->isPkmEmpty(&box->slot[i]) && i < BOX_PKM_COUNT; i++);
				save->pastePkm(sPkms[ii], &box->slot[i], sPkmBanked[ii], cursorBox.inBank);
				box->slot[i].checked = false;
				i++;
			}

			cancelMovePokemon();
		}
	}
}


// --------------------------------------------------
void BoxViewer::cancelMovePokemon()
// --------------------------------------------------
{
	// Reset the selection
	isPkmHeld = false;
	isPkmDragged = false;
	isPkmChecking = false;

	// Reset the selected
	sPkm = NULL;
	for (u8 ii = 0; ii < BOX_PKM_COUNT; ii++)
	{
		if (sPkms[ii]) sPkms[ii]->checked = false;
		sPkmBanked[ii] = false;
		sPkms[ii] = NULL;
	}
	sPkmCount = 0;
}

// --------------------------------------------------
static void _loadVPkmIcon(void* arg)
// --------------------------------------------------
{
	vPkm_s* vPkm = (vPkm_s*) arg;
	char path[0x40];

	// Try to load from sdmc:/
	snprintf(path, 0x40, SDMC ROMFS_FOLDER "pokemon_sprites/%03u%s.png", vPkm->pkm->speciesID, vPkm->pkm->isShiny ? "s" : "");
	vPkm->icon = sfil_load_PNG_file(path, SF2D_PLACE_RAM);

	if (!vPkm->icon)
	{
		// Try to load from romfs:/
		snprintf(path, 0x40, ROMFS ROMFS_FOLDER "pokemon_sprites/%03u%s.png", vPkm->pkm->speciesID, vPkm->pkm->isShiny ? "s" : "");
		vPkm->icon = sfil_load_PNG_file(path, SF2D_PLACE_RAM);
	}
}

// --------------------------------------------------
static void loadVPkmIconAsync(vPkm_s* vPkm)
// --------------------------------------------------
{
	s32 prio = 0;
	svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
	threadCreate(_loadVPkmIcon, (void*) vPkm, 4*1024, prio-1, -2, true);
}

// --------------------------------------------------
void BoxViewer::populateVPkmData(vPkm_s* vPkm, pkm_s* pkm)
// --------------------------------------------------
{
	if (!pkm) pkm = vPkm->pkm;

	if (vPkm->icon)
	{
		sf2d_free_texture(vPkm->icon);
		vPkm->icon = NULL;
	}

	vPkm->emptySlot = save->isPkmEmpty(pkm);

	if (!vPkm->emptySlot) loadVPkmIconAsync(vPkm);
	else if (sPkm)
	{
		populateVPkmData(vPkm, sPkm);
		vPkm->emptySlot = true;
		return;
	}

	memset(vPkm->NKName, 0, 0xD * sizeof(uint32_t));
	memset(vPkm->OTName, 0, 0xD * sizeof(uint32_t));
	memset(vPkm->HTName, 0, 0xD * sizeof(uint32_t));
	utf16_to_utf32(vPkm->NKName, Pokemon::NK_name(pkm), 0xD);
	utf16_to_utf32(vPkm->OTName, Pokemon::OT_name(pkm), 0xD);
	utf16_to_utf32(vPkm->HTName, Pokemon::HT_name(pkm), 0xD);
	str32nfix(vPkm->NKName, 0xD);
	str32nfix(vPkm->OTName, 0xD);
	str32nfix(vPkm->HTName, 0xD);

	vPkm->isKalosBorn = Pokemon::isKalosBorn(pkm);
	vPkm->isInfected = Pokemon::isInfected(pkm);
	vPkm->isCured = Pokemon::isCured(pkm);

	vPkm->circle = Pokemon::circle(pkm);
	vPkm->triangle = Pokemon::triangle(pkm);
	vPkm->square = Pokemon::square(pkm);
	vPkm->heart = Pokemon::heart(pkm);
	vPkm->star = Pokemon::star(pkm);
	vPkm->diamond = Pokemon::diamond(pkm);

	vPkm->species = data->species(pkm->speciesID);
	vPkm->item = data->items(pkm->itemID);
	vPkm->nature = data->natures(Pokemon::nature(pkm));
	vPkm->ability = data->abilities(Pokemon::ability(pkm));
	vPkm->hpType = data->types(Pokemon::HPType(pkm)+1);

	vPkm->moves[0] = data->moves(Pokemon::move1(pkm));
	vPkm->moves[1] = data->moves(Pokemon::move2(pkm));
	vPkm->moves[2] = data->moves(Pokemon::move3(pkm));
	vPkm->moves[3] = data->moves(Pokemon::move4(pkm));

	vPkm->PSV = Pokemon::PSV(pkm);
	vPkm->level = Pokemon::level(pkm);
	vPkm->stats[Stat::HP] = Pokemon::HP(pkm);
	vPkm->stats[Stat::ATK] = Pokemon::ATK(pkm);
	vPkm->stats[Stat::DEF] = Pokemon::DEF(pkm);
	vPkm->stats[Stat::SPA] = Pokemon::SPA(pkm);
	vPkm->stats[Stat::SPD] = Pokemon::SPD(pkm);
	vPkm->stats[Stat::SPE] = Pokemon::SPE(pkm);
	vPkm->ivs[Stat::HP] = Pokemon::IV_HP(pkm);
	vPkm->ivs[Stat::ATK] = Pokemon::IV_ATK(pkm);
	vPkm->ivs[Stat::DEF] = Pokemon::IV_DEF(pkm);
	vPkm->ivs[Stat::SPA] = Pokemon::IV_SPA(pkm);
	vPkm->ivs[Stat::SPD] = Pokemon::IV_SPD(pkm);
	vPkm->ivs[Stat::SPE] = Pokemon::IV_SPE(pkm);
	vPkm->evs[Stat::HP] = Pokemon::EV_HP(pkm);
	vPkm->evs[Stat::ATK] = Pokemon::EV_ATK(pkm);
	vPkm->evs[Stat::DEF] = Pokemon::EV_DEF(pkm);
	vPkm->evs[Stat::SPA] = Pokemon::EV_SPA(pkm);
	vPkm->evs[Stat::SPD] = Pokemon::EV_SPD(pkm);
	vPkm->evs[Stat::SPE] = Pokemon::EV_SPE(pkm);

	vPkm->ball = Pokemon::ball(pkm) - 1;

	if (Pokemon::isGen6Born(pkm)) vPkm->gen = 6;
	else if (Pokemon::isGen5Born(pkm)) vPkm->gen = 5;
	else if (Pokemon::isGen4Born(pkm)) vPkm->gen = 4;
	else if (Pokemon::isGen3Born(pkm)) vPkm->gen = 3;
	else vPkm->gen = 0;
}
