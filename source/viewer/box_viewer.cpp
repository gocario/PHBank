#include "box_viewer.hpp"

#include "box_background.h"
#include "box_icons.h"
#include "box_tiles.h"


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
	if (background) sf2d_free_texture(background);
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


	if (!background)
		 background = sf2d_create_texture_mem_RGBA8(boxBackground23o_img.pixel_data, boxBackground23o_img.width, boxBackground23o_img.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
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
	if (isChildRegular()) { if (this->child->drawTopScreen() == PARENT_STEP); else return CHILD_STEP; }
	// Viewer::drawTopScreen();

	printf("\x1B[0;0H");

	printf("[%s] Select a slot:\n", (cursorBox.inBank ? "BK" : "PC"));
	printf("Box: %-2i Row: %-2i Col: %-2i\n", *cursorBox.box + 1, cursorBox.row + 1, cursorBox.col + 1);

	printf("\x1B[15;1H%s", PHBank::pKBank()->savedata->OTName);
	printf("\x1B[16;1H TID: %-5u", PHBank::pKBank()->savedata->TID);
	printf("\x1B[17;1H SID: %-5u", PHBank::pKBank()->savedata->SID);
	printf("\x1B[18;1H TSV: %-5u", PHBank::pKBank()->savedata->TSV);

	if (vPkm)
	{
		if (PHBank::pKBank()->isPkmEmpty(vPkm))
		{
			printf("\x1B[5;21HNo Pokemon     ");
			printf("\x1B[6;21H Empty Slot       ");
			printf("\x1B[7;21H           ");
			printf("\x1B[8;21H           ");
			printf("\x1B[9;21H                ");
			printf("\x1B[10;21H                ");
		}
		else
		{
			printf("\x1B[5;21HCurrent Pokemon ");
			printf("\x1B[6;21H                  ");
			printf("\x1B[6;21H %s", vPkm->species);
			printf("\x1B[7;21H TID: %-5u", vPkm->TID);
			printf("\x1B[8;21H SID: %-5u", vPkm->SID);
			printf("\x1B[9;21H PID: %-10lu", vPkm->PID);
			printf("\x1B[10;21H PSV: %-5u", vPkm->PSV);
		}
	}
	else
	{
		printf("\x1B[5;21H               ");
		printf("\x1B[6;21H                  ");
		printf("\x1B[7;21H           ");
		printf("\x1B[8;21H           ");
		printf("\x1B[9;21H                ");
		printf("\x1B[10;21H                ");
	}

	if (sPkm)
	{
		if (PHBank::pKBank()->isPkmEmpty(sPkm))
		{
			printf("\x1B[12;21HSelected Pokemon");
			printf("\x1B[13;21H Empty Slot       ");
			printf("\x1B[14;21H           ");
			printf("\x1B[15;21H           ");
			printf("\x1B[16;21H                ");
			printf("\x1B[17;21H                ");
		}
		else
		{
			printf("\x1B[12;21HSelected Pokemon");
			printf("\x1B[13;21H                  ");
			printf("\x1B[13;21H %s", sPkm->species);
			printf("\x1B[14;21H TID: %-5u", sPkm->TID);
			printf("\x1B[15;21H SID: %-5u", sPkm->SID);
			printf("\x1B[16;21H PID: %-10lu", sPkm->PID);
			printf("\x1B[17;21H PSV: %-5u", sPkm->PSV);
		}
	}
	else
	{
		printf("\x1B[12;21HNo Selection    ");
		printf("\x1B[13;21H                  ");
		printf("\x1B[14;21H           ");
		printf("\x1B[15;21H           ");
		printf("\x1B[16;21H                ");
		printf("\x1B[17;21H                ");
	}

	if (cursorType == CursorType::SingleSelect)
	{
		printf("\x1B[19;21HSingle selection  ");
	}
	else if (cursorType == CursorType::QuickSelect)
	{
		printf("\x1B[19;21HQuick selection   ");
	}
	else if (cursorType == CursorType::MultipleSelect)
	{
		printf("\x1B[19;21HMultiple selection");
	}


	if (isChildOverlay()) { this->child->drawTopScreen(); }
	return SUCCESS_STEP;
}


