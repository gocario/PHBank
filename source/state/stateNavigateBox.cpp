#include "ui.hpp"

#include "pkbank.hpp"


// --------------------------------------------------
void _computeSlot(CursorBox_t* cursorBox)
// --------------------------------------------------
{
	bool BK = cursorBox->inBank;
	cursorBox->slot = (BK ? cursorBox->bBox : cursorBox->box) * BOX_PKMCOUNT + (BK ? cursorBox->bRow : cursorBox->row) * BOX_COL_PKMCOUNT + (BK ? cursorBox->bCol : cursorBox->col);
}


// --------------------------------------------------
void _selectPkm(State_t* state)
// --------------------------------------------------
{
	bool BK = state->cursorBox.inBank;
	uint16_t box = (BK ? state->cursorBox.bBox : state->cursorBox.box);
	uint16_t slot = (BK ? state->cursorBox.bRow : state->cursorBox.row) * BOX_COL_PKMCOUNT + (BK ? state->cursorBox.bCol : state->cursorBox.col);
	state->pkBank->getPkm(box, slot, &state->cursorBox.vPkm, BK);
	
	_computeSlot(&state->cursorBox);

	consoleSelect(state->console[0]);
	printf("Sel. %s ", (BK ? "BK" : "PC "));
	printf("Slot:%-3u (box:%-2u idx:%-2u) [@%8p]\n", state->cursorBox.slot, box, slot, state->cursorBox.vPkm);
	consoleSelect(state->console[1]);
}


/*------------------------------------------------------------*\
 |                     StateNavigatePCBox                     |
\*------------------------------------------------------------*/


// --------------------------------------------------
void stateNavigatePCBoxInit(State_t* state)
// --------------------------------------------------
{
	consoleSelect(state->console[1]);
	consoleClear();

	state->cursorBox.inBank = false;
	_selectPkm(state);
}


// --------------------------------------------------
void stateNavigatePCBoxDisplay(State_t* state)
// --------------------------------------------------
{
	printf("\x1B[0;0H");

	printf("[PC] Select a slot :\n");
	printf("Box: %-2i Row: %-2i Col: %-2i\n", state->cursorBox.box +1, state->cursorBox.row +1, state->cursorBox.col +1);


	if (state->cursorBox.vPkm)
	{
		printf("Species: %04u\n", state->cursorBox.vPkm->species);
		printf("TID: %05u\n", state->cursorBox.vPkm->TID);
		printf("SID: %05u\n", state->cursorBox.vPkm->SID);
		printf("PID: %010lu\n", state->cursorBox.vPkm->PID);
	}
	else
	{
		printf("\n\n\n\n");
	}
	printf("  +------+\n");

	for (s32 row = 0; row < BOX_ROW_PKMCOUNT; row++)
	{
		printf("  |");
		for (s32 col = 0; col < BOX_COL_PKMCOUNT; col++)
		{
			if ((row == state->cursorBox.row) && (col == state->cursorBox.col))
			{
				printf("x");
			}
			else
			{
				printf("o");
			}
		}
		printf("|\n");
	}

	printf("  +------+\n");

	for (uint32_t i = 0; i < 11; i++)
		printf("\n");
	for (uint32_t i = 0; i < 40; i++)
		printf("-");
	printf("DPad : Move Inbox\n");
	printf("L/R : Previous/Next Box\n");
	printf("A : Select Pokemon\n");
	printf("Y : Display Information\n");
	printf("Start : Return");
}


// --------------------------------------------------
void stateNavigatePCBoxInput(State_t* state)
// --------------------------------------------------
{
	u32 kDown = state->kDown;
	
	s16 boxMod = 0;
	s16 rowMod = 0;
	s16 colMod = 0;

	if (kDown & KEY_START)
	{
		state->ret = 0;
		return;
	}


	if (kDown & KEY_L)
	{
		boxMod--;
	}
	else if (kDown & KEY_R)
	{
		boxMod++;
	}

	if (kDown & KEY_UP)
	{
		rowMod--;
	}
	else if (kDown & KEY_DOWN)
	{
		rowMod++;
	}

	if (kDown & KEY_LEFT)
	{
		colMod--;
	}
	else if (kDown & KEY_RIGHT)
	{
		colMod++;
	}

	if (boxMod != 0 || rowMod != 0 || colMod != 0)
	{
		CursorBox_t* cursorBox = &(state->cursorBox);

		printf("\x1B[18;0H");
		printf(" Box: %-2i Row: %-2i Col: %-2i\n", cursorBox->box, cursorBox->row, cursorBox->col);

		cursorBox->box += boxMod;
		cursorBox->row += rowMod;
		cursorBox->col += colMod;

		printf(" Box: %-2i Row: %-2i Col: %-2i\n", cursorBox->box, cursorBox->row, cursorBox->col);

		if (cursorBox->box < 0)
			cursorBox->box = PC_BOXCOUNT -1;
		if (cursorBox->box > PC_BOXCOUNT -1)
			cursorBox->box = 0;

		if (cursorBox->row < 0)
			cursorBox->row = BOX_ROW_PKMCOUNT -1;
		if (cursorBox->row > BOX_ROW_PKMCOUNT -1)
			cursorBox->row = 0;

		if (cursorBox->col < 0)
			cursorBox->col = BOX_COL_PKMCOUNT -1;
		if (cursorBox->col > BOX_COL_PKMCOUNT -1)
			cursorBox->col = 0;

		printf(" Box: %-2i Row: %-2i Col: %-2i\n", cursorBox->box, cursorBox->row, cursorBox->col);

		_selectPkm(state);

		// cursorBox->slot = cursorBox->box * BOX_PKMCOUNT + cursorBox->row * BOX_COL_PKMCOUNT + cursorBox->col;
		cursorBox = NULL;		
	}

	if (kDown & KEY_Y)
	{
		consoleSelect(state->console[0]);

		printf("\n");
		PKBank::printPkm(state->cursorBox.vPkm, 0, PK6_SIZE);

		consoleSelect(state->console[1]);
	}

	if (kDown & (KEY_ZL | KEY_ZR))
	{
		switchState(state, stateNavigateBKBox);
	}
}


