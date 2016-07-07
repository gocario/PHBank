#include "box_browser.hpp"

#include "lang.h"
#include "text.h"
#include "utils.h"
#include "pkdir.h"
#include "personal.hpp"

#include "box_lister.hpp"
#include "savexiter.hpp"

#include <sf2d.h>
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

#define HELD_TICK (32000000ULL)


void computeInbox(CursorInbox_s* inbox)
{
	if (inbox->row == BOX_HEADER_SELECTED)
		inbox->slot = -1;
	else
		inbox->slot = inbox->row * BOX_ROW_PKM_COUNT + inbox->col;
}

void extractCursor(CursorSlot_s* slot, const CursorInbox_s* inbox)
{
	slot->box = inbox->box;
	slot->row = inbox->row;
	slot->col = inbox->col;
	slot->inBank = inbox->inBank;
}

void injectCursor(CursorInbox_s* inbox, const CursorSlot_s* slot)
{
	inbox->box = slot->box;
	inbox->row = slot->row;
	inbox->col = slot->col;
}

void _printBox(const Box_s* box)
{
	printf("Box %u:\n", box->number);
	for (u8 i = 0; i < BOX_PKM_COUNT; i++)
	{
		printf("%03u/%u ", box->slot[i].pk6.Species, box->slot[i].pk6.AltForm);
		if (i % BOX_ROW_PKM_COUNT == BOX_ROW_PKM_COUNT-1) printf("\n");
	}
	printf("\n");
}

// --------------------------------------------------
BoxBrowser::BoxBrowser(Scene* parent) : Scene(parent) { }
// --------------------------------------------------

// --------------------------------------------------
BoxBrowser::BoxBrowser(SceneType sType, Scene* parent) : Scene(sType, parent) { }
// --------------------------------------------------

// --------------------------------------------------
BoxBrowser::~BoxBrowser()
// --------------------------------------------------
{
	if (vPkm.icon) sf2d_free_texture(vPkm.icon);
}

// --------------------------------------------------
Result BoxBrowser::initialize()
// --------------------------------------------------
{
	if (hasChild()) { if (this->child->initialize() == PARENT_STEP); else return CHILD_STEP; }

	// Use the Save&Data Manager of PHBank
	save = PHBank::Save;
	data = PHBank::Data;

	// Initialize the 2 boxes content
	cPC.inBank = false; cBK.inBank = true;
	cPC.box = cBK.box = 0;
	cPC.row = cBK.row = 0;
	cPC.col = cBK.col = 0;
	cPC.slot = cBK.slot = 0;
	cBox = &cBK; selectViewBox();
	cBox = &cPC; selectViewBox();

	// Initialize the current Pokémon
	selectViewPokemon();
	cancelMovePokemon();

	// Change the clear color of sf2d (not useful)
	sf2d_set_clear_color(RGBA8(0x00,0x00,0x00,0xFF));

	return PARENT_STEP;
}