// --------------------------------------------------
Result BoxViewer::drawBotScreen()
// --------------------------------------------------
{
	if (isChildRegular()) { if (this->child->drawBotScreen() == PARENT_STEP); else return CHILD_STEP; }
	// Viewer::drawBotScreen();
	
	int16_t boxShift;
	box_t** vBox = NULL;


	/*\* Current Box *\*/
	{
		boxShift = (cursorBox.inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED);
		vBox = &(cursorBox.inBank ? vBKBox : vPCBox);

		// Draw Background
		sf2d_draw_texture(background, boxShift, 20);
		
		// Draw Pokémon icons
		for (uint32_t i = 0; i < 30; i++)
			sf2d_draw_texture_part(icons, boxShift + (i % BOX_COL_PKMCOUNT) * 35, (i / BOX_COL_PKMCOUNT) * 35 + 50, (((*vBox)->slot[i].speciesID-1) % 25) * 40, (((*vBox)->slot[i].speciesID-1) / 25) * 30, 40, 30);

		// Draw CursorType buttons
		sf2d_draw_texture_part(tiles, boxShift + 21 +   0, 0,   0, 0, 64, 32);
		sf2d_draw_texture_part(tiles, boxShift + 21 +  64, 0,  64, 0, 64, 32);
		sf2d_draw_texture_part(tiles, boxShift + 21 + 128, 0, 128, 0, 64, 32);

		// Draw SwapBox buttons
		sf2d_draw_texture_part(tiles, boxShift + 10 +  0, 20,   0, 64, 16, 24);
		sf2d_draw_texture_part(tiles, boxShift + BACKGROUND_WIDTH - 24, 20,  16, 64, 16, 24);

		// Draw Cursor
		if (cursorBox.inslot == SLOT_NO_SELECTION)
			sf2d_draw_texture_part(tiles, boxShift + 105, 18, 32 * cursorType, 32, 32, 32);
		else
			sf2d_draw_texture_part(tiles, boxShift + 17 + (cursorBox.inslot % 6) * 35, 20 + 13 + (cursorBox.inslot / 6) * 35, 32 * cursorType, 32, 32, 32);
	}

	/*\* The other Box *\*/
	{
		boxShift = (cursorBox.inBank ? PC_BOX_SHIFT_UNUSED : BK_BOX_SHIFT_UNUSED);
		vBox = &(cursorBox.inBank ? vPCBox : vBKBox);

		// Draw Background
		sf2d_draw_texture(background, boxShift, 20);
		
		// Draw Pokémon icons
		for (uint32_t i = 0; i < 30; i++)
			sf2d_draw_texture_part(icons, boxShift + (i % 6) * 35, (i / 6) * 35 + 50, (((*vBox)->slot[i].speciesID-1) % 25) * 40, (((*vBox)->slot[i].speciesID-1) / 25) * 30, 40, 30);
	}


	if (isChildOverlay()) { this->child->drawBotScreen(); }
	return SUCCESS_STEP;
}


