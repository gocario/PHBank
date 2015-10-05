#include "ui.hpp"

#include <wchar.h>

#include "pkbank.hpp"

#define BOX_HEADER_SELECTED -1

/*------------------------------------------------------------*\
 |                        Navigate Box                        |
\*------------------------------------------------------------*/


// ==================================================
CursorPosition_t* _currentCursorPosition(CursorBox_t* cursorBox);
CursorPosition_t* _currentCursorPosition(State_t* state);
void _computeActualSlot(CursorBox_t* cursorBox);
void _computeActualSlot(State_t* state);
void _selectViewPkm(State_t* state);
void _movePkm(State_t* state);
void _moveBox(State_t* state);
void _init(State_t* state, bool inBank);
void _printBox(State_t* state, bool BK, int32_t rowOffset = 0, int32_t colOffset = 0);
void _printCommands(State_t* state);
void _printBotscreen(State_t* state);
Result _moveCursorInput(State_t* state);
Result _movePokemonInput(State_t* state);
// --------------------------------------------------


// ==================================================
CursorPosition_t* _currentCursorPosition(CursorBox_t* cursorBox)
// --------------------------------------------------
{
	return &(cursorBox->inBank ? cursorBox->cPosBK : cursorBox->cPosPC);
}


// ==================================================
CursorPosition_t* _currentCursorPosition(State_t* state)
// --------------------------------------------------
{
	return _currentCursorPosition(&state->cursorBox);
}


// ==================================================
void _computeActualSlot(CursorBox_t* cursorBox)
// --------------------------------------------------
{
	CursorPosition_t* cPos = _currentCursorPosition(cursorBox);

	cPos->inslot = (cPos->row == BOX_HEADER_SELECTED ? -1 : cPos->row * BOX_COL_PKMCOUNT + cPos->col);
	cPos->slot = (cPos->row == BOX_HEADER_SELECTED ? -1 : cPos->box * BOX_PKMCOUNT + cPos->inslot);
	cursorBox->slot = cPos->slot;
}


// ==================================================
void _computeActualSlot(State_t* state)
// --------------------------------------------------
{
	_computeActualSlot(&state->cursorBox);
}


// ==================================================
void _selectViewPkm(State_t* state)
// --------------------------------------------------
{
	CursorPosition_t* cPos;
	bool BK = state->cursorBox.inBank;

	_computeActualSlot(&state->cursorBox);
	cPos = _currentCursorPosition(&state->cursorBox);

	if (cPos->row == BOX_HEADER_SELECTED)
	{
		state->cursorBox.vPkm = NULL;
		return;
	}

	state->pkBank->getPkm(cPos->box, cPos->inslot, &state->cursorBox.vPkm, BK);
	
	consoleSelect(state->console[0]);
	printf("Sel %s   ", (BK ? "BK" : "PC"));
	printf("Slot:%-4i (box:%-3u idx:%-2i)  [@%8p]\n", state->cursorBox.slot, cPos->box, cPos->inslot, state->cursorBox.vPkm);
	consoleSelect(state->console[1]);
}


