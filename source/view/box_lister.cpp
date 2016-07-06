#include "box_lister.hpp"
#include "box_browser.hpp"

#include "text.h"

#include <stdio.h>

#define SCREEN_WIDTH (320)
#define SCREEN_HEIGHT (240)

#define ROW_COUNT (7)
#define COL_COUNT (10)
#define BOX_WIDTH (32) // Real: 64
#define BOX_HEIGHT (32) // Real: 64

void computeListerInbox(CursorInbox_s* inbox)
{
	inbox->slot = inbox->row * COL_COUNT + inbox->col;
}

// --------------------------------------------------
BoxLister::BoxLister(Scene* parent) : Scene(parent) { }
// --------------------------------------------------

// --------------------------------------------------
BoxLister::BoxLister(SceneType sType, Scene* parent) : Scene(sType, parent) { }
// --------------------------------------------------

// --------------------------------------------------
BoxLister::~BoxLister()
// --------------------------------------------------
{
	// Destruct your allocated memory here!
	// TODO: Content may be missing!
}

// --------------------------------------------------
Result BoxLister::initialize()
// --------------------------------------------------
{
	if (hasChild()) { if (child->initialize() == PARENT_STEP); else return CHILD_STEP; }

	consoleClear();
	printf("Initialize();\n");
	computeListerInbox(&cBox);
	selectViewBox();

	boxCount = (cBox.inBank ? PHBank::Save->bnk.boxUnlocked : PHBank::Save->sav.boxUnlocked);
	rowCount = (boxCount / COL_COUNT) + 1;
	colCount = (boxCount % COL_COUNT);

	printf("\x1B[8;0H[%i | %i | %i]", boxCount, rowCount, colCount);

	return SUCCESS_STEP;
}

// --------------------------------------------------
Result BoxLister::drawTopScreen()
// --------------------------------------------------
{
	if (hasRegularChild()) { if (child->drawTopScreen() == PARENT_STEP); else return CHILD_STEP; }

	// printf("\x1B[28;0HSlot: %-2i Row: %-2i Col: %-2i", cBox.slot + 1, cBox.row + 1, cBox.col + 1);

	// printf("\x1B[9;8H                                   ");
	// printf("\x1B[10;8H /------------------------------\\ ");
	// printf("\x1B[11;8H |                              | ");
	// printf("\x1B[12;8H | You are about to exit PHBank | ");
	// printf("\x1B[13;8H |   A - Save and exit          | ");
	// printf("\x1B[14;8H |   B - Exit without saving    | ");
	// printf("\x1B[15;8H |   X - Return to PHBank       | ");
	// printf("\x1B[16;8H |   Y - Backup the save        | ");
	// printf("\x1B[17;8H |                              | ");
	// printf("\x1B[18;8H \\------------------------------/ ");
	// printf("\x1B[19;8H                                  ");

	if (hasOverlayChild()) { child->drawTopScreen(); }
	return SUCCESS_STEP;
}

// --------------------------------------------------
Result BoxLister::drawBotScreen()
// --------------------------------------------------
{
	if (hasRegularChild()) { if (child->drawBotScreen() == PARENT_STEP); else return CHILD_STEP; }

	// Draw the box icons
	for (u16 row = 0; row < rowCount; row++)
	{
		for (u16 col = 0; col < currentColCount(row); col++)
		{
			// Draw the box icon
			sf2d_draw_texture_part_scale(PHBank::Texture->boxTiles, col * BOX_WIDTH, row * BOX_HEIGHT - offsetTop, 0, 128, 64, 64, 0.5f, 0.5f);

			// If the current box is the box currently selected
			if (row == cBox.row && col == cBox.col)
			{
				// Draw the selected box icon
				sf2d_draw_rectangle(col * BOX_WIDTH, row * BOX_HEIGHT - offsetTop, BOX_WIDTH, BOX_HEIGHT, RGBA8(0x55,0xAA,0x88,0xAA));
			}
		}
	}

	// Draw the bottom-bar
	// TODO: REMOVE THIS SHIT OMAGAWD, IT'S HORRIBLE
	sf2d_draw_rectangle(0, 224, 320, 16, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));

	sftd_draw_wtextf_black(8, 225, L"%S "/*(%u/%u)*/, vBox->title/*, vBox->count, BOX_PKM_COUNT*/);

	if (hasOverlayChild()) { child->drawBotScreen(); }
	return SUCCESS_STEP;
}