UIState_t stateNavigatePCBox = {&stateNavigatePCBoxInit, &stateNavigatePCBoxDisplay, &stateNavigatePCBoxInput};


/*------------------------------------------------------------*\
 |                     StateNavigateBKBox                     |
\*------------------------------------------------------------*/


// --------------------------------------------------
void stateNavigateBKBoxInit(State_t* state)
// --------------------------------------------------
{
	consoleSelect(state->console[1]);
	consoleClear();

	state->cursorBox.inBank = true;
	_selectPkm(state);
}

// --------------------------------------------------
void stateNavigateBKBoxDisplay(State_t* state)
// --------------------------------------------------
{
	printf("\x1B[0;0H");

	printf("[BK] Select a slot :\n");
	printf("Box: %-2i Row: %-2i Col: %-2i\n", state->cursorBox.bBox +1, state->cursorBox.bRow +1, state->cursorBox.bCol +1);


	if (state->cursorBox.vPkm)
	{
		printf("Species: %04u\n", state->cursorBox.vPkm->species);
		printf("TID: %05u\n", state->cursorBox.vPkm->TID);
		printf("SID: %05u\n", state->cursorBox.vPkm->SID);
		printf("PID: %010lu\n", state->cursorBox.vPkm->PID);
	}
	else
	{
		printf("\n\n\n\n");
	}
	printf("  +------+\n");

	for (s32 row = 0; row < BOX_ROW_PKMCOUNT; row++)
	{
		printf("  |");
		for (s32 col = 0; col < BOX_COL_PKMCOUNT; col++)
		{
			if ((row == state->cursorBox.bRow) && (col == state->cursorBox.bCol))
			{
				printf("x");
			}
			else
			{
				printf("o");
			}
		}
		printf("|\n");
	}

	printf("  +------+\n");

	for (uint32_t i = 0; i < 11; i++)
		printf("\n");
	for (uint32_t i = 0; i < 40; i++)
		printf("-");
	printf("DPad : Move Inbox\n");
	printf("L/R : Previous/Next Box\n");
	printf("A : Select Pokemon\n");
	printf("Y : Display Information\n");
	printf("Start : Return");
}


// --------------------------------------------------
void stateNavigateBKBoxInput(State_t* state)
// --------------------------------------------------
{
	u32 kDown = state->kDown;
	
	s16 boxMod = 0;
	s16 rowMod = 0;
	s16 colMod = 0;

	if (kDown & KEY_START)
	{
		state->ret = 0;
		return;
	}


	if (kDown & KEY_L)
	{
		boxMod--;
	}
	else if (kDown & KEY_R)
	{
		boxMod++;
	}

	if (kDown & KEY_UP)
	{
		rowMod--;
	}
	else if (kDown & KEY_DOWN)
	{
		rowMod++;
	}

	if (kDown & KEY_LEFT)
	{
		colMod--;
	}
	else if (kDown & KEY_RIGHT)
	{
		colMod++;
	}

	if (boxMod != 0 || rowMod != 0 || colMod != 0)
	{
		CursorBox_t* cursorBox = &(state->cursorBox);

		printf("\x1B[18;0H");
		printf(" Box: %-2i Row: %-2i Col: %-2i\n", cursorBox->bBox, cursorBox->bRow, cursorBox->bCol);

		cursorBox->bBox += boxMod;
		cursorBox->bRow += rowMod;
		cursorBox->bCol += colMod;

		printf(" Box: %-2i Row: %-2i Col: %-2i\n", cursorBox->bBox, cursorBox->bRow, cursorBox->bCol);

		if (cursorBox->bBox < 0)
			cursorBox->bBox = BANK_BOXCOUNT -1;
		if (cursorBox->bBox > BANK_BOXCOUNT -1)
			cursorBox->bBox = 0;

		if (cursorBox->bRow < 0)
			cursorBox->bRow = BOX_ROW_PKMCOUNT -1;
		if (cursorBox->bRow > BOX_ROW_PKMCOUNT -1)
			cursorBox->bRow = 0;

		if (cursorBox->bCol < 0)
			cursorBox->bCol = BOX_COL_PKMCOUNT -1;
		if (cursorBox->bCol > BOX_COL_PKMCOUNT -1)
			cursorBox->bCol = 0;

		printf(" Box: %-2i Row: %-2i Col: %-2i\n", cursorBox->bBox, cursorBox->bRow, cursorBox->bCol);

		_selectPkm(state);

		// cursorBox->slot = cursorBox->bBox * BOX_PKMCOUNT + cursorBox->bRow * BOX_COL_PKMCOUNT + cursorBox->bCol;
		cursorBox = NULL;		
	}

	if (kDown & KEY_Y)
	{
		consoleSelect(state->console[0]);

		printf("\n");
		PKBank::printPkm(state->cursorBox.vPkm, 0, PK6_SIZE);

		consoleSelect(state->console[1]);
	}

	if (kDown & (KEY_ZL | KEY_ZR))
	{
		switchState(state, stateNavigatePCBox);
	}
}


UIState_t stateNavigateBKBox = {&stateNavigateBKBoxInit, &stateNavigateBKBoxDisplay, &stateNavigateBKBoxInput};