#ifndef UI_HPP
#define UI_HPP

#include <3ds.h>

#include "pkbank.hpp"

struct State_t;

struct UIState_t
{
	void (*initf)(State_t* state);
	void (*dispf)(State_t* state);
	void (*inputf)(State_t* state);
};

struct State_t
{
	UIState_t uiState;
	PKBank* pkBank;
	CursorBox_t cursorBox;

	PrintConsole* console[2];
	Result ret;
	u32 kDown;
	u32 kHeld;
};

extern UIState_t stateNavigatePCBox;
extern UIState_t stateNavigateBKBox;


void switchState(State_t* state, struct UIState_t newState);

Result mainLoop(PKBank* bank, PrintConsole* top, PrintConsole* bot);

#endif // UI_HPP