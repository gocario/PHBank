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

	sf2d_draw_rectangle(79, 66, 240, 135, RGBA8(0xFA, 0xE6, 0xFA, 0xF0));

	uint32_t x, y;

	x = 88;
	y = 67;
	sftd_draw_text_pkm(x, (y+=15), "You are about to exit PHBank");
	sftd_draw_text_pkm(x, (y+=15), "  A - Save and exit");
	sftd_draw_text_pkm(x, (y+=15), "  B - Exit without saving");
	sftd_draw_text_pkm(x, (y+=15), "  X - Return to PHBank");
	sftd_draw_text_pkm(x, (y+=15), "  Y - Backup the save");

	// printf("\x1B[9;8H                                   ");
	// printf("\x1B[10;8H /------------------------------\\ ");
	// printf("\x1B[11;8H |                              | ");
	// printf("\x1B[12;8H | You are about to exit PHBank | ");
	// printf("\x1B[13;8H |   A - Save and exit          | ");
	// printf("\x1B[14;8H |   B - Exit without saving    | ");
	// printf("\x1B[15;8H |   X - Return to PHBank       | ");
	// printf("\x1B[16;8H |   Y - Backup the save        | ");
	// printf("\x1B[17;8H |                              | ");
	// printf("\x1B[18;8H \\------------------------------/ ");
	// printf("\x1B[19;8H                                  ");

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
		printf("A pressed\n");
		parent->setLStateView(StateView::Saving);
		// consoleClear();
		return close();
	}

	if (kDown & KEY_B)
	{
		printf("B pressed\n");
		parent->setLStateView(StateView::Exiting);
		// consoleClear();
		return close();
	}

	if (kDown & KEY_X)
	{
		printf("X pressed\n");
		parent->setLStateView(StateView::Continuing);
		// consoleClear();
		return close();
	}

	if (kDown & KEY_Y)
	{
		printf("Y pressed\n");
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

