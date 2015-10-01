#include "ui.hpp"

#include "pkbank.hpp"


/*------------------------------------------------------------*\
 |                        Navigate Box                        |
\*------------------------------------------------------------*/


// --------------------------------------------------
void _computeActualSlot(CursorBox_t* cursorBox, uint16_t* pbox = NULL, uint16_t* prow = NULL, uint16_t* pcol = NULL, uint16_t* pins = NULL);
void _selectViewPkm(State_t* state);
void _movePkm(State_t* state);
void _init(State_t* state, bool inBank);
void _printBox(State_t* state, bool BK, uint32_t rowOffset = 0, uint32_t colOffset = 0);
void _printCommands(State_t* state);
void _printBotscreen(State_t* state);
Result _moveCursorInput(State_t* state);
Result _movePokemonInput(State_t* state);
// --------------------------------------------------


// --------------------------------------------------
void _computeActualSlot(CursorBox_t* cursorBox, uint16_t* pbox, uint16_t* prow, uint16_t* pcol, uint16_t* pins)
// --------------------------------------------------
{
	bool BK = cursorBox->inBank;
	uint16_t box = (BK ? cursorBox->bkBox : cursorBox->pcBox);
	uint16_t row = (BK ? cursorBox->bkRow : cursorBox->pcRow);
	uint16_t col = (BK ? cursorBox->bkCol : cursorBox->pcCol);
	uint16_t ins = row * BOX_COL_PKMCOUNT + col;

	cursorBox->slot = box * BOX_PKMCOUNT + ins;

	if (BK)
		cursorBox->bkSlot = cursorBox->slot;
	else
		cursorBox->pcSlot = cursorBox->slot;

	if (pbox) *pbox = box;
	if (prow) *prow = row;
	if (pcol) *pcol = col;
	if (pins) *pins = ins;
}


// --------------------------------------------------
void _selectViewPkm(State_t* state)
// --------------------------------------------------
{
	bool BK = state->cursorBox.inBank;
	uint16_t box, row, col, inslot;

	_computeActualSlot(&state->cursorBox, &box, &row, &col, &inslot);

	state->pkBank->getPkm(box, inslot, &state->cursorBox.vPkm, BK);
	
	consoleSelect(state->console[0]);
	printf("Sel %s   ", (BK ? "BK" : "PC"));
	printf("Slot:%-4u (box:%-3u idx:%-2u)  [@%8p]\n", state->cursorBox.slot, box, inslot, state->cursorBox.vPkm);
	consoleSelect(state->console[1]);
}


// --------------------------------------------------
void _movePkm(State_t* state)
// --------------------------------------------------
{
	if (!state->cursorBox.sPkm)
	{
		state->cursorBox.sPkm = state->cursorBox.vPkm;
	}
	else
	{
		if (state->cursorBox.sPkm != state->cursorBox.vPkm)
		{
			state->pkBank->movePkm(state->cursorBox.sPkm, state->cursorBox.vPkm);
		}

		state->cursorBox.sPkm = NULL;
	}
}


// --------------------------------------------------
void _init(State_t* state, bool inBank)
// --------------------------------------------------
{
	consoleSelect(state->console[1]);
	consoleClear();
	state->cursorBox.inBank = inBank;
	_selectViewPkm(state);
}


// --------------------------------------------------
void _printBox(State_t* state, bool BK, uint32_t rowOffset, uint32_t colOffset)
// --------------------------------------------------
{
	int32_t selBox = (BK ? state->cursorBox.bkBox : state->cursorBox.pcBox);
	int32_t selRow = (BK ? state->cursorBox.bkRow : state->cursorBox.pcRow);
	int32_t selCol = (BK ? state->cursorBox.bkCol : state->cursorBox.pcCol);
	int32_t maxRow = BOX_ROW_PKMCOUNT;
	int32_t maxCol = BOX_COL_PKMCOUNT;

	if (BK == state->cursorBox.inBank)
	{
		printf("\x1B[%lu;%luH>", rowOffset + 1, colOffset - 1);
	}

	printf("\x1B[%lu;%luH+------+", rowOffset + 0, colOffset);
	printf("\x1B[%lu;%luH|%c%s%3lu|", rowOffset + 1, colOffset, (selRow == -1 ? '>' : ' '), (BK ? "BK" : "PC"), selBox + 1);
	printf("\x1B[%lu;%luH+------+", rowOffset + 2, colOffset);
	for (int32_t row = 0; row < maxRow; row++)
	{
		printf("\x1B[%lu;%luH|", rowOffset + 3 + row, colOffset);
		for (int32_t col = 0; col < maxCol; col++)
		{
			if ((row == selRow) && (col == selCol))
			{
				printf("x");
			}
			else
			{
				printf("o");
			}
		}
		printf("|");
	}
	printf("\x1B[%lu;%luH+------+", rowOffset + 3 + maxRow, colOffset);
}