// --------------------------------------------------
Result BoxBrowser::drawTopScreen()
// --------------------------------------------------
{
	if (hasRegularChild()) { if (this->child->drawTopScreen() == PARENT_STEP); else return CHILD_STEP; }

	// Draw the resume background
	sf2d_draw_texture(PHBank::Texture->resumeBackground, 0, 0);

	sftd_draw_wtext_white(11, 40, data->text(BankText::GameTrainer));

	// If there is a current Pokémon
	if ((vPkm.pkm && !vPkm.emptySlot) || (sPkm.pkm))
	{
		vPkm_s* viewPkm = vPkm.pkm && !vPkm.emptySlot ? &vPkm : &sPkm;

		sftd_draw_wtextf_white(91, 40, L"%S (%lu/%lu)", save->sav.wT_Name, save->sav.TSV, viewPkm->PSV);

		uint32_t x, y;

		sf2d_draw_texture_part(PHBank::Texture->ballIcons, 5, 5, (viewPkm->ball % BALL_ROW_COUNT) * BALL_SIZE, (viewPkm->ball / BALL_ROW_COUNT) * BALL_SIZE, BALL_SIZE, BALL_SIZE);

		x = 32;
		y = 16 - 2;
		// Is the Pokémon an egg?
		if (viewPkm->pkm->pk6.IsEgg)
		{
			sftd_draw_wtext_white(x, y, data->species(0));
		}
		else
		{
			// Is the Pokémon nicknamed?
			if (viewPkm->pkm->pk6.IsNicknamed)
			{
				sftd_draw_wtext_white(x, y, viewPkm->Nickname);
			}
			else
			{
				sftd_draw_wtext_white(x, y, viewPkm->species);
			}
		}

		sftd_draw_wtextf_white(x + 168, y, L"%S%u", data->text(BankText::Level), viewPkm->level);

		x = 11;
		y = 42 - 2;
		sftd_draw_wtext_white(x, (y += 15), data->text(BankText::DexNo));
		sftd_draw_text_white(x+50, y, "%03u", viewPkm->pkm->pk6.Species);
		sftd_draw_wtext_white(x+80, y, viewPkm->species);
		sftd_draw_wtext_white(x, (y += 15), data->text(BankText::OriginalTrainer));
		sftd_draw_wtext_white(x+50, y, viewPkm->OT_Name);
		y += 15; // sftd_draw_text_white(x, (y += 15), "Stat");
		sftd_draw_wtext_white(x+90, y, data->text(BankText::Value));
		sftd_draw_wtext_white(x+128, y, data->text(BankText::IV));
		sftd_draw_wtext_white(x+158, y, data->text(BankText::EV));
		sftd_draw_wtext_white(x, (y+=15), data->text(BankText::HitPoint));
		sftd_draw_text_white(x+98, y, "% 3u", viewPkm->stats[Stat::HP]);
		sftd_draw_text_white(x+130, y, "% 2u", viewPkm->ivs[Stat::HP]);
		sftd_draw_text_white(x+160, y, "% 3u", viewPkm->evs[Stat::HP]);
		sftd_draw_wtext_white(x, (y+=15), data->text(BankText::Attack));
		sftd_draw_text_white(x+98, y, "% 3u", viewPkm->stats[Stat::ATK]);
		sftd_draw_text_white(x+130, y, "% 2u", viewPkm->ivs[Stat::ATK]);
		sftd_draw_text_white(x+160, y, "% 3u", viewPkm->evs[Stat::ATK]);
		sftd_draw_wtext_white(x, (y+=15), data->text(BankText::Defense));
		sftd_draw_text_white(x+98, y, "% 3u", viewPkm->stats[Stat::DEF]);
		sftd_draw_text_white(x+130, y, "% 2u", viewPkm->ivs[Stat::DEF]);
		sftd_draw_text_white(x+160, y, "% 3u", viewPkm->evs[Stat::DEF]);
		sftd_draw_wtext_white(x, (y+=15), data->text(BankText::SpAttack));
		sftd_draw_text_white(x+98, y, "% 3u", viewPkm->stats[Stat::SPA]);
		sftd_draw_text_white(x+130, y, "% 2u", viewPkm->ivs[Stat::SPA]);
		sftd_draw_text_white(x+160, y, "% 3u", viewPkm->evs[Stat::SPA]);
		sftd_draw_wtext_white(x, (y+=15), data->text(BankText::SpDefense));
		sftd_draw_text_white(x+98, y, "% 3u", viewPkm->stats[Stat::SPD]);
		sftd_draw_text_white(x+130, y, "% 2u", viewPkm->ivs[Stat::SPD]);
		sftd_draw_text_white(x+160, y, "% 3u", viewPkm->evs[Stat::SPD]);
		sftd_draw_wtext_white(x, (y+=15), data->text(BankText::Speed));
		sftd_draw_text_white(x+98, y, "% 3u", viewPkm->stats[Stat::SPE]);
		sftd_draw_text_white(x+130, y, "% 2u", viewPkm->ivs[Stat::SPE]);
		sftd_draw_text_white(x+160, y, "% 3u", viewPkm->evs[Stat::SPE]);
		sftd_draw_wtext_white(x, (y += 15), data->text(BankText::Nature));
		sftd_draw_wtext_white(x+70, y, viewPkm->nature);
		sftd_draw_wtext_white(x, (y += 15), data->text(BankText::Ability));
		sftd_draw_wtext_white(x+70, y, viewPkm->ability);
		sftd_draw_wtext_white(x, (y += 15), data->text(BankText::Item));
		sftd_draw_wtext_white(x+70, y, viewPkm->item);

		x = 246;
		y = 147 - 15 - 2;
		sftd_draw_wtextf_white(x, (y += 15), L"%S   %S", data->text(BankText::HiddenPower), viewPkm->hpType);
		sftd_draw_wtext_white(x, (y += 15), data->text(BankText::Moves));
		sftd_draw_wtext_white(x+10, (y += 15), viewPkm->moves[0]);
		sftd_draw_wtext_white(x+10, (y += 15), viewPkm->moves[1]);
		sftd_draw_wtext_white(x+10, (y += 15), viewPkm->moves[2]);
		sftd_draw_wtext_white(x+10, (y += 15), viewPkm->moves[3]);

		// Draw the Pokémon icon and the egg icon if so
		drawViewPokemon(viewPkm, 256, 36);

		// Show Wurmple's final evolution indicator
		if (viewPkm->pkm->pk6.Species == 265)
		{
			sf2d_texture* pkmIcons = (viewPkm->pkm->isShiny ? PHBank::Texture->pkmShinyIcons : PHBank::Texture->pkmIcons);

			// If < 5 then Beautifly else Dustox
			u16 sprite = ((viewPkm->pkm->pk6.EncryptionKey >> 16) % 10 < 5 ? 267 : 269);

			sf2d_draw_texture_part(pkmIcons, 365, 27, (sprite % 25) * 40, (sprite / 25) * 30, 40, 30);
		}

		if (viewPkm->pkm->isShiny)
		{
			sf2d_draw_texture_part(PHBank::Texture->boxTiles, 240, 135, 54, 64, 9, 9);
		}

		if (viewPkm->isCured)
		{
			sf2d_draw_texture_part(PHBank::Texture->boxTiles, 260, 135, 72, 64, 9, 9);
		}
		else if (viewPkm->isInfected)
		{
			sf2d_draw_texture_part(PHBank::Texture->boxTiles, 260, 135, 81, 64, 27, 9);
		}

		switch (viewPkm->gen)
		{
			case 6: // Kalos Born (Gen VI)
				sf2d_draw_texture_part(PHBank::Texture->boxTiles, 250, 135, 63, 64, 9, 9);
				break;
			case 5: // Gen V
				sf2d_draw_texture_part(PHBank::Texture->boxTiles, 250, 135, 72, 73, 9, 9);
				break;
			case 4: // Gen IV
				sf2d_draw_texture_part(PHBank::Texture->boxTiles, 250, 135, 63, 73, 9, 9);
				break;
			case 3: // Gen III
				sf2d_draw_texture_part(PHBank::Texture->boxTiles, 250, 135, 54, 73, 9, 9);
				break;
		}

		sf2d_draw_texture_part(PHBank::Texture->boxTiles, 290 ,135, 9*0, 64 + 9*viewPkm->pkm->pk6.Circle, 9, 9);
		sf2d_draw_texture_part(PHBank::Texture->boxTiles, 300 ,135, 9*1, 64 + 9*viewPkm->pkm->pk6.Triangle, 9, 9);
		sf2d_draw_texture_part(PHBank::Texture->boxTiles, 310 ,135, 9*2, 64 + 9*viewPkm->pkm->pk6.Square, 9, 9);
		sf2d_draw_texture_part(PHBank::Texture->boxTiles, 320 ,135, 9*3, 64 + 9*viewPkm->pkm->pk6.Heart, 9, 9);
		sf2d_draw_texture_part(PHBank::Texture->boxTiles, 330 ,135, 9*4, 64 + 9*viewPkm->pkm->pk6.Star, 9, 9);
		sf2d_draw_texture_part(PHBank::Texture->boxTiles, 340 ,135, 9*5, 64 + 9*viewPkm->pkm->pk6.Diamond, 9, 9);

		switch (viewPkm->pkm->pk6.Gender)
		{
			case 0: // Male
				sf2d_draw_texture_part(PHBank::Texture->boxTiles, 385, 130, 60, 82, 12, 12);
				break;
			case 1: // Female
				sf2d_draw_texture_part(PHBank::Texture->boxTiles, 388, 130, 72, 82, 12, 12);
				break;
		}
	}
	// Else if empty slot or no Pokémon
	else
	{
		sftd_draw_wtextf_white(91, 40, L"%S (%lu)", save->sav.wT_Name, save->sav.TSV);
	}

	if (hasOverlayChild()) { this->child->drawTopScreen(); }
	return SUCCESS_STEP;
}

