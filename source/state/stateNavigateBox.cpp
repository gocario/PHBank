#include "ui.hpp"

#include "pkbank.hpp"

void stateNavigateBoxInit(State_t *state)
{
	consoleSelect(state->console[1]);
	consoleClear();
}

void stateNavigateBoxDisplay(State_t *state)
{
	printf("\x1B[0;0H");

	printf("Select a slot :\n");
	printf("Box: %-2i Row: %-2i Col: %-2i\n", state->cursorBox.box +1, state->cursorBox.row +1, state->cursorBox.col +1);


 	if (state->cursorBox.pkm)
 	{
 		printf("Species: %05u\n", (*state->cursorBox.pkm)[0x08] + 0x100 * (*state->cursorBox.pkm)[0x09]);
 		printf("TID: %05u\n", (*state->cursorBox.pkm)[0x0c] + 0x100 * (*state->cursorBox.pkm)[0x0d]);
 		printf("SID: %05u\n", (*state->cursorBox.pkm)[0x0e] + 0x100 * (*state->cursorBox.pkm)[0x0f]);
 	}
 	else
 	{
		printf("\n\n\n");
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

	for (uint32_t i = 0; i < 12; i++)
		printf("\n");
	for (uint32_t i = 0; i < 40; i++)
		printf("-");
	printf("<^v> : Move Inbox\n");
	printf("L/R : Previous/Next Box\n");
	printf("A : Display Information\n");
	printf("X : Display Encrypt summary\n");
	printf("Start : Return");
}

void stateNavigateBoxInput(State_t *state)
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

		printf("\x1B[20;0H");
		printf("\n Box: %-2i Row: %-2i Col: %-2i", cursorBox->box, cursorBox->row, cursorBox->col);

		cursorBox->box += boxMod;
		cursorBox->row += rowMod;
		cursorBox->col += colMod;

		printf("\n Box: %-2i Row: %-2i Col: %-2i", cursorBox->box, cursorBox->row, cursorBox->col);

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

		printf("\n Box: %-2i Row: %-2i Col: %-2i", cursorBox->box, cursorBox->row, cursorBox->col);

		cursorBox->slot = cursorBox->box * BOX_PKMCOUNT + cursorBox->row * BOX_COL_PKMCOUNT + cursorBox->col;
		cursorBox->pkm = &(state->pkBank->savedata->pc[cursorBox->box][cursorBox->row * BOX_COL_PKMCOUNT + cursorBox->col]);
		cursorBox = NULL;

		consoleSelect(state->console[0]);
		printf("\nSelected slot:%u (box:%u idx:%u)", state->cursorBox.slot, state->cursorBox.box, state->cursorBox.row * BOX_COL_PKMCOUNT + state->cursorBox.col);
		consoleSelect(state->console[1]);
		
	}

	if (kDown & KEY_A)
	{
		consoleSelect(state->console[0]);

		printf("\n");
		state->pkBank->printPkm(*state->cursorBox.pkm);

		consoleSelect(state->console[1]);
	}
}

UIState_t stateNavigateBox = {&stateNavigateBoxInit, &stateNavigateBoxDisplay, &stateNavigateBoxInput};