#ifndef UI_HPP
#define UI_HPP

#include <3ds.h>

#include "pkbank.hpp"

struct State_t;

struct UIState_t
{
	void (*initf)(State_t *state);
	void (*dispf)(State_t *state);
	void (*inputf)(State_t *state);
};

namespace CursorType
{
	enum CursorType_e
	{
		None = 0x0,
		SingleSelect = 0x1,
		QuickSelect = 0x2,
		MultipleSelect = 0x3,
	};
}

typedef CursorType::CursorType_e CursorType_e;

struct CursorBox_t
{
	pkm_t* pkm = NULL;

	u16 slot = 0;
	s16 box = 0;
	s16 row = 0;
	s16 col = 0;

	u16 bSlot = 0;
	s16 bBox = 0;
	s16 bRow = 0;
	s16 bCol = 0;

	bool inBank = 0;
	CursorType_e cursorType = CursorType::SingleSelect;
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

extern UIState_t stateNavigateBox;


void switchState(State_t *state, struct UIState_t newState);

Result mainLoop(PKBank* bank, PrintConsole* top, PrintConsole* bot);

#endif // UI_HPP