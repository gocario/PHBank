#include "ultra_box_viewer.hpp"

#include "text.h"

#include "box_viewer.hpp"

#include <stdio.h>

#define SCREEN_WIDTH (320)
#define SCREEN_HEIGHT (240)

#define ROW_COUNT (7)
#define COL_COUNT (10)
#define BOX_WIDTH (32) // Real: 64
#define BOX_HEIGHT (32) // Real: 64


// --------------------------------------------------
void computeSlot(CursorUBox_s* cursorUBox)
// --------------------------------------------------
{
	cursorUBox->slot = cursorUBox->row * COL_COUNT + cursorUBox->col;
}


/*----------------------------------------------------------*\
 |                       Viewer Class                       |
\*----------------------------------------------------------*/


	/*--------------------------------------------------*\
	 |             Constructor / Destructor             |
	\*--------------------------------------------------*/


// --------------------------------------------------
UltraBoxViewer::UltraBoxViewer(Viewer* parent) : Viewer(parent) { }
// --------------------------------------------------


// --------------------------------------------------
UltraBoxViewer::UltraBoxViewer(ViewType vType, Viewer* parent) : Viewer(vType, parent) { }
// --------------------------------------------------


// --------------------------------------------------
UltraBoxViewer::~UltraBoxViewer()
// --------------------------------------------------
{
	// Destruct your allocated memory here!
	// TODO: Content may be missing!
}


	/*--------------------------------------------------*\
	 |                  Viewer Methods                  |
	\*--------------------------------------------------*/


		/*------------------------------------------*\
		 |              Public Methods              |
		\*------------------------------------------*/


// --------------------------------------------------
Result UltraBoxViewer::initialize()
// --------------------------------------------------
{
	if (hasChild()) { if (child->initialize() == PARENT_STEP); else return CHILD_STEP; }

	consoleClear();
	printf("Initialize();\n");
	computeSlot(&cursorUBox);
	selectViewBox();

	boxCount = (cursorUBox.inBank ? PHBanku::save->bankdata.bk.boxUnlocked : PHBanku::save->savedata.pc.boxUnlocked);
	rowCount = (boxCount / COL_COUNT) + 1;
	colCount = (boxCount % COL_COUNT);

	printf("\x1B[8;0H[%i | %i | %i]", boxCount, rowCount, colCount);

	return SUCCESS_STEP;
}


// --------------------------------------------------
Result UltraBoxViewer::drawTopScreen()
// --------------------------------------------------
{
	if (hasRegularChild()) { if (child->drawTopScreen() == PARENT_STEP); else return CHILD_STEP; }

	printf("\x1B[28;0HSlot: %-2i Row: %-2i Col: %-2i", cursorUBox.slot + 1, cursorUBox.row + 1, cursorUBox.col + 1);

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
Result UltraBoxViewer::drawBotScreen()
// --------------------------------------------------
{
	if (hasRegularChild()) { if (child->drawBotScreen() == PARENT_STEP); else return CHILD_STEP; }

	// Draw the box icons
	for (uint16_t row = 0; row < rowCount; row++)
	{
		for (uint16_t col = 0; col < currentColCount(row); col++)
		{
			// Draw the box icon
			sf2d_draw_texture_part_scale(PHBanku::texture->boxTiles, col * BOX_WIDTH, row * BOX_HEIGHT - offsetTop, 0, 128, 64, 64, 0.5f, 0.5f);

			// If the current box is the box currently selected
			if (row == cursorUBox.row && col == cursorUBox.col)
			{
				// Draw the selected box icon
				sf2d_draw_rectangle(col * BOX_WIDTH, row * BOX_HEIGHT - offsetTop, BOX_WIDTH, BOX_HEIGHT, RGBA8(0x55,0xAA,0x88,0xAA));
			}
		}
	}

	// Draw the bottom-bar
	// TODO: REMOVE THIS SHIT OMAGAWD, IT'S HORRIBLE
	sf2d_draw_rectangle(0, 224, 320, 16, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));

	sftd_draw_text_black(8, 225, "Box: %i (%u/%u)", cursorUBox.slot+1, vBox->count, BOX_PKM_COUNT);

	if (hasOverlayChild()) { child->drawBotScreen(); }
	return SUCCESS_STEP;
}


