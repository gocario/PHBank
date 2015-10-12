#include "savexit_viewer.hpp"

#include "phbank.hpp"

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
SavexitViewer::SavexitViewer(StateView_e state, Viewer* parent) : Viewer(state, parent) { }
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
	// Viewer::initialize(); // Not useful here!
	
	consoleClear();

	return SUCCESS_STEP;
}


// --------------------------------------------------
Result SavexitViewer::drawTopScreen()
// --------------------------------------------------
{
	if (hasRegularChild()) { if (child->drawTopScreen() == PARENT_STEP); else return CHILD_STEP; }
	// Viewer::drawTopScreen(); // Not useful here!

	sf2d_draw_rectangle(79, 66, 240, 135, RGBA8(0x48, 0xA2, 0x3E, 0xE8));

	uint32_t x, y;

	x = 88;
	y = 67;
	sftd_draw_text_pkm(x, (y+=15), "You are about to exit PHBank");
	sftd_draw_text_pkm(x, (y+=15), "  A - Save and exit");
	sftd_draw_text_pkm(x, (y+=15), "  Y - Exit without saving");
	sftd_draw_text_pkm(x, (y+=15), "  X - Backup the save");
	sftd_draw_text_pkm(x, (y+=15), "  B - Return to PHBank");

	if (hasOverlayChild()) { child->drawTopScreen(); }
	return SUCCESS_STEP;
}


// --------------------------------------------------
Result SavexitViewer::drawBotScreen()
// --------------------------------------------------
{
	if (hasRegularChild()) { if (child->drawBotScreen() == PARENT_STEP); else return CHILD_STEP; }
	// Viewer::drawBotScreen(); // Not useful here!

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
	// Viewer::updateControls(kDown, kHeld, kUp);
	
	if (kDown & KEY_A)
	{
		parent->setLStateView(StateView::Saving);
		// consoleClear();
		return close();
	}

	if (kDown & (KEY_B | KEY_START))
	{
		parent->setLStateView(StateView::Continuing);
		// consoleClear();
		return close();
	}

	if (kDown & KEY_X)
	{
		parent->setLStateView(StateView::Exiting);
		// consoleClear();
		return close();
	}

	if (kDown & KEY_Y)
	{
		consoleClear();
		PHBank::pKBank()->backupFile();
		
		parent->setLStateView(StateView::Continuing);
		// consoleClear();
		return close();
	}

	return SUCCESS_STEP;
}


		/*------------------------------------------*\
		 |             Protected Methods            |
		\*------------------------------------------*/


		/*------------------------------------------*\
		 |              Private Methods             |
		\*------------------------------------------*/

