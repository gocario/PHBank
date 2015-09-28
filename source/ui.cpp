#include "ui.hpp"

void switchState(State_t* state, struct UIState_t newState)
{
	state->uiState = newState;
	state->uiState.initf(state);
}


Result mainLoop(PKBank* pkBank, PrintConsole* top, PrintConsole* bot)
{
	State_t state;

	state.pkBank = pkBank;
	// state.cursorBox.slot = 0;
	// state.cursorBox.box = 0;
	// state.cursorBox.row = 0;
	// state.cursorBox.col = 0;
	// state.cursorBox.cursorType = CursorType::SelectSingle;
	state.console[0] = top;
	state.console[1] = bot;
	state.ret = 1;

	switchState(&state, stateNavigateBox);

	while (state.ret > 0 && aptMainLoop())
	{
		hidScanInput();
		state.kDown = hidKeysDown();
		state.kHeld = hidKeysHeld();
		state.uiState.dispf(&state);
		state.uiState.inputf(&state);

		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}

	return (state.ret == 0 ? 0 : 1);
}