// --------------------------------------------------
Result BoxLister::update()
// --------------------------------------------------
{
	if (hasRegularChild() || hasOverlayChild()) { if (child->update() == PARENT_STEP); else return CHILD_STEP; }

	if (kDown & KEY_START)
	{
		// Savexit viewer
		closeViewer(false);
		return PARENT_STEP;
	}

	if (kDown & KEY_A)
	{
		// Close the viewer and update the box of the main viewer
		closeViewer(true);
		return CHILD_STEP;
	}

	if (kDown & KEY_B)
	{
		// Close the viewer
		closeViewer(false);
		return CHILD_STEP;
	}

	{
		bool boolMod = false;
		int16_t rowMod = 0;
		int16_t colMod = 0;

		if (kDown & KEY_UP) rowMod--;
		else if (kDown & KEY_DOWN) rowMod++;

		if (kDown & KEY_LEFT) colMod--;
		else if (kDown & KEY_RIGHT) colMod++;

		if (rowMod || colMod)
		{
			cBox.row += rowMod;
			cBox.col += colMod;

 			// If in pc
			if (rowCount < 6)
			{
				if (cBox.row < 0) { if (cBox.col > colCount-1) cBox.row = rowCount-2; else cBox.row = rowCount-1; }
				else if (cBox.row > rowCount-1) { cBox.row = 0; }
				else if (cBox.row > rowCount-2) { if (cBox.col > colCount-1) if (colMod) cBox.row = rowCount-1; else cBox.row = 0; else cBox.row = rowCount-1; }

				offsetTop = 0;
			}
			// If in bank
			else
			{
				if (cBox.row < 0)
				{
					cBox.row = rowCount + (cBox.col > colCount-1 ? -2 : -1);
					offsetTop = (rowCount > ROW_COUNT ? cBox.row+1 - ROW_COUNT : 0) * BOX_HEIGHT;
				}

				if (cBox.row > rowCount-1)
				{
					cBox.row = 0;
					offsetTop = 0;
				}

				if (cBox.row > rowCount-2)
				{
					cBox.row = (cBox.col > colCount-1 && !colMod ? 0 : rowCount-1);
				}

				if (offsetTop >= cBox.row * BOX_HEIGHT)
				{
					offsetTop = (cBox.row + (cBox.row > 0 ? -1 : 0)) * BOX_HEIGHT;
				}

				else if (cBox.row > ROW_COUNT-2 && rowMod > 0 && offsetTop + (ROW_COUNT-2) * BOX_HEIGHT < cBox.row * BOX_HEIGHT)
				{
					offsetTop = (cBox.row + (cBox.row < rowCount-1 ? 1 : 0) - ROW_COUNT) * BOX_HEIGHT;
				}
			}

			if (cBox.col < 0) cBox.col = currentColCount(cBox.row)-1;
			else if (cBox.col > currentColCount(cBox.row)-1) cBox.col = 0;

			boolMod = true;
		}

		if (boolMod)
		{
			// Update the current box
			selectViewBox();
		}
	}

	{
		if (kDown & KEY_TOUCH)
		{
			originalTouch.py = touch.py;
			originalOffsetTop = offsetTop;
			// originalTouch.px = touch.px;
			// originalOffsetLeft = offsetLeft;

			u16 px = touch.px;
			u16 py = touch.py;

			// if (touchWithin(px, py, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - 40))
			// {
				printf("\x1B[1;20H{%3u, %3u}", ((py + offsetTop) / BOX_WIDTH), ((px + offsetLeft) / BOX_HEIGHT));

				u16 oldRow = cBox.row;
				u16 oldCol = cBox.col;

				cBox.row = ((py + offsetTop) / BOX_WIDTH);
				cBox.col = ((px + offsetLeft) / BOX_HEIGHT);

				// If we click on the selected box
				if (cBox.row == oldRow && cBox.col == oldCol)
				{
					closeViewer(true);
					return CHILD_STEP;
				}
				else if (cBox.row < 0 || cBox.row > (rowCount-1) || cBox.col < 0 || cBox.col > (currentColCount(cBox.row)-1))
				{
					printf("\x1B[11;20HOut of bounds  \n");
					cBox.row = oldRow;
					cBox.col = oldCol;
				}
				else
				{
					printf("\x1B[11;20HselectViewBox()\n");
					selectViewBox();
				}

				printf("\x1B[2;20H{%3u, %3u}", cBox.row, cBox.col);

				printf("\x1B[15;20H");
			// }
		}
		else if (kHeld & KEY_TOUCH)
		{
			if (boxCount == PHBank::Save->bnk.boxUnlocked)
			{
				offsetTop = originalOffsetTop + originalTouch.py - touch.py;
				// offsetLeft = originalOffsetLeft + originalTouch.px - touch.px;

				if (offsetTop < 0)
					offsetTop = 0;
				else if (offsetTop + ROW_COUNT * BOX_HEIGHT > (rowCount-1) * BOX_HEIGHT)
					offsetTop = (rowCount-1) * BOX_HEIGHT - ROW_COUNT * BOX_HEIGHT;
			}
		}
	}

	if (kDown & KEY_Y)
	{
		printf("OffsetTop = %i (%i)\n", offsetTop, cBox.row);
	}

	return SUCCESS_STEP;
}

// --------------------------------------------------
bool BoxLister::selectViewBox()
// --------------------------------------------------
{
	computeListerInbox(&cBox);

	if (cBox.slot != -1)
	{
		// PHBank::Save->countBox(cBox.slot, cBox.inBank);
		vBox = PHBank::Save->getBox(cBox.slot, cBox.inBank);
		printf("View Box: [@%p]\n", vBox);
		return true;
	}

	return false;
}

// --------------------------------------------------
bool BoxLister::selectViewBox(uint16_t boxID, bool inBank)
// --------------------------------------------------
{
	cBox.inBank = inBank;
	cBox.slot = boxID;
	cBox.row = (boxID / COL_COUNT);
	cBox.col = (boxID % COL_COUNT);

	return selectViewBox();
}

// --------------------------------------------------
bool BoxLister::selectMoveBox()
// --------------------------------------------------
{
	if (vBox)
	{
		((BoxBrowser*) parent)->selectViewBox(cBox.slot, cBox.inBank);
		return true;
	}

	return false;
}

// --------------------------------------------------
s16 BoxLister::currentColCount(int16_t row)
// --------------------------------------------------
{
	return (row == rowCount - 1 ? colCount : COL_COUNT);
}

// --------------------------------------------------
SceneState BoxLister::closeViewer(bool save)
// --------------------------------------------------
{
	if (save) selectMoveBox();
	this->setState(SceneState::Exiting);
	return close();
}
