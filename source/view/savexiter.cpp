#include "savexiter.hpp"

#include "text.h"
#include "phbank.hpp"

#include <stdio.h>

// --------------------------------------------------
Savexiter::Savexiter(Scene* parent) : Scene(parent) { }
// --------------------------------------------------

// --------------------------------------------------
Savexiter::Savexiter(SceneType sType, Scene* parent) : Scene(sType, parent) { }
// --------------------------------------------------

// --------------------------------------------------
Savexiter::~Savexiter()
// --------------------------------------------------
{
	// Destruct your allocated memory here!
	// TODO: Content may be missing!
}

// --------------------------------------------------
Result Savexiter::initialize()
// --------------------------------------------------
{
	if (hasChild()) { if (child->initialize() == PARENT_STEP) ; else return CHILD_STEP; }

	return SUCCESS_STEP;
}

// --------------------------------------------------
Result Savexiter::drawTopScreen()
// --------------------------------------------------
{
	if (hasRegularChild()) { if (child->drawTopScreen() == PARENT_STEP); else return CHILD_STEP; }

	sf2d_draw_rectangle(79, 66, 240, 135, RGBA8(0x48, 0xA2, 0x3E, 0xE8));

	uint32_t x = 88;
	uint32_t y = 67;

	sftd_draw_wtext_pkm(x, (y+=15), PHBank::Data->text(BankText::ExitMessage));
	sftd_draw_wtextf_pkm(x, (y+=15), L"  A - %S", PHBank::Data->text(BankText::SaveExit));
	sftd_draw_wtextf_pkm(x, (y+=15), L"  X - %S", PHBank::Data->text(BankText::Exit));
	sftd_draw_wtextf_pkm(x, (y+=15), L"  Y - %S", PHBank::Data->text(BankText::Backup));
	sftd_draw_wtextf_pkm(x, (y+=15), L"  B - %S", PHBank::Data->text(BankText::Return));

	if (hasOverlayChild()) { child->drawTopScreen(); }
	return SUCCESS_STEP;
}

// --------------------------------------------------
Result Savexiter::drawBotScreen()
// --------------------------------------------------
{
	if (hasRegularChild()) { if (child->drawBotScreen() == PARENT_STEP); else return CHILD_STEP; }

	// Draw bottom screen here!
	// TODO: Content is missing!

	if (hasOverlayChild()) { child->drawBotScreen(); }
	return SUCCESS_STEP;
}

// --------------------------------------------------
Result Savexiter::update()
// --------------------------------------------------
{
	if (hasRegularChild() || hasOverlayChild()) { if (child->update() == PARENT_STEP); else return CHILD_STEP; }

	if (kDown & KEY_A)
	{
		// Return the "You have to save" code
		parent->setState(SceneState::Saving);
		close();
		return CHILD_STEP;
	}

	if (kDown & (KEY_B | KEY_START))
	{
		// Return to the main viewer
		parent->setState(SceneState::Continuing);
		close();
		return PARENT_STEP;
	}

	if (kDown & KEY_X)
	{
		// Exit the homebrew
		parent->setState(SceneState::Exiting);
		close();
		return CHILD_STEP;
	}

	if (kDown & KEY_Y)
	{
		// Backup the data and return to the main viewer
		PHBank::Save->backup();
		parent->setState(SceneState::Continuing);
		close();
		return PARENT_STEP;
	}

	return SUCCESS_STEP;
}