// ==================================================
void _movePkm(State_t* state)
// --------------------------------------------------
{
	if (_currentCursorPosition(state)->row == BOX_HEADER_SELECTED)
		return;

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

// ==================================================
void _moveBox(State_t* state)
// --------------------------------------------------
{
	consoleSelect(state->console[0]);
	state->pkBank->moveBox(state->cursorBox.cPosPC.box, false, state->cursorBox.cPosBK.box, true);
	consoleSelect(state->console[1]);
}


// ==================================================
void _init(State_t* state, bool inBank)
// --------------------------------------------------
{
	state->cursorBox.inBank = inBank;
	_selectViewPkm(state);
}


// ==================================================
void _printBox(State_t* state, bool BK, int32_t rowOffset, int32_t colOffset)
// --------------------------------------------------
{
	CursorPosition_t* cPos = &(BK ? state->cursorBox.cPosBK : state->cursorBox.cPosPC);
	int32_t maxRow = BOX_ROW_PKMCOUNT;
	int32_t maxCol = BOX_COL_PKMCOUNT;

	if (BK == state->cursorBox.inBank)
	{
		printf("\x1B[%lu;%luHvvvvvv", rowOffset - 1, colOffset + 1);
	}
	else
	{
		printf("\x1B[%lu;%luH      ", rowOffset - 1, colOffset + 1);
	}

	printf("\x1B[%lu;%luH/------\\", rowOffset + 0, colOffset);
	printf("\x1B[%lu;%luH|%c%s%3i|", rowOffset + 1, colOffset, (cPos->row == BOX_HEADER_SELECTED ? '>' : ' '), (BK ? "BK" : "PC"), cPos->box + 1);
	printf("\x1B[%lu;%luH+------+", rowOffset + 2, colOffset);
	for (int32_t row = 0; row < maxRow; row++)
	{
		printf("\x1B[%lu;%luH|", rowOffset + 3 + row, colOffset);
		for (int32_t col = 0; col < maxCol; col++)
		{
			if ((row == cPos->row) && (col == cPos->col))
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
	printf("\x1B[%lu;%luH\\------/", rowOffset + 3 + maxRow, colOffset);
}


// ==================================================
void _printCommands(State_t* state)
// --------------------------------------------------
{
	printf("\x1B[24;0H");
	printf("----------------------------------------");

	if (state->cursorBox.cursorType == CursorType::MultipleSelect)
	{
		if (_currentCursorPosition(state)->row == BOX_HEADER_SELECTED)
		printf(" Left/Right: M-Box | Up/Down: M-Inbox   ");
		else
		printf(" DPad: Move inbox  | CPad: Move inbox   ");
		if (state->cursorBox.inBank)
		printf(" L/R: Change box   | X/LZ/RZ: To PC     ");
		else
		printf(" L/R: Change box   | X/LZ/RZ: To Bank   ");
		printf("                   |                    ");
		printf("                   | Select: Chg Cursor ");
		printf(" Start: Exit       | R+Start: Save&exit ");
	}
	else if (state->cursorBox.inBank)
	{
		if (_currentCursorPosition(state)->row == BOX_HEADER_SELECTED)
		printf(" Left/Right: M-Box | Up/Down: M-Inbox   ");
		else
		printf(" DPad: Move inbox  | CPad: Move inbox   ");
		printf(" L/R: Change box   | X/LZ/RZ: To PC     ");
		if (state->cursorBox.cursorType == CursorType::SingleSelect)
		printf(" A: Select pkmn    | Y: Hexvalue (Pk6)  ");
		else
		printf(" A: Select pkmn    | Y: Swap CurrentBox ");
		printf(" B: Cancel sel.    |                    ");
		printf(" Start: Exit       | R+Start: Save&exit ");
	}
	else
	{
		if (_currentCursorPosition(state)->row == BOX_HEADER_SELECTED)
		printf(" Left/Right: MBox  | Up/Down: MInbox    ");
		else
		printf(" DPad: Move inbox  | CPad: Move inbox   ");
		printf(" L/R: Change box   | X/LZ/RZ: To Bank   ");
		if (state->cursorBox.cursorType == CursorType::SingleSelect)
		printf(" A: Select pkmn    | Y: Hexvalue (Pk6)  ");
		else
		printf(" A: Select pkmn    | Y: Swap CurrentBox ");
		printf(" B: Cancel sel.    |                    ");
		printf(" Start: Exit       | R+Start: Save&exit ");
	}
}


// ==================================================
void _printBotscreen(State_t* state)
// --------------------------------------------------
{
	consoleSelect(state->console[1]);

	CursorPosition_t* cPos = _currentCursorPosition(state);

	printf("\x1B[0;0H");

	printf("[%s] Select a slot:\n", (state->cursorBox.inBank ? "BK" : "PC"));
	printf("Box: %-2i Row: %-2i Col: %-2i\n", cPos->box +1, cPos->row +1, cPos->col +1);

	_printBox(state, false, 4, 1);
	_printBox(state, true, 4, 11);


	printf("\x1B[15;1H%s", state->pkBank->savedata->OTName);
	printf("\x1B[16;1H TID: %-5u", state->pkBank->savedata->TID);
	printf("\x1B[17;1H SID: %-5u", state->pkBank->savedata->SID);
	printf("\x1B[18;1H TSV: %-5u", state->pkBank->savedata->TSV);

	if (state->cursorBox.vPkm)
	{
		if (state->pkBank->isPkmEmpty(state->cursorBox.vPkm))
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
			printf("\x1B[6;21H %s", state->cursorBox.vPkm->species);
			printf("\x1B[7;21H TID: %-5u", state->cursorBox.vPkm->TID);
			printf("\x1B[8;21H SID: %-5u", state->cursorBox.vPkm->SID);
			printf("\x1B[9;21H PID: %-10lu", state->cursorBox.vPkm->PID);
			printf("\x1B[10;21H PSV: %-5u", state->cursorBox.vPkm->PSV);
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

	if (state->cursorBox.sPkm)
	{
		if (state->pkBank->isPkmEmpty(state->cursorBox.sPkm))
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
			printf("\x1B[13;21H %s", state->cursorBox.sPkm->species);
			printf("\x1B[14;21H TID: %-5u", state->cursorBox.sPkm->TID);
			printf("\x1B[15;21H SID: %-5u", state->cursorBox.sPkm->SID);
			printf("\x1B[16;21H PID: %-10lu", state->cursorBox.sPkm->PID);
			printf("\x1B[17;21H PSV: %-5u", state->cursorBox.sPkm->PSV);
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

	if (state->cursorBox.cursorType == CursorType::SingleSelect)
	{
		printf("\x1B[19;21HSingle selection  ");
	}
	else if (state->cursorBox.cursorType == CursorType::QuickSelect)
	{
		printf("\x1B[19;21HQuick selection   ");
	}
	else if (state->cursorBox.cursorType == CursorType::MultipleSelect)
	{
		printf("\x1B[19;21HMultiple selection");
	}

	_printCommands(state);
}


// ==================================================
Result _moveCursorInput(State_t* state)
// --------------------------------------------------
{
	Result ret = 0;
	u32 kDown = state->kDown;
	u32 kHeld = state->kHeld;
	

	if (kDown & KEY_START)
	{
		if (kHeld & KEY_R)
		{
			state->ret = STATE_SAVE;
			return 1;
		}
		else //if (kHeld & KEY_L)
		{
			state->ret = STATE_EXIT;
			return 1;
		}
	}


	int16_t boxMod = 0;
	int16_t rowMod = 0;
	int16_t colMod = 0;

	// Box
	if (kDown & KEY_L) boxMod--;
	else if (kDown & KEY_R) boxMod++;

	// Row
	if (kDown & KEY_UP) rowMod--;
	else if (kDown & KEY_DOWN) rowMod++;

	// Column (Box if row == BOX_HEADER_SELECTED)
	if (kDown & KEY_LEFT) { if ((state->cursorBox.inBank ? state->cursorBox.cPosBK.row : state->cursorBox.cPosPC.row) == BOX_HEADER_SELECTED) boxMod--; else colMod--; }
	else if (kDown & KEY_RIGHT) { if ((state->cursorBox.inBank ? state->cursorBox.cPosBK.row : state->cursorBox.cPosPC.row) == BOX_HEADER_SELECTED) boxMod++; else colMod++; }

	if (boxMod != 0 || rowMod != 0 || colMod != 0)
	{
		bool BK = state->cursorBox.inBank;
		CursorPosition_t* cPos = &(BK ? state->cursorBox.cPosBK : state->cursorBox.cPosPC);

		// printf("\x1B[18;0H");
		// printf(" Box: %-2i Row: %-2i Col: %-2i\n", *box, *row, *col);

		cPos->box += boxMod;
		cPos->row += rowMod;
		cPos->col += colMod;

		// printf(" Box: %-2i Row: %-2i Col: %-2i\n", *box, *row, *col);

		if (cPos->box < 0)
			cPos->box = (BK ? BANK_BOXCOUNT : PC_BOXCOUNT) -1;
		if (cPos->box > (BK ? BANK_BOXCOUNT : PC_BOXCOUNT) -1)
			cPos->box = 0;

		if (cPos->row < BOX_HEADER_SELECTED)
			cPos->row = BOX_ROW_PKMCOUNT -1;
		if (cPos->row > BOX_ROW_PKMCOUNT -1)
			cPos->row = BOX_HEADER_SELECTED;

		if (cPos->col < 0)
			cPos->col = BOX_COL_PKMCOUNT -1;
		if (cPos->col > BOX_COL_PKMCOUNT -1)
			cPos->col = 0;

		// printf(" Box: %-2i Row: %-2i Col: %-2i\n", *box, *row, *col);

		_selectViewPkm(state);
	}

	if (kDown & (KEY_X | KEY_ZL | KEY_ZR))
	{
		// Swap: PC <-> BK
		if (state->cursorBox.inBank)
			_init(state, false);
		else
			_init(state, true);

		// _init(state, !state->cursorBox.inBank);
	}

	return ret;
}


// ==================================================
Result _movePokemonInput(State_t* state)
// --------------------------------------------------
{
	Result ret = 0;
	u32 kDown = state->kDown;
	// u32 kHeld = state->kHeld;
	

	switch (state->cursorBox.cursorType)
	{
		case CursorType::SingleSelect:
		{
			if (kDown & KEY_Y)
			{
				consoleSelect(state->console[0]);

				printf("\n");
				PKBank::printPkm(state->cursorBox.vPkm, 0, PK6_SIZE);

				consoleSelect(state->console[1]);
			}

			if (kDown & KEY_A)
			{
				_movePkm(state);
			}

			if (kDown & KEY_B)
			{
				state->cursorBox.sPkm = NULL;
			}

			break;
		}
		case CursorType::QuickSelect:
		{
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
				_moveBox(state);
			}

			break;
		}
		case CursorType::MultipleSelect:
		case CursorType::None:
		default:
		{
			break;
		}
	}


	if (kDown & KEY_SELECT)
	{
		switch (state->cursorBox.cursorType)
		{
			case CursorType::SingleSelect:
			{
				state->cursorBox.cursorType = CursorType::QuickSelect;
				break;
			}
			case CursorType::QuickSelect:
			{
				state->cursorBox.cursorType = CursorType::MultipleSelect;
				break;
			}
			case CursorType::MultipleSelect:
			{
				state->cursorBox.cursorType = CursorType::SingleSelect;
				break;
			}
			case CursorType::None:
			default:
			{
				break;
			}
		}
	}

	return ret;
}


/*------------------------------------------------------------*\
 |                      StateNavigateBox                      |
\*------------------------------------------------------------*/


// ==================================================
void stateNavigateBoxInit(State_t* state);
void stateNavigateBoxDisplay(State_t* state);
void stateNavigateBoxInput(State_t* state);
// --------------------------------------------------


// ==================================================
void stateNavigateBoxInit(State_t* state)
// --------------------------------------------------
{
	consoleSelect(state->console[1]);
	consoleClear();

	_init(state, state->cursorBox.inBank);
}


// ==================================================
void stateNavigateBoxDisplay(State_t* state)
// --------------------------------------------------
{
	_printBotscreen(state);
}


// ==================================================
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

/*
// ==================================================
void stateNavigatePCBoxInit(State_t* state)
// --------------------------------------------------
{
	_init(state, false);
}


// ==================================================
void stateNavigatePCBoxDisplay(State_t* state)
// --------------------------------------------------
{
	_printBotscreen(state);
}


// ==================================================
void stateNavigatePCBoxInput(State_t* state)
// --------------------------------------------------
{
	if (_moveCursorInput(state)) return;
	if (_movePokemonInput(state)) return;
}


UIState_t stateNavigatePCBox = {&stateNavigatePCBoxInit, &stateNavigatePCBoxDisplay, &stateNavigatePCBoxInput};
*/

/*------------------------------------------------------------*\
 |                     StateNavigateBKBox                     |
\*------------------------------------------------------------*/

/*
// ==================================================
void stateNavigateBKBoxInit(State_t* state)
// --------------------------------------------------
{
	_init(state, true);
}


// ==================================================
void stateNavigateBKBoxDisplay(State_t* state)
// --------------------------------------------------
{
	_printBotscreen(state);
}


// ==================================================
void stateNavigateBKBoxInput(State_t* state)
// --------------------------------------------------
{
	if (_moveCursorInput(state)) return;
	if (_movePokemonInput(state)) return;
}


UIState_t stateNavigateBKBox = {&stateNavigateBKBoxInit, &stateNavigateBKBoxDisplay, &stateNavigateBKBoxInput};
*/
