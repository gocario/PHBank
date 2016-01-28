#include "savexit_viewer.hpp"

#include "phbank.hpp"
#include "text.hpp"

#include <stdio.h>

/*----------------------------------------------------------*\
 |                       Viewer Class                       |
\*----------------------------------------------------------*/


	/*--------------------------------------------------*\
	 |             Constructor / Destructor             |
	\*--------------------------------------------------*/


// --------------------------------------------------
SavexitViewer::SavexitViewer(Viewer* parent) : Viewer(parent) { }
// --------------------------------------------------


// --------------------------------------------------
SavexitViewer::SavexitViewer(ViewType vType, Viewer* parent) : Viewer(vType, parent) { }
// --------------------------------------------------


// --------------------------------------------------
SavexitViewer::~SavexitViewer()
// --------------------------------------------------
{
	// Destruct your allocated memory here!
	// TODO: Content may be missing!
}


	/*--------------------------------------------------*\
	 |                  Viewer Methods                  |
	\*--------------------------------------------------*/


		/*------------------------------------------*\
		 |              Public Methods              |
		\*------------------------------------------*/


// --------------------------------------------------
Result SavexitViewer::initialize()
// --------------------------------------------------
{
	if (hasChild()) { if (child->initialize() == PARENT_STEP) ; else return CHILD_STEP; }
	
	// consoleClear();

	return SUCCESS_STEP;
}


// --------------------------------------------------
Result SavexitViewer::drawTopScreen()
// --------------------------------------------------
{
	if (hasRegularChild()) { if (child->drawTopScreen() == PARENT_STEP); else return CHILD_STEP; }

	sf2d_draw_rectangle(79, 66, 240, 135, RGBA8(0x48, 0xA2, 0x3E, 0xE8));

	uint32_t x, y;

	x = 88;
	y = 67;
	sftd_draw_text_pkm(x, (y+=15), "You are about to exit PHBank");
	sftd_draw_text_pkm(x, (y+=15), "  A - Save and exit");
	sftd_draw_text_pkm(x, (y+=15), "  X - Exit without saving");
	sftd_draw_text_pkm(x, (y+=15), "  Y - Backup the save");
	sftd_draw_text_pkm(x, (y+=15), "  B - Return to PHBank");

	if (hasOverlayChild()) { child->drawTopScreen(); }
	return SUCCESS_STEP;
}


// --------------------------------------------------
Result SavexitViewer::drawBotScreen()
// --------------------------------------------------
{
	if (hasRegularChild()) { if (child->drawBotScreen() == PARENT_STEP); else return CHILD_STEP; }

	// Draw bottom screen here!
	// TODO: Content is missing!

	if (hasOverlayChild()) { child->drawBotScreen(); }
	return SUCCESS_STEP;
}


// --------------------------------------------------
Result SavexitViewer::updateControls(const u32& kDown, const u32& kHeld, const u32& kUp, const touchPosition* touch)
// --------------------------------------------------
{
	if (hasRegularChild() || hasOverlayChild()) { if (child->updateControls(kDown, kHeld, kUp, touch) == PARENT_STEP); else return CHILD_STEP; }
	
	if (kDown & KEY_A)
	{
		// Return the "You have to save" code
		parent->setState(ViewState::Saving);
			consoleInit(GFX_TOP, NULL);
			printf("Saving asked...\n");
		close();
		return CHILD_STEP;
	}

	if (kDown & (KEY_B | KEY_START))
	{
		// Return to the main viewer
		parent->setState(ViewState::Continuing);
		close();
		return PARENT_STEP;
	}

	if (kDown & KEY_X)
	{
		// Exit the homebrew
		parent->setState(ViewState::Exiting);
		close();
		return CHILD_STEP;
	}

	if (kDown & KEY_Y)
	{
		// Backup the data and return to the main viewer
		PHBanku::save->backupFile();
		parent->setState(ViewState::Continuing);
		close();
		return PARENT_STEP;
	}

	return SUCCESS_STEP;
}


		/*------------------------------------------*\
		 |             Protected Methods            |
		\*------------------------------------------*/


		/*------------------------------------------*\
		 |              Private Methods             |
		\*------------------------------------------*/

