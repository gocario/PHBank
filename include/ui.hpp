#ifndef UI_HPP
#define UI_HPP

#include <3ds.h>

#include "pkbank.hpp"


#define STATE_CONTINUE 1
#define STATE_PAUSE 2
#define STATE_EXIT 3
#define STATE_SAVE 4


typedef struct State_t State_t;

typedef struct UIState_t
{
	void (*initf)(State_t* state);
	void (*dispf)(State_t* state);
	void (*inputf)(State_t* state);
} UIState_t;

typedef struct State_t
{
	UIState_t uiState;
	CursorBox_t cursorBox;
	PKBank* pkBank;

	PrintConsole *console[2];
	Result ret;
	u32 kDown;
	u32 kHeld;
} State_t;


extern UIState_t stateNavigateBox;
// extern UIState_t stateNavigatePCBox;
// extern UIState_t stateNavigateBKBox;


void switchState(State_t* state, struct UIState_t newState);

Result mainLoop(PKBank* bank, PrintConsole* top, PrintConsole* bot);

#endif // UI_HPP