// --------------------------------------------------
void _printCommands(State_t* state)
// --------------------------------------------------
{
	printf("\x1B[24;0H");
	printf("----------------------------------------");

	if (state->cursorBox.inBank)
	{
		printf(" DPad: Move inbox  | CPad: Move inbox   ");
		printf(" L/R: Change box   | LZ/RZ: To PC       ");
		printf(" A: Select pkmn    | B: Cancel sel.     ");
		printf(" X: To PC          | Y: Hexvalue (Pk6)  ");
		printf(" Start: Exit       | Select: Save&exit  ");
	}
	else
	{
		printf(" DPad: Move inbox  | CPad: Move inbox   ");
		printf(" L/R: Change box   | LZ/RZ: To Bank     ");
		printf(" A: Select pkmn    | B: Cancel sel.     ");
		printf(" X: To Bank        | Y: Hexvalue (Pk6)  ");
		printf(" Start: Exit       | Select: Save&exit  ");
	}
}


// --------------------------------------------------
void _printBotscreen(State_t* state)
// --------------------------------------------------
{
	consoleSelect(state->console[1]);

	printf("\x1B[0;0H");

	printf("[%s] Select a slot:\n", (state->cursorBox.inBank ? "BK" : "PC"));
	printf("Box: %-2i Row: %-2i Col: %-2i\n", state->cursorBox.pcBox +1, state->cursorBox.pcRow +1, state->cursorBox.pcCol +1);

	_printBox(state, false, 4, 1);
	_printBox(state, true, 4, 11);

	if (state->cursorBox.vPkm)
	{
		printf("\x1B[5;21HCurrent Pokemon");
		printf("\x1B[6;21H Species: %-3u", state->cursorBox.vPkm->species);
		printf("\x1B[7;21H TID: %-5u", state->cursorBox.vPkm->TID);
		printf("\x1B[8;21H SID: %-5u", state->cursorBox.vPkm->SID);
		printf("\x1B[9;21H PID: %-10lu", state->cursorBox.vPkm->PID);
	}

	if (state->cursorBox.sPkm)
	{
		printf("\x1B[12;21HSelected Pokemon");
		printf("\x1B[13;21H Species: %-3u", state->cursorBox.sPkm->species);
		printf("\x1B[14;21H TID: %-5u", state->cursorBox.sPkm->TID);
		printf("\x1B[15;21H SID: %-5u", state->cursorBox.sPkm->SID);
		printf("\x1B[16;21H PID: %-10lu", state->cursorBox.sPkm->PID);
	}
	else
	{
		printf("\x1B[12;21HNo Selection    ");
		printf("\x1B[13;21H             ");
		printf("\x1B[14;21H           ");
		printf("\x1B[15;21H           ");
		printf("\x1B[16;21H                ");
	}

	_printCommands(state);
}


// --------------------------------------------------
Result _moveCursorInput(State_t* state)
// --------------------------------------------------
{
	Result ret = 0;
	u32 kDown = state->kDown;
	
	if (kDown & KEY_START)
	{
		state->ret = STATE_EXIT;
		return 1;
	}

	if (kDown & KEY_SELECT)
	{
		state->ret = STATE_SAVE;
		return 1;
	}

	s16 boxMod = 0;
	s16 rowMod = 0;
	s16 colMod = 0;

	// Box
	if (kDown & KEY_L) boxMod--;
	else if (kDown & KEY_R) boxMod++;

	// Row
	if (kDown & KEY_UP) rowMod--;
	else if (kDown & KEY_DOWN) rowMod++;

	// Column (Box if row == -1)
	if (kDown & KEY_LEFT) { if (state->cursorBox.bkRow == -1) boxMod--; else colMod--; }
	else if (kDown & KEY_RIGHT) { if (state->cursorBox.bkRow == -1) boxMod++; else colMod++; }

	if (boxMod != 0 || rowMod != 0 || colMod != 0)
	{
		s16 *box, *row, *col;

		if (state->cursorBox.inBank)
		{
			box = &state->cursorBox.bkBox;
			row = &state->cursorBox.bkRow;
			col = &state->cursorBox.bkCol;
		}
		else
		{
			box = &state->cursorBox.pcBox;
			row = &state->cursorBox.pcRow;
			col = &state->cursorBox.pcCol;
		}

		// printf("\x1B[18;0H");
		// printf(" Box: %-2i Row: %-2i Col: %-2i\n", *box, *row, *col);

		*box += boxMod;
		*row += rowMod;
		*col += colMod;

		// printf(" Box: %-2i Row: %-2i Col: %-2i\n", *box, *row, *col);

		if (*box < 0)
			*box = BANK_BOXCOUNT -1;
		if (*box > BANK_BOXCOUNT -1)
			*box = 0;

		if (*row < -1)
			*row = BOX_ROW_PKMCOUNT -1;
		if (*row > BOX_ROW_PKMCOUNT -1)
			*row = -1;

		if (*col < 0)
			*col = BOX_COL_PKMCOUNT -1;
		if (*col > BOX_COL_PKMCOUNT -1)
			*col = 0;

		// printf(" Box: %-2i Row: %-2i Col: %-2i\n", *box, *row, *col);

		if (*row != -1)
		{
			_selectViewPkm(state);
		}
	}

	if (kDown & (KEY_X | KEY_ZL | KEY_ZR))
	{
		if (state->cursorBox.inBank)
			switchState(state, stateNavigatePCBox);
		else
			switchState(state, stateNavigateBKBox);
		return 1;
	}

	return ret;
}