// --------------------------------------------------
Result UltraBoxViewer::updateControls(const u32& kDown, const u32& kHeld, const u32& kUp, const touchPosition* touch)
// --------------------------------------------------
{
	if (hasRegularChild() || hasOverlayChild()) { if (child->updateControls(kDown, kHeld, kUp, touch) == PARENT_STEP); else return CHILD_STEP; }

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
			cursorUBox.row += rowMod;
			cursorUBox.col += colMod;

			if (rowCount < 6) // in pc
			{
				if (cursorUBox.row < 0) { if (cursorUBox.col > colCount-1) cursorUBox.row = rowCount-2; else cursorUBox.row = rowCount-1; }
				else if (cursorUBox.row > rowCount-1) { cursorUBox.row = 0; }
				else if (cursorUBox.row > rowCount-2) { if (cursorUBox.col > colCount-1) if (colMod) cursorUBox.row = rowCount-1; else cursorUBox.row = 0; else cursorUBox.row = rowCount-1; }

				offsetTop = 0;
			}
			else // if in bank
			{
				if (cursorUBox.row < 0)
				{
					cursorUBox.row = rowCount + (cursorUBox.col > colCount-1 ? -2 : -1);
					offsetTop = (rowCount > ROW_COUNT ? cursorUBox.row+1 - ROW_COUNT : 0) * BOX_HEIGHT;
				}

				if (cursorUBox.row > rowCount-1)
				{
					cursorUBox.row = 0;
					offsetTop = 0;
				}

				if (cursorUBox.row > rowCount-2)
				{
					cursorUBox.row = (cursorUBox.col > colCount-1 && !colMod ? 0 : rowCount-1);
				}

				if (offsetTop >= cursorUBox.row * BOX_HEIGHT)
				{
					offsetTop = (cursorUBox.row + (cursorUBox.row > 0 ? -1 : 0)) * BOX_HEIGHT;
				}

				else if (cursorUBox.row > ROW_COUNT-2 && rowMod > 0 && offsetTop + (ROW_COUNT-2) * BOX_HEIGHT < cursorUBox.row * BOX_HEIGHT)
				{
					offsetTop = (cursorUBox.row + (cursorUBox.row < rowCount-1 ? 1 : 0) - ROW_COUNT) * BOX_HEIGHT;
				}
			}

			if (cursorUBox.col < 0) cursorUBox.col = currentColCount(cursorUBox.row)-1;
			else if (cursorUBox.col > currentColCount(cursorUBox.row)-1) cursorUBox.col = 0;

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
			originalTouch.py = touch->py;
			originalOffsetTop = offsetTop;
			// originalTouch.px = touch->px;
			// originalOffsetLeft = offsetLeft;

			uint16_t px = touch->px;
			uint16_t py = touch->py;

			// if (touchWithin(px, py, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - 40))
			// {
				printf("\x1B[1;20H{%3u, %3u}", ((py + offsetTop) / BOX_WIDTH), ((px + offsetLeft) / BOX_HEIGHT));

				uint16_t oldRow = cursorUBox.row;
				uint16_t oldCol = cursorUBox.col;

				cursorUBox.row = ((py + offsetTop) / BOX_WIDTH);
				cursorUBox.col = ((px + offsetLeft) / BOX_HEIGHT);

				// If we click on the selected box
				if (cursorUBox.row == oldRow && cursorUBox.col == oldCol)
				{
					closeViewer(true);
					return CHILD_STEP;
				}
				else if (cursorUBox.row < 0 || cursorUBox.row > (rowCount-1) || cursorUBox.col < 0 || cursorUBox.col > (currentColCount(cursorUBox.row)-1))
				{
					printf("\x1B[11;20HHors-limites   \n");
					cursorUBox.row = oldRow;
					cursorUBox.col = oldCol;
				}
				else
				{
					printf("\x1B[11;20HselectViewBox()\n");
					selectViewBox();
				}

				printf("\x1B[2;20H{%3u, %3u}", cursorUBox.row, cursorUBox.col);

				printf("\x1B[15;20H");
			// }
		}
		else if (kHeld & KEY_TOUCH)
		{
			this->touch = *touch;

			if (boxCount == PHBanku::save->bankdata.bk.boxUnlocked)
			{
				offsetTop = originalOffsetTop + originalTouch.py - touch->py;
				// offsetLeft = originalOffsetLeft + originalTouch.px - touch->px;

				if (offsetTop < 0)
					offsetTop = 0;
				else if (offsetTop + ROW_COUNT * BOX_HEIGHT > (rowCount-1) * BOX_HEIGHT)
					offsetTop = (rowCount-1) * BOX_HEIGHT - ROW_COUNT * BOX_HEIGHT;
			}
		}
	}

	if (kDown & KEY_Y)
	{
		printf("OffsetTop = %i (%i)\n", offsetTop, cursorUBox.row);
	}

	return SUCCESS_STEP;
}


// --------------------------------------------------
bool UltraBoxViewer::selectViewBox(uint16_t boxID, bool inBank)
// --------------------------------------------------
{
	cursorUBox.inBank = inBank;
	cursorUBox.slot = boxID;
	cursorUBox.row = (boxID / COL_COUNT);
	cursorUBox.col = (boxID % COL_COUNT);

	return selectViewBox();
}


		/*------------------------------------------*\
		 |             Protected Methods            |
		\*------------------------------------------*/


		/*------------------------------------------*\
		 |              Private Methods             |
		\*------------------------------------------*/


// --------------------------------------------------
bool UltraBoxViewer::selectViewBox()
// --------------------------------------------------
{
	computeSlot(&cursorUBox);

	if (cursorUBox.slot != -1)
	{
		vBox = PHBanku::save->getBox(cursorUBox.slot, cursorUBox.inBank);
		printf("View Box: [@%p]\n", vBox);
		return true;
	}

	return false;
}


// --------------------------------------------------
bool UltraBoxViewer::selectMoveBox()
// --------------------------------------------------
{
	if (vBox)
	{
		((BoxViewer*) parent)->selectViewBox(cursorUBox.slot, cursorUBox.inBank);
		return true;
	}
	return false;
}


// --------------------------------------------------
int16_t UltraBoxViewer::currentColCount(int16_t row)
// --------------------------------------------------
{
	return (row == rowCount - 1 ? colCount : COL_COUNT);
}


// --------------------------------------------------
ViewState UltraBoxViewer::closeViewer(bool save)
// --------------------------------------------------
{
	if (save) selectMoveBox();
	this->setState(ViewState::Exiting);
	return close();
}
