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

	printf("\x1B[9;8H                                   ");
	printf("\x1B[10;8H /------------------------------\\ ");
	printf("\x1B[11;8H |                              | ");
	printf("\x1B[12;8H | You are about to exit PHBank | ");
	printf("\x1B[13;8H |   A - Save and exit          | ");
	printf("\x1B[14;8H |   B - Exit without saving    | ");
	printf("\x1B[15;8H |   X - Return to PHBank       | ");
	printf("\x1B[16;8H |   Y - Backup the save        | ");
	printf("\x1B[17;8H |                              | ");
	printf("\x1B[18;8H \\------------------------------/ ");
	printf("\x1B[19;8H                                  ");

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
		consoleClear();
		return close();
	}

	if (kDown & KEY_B)
	{
		parent->setLStateView(StateView::Exiting);
		consoleClear();
		return close();
	}

	if (kDown & KEY_X)
	{
		parent->setLStateView(StateView::Continuing);
		consoleClear();
		return close();
	}

	if (kDown & KEY_Y)
	{
		consoleClear();
		PHBank::pKBank()->backupFile();
		
		parent->setLStateView(StateView::Continuing);
		consoleClear();
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