// --------------------------------------------------
Result BoxBrowser::drawBotScreen()
// --------------------------------------------------
{
	if (hasRegularChild()) { if (this->child->drawBotScreen() == PARENT_STEP); else return CHILD_STEP; }

	{
		// Retrieve the current box, and the drawing offset.
		s16 boxShift = (cBox->inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED);
		s16 middleBoxShift = (cBox->inBank ? PC_BOX_SHIFT_UNUSED : PC_BOX_SHIFT_USED) + BACKGROUND_WIDTH;

		// Draw the current box: the background and the icons.
		drawBox((cBox->inBank ? cBK : cPC).vBox, boxShift, 0, true);

		// Draw the other box: the background and the icons.
		drawBox((cBox->inBank ? cPC : cBK).vBox, (cBox->inBank ? PC_BOX_SHIFT_UNUSED : BK_BOX_SHIFT_UNUSED), 0, false);

		// Draw bottom bar
		sf2d_draw_texture_part(PHBank::Texture->boxBackgrounds, (cBox->inBank ? PC_BOX_SHIFT_UNUSED : PC_BOX_SHIFT_USED), BACKGROUND_HEIGHT, 440, 968, 480, 30);
		sf2d_draw_texture_part(PHBank::Texture->boxBackgrounds, middleBoxShift, 0, 880, 758, 40, BACKGROUND_HEIGHT);

		// Draw SwapBox button
		sf2d_draw_texture_part(PHBank::Texture->boxTiles, middleBoxShift + 10, 215, 128, 32, 20, 20);

		// If a Pokémon is currently selected
		if (sPkm.pkm)
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
				if (cBox->slot == SLOT_NO_SELECTION)
				{
					// Draw the Pokémon icon on the box title
					pkm_x = boxShift + (BACKGROUND_WIDTH - PKM_WIDTH) / 2;
					pkm_y = 10;
				}
				else
				{
					// Draw the Pokémon icon on the current slot a bit shifted
					pkm_x = boxShift + cBox->col * PKM_WIDTH;
					pkm_y = PKM_BOX_MARGIN_TOP - 12 + cBox->row * PKM_HEIGHT;
				}
			}

			drawPokemon(sPkm.pkm, pkm_x, pkm_y, true);

			if (!isPkmDragged)
			{
				sf2d_draw_texture_part(PHBank::Texture->boxTiles, pkm_x + 20, pkm_y - 11, (u8) cursorType * 32, 32, 32, 32);
			}
		}
		else
		{
			// Draw the cursor
			if (cBox->slot == SLOT_NO_SELECTION)
			{
				// Draw the cursor icon on the box title
				sf2d_draw_texture_part(PHBank::Texture->boxTiles, boxShift + BACKGROUND_WIDTH / 2, 3 - cursorPositionOffY, (u8) cursorType * 32, 32, 32, 32);
			}
			else
			{
				// Draw the cursor icon on the current slot a bit shifted
				sf2d_draw_texture_part(PHBank::Texture->boxTiles, boxShift + 16 + (cBox->slot % 6) * PKM_WIDTH + cursorPositionOffY / 2, PKM_BOX_MARGIN_TOP - 7 + (cBox->slot / 6) * PKM_HEIGHT - cursorPositionOffY, (u8) cursorType * 32, 32, 32, 32);
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
Result BoxBrowser::update()
// --------------------------------------------------
{
	if (hasRegularChild() || hasOverlayChild()) { if (this->child->update() == PARENT_STEP); else return CHILD_STEP; }

	// TODO: Detect if mediatype and the cartridge has been removed!
	// Result FSUSER_CardSlotIsInserted(bool* inserted)

	if (kDown & KEY_START)
	{
		new Savexiter(SceneType::Overlay, this);
		child->initialize();
		return CHILD_STEP;
	}

	bool bboxMod = false;

	{
		bool boolMod = false;
		s16 boxMod = 0;
		s16 rowMod = 0;
		s16 colMod = 0;

		if (kDown & KEY_L) {
			boxMod--;
			heldTick.KEY_L = svcGetSystemTick() + HELD_TICK * 2;
		} else if (kHeld & KEY_L && heldTick.KEY_L + HELD_TICK < svcGetSystemTick()) {
			boxMod--;
			heldTick.KEY_L = svcGetSystemTick();
		}
		if (kDown & KEY_R) {
			boxMod++;
			heldTick.KEY_R = svcGetSystemTick() + HELD_TICK * 2;
		} else if (kHeld & KEY_R && heldTick.KEY_R + HELD_TICK < svcGetSystemTick()) {
			boxMod++;
			heldTick.KEY_R = svcGetSystemTick();
		}

		if (kDown & KEY_UP) {
			rowMod--;
			heldTick.KEY_UP = svcGetSystemTick() + HELD_TICK * 2;
		} else if (kHeld & KEY_UP && heldTick.KEY_UP + HELD_TICK < svcGetSystemTick()) {
			rowMod--;
			heldTick.KEY_UP = svcGetSystemTick();
		}
		if (kDown & KEY_DOWN) {
			rowMod++;
			heldTick.KEY_DOWN = svcGetSystemTick() + HELD_TICK * 2;
		} else if (kHeld & KEY_DOWN && heldTick.KEY_DOWN + HELD_TICK < svcGetSystemTick()) {
			rowMod++;
			heldTick.KEY_DOWN = svcGetSystemTick();
		}

		if (kDown & KEY_LEFT) {
			if (cBox->row == BOX_HEADER_SELECTED) boxMod--;
			else colMod--;
			heldTick.KEY_LEFT = svcGetSystemTick() + HELD_TICK * 2;
		} else if (kHeld & KEY_LEFT && heldTick.KEY_LEFT + HELD_TICK < svcGetSystemTick()) {
			if (cBox->row == BOX_HEADER_SELECTED) boxMod--;
			else colMod--;
			heldTick.KEY_LEFT = svcGetSystemTick();
		}
		if (kDown & KEY_RIGHT) {
			if (cBox->row == BOX_HEADER_SELECTED) boxMod++;
			else colMod++;
			heldTick.KEY_RIGHT = svcGetSystemTick() + HELD_TICK * 2;
		} else if (kHeld & KEY_RIGHT && heldTick.KEY_RIGHT + HELD_TICK < svcGetSystemTick()) {
			if (cBox->row == BOX_HEADER_SELECTED) boxMod++;
			else colMod++;
			heldTick.KEY_RIGHT = svcGetSystemTick();
		}

		if (kDown & KEY_ZL) { swapViewBox(&cPC); boolMod = true; }
		else if (kDown & KEY_ZR) { swapViewBox(&cBK); boolMod = true; }
			
		if (kDown & KEY_TOUCH)
		{
			s16 boxShift = (cBox->inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED);

			// If the box arrows are touched down
			if (touchWithin(touch.px, touch.py, boxShift + 4, 18, 16, 24)) boxMod--;
			else if (touchWithin(touch.px, touch.py, boxShift + 200, 18, 16, 24)) boxMod++;

			boxShift = (cBox->inBank ? PC_BOX_SHIFT_UNUSED : BK_BOX_SHIFT_UNUSED);

			if (touchWithin(touch.px, touch.py, boxShift, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT)) {
				swapViewBox(NULL);
				boolMod = bboxMod = true;
			}
		}
		else if (kHeld & KEY_TOUCH)
		{
			if (sPkm.pkm || isPkmChecking)
			{
				s16 boxShift = (cBox->inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED);

				// If the box arrows are touched held once
				if (touchWithin(touch.px, touch.py, boxShift + 4, 18, 16, 24) && !touchWithin(oldTouch.px, oldTouch.py, boxShift + 4, 18, 16, 24)) boxMod--;
				else if (touchWithin(touch.px, touch.py, boxShift + 200, 18, 16, 24) && !touchWithin(oldTouch.px, oldTouch.py, boxShift + 200, 18, 16, 24)) boxMod++;

				boxShift = (cBox->inBank ? PC_BOX_SHIFT_UNUSED : BK_BOX_SHIFT_UNUSED);

				// If the other box (PC|BK) is touched held
				if (touchWithin(touch.px, touch.py, boxShift, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT)) {
					swapViewBox(NULL);
					boolMod = bboxMod = true;
				}
			}
		}

		if (boxMod || rowMod || colMod)
		{
			cBox->box += boxMod;
			cBox->row += rowMod;
			cBox->col += colMod;

			// TODO: Wonder & Trash boxes

			if (cBox->box < 0) cBox->box = (cBox->inBank ? save->bnk.boxUnlocked : save->sav.boxUnlocked)-1;
			else if (cBox->box > (cBox->inBank ? save->bnk.boxUnlocked : save->sav.boxUnlocked)-1) cBox->box = 0;

			if (cBox->row < BOX_HEADER_SELECTED) cBox->row = BOX_COL_PKM_COUNT-1;
			else if (cBox->row > BOX_COL_PKM_COUNT-1) cBox->row = BOX_HEADER_SELECTED;

			if (cBox->col < 0) {
				swapViewBox(NULL);
				cBox->col = BOX_ROW_PKM_COUNT-1;
			} else if (cBox->col > BOX_ROW_PKM_COUNT-1) {
				swapViewBox(NULL);
				cBox->col = 0;
			}

			boolMod = true;
		}

		if (boolMod)
		{
			selectViewBox();
			selectViewPokemon();
		}
	}

	if (kDown & KEY_SELECT)
	{
		switchCursorType();
	}

	switch (cursorType)
	{
		case CursorType::SingleSelect:
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

			if (kDown & KEY_X)
			{
				// TODO: Single X
			}

			if (kDown & KEY_Y)
			{
				// Open the UltraBox viewer
				BoxLister* boxLister = new BoxLister(SceneType::Regular, this);
				boxLister->selectViewBox(cBox->box, cBox->inBank);
				child->initialize();
				return CHILD_STEP;
			}

			break;
		}
		case CursorType::QuickSelect:
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

			if (kDown & KEY_X)
			{
				// Swap the two boxes (PC|BK)
				save->moveBox(cPC.vBox, cBK.vBox, false, true);
			}

			if (kDown & KEY_Y && !sPkm.pkm)
			{
				// Open the UltraBox viewer
				BoxLister* boxLister = new BoxLister(SceneType::Regular, this);
				boxLister->selectViewBox(cBox->box, cBox->inBank);
				child->initialize();
				return CHILD_STEP;
			}

			break;
		}
		case CursorType::MultiSelect:
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
				BoxLister* boxLister = new BoxLister(SceneType::Regular, this);
				boxLister->selectViewBox(cBox->box, cBox->inBank);
				child->initialize();
				return CHILD_STEP;
			}

			break;
		}
		case CursorType::DevSelect:
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

			break;
		}
		default: break;
	}

	{
		if (bboxMod);
		else if (kDown & KEY_TOUCH)
		{
			s16 boxShift = (cBox->inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED);
			s16 middleBoxShift = (cBox->inBank ? PC_BOX_SHIFT_UNUSED : PC_BOX_SHIFT_USED) + BACKGROUND_WIDTH;
			u16 px = touch.px;
			u16 py = touch.py;

			if (!sPkm.pkm && sPkmCount == 0)
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
					save->moveBox(cPC.vBox, cBK.vBox, false, true);
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
				if (!sPkm.pkm && !isPkmDragged)
				{
					// Move the cursor to the new slot
					cBox->row = (py - PKM_BOX_MARGIN_TOP) / PKM_HEIGHT;
					cBox->col = (px - boxShift) / PKM_WIDTH;

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
					u16 oldRow = cBox->row;
					u16 oldCol = cBox->col;

					// Move the cursor to the new slot
					cBox->row = (py - PKM_BOX_MARGIN_TOP) / PKM_HEIGHT;
					cBox->col = (px - boxShift) / PKM_WIDTH;

					// Update the current Pokémon
					selectViewPokemon();

					// And drop the Pokémon if one is held and it is the same current slot (mean double tap to move a held Pokémon)
					if (isPkmHeld && oldRow == cBox->row && oldCol == cBox->col)
					{
						selectMovePokemon();
					}
				}
			}
		}
		else if (kHeld & KEY_TOUCH)
		{
			if (isPkmDragged)
			{
				s16 boxShift = (cBox->inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED);
				u16 px = touch.px;
				u16 py = touch.py;

				// If the TouchArea is within the Pokémon icons area of the box
				if (touchWithin(px, py, boxShift, PKM_BOX_MARGIN_TOP, BACKGROUND_WIDTH - PKM_BOX_MARGIN_RIGHT, BACKGROUND_HEIGHT - PKM_BOX_MARGIN_TOP))
				{
					// Move the cursor to the new slot
					cBox->row = (py - PKM_BOX_MARGIN_TOP) / PKM_HEIGHT;
					cBox->col = (px - boxShift) / PKM_WIDTH;

					// Update the current Pokémon
					selectViewPokemon();
				}
				else
				{
					// ASK Is it dirty..?
					vPkm.pkm = NULL;
					populateVPkmData(&sPkm, NULL);
				}
			}
			else if (cursorType == CursorType::MultiSelect)
			{
				s16 boxShift = (cBox->inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED);
				u16 px = touch.px;
				u16 py = touch.py;

				// If the TouchArea is within the Pokémon icons area of the box
				if (touchWithin(px, py, boxShift, PKM_BOX_MARGIN_TOP, BACKGROUND_WIDTH - PKM_BOX_MARGIN_RIGHT, BACKGROUND_HEIGHT - PKM_BOX_MARGIN_TOP))
				{
					s16 row = (py - PKM_BOX_MARGIN_TOP) / PKM_HEIGHT;
					s16 col = (px - boxShift) / PKM_WIDTH;

					// If the touch position is not the same slot as before.
					if (cBox->row != row || cBox->col != col)
					{
						// Move the cursor to the new slot
						cBox->row = row;
						cBox->col = col;

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
		}
		else if (kUp & KEY_TOUCH)
		{
			s16 boxShift = (cBox->inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED);
			u16 px = touch.px;
			u16 py = touch.py;

			// If a Pokémon is currently dragged
			if (sPkm.pkm && isPkmDragged)
			{
				// If the TouchArea is within the Pokémon icons area of the box
				if (touchWithin(px, py, boxShift, PKM_BOX_MARGIN_TOP, BACKGROUND_WIDTH - PKM_BOX_MARGIN_RIGHT, BACKGROUND_HEIGHT - PKM_BOX_MARGIN_TOP))
				{
					// Move the cursor to the new slot
					cBox->row = (py - PKM_BOX_MARGIN_TOP) / PKM_HEIGHT;
					cBox->col = (px - boxShift) / PKM_WIDTH;

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
					cBox = (sSlot.inBank ? &cBK : &cPC);
					injectCursor(cBox, &sSlot);

					// Update the old box
					selectViewBox();

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
void BoxBrowser::drawBox(const Box_s* box, int16_t x, int16_t y, bool cursor)
// --------------------------------------------------
{
	// Draw the box background
	sf2d_draw_texture_part(PHBank::Texture->boxBackgrounds, x, y, ((box->background % 16) % 4) * BACKGROUND_WIDTH, ((box->background % 16) / 4) * BACKGROUND_HEIGHT, BACKGROUND_WIDTH, BACKGROUND_HEIGHT);

	if (cursor)
	{
		// Draw CursorType buttons (Red|Blue|Green)
		drawCursorButton(x, y);
	}

	// Draw the box header
	sf2d_draw_texture_part(PHBank::Texture->boxBackgrounds, x, y + 15, ((box->background % 16) % 4) * BOX_HEADER_WIDTH, 840 + ((box->background % 16) / 4) * BOX_HEADER_HEIGHT, BOX_HEADER_WIDTH, BOX_HEADER_HEIGHT);

	// Draw the box arrows
	sf2d_draw_texture_part(PHBank::Texture->boxTiles, x + 4, y + 18, 96, 32, 16, 24);
	sf2d_draw_texture_part(PHBank::Texture->boxTiles, x + 200, y + 18, 112, 32, 16, 24);

	// Draw the box title
	wchar_t boxTitle[17];
	// TODO: Remove that if statement when complete box->title.
	if (box->title[0] != '\0') swprintf(boxTitle, 17, (wchar_t*) box->title);
	else swprintf(boxTitle, 17, L"Box %i", box->number+1);
	int boxTitleWidth = sftd_get_wtext_width(font, 12, boxTitle);
	sftd_draw_wtext(font, x + (BACKGROUND_WIDTH - boxTitleWidth) / 2, y + 21, RGBA8(0x22,0x22,0x22,0xFF), 12, boxTitle);

	// TODO: v DRY v

	// If there is a Pokémon currently selected
	if (sPkm.pkm)
	{
		// Draw Pokémon icons
		for (u8 i = 0; i < BOX_PKM_COUNT; i++)
		{
			// If the Pokémon isn't the selected Pokémon
			if (sPkm.pkm != &(box->slot[i]))
			{
				drawPokemon(&box->slot[i], x + (i % BOX_ROW_PKM_COUNT) * PKM_WIDTH, y + PKM_BOX_MARGIN_TOP + (i / BOX_ROW_PKM_COUNT) * PKM_HEIGHT, false);
			}
		}
	}
	// If there is no Pokémon currently selected
	else
	{
		// Draw Pokémon icons
		for (u8 i = 0; i < BOX_PKM_COUNT; i++)
		{
			drawPokemon(&box->slot[i], x + (i % BOX_ROW_PKM_COUNT) * PKM_WIDTH, y + PKM_BOX_MARGIN_TOP + (i / BOX_ROW_PKM_COUNT) * PKM_HEIGHT, false);
		}
	}
}

// --------------------------------------------------
void BoxBrowser::drawPokemon(const Pkm_s* pkm, int16_t x, int16_t y, bool shadow)
// --------------------------------------------------
{
	if (pkm->pk6.Species == 0) return;

	// sf2d_draw_rectangle(x, y, PKM_WIDTH, PKM_HEIGHT, RGBA8(0x00,0x00,0x00,0x22));

	u16 widthCount = 25;
	u16 sprite = pkm->pk6.Species;
	sf2d_texture* pkmIcons = PHBank::Texture->pkmIcons;

	// If a particular form and the species isn't Scatterbug/Spewpa (that is ugly tho)
	if (pkm->pk6.AltForm > 0 && pkm->pk6.Species != 664 && pkm->pk6.Species != 665)
	{
		if (pkm->isShiny) pkmIcons = PHBank::Texture->pkmShinyFormIcons;
		else pkmIcons = PHBank::Texture->pkmFormIcons;

		widthCount = 12;
		sprite = Personal(pkm->pk6.Species).FormSprite + pkm->pk6.AltForm - 1;
	}
	// If female form with Pyroar/Frillish/Jellicent/Unfezant
	else if (pkm->pk6.Gender == 1 && (pkm->pk6.Species == 521 || pkm->pk6.Species == 592 || pkm->pk6.Species == 593 || pkm->pk6.Species == 668))
	{
		if (pkm->isShiny) pkmIcons = PHBank::Texture->pkmShinyFormIcons;
		else pkmIcons = PHBank::Texture->pkmFormIcons;

		widthCount = 12;
		sprite = 173 +
			(pkm->pk6.Species == 521 ? 1 :
			(pkm->pk6.Species == 592 ? 2 :
			(pkm->pk6.Species == 593 ? 3 :
			(pkm->pk6.Species == 668 ? 4 :
			0))));
	}
	else if (pkm->isShiny) pkmIcons = PHBank::Texture->pkmShinyIcons;

	// Draw the shadow
	if (shadow)
	{
		sf2d_draw_texture_part_blend(pkmIcons, x + 4, y + 4, (sprite % widthCount) * 40, (sprite / widthCount) * 30, 40, 30, RGBA8(0x00,0x00,0x00,0x55));
	}

	if (pkm->pk6.IsEgg)
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
		if (pkm->pk6.HeldItem > 0)
		{
			sf2d_draw_texture_part(PHBank::Texture->boxTiles, x + 23, y + 21, 48, 82, 12, 12);
		}
	}

	// Draw the checkbox
	if (pkm->checked)
	{
		sf2d_draw_texture_part(PHBank::Texture->boxTiles, x + 3, y + 22, 96, 56, 8, 8);
	}
}

// --------------------------------------------------
void BoxBrowser::drawPokemonScale(const Pkm_s* pkm, int16_t x, int16_t y, float scale)
// --------------------------------------------------
{
	if (pkm->pk6.Species == 0) return;

	u16 widthCount = 25;
	u16 sprite = pkm->pk6.Species;
	sf2d_texture* pkmIcons = PHBank::Texture->pkmIcons;

	// If a particular form and the species isn't Scatterbug/Spewpa (that is ugly tho)
	if (pkm->pk6.AltForm > 0 && pkm->pk6.Species != 664 && pkm->pk6.Species != 665)
	{
		if (pkm->isShiny) pkmIcons = PHBank::Texture->pkmShinyFormIcons;
		else pkmIcons = PHBank::Texture->pkmFormIcons;

		widthCount = 12;
		sprite = Personal(pkm->pk6.Species).FormSprite + pkm->pk6.AltForm - 1;
	}
	// If female form with Pyroar/Frillish/Jellicent/Unfezant
	else if (pkm->pk6.Gender == 1 && (pkm->pk6.Species == 521 || pkm->pk6.Species == 592 || pkm->pk6.Species == 593 || pkm->pk6.Species == 668))
	{
		if (pkm->isShiny) pkmIcons = PHBank::Texture->pkmShinyFormIcons;
		else pkmIcons = PHBank::Texture->pkmFormIcons;

		widthCount = 12;
		sprite = 173 +
			(pkm->pk6.Species == 521 ? 1 :
			(pkm->pk6.Species == 592 ? 2 :
			(pkm->pk6.Species == 593 ? 3 :
			(pkm->pk6.Species == 668 ? 4 :
		0))));
	}
	else if (pkm->isShiny) pkmIcons = PHBank::Texture->pkmShinyIcons;

	// Draw the shadow
	{
		sf2d_draw_texture_part_scale_blend(pkmIcons, x + 2 * scale, y + 2 * scale, (sprite % widthCount) * 40, (sprite / widthCount) * 30, 40, 30, scale, scale, RGBA8(0x00,0x00,0x00,0x55));
	}

	if (pkm->pk6.IsEgg)
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
		if (pkm->pk6.HeldItem > 0)
		{
			// TODO: Enhance the item display
			sf2d_draw_texture_part(PHBank::Texture->itemIcons, x + 5, y + 30 * (scale - 0.5f), (pkm->pk6.HeldItem % 34) * 30, (pkm->pk6.HeldItem / 34) * 30, 30, 30);
		}
	}
}

// --------------------------------------------------
void BoxBrowser::drawViewPokemon(vPkm_s* vPkm, int16_t x, int16_t y)
// --------------------------------------------------
{
	if (!vPkm->pkm || vPkm->emptySlot) return;

	// If the big icon is loaded
	if (vPkm->icon)
	{
		// TODO: Draw the Pokémon's form

		sf2d_draw_texture_blend(vPkm->icon, x + 18, y + 2, RGBA8(0x00,0x00,0x00,0x55));
		sf2d_draw_texture(vPkm->icon, x + 16, y);

		if (vPkm->pkm->pk6.IsEgg)
		{
			sf2d_draw_texture_part(vPkm->pkm->isShiny ? PHBank::Texture->pkmShinyIcons : PHBank::Texture->pkmIcons, x, y + 75, (EGG_ID % 25) * 40, (EGG_ID / 25) * 30, 40, 30);
		}
		else if (vPkm->pkm->pk6.HeldItem > 0)
		{
			sf2d_draw_texture_part(PHBank::Texture->itemIcons, x + 5, y + 75, (vPkm->pkm->pk6.HeldItem % 34) * 30, (vPkm->pkm->pk6.HeldItem / 34) * 30, 30, 30);
		}
	}
	else
	{
		drawPokemonScale(vPkm->pkm, x, y, 3.0f);
	}
}

// --------------------------------------------------
void BoxBrowser::drawCursorButton(int16_t x, int16_t y)
// --------------------------------------------------
{
	if (!sPkm.pkm && sPkmCount == 0)
	{
		sf2d_draw_texture_part(PHBank::Texture->boxTiles, x + 21 +   0, y,   0, 0, 50, 32);
		sf2d_draw_texture_part(PHBank::Texture->boxTiles, x + 21 +  64, y,  64, 0, 50, 32);
		sf2d_draw_texture_part(PHBank::Texture->boxTiles, x + 21 + 128, y, 128, 0, 50, 32);
	}
	else
	{
		sf2d_draw_texture_part_blend(PHBank::Texture->boxTiles, x + 21 +   0, y,   0, 0, 50, 32, RGBA8(0xCC,0xCC,0xCC,0xAA));
		sf2d_draw_texture_part_blend(PHBank::Texture->boxTiles, x + 21 +  64, y,  64, 0, 50, 32, RGBA8(0xCC,0xCC,0xCC,0xAA));

		if (cursorType == CursorType::MultiSelect)
			sf2d_draw_texture_part(PHBank::Texture->boxTiles, x + 21 + 128, y, 128, 0, 64, 32);
		else
			sf2d_draw_texture_part_blend(PHBank::Texture->boxTiles, x + 21 + 128, y, 128, 0, 50, 32, RGBA8(0xCC,0xCC,0xCC,0xAA));
	}
}


// --------------------------------------------------
void BoxBrowser::selectCursorType(CursorType cursorType)
// --------------------------------------------------
{
	this->cursorType = cursorType;
	cancelMovePokemon();
}

// --------------------------------------------------
void BoxBrowser::switchCursorType()
// --------------------------------------------------
{
	// ... -> Single -> Quick -> Multi -> ... //
	switch (cursorType)
	{
		default:
		case CursorType::DevSelect:
			selectCursorType(CursorType::SingleSelect);
			break;
		case CursorType::SingleSelect:
			selectCursorType(CursorType::QuickSelect);
			break;
		case CursorType::QuickSelect:
			selectCursorType(CursorType::MultiSelect);
			break;
		case CursorType::MultiSelect:
			selectCursorType(CursorType::SingleSelect);
			break;
	}
}

// --------------------------------------------------
void BoxBrowser::swapViewBox(CursorInbox_s* cInbox)
// --------------------------------------------------
{
	CursorInbox_s* cNew = (cInbox ? cInbox : (cBox->inBank ? &cPC : &cBK));
	cNew->row = cBox->row;
	cNew->col = cBox->col;
	cBox = cNew;
}

// --------------------------------------------------
void BoxBrowser::selectViewBox()
// --------------------------------------------------
{
	// Compute the current cursor slot
	computeInbox(cBox);

	cBox->vBox = save->getBox(cBox->box, cBox->inBank);
}

// --------------------------------------------------
void BoxBrowser::selectViewBox(uint16_t boxID, bool inBank)
// --------------------------------------------------
{
	cBox->box = boxID;
	selectViewBox();
	selectViewPokemon();
}

// --------------------------------------------------
void BoxBrowser::selectViewPokemon()
// --------------------------------------------------
{
	// Compute the current cursor slot
	computeInbox(cBox);

	// If the cursor isn't in a box slot
	if (cBox->slot == SLOT_NO_SELECTION)
	{
		vPkm.pkm = NULL;
	}
	// If the cursor is in a box slot
	else
	{
		vPkm.pkm = save->getPkm(cBox->box, cBox->slot, cBox->inBank);
		populateVPkmData(&vPkm, NULL);
	}
}

// --------------------------------------------------
void BoxBrowser::selectMovePokemon()
// --------------------------------------------------
{
	// If no Pokémon is currently selected
	if (!sPkm.pkm)
	{
		// If the current Pokémon slot isn't empty (to avoid empty slot move)
		if (!vPkm.emptySlot)
		{
			// Select the current Pokémon
			sPkm.pkm = vPkm.pkm;
			populateVPkmData(&sPkm, sPkm.pkm);

			extractCursor(&sSlot, cBox);

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
		if (sPkm.pkm != vPkm.pkm)
		{
			// Swap the selected Pokémon and the current Pokémon, and keep the return value (true: had moved, false: hadn't)
			bool moved = save->movePkm(sPkm.pkm, vPkm.pkm, sSlot.inBank, cBox->inBank);

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
void BoxBrowser::selectMultiMovePokemon(bool check)
// --------------------------------------------------
{
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
				sPkmBanked[sPkmCount] = cBox->inBank;
				sPkmCount++;
			}
		}
	}
	// Else we move
	else
	{
		u8 vPkmCount = 0;
		Box_s* box = cBox->vBox;

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
				save->movePkm(sPkms[ii], &box->slot[i], sPkmBanked[ii], cBox->inBank);
				box->slot[i].checked = false;
				i++;
			}

			cancelMovePokemon();
		}
	}
}

// --------------------------------------------------
void BoxBrowser::selectPastePokemon()
// --------------------------------------------------
{
	// If no Pokémon is currently selected
	if (!sPkm.pkm)
	{
		// If the current Pokémon slot isn't empty (to avoid empty slot paste)
		if (!vPkm.emptySlot)
		{
			// Select the current Pokémon
			sPkm.pkm = vPkm.pkm;
			populateVPkmData(&sPkm, sPkm.pkm);

			extractCursor(&sSlot, cBox);

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
		bool pasted = save->pastePkm(sPkm.pkm, vPkm.pkm, sSlot.inBank, cBox->inBank);

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
void BoxBrowser::selectMultiPastePokemon(bool check)
// --------------------------------------------------
{
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
				sPkmBanked[sPkmCount] = cBox->inBank;
				sPkmCount++;
			}
		}
	}
	// Else we paste
	else
	{
		u8 vPkmCount = 0;
		Box_s* box = cBox->vBox;

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
				save->pastePkm(sPkms[ii], &box->slot[i], sPkmBanked[ii], cBox->inBank);
				box->slot[i].checked = false;
				i++;
			}

			cancelMovePokemon();
		}
	}
}

// --------------------------------------------------
void BoxBrowser::cancelMovePokemon()
// --------------------------------------------------
{
	// Reset the selection
	isPkmHeld = false;
	isPkmDragged = false;
	isPkmChecking = false;

	// Reset the selected
	sPkm.pkm = NULL;
	for (u8 ii = 0; ii < BOX_PKM_COUNT; ii++)
	{
		if (sPkms[ii]) {
			sPkms[ii]->checked = false;
		printf("%p %u\n", &sPkms[ii], sPkms[ii]->checked);
	}
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
	char form[0x4];
	char gender[0x2];
	char shiny[0x2];

	if (vPkm->pkm->pk6.AltForm == 0) form[0] = '\0';
	else sprintf(form, "-%d", vPkm->pkm->pk6.AltForm);

	if (vPkm->pkm->pk6.Gender == 1 && (
		vPkm->pkm->pk6.Species == 521 ||
		vPkm->pkm->pk6.Species == 592 ||
		vPkm->pkm->pk6.Species == 593 ||
		vPkm->pkm->pk6.Species == 668))
		gender[0] = 'f';
	else gender[0] = '\0';
	gender[1] = '\0';

	if (vPkm->pkm->isShiny) shiny[0] = 's';
	else shiny[0] = '\0';
	shiny[1] = '\0';

	if (!vPkm->icon)
	{
		// Try to load from sdmc:/
		snprintf(path, 0x40, SDMC ROMFS_FOLDER "pokemon_sprites/%03u%s%s%s.png", vPkm->pkm->pk6.Species, form, gender, shiny);
		vPkm->icon = sfil_load_PNG_file(path, SF2D_PLACE_RAM);
	}

	if (!vPkm->icon)
	{
		// Try to load from romfs:/
		snprintf(path, 0x40, ROMFS ROMFS_FOLDER "pokemon_sprites/%03u%s%s%s.png", vPkm->pkm->pk6.Species, form, gender, shiny);
		vPkm->icon = sfil_load_PNG_file(path, SF2D_PLACE_RAM);
	}

	if (!vPkm->icon)
	{
		// Try to load from sdmc:/
		snprintf(path, 0x40, SDMC ROMFS_FOLDER "pokemon_sprites/%03u.png", vPkm->pkm->pk6.Species);
		vPkm->icon = sfil_load_PNG_file(path, SF2D_PLACE_RAM);
	}

	if (!vPkm->icon)
	{
		// Try to load from romfs:/
		snprintf(path, 0x40, ROMFS ROMFS_FOLDER "pokemon_sprites/%03u.png", vPkm->pkm->pk6.Species);
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
void BoxBrowser::populateVPkmData(vPkm_s* vPkm, Pkm_s* pkm)
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

	memset(vPkm->Nickname, 0, 11 * sizeof(uint32_t));
	memset(vPkm->OT_Name, 0, 11 * sizeof(uint32_t));
	memset(vPkm->HT_Name, 0, 11 * sizeof(uint32_t));
	utf16_to_utf32(vPkm->Nickname, pkm->pk6.Nickname, 11);
	utf16_to_utf32(vPkm->OT_Name, pkm->pk6.OT_Name, 11);
	utf16_to_utf32(vPkm->HT_Name, pkm->pk6.HT_Name, 11);
	str32nfix(vPkm->Nickname, 11);
	str32nfix(vPkm->OT_Name, 11);
	str32nfix(vPkm->HT_Name, 11);

	vPkm->gen = PK6::generation(&pkm->pk6);
	vPkm->isKalosBorn = PK6::isKalosBorn(&pkm->pk6);
	vPkm->isInfected = PK6::isInfected(&pkm->pk6);
	vPkm->isCured = PK6::isCured(&pkm->pk6);

	vPkm->species = data->species(pkm->pk6.Species);
	vPkm->item = data->items(pkm->pk6.HeldItem);
	vPkm->nature = data->natures(pkm->pk6.Nature);
	vPkm->ability = data->abilities(pkm->pk6.Ability);
	vPkm->hpType = data->types(PK6::HPType(&pkm->pk6)+1);

	vPkm->moves[0] = data->moves(pkm->pk6.Moves[0]);
	vPkm->moves[1] = data->moves(pkm->pk6.Moves[1]);
	vPkm->moves[2] = data->moves(pkm->pk6.Moves[2]);
	vPkm->moves[3] = data->moves(pkm->pk6.Moves[3]);

	vPkm->PSV = PK6::getPSV(pkm->pk6.PID);
	vPkm->level = PK6::level(&pkm->pk6);
	vPkm->stats[Stat::HP] = PK6::HP(&pkm->pk6, vPkm->level);
	vPkm->stats[Stat::ATK] = PK6::ATK(&pkm->pk6, vPkm->level);
	vPkm->stats[Stat::DEF] = PK6::DEF(&pkm->pk6, vPkm->level);
	vPkm->stats[Stat::SPA] = PK6::SPA(&pkm->pk6, vPkm->level);
	vPkm->stats[Stat::SPD] = PK6::SPD(&pkm->pk6, vPkm->level);
	vPkm->stats[Stat::SPE] = PK6::SPE(&pkm->pk6, vPkm->level);
	// PK6::stats(vPkm->stats, &pkm->pk6, vPkm->level);
	vPkm->ivs[Stat::HP] = pkm->pk6.IV_HP;
	vPkm->ivs[Stat::ATK] = pkm->pk6.IV_ATK;
	vPkm->ivs[Stat::DEF] = pkm->pk6.IV_DEF;
	vPkm->ivs[Stat::SPA] = pkm->pk6.IV_SPA;
	vPkm->ivs[Stat::SPD] = pkm->pk6.IV_SPD;
	vPkm->ivs[Stat::SPE] = pkm->pk6.IV_SPE;
	vPkm->evs[Stat::HP] = pkm->pk6.EV_HP;
	vPkm->evs[Stat::ATK] = pkm->pk6.EV_ATK;
	vPkm->evs[Stat::DEF] = pkm->pk6.EV_DEF;
	vPkm->evs[Stat::SPA] = pkm->pk6.EV_SPA;
	vPkm->evs[Stat::SPD] = pkm->pk6.EV_SPD;
	vPkm->evs[Stat::SPE] = pkm->pk6.EV_SPE;

	vPkm->ball = pkm->pk6.Ball - 1;
}