// --------------------------------------------------
Result BoxViewer::updateControls(const u32& kDown, const u32& kHeld, const u32& kUp, const touchPosition* touch)
// --------------------------------------------------
{
	if (isChildRegular()) { if (this->child->updateControls(kDown, kHeld, kUp, touch) == PARENT_STEP); else return CHILD_STEP; }
	else if (isChildOverlay()) { if (this->child->updateControls(kDown, kHeld, kUp, touch) == PARENT_STEP); else return CHILD_STEP; }
		
	if (kDown & KEY_START)
	{
      // Messages disappear with time?
      // I'm probably missing something about how the screen refreshes...
      printf("\x1B[12;12H------------");
  		printf("\x1B[13;12H EXITING    ");
	  	printf("\x1B[14;12H------------");
		  printf("\x1B[15;12H SAVE?      ");
			printf("\x1B[16;12H R=YES      ");    
  		printf("\x1B[17;12H B=NO       ");    
	  	printf("\x1B[18;12H L=CANCEL   ");    

      // Logic is here, might not be most eloquent way.
      // Tested and works
      bool breaker=false;
      while(!breaker){

        hidScanInput();
    		u32 down = hidKeysDown();
        if(down & KEY_R){
          breaker=true;
  			  lStateView = StateView::Saving;
        }
        else if(down & KEY_B){
          breaker=true;          
	  	  	lStateView = StateView::Exiting;
        } 
        // If L-cancel is added, return statement
        // must change.
        if(down & KEY_L){
          breaker=true;
          return SUCCESS_STEP;
        }
      }

      return this->exit();

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

		if (boxMod || rowMod || colMod || boolMod)
		{
			currentBox(&cursorBox);
			*cursorBox.box += boxMod;
			cursorBox.row += rowMod;
			cursorBox.col += colMod;

			if (*cursorBox.box < 0) *cursorBox.box = (cursorBox.inBank ? BANK_BOXCOUNT : PC_BOXCOUNT)-1;
			else if (*cursorBox.box > (cursorBox.inBank ? BANK_BOXCOUNT : PC_BOXCOUNT)-1) *cursorBox.box = 0;

			if (cursorBox.row < BOX_HEADER_SELECTED) cursorBox.row = BOX_ROW_PKMCOUNT-1;
			else if (cursorBox.row > BOX_ROW_PKMCOUNT-1) cursorBox.row = BOX_HEADER_SELECTED;

			// if (cursorBox.col < 0) cursorBox.col = BOX_COL_PKMCOUNT-1;
			// else if (cursorBox.col > BOX_COL_PKMCOUNT-1) cursorBox.col = 0;

			if (cursorBox.col < 0) { cursorBox.col = BOX_COL_PKMCOUNT-1; cursorBox.inBank = !cursorBox.inBank; }
			else if (cursorBox.col > BOX_COL_PKMCOUNT-1) { cursorBox.col = 0; cursorBox.inBank = !cursorBox.inBank; }

			selectViewBox();
			selectViewPokemon();
		}
	}

	if (kDown & KEY_SELECT)
	{
		// Swap cursor mode (quick/single/multi selection)
		switchCursorType();
	}

	if (kDown & KEY_X)
	{
		printf("\x1b[2J");
	}

	if (cursorType == CursorType::SingleSelect)
	{
		if (kDown & KEY_A)
		{
			selectMovePokemon();
		}

		if (kDown & KEY_B)
		{
			cancelMovePokemon();
		}

		if (kDown & KEY_Y)
		{
			if (vPkm)
			{
				// printf("\n");
				PHBank::pKBank()->printPkm(vPkm, 0, PK6_SIZE);
			}
		}
	}
	else if (cursorType == CursorType::QuickSelect)
	{
		if (kDown & KEY_A)
		{
			selectMovePokemon();
		}

		if (kDown & KEY_B)
		{
			cancelMovePokemon();
		}

		if (kDown & KEY_Y)
		{
			if (vPCBox && vBKBox)
			{
				PHBank::pKBank()->moveBox(cursorBox.boxPC, false, cursorBox.boxBK, true);
			}
		}
	}
	else if (cursorType == CursorType::MultipleSelect)
	{

	}
	
	if (kDown & KEY_TOUCH)
	{
		int16_t boxShift = (cursorBox.inBank ? BK_BOX_SHIFT_USED : PC_BOX_SHIFT_USED);
		uint16_t px = touch->px;
		uint16_t py = touch->py;
		// printf("{%3u, %3u}", px, py);
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

		if (touchWithin(px, py, boxShift, 20, BACKGROUND_WIDTH, BACKGROUND_HEIGHT))
		{
			int16_t oldRow = cursorBox.row;
			int16_t oldCol = cursorBox.col;

			cursorBox.row = ((py - 50) / 35);
			cursorBox.col = ((px - boxShift) / 35);

			if (cursorBox.row == oldRow && cursorBox.col == oldCol)
				selectMovePokemon();
			selectViewPokemon();
		}

		// printf("\n");
	}

	return SUCCESS_STEP;
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
		vPkm = NULL;
	}
	else
	{
		PHBank::pKBank()->getPkm(*cursorBox.box, cursorBox.inslot, &vPkm, cursorBox.inBank);
	}

	// printf("View Pokemon: [@%p]\n", vPkm);
}


// --------------------------------------------------
void BoxViewer::selectMovePokemon()
// --------------------------------------------------
{
	computeSlot(&cursorBox);

	if (!sPkm)
	{
		sPkm = vPkm;
	}
	else if (vPkm)
	{
		if (sPkm != vPkm)
		{
			PHBank::pKBank()->movePkm(sPkm, vPkm, false, cursorBox.inBank);
		}

		cancelMovePokemon();
	}

	// printf("Selected Pokemon: [@%p]\n", vPkm);
}


// --------------------------------------------------
void BoxViewer::cancelMovePokemon()
// --------------------------------------------------
{
	sPkm = NULL;
}
