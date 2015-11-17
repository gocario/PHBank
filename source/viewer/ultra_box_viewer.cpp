#include "ultra_box_viewer.hpp"

#include "box_viewer.hpp"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define ROWCOUNT 5
#define COLCOUNT 8


// --------------------------------------------------
void computeSlot(CursorUBox_t* cursorUBox)
// --------------------------------------------------
{
	cursorUBox->slot = cursorUBox->row * COLCOUNT + cursorUBox->col;
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
UltraBoxViewer::UltraBoxViewer(StateView_e state, Viewer* parent) : Viewer(state, parent) { }
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
	if (hasChild()) { if (child->initialize() == PARENT_STEP) ; else return CHILD_STEP; }
	// Viewer::initialize(); // Not useful here!

	consoleClear();
	printf("Initialize();\n");
	computeSlot(&cursorUBox);
	selectViewBox();

	boxCount = (cursorUBox.inBank ? BANK_BOXCOUNT : PC_BOXCOUNT);
	rowCount = (boxCount / COLCOUNT) + 1;
	colCount = (boxCount % COLCOUNT);

	printf("\x1B[8;0H[%i | %i | %i]", boxCount, rowCount, colCount);

	return SUCCESS_STEP;
}


// --------------------------------------------------
Result UltraBoxViewer::drawTopScreen()
// --------------------------------------------------
{
	if (hasRegularChild()) { if (child->drawTopScreen() == PARENT_STEP); else return CHILD_STEP; }
	// Viewer::drawTopScreen(); // Not useful here!

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
			// If the current box is the box currently selected
			if (row == cursorUBox.row && col == cursorUBox.col)
			{
				// Draw the selected box icon
				sf2d_draw_rectangle(col * 40, row * 40 - offsetTop, 40, 40, RGBA8(0x00, 0x7F, 0xFF, 0xFF));
			}
			else
			{
				// Draw the normal box icon
				sf2d_draw_rectangle(col * 40, row * 40 - offsetTop, 40, 40, ((col % 2 == 0 && row % 2 == 1) || (col % 2 == 1 && row % 2 == 0) ? RGBA8(0xFF, 0x00, 0x00, 0xFF) : RGBA8(0x00, 0xFF, 0x00, 0xFF)));
			}
		}
	}

	// Draw the bottom-bar
	// TODO: REMOVE THIS SHIT OMAGAWD, IT'S HORRIBLE
	sf2d_draw_rectangle(0, 5 * 40, 320, 40, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));

	if (hasOverlayChild()) { child->drawBotScreen(); }
	return SUCCESS_STEP;
}


// --------------------------------------------------
Result UltraBoxViewer::updateControls(const u32& kDown, const u32& kHeld, const u32& kUp, const touchPosition* touch)
// --------------------------------------------------
{
	if (hasRegularChild() || hasOverlayChild()) { if (child->updateControls(kDown, kHeld, kUp, touch) == PARENT_STEP); else return CHILD_STEP; }

	if (kDown & KEY_B)
	{
		// Close the viewer
		return closeViewer(false);
	}

	if (kDown & KEY_A)
	{
		// Close the viewer and update the box of the main viewer
		return closeViewer(true);
	}


	{
		bool boolMod = false;
		int16_t rowMod = 0;
		int16_t colMod = 0;

		if (kDown & KEY_UP) { rowMod--; offsetTop -= 40; }
		else if (kDown & KEY_DOWN) { rowMod++; offsetTop += 40; }

		if (kDown & KEY_LEFT) colMod--;
		else if (kDown & KEY_RIGHT) colMod++;

		if (rowMod || colMod)
		{
			cursorUBox.row += rowMod;
			cursorUBox.col += colMod;

			if (cursorUBox.row < 0) { if (cursorUBox.col > colCount-1) cursorUBox.row = rowCount-2; else cursorUBox.row = rowCount-1; }
			else if (cursorUBox.row > rowCount-1) { cursorUBox.row = 0; }
			else if (cursorUBox.row > rowCount-2) { if (cursorUBox.col > colCount-1) if (colMod) cursorUBox.row = rowCount-1; else cursorUBox.row = 0; else cursorUBox.row = rowCount-1; }


			if (cursorUBox.col < 0) cursorUBox.col = currentColCount(cursorUBox.row)-1;
			else if (cursorUBox.col > currentColCount(cursorUBox.row)-1) cursorUBox.col = 0;


			if (rowCount < 6)
			{
				offsetTop = 0;
			}
			else
			{
				if (cursorUBox.row < rowCount -2)
				{
					offsetTop = (cursorUBox.row -2)  * 40;	
				}
				if (cursorUBox.row > rowCount - 3)
				{
					offsetTop = (rowCount - 5) * 40;
				}
				if (cursorUBox.row < 2)
				{
					offsetTop = 0;
				}
			}

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
			this->originalTouch.py = touch->py;
			originalOffsetTop = offsetTop;
			// this->originalTouch.px = touch->px;
			// originalOffsetLeft = offsetLeft;

			uint16_t px = touch->px;
			uint16_t py = touch->py;

			// if (touchWithin(px, py, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT))
			// {
				printf("\x1B[1;20H{%3u, %3u}", ((py + offsetTop) / 40), ((px + offsetLeft) / 40));

				uint16_t oldRow = cursorUBox.row;
				uint16_t oldCol = cursorUBox.col;

				cursorUBox.row = ((py + offsetTop) / 40);
				cursorUBox.col = ((px + offsetLeft) / 40);

				if (cursorUBox.row == oldRow && cursorUBox.col == oldCol)
				{
					return closeViewer(true);
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
			// }
		}
		else if (kHeld & KEY_TOUCH)
		{
			this->touch.px = touch->px;
			this->touch.py = touch->py;

			if (boxCount == BANK_BOXCOUNT)
			{
				offsetTop = originalOffsetTop + originalTouch.py - touch->py;
				// offsetLeft = originalOffsetLeft + originalTouch.px - touch->px;

				if (offsetTop < 0)
					offsetTop = 0;
				else if (offsetTop + 40 * 5 > 40 * (rowCount+1) - marginBottom)
					offsetTop = 40 * (rowCount+1) - (40 * 5) - marginBottom;
			}
		}
		else if (kUp & KEY_TOUCH)
		{
			touch = &(this->touch);
			printf("\x1B[3;20H{%3u, %3u} ", touch->px, touch->py);
		}
	}

	return SUCCESS_STEP;
}


// --------------------------------------------------
bool UltraBoxViewer::selectViewBox(uint16_t boxID, bool inBank)
// --------------------------------------------------
{
	cursorUBox.inBank = inBank;
	cursorUBox.slot = boxID;
	cursorUBox.row = (boxID / COLCOUNT);
	cursorUBox.col = (boxID % COLCOUNT);

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
		PHBank::pKBank()->getBox(cursorUBox.slot, &vBox, cursorUBox.inBank);
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
	return (row == rowCount - 1 ? colCount : COLCOUNT);
}


// --------------------------------------------------
Result UltraBoxViewer::closeViewer(bool save)
// --------------------------------------------------
{
	if (save)
		selectMoveBox();
	this->setLStateView(StateView::Exiting);
	// parent->setLStateView(StateView::Exiting);
	consoleClear();
	return close();
}