// --------------------------------------------------
Result _movePokemonInput(State_t* state)
// --------------------------------------------------
{
	Result ret = 0;
	u32 kDown = state->kDown;
	

	if (kDown & KEY_A)
	{
		_movePkm(state);
	}

	if (kDown & KEY_B)
	{
		state->cursorBox.sPkm = NULL;
	}
	
	if (kDown & KEY_Y)
	{
		consoleSelect(state->console[0]);

		printf("\n");
		PKBank::printPkm(state->cursorBox.vPkm, 0, PK6_SIZE);

		consoleSelect(state->console[1]);
	}

	return ret;
}


/*------------------------------------------------------------*\
 |                      StateNavigateBox                      |
\*------------------------------------------------------------*/


// --------------------------------------------------
void stateNavigateBoxInit(State_t* state);
void stateNavigateBoxDisplay(State_t* state);
void stateNavigateBoxInput(State_t* state);
// --------------------------------------------------


// --------------------------------------------------
void stateNavigateBoxInit(State_t* state)
// --------------------------------------------------
{
	_init(state, state->cursorBox.inBank);
}


// --------------------------------------------------
void stateNavigateBoxDisplay(State_t* state)
// --------------------------------------------------
{
	_printBotscreen(state);
}


// --------------------------------------------------
void stateNavigateBoxInput(State_t* state)
// --------------------------------------------------
{
	if (_moveCursorInput(state)) return;
	if (_movePokemonInput(state)) return;
}


UIState_t stateNavigateBox = {&stateNavigateBoxInit, &stateNavigateBoxDisplay, &stateNavigateBoxInput};


/*------------------------------------------------------------*\
 |                     StateNavigatePCBox                     |
\*------------------------------------------------------------*/


// --------------------------------------------------
void stateNavigatePCBoxInit(State_t* state)
// --------------------------------------------------
{
	_init(state, false);
}


// --------------------------------------------------
void stateNavigatePCBoxDisplay(State_t* state)
// --------------------------------------------------
{
	_printBotscreen(state);
}


// --------------------------------------------------
void stateNavigatePCBoxInput(State_t* state)
// --------------------------------------------------
{
	if (_moveCursorInput(state)) return;
	if (_movePokemonInput(state)) return;
}


UIState_t stateNavigatePCBox = {&stateNavigatePCBoxInit, &stateNavigatePCBoxDisplay, &stateNavigatePCBoxInput};


/*------------------------------------------------------------*\
 |                     StateNavigateBKBox                     |
\*------------------------------------------------------------*/


// --------------------------------------------------
void stateNavigateBKBoxInit(State_t* state)
// --------------------------------------------------
{
	_init(state, true);
}


// --------------------------------------------------
void stateNavigateBKBoxDisplay(State_t* state)
// --------------------------------------------------
{
	_printBotscreen(state);
}


// --------------------------------------------------
void stateNavigateBKBoxInput(State_t* state)
// --------------------------------------------------
{
	if (_moveCursorInput(state)) return;
	if (_movePokemonInput(state)) return;
}


UIState_t stateNavigateBKBox = {&stateNavigateBKBoxInit, &stateNavigateBKBoxDisplay, &stateNavigateBKBoxInput};