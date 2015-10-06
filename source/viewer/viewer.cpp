#include "viewer.hpp"


/*----------------------------------------------------------*\
 |                       Viewer Class                       |
\*----------------------------------------------------------*/


	/*--------------------------------------------------*\
	 |             Constructor / Destructor             |
	\*--------------------------------------------------*/


// --------------------------------------------------
Viewer::Viewer(Viewer* parent)
// --------------------------------------------------
{
	this->parent = parent;
	this->child = NULL;
	
	if (this->parent)
	{
		this->parent->child = this;
	}
}


// --------------------------------------------------
Viewer::Viewer(StateView_e state, Viewer* parent) : Viewer(parent)
// --------------------------------------------------
{
	this->stateView = state;
}


// --------------------------------------------------
Viewer::~Viewer()
// --------------------------------------------------
{
	if (this->parent)
	{
		this->parent->child = NULL;
		this->parent = NULL;
	}

	delete child;
}


	/*--------------------------------------------------*\
	 |                  Viewer Methods                  |
	\*--------------------------------------------------*/


		/*------------------------------------------*\
		 |              Public Methods              |
		\*------------------------------------------*/


// --------------------------------------------------
Result Viewer::initialize()
// --------------------------------------------------
{
	return SUCCESS_STEP;
}


// --------------------------------------------------
Result Viewer::drawTopScreen()
// --------------------------------------------------
{
	return SUCCESS_STEP;
}


// --------------------------------------------------
Result Viewer::drawBotScreen()
// --------------------------------------------------
{
	return SUCCESS_STEP;
}


// --------------------------------------------------
Result Viewer::updateControls(const u32& kDown, const u32& kHeld, const u32& kUp, const touchPosition* touch)
// --------------------------------------------------
{
	return SUCCESS_STEP;
}


// --------------------------------------------------
Result Viewer::exit()
// --------------------------------------------------
{
	if (this->isChild()) delete this;
	return this->state();
}


// --------------------------------------------------
Result Viewer::state()
// --------------------------------------------------
{
	return this->lStateView;
}


		/*------------------------------------------*\
		 |             Protected Methods            |
		\*------------------------------------------*/


// --------------------------------------------------
bool Viewer::touchWithin(u16 px, u16 py, s16 x, s16 y, s16 w, s16 h)
// --------------------------------------------------
{
	return (px >= x && py >= y) && (px <= x + w && py <= y + h);
}


// --------------------------------------------------
bool Viewer::hasParent() { return (this->parent); }
// --------------------------------------------------


// --------------------------------------------------
bool Viewer::hasChild() { return (this->child); }
// --------------------------------------------------


// --------------------------------------------------
bool Viewer::isParent() { return (this->child); }
// --------------------------------------------------


// --------------------------------------------------
bool Viewer::isChild() { return (this->parent); }
// --------------------------------------------------



// --------------------------------------------------
bool Viewer::isRegular()
// --------------------------------------------------
{
	return (stateView == StateView::Regular);
}


// --------------------------------------------------
bool Viewer::isOverlay()
// --------------------------------------------------
{
	return (stateView == StateView::Overlay); // && (this->hasParent());
}


// --------------------------------------------------
bool Viewer::isRunning()
// --------------------------------------------------
{
	return (lStateView == StateView::Running) || (lStateView == StateView::Continuing);
}


// --------------------------------------------------
bool Viewer::isChildRegular()
// --------------------------------------------------
{
	return (this->hasChild()) && (child->isRegular());
}


// --------------------------------------------------
bool Viewer::isChildOverlay()
// --------------------------------------------------
{
	return (this->hasChild()) && (child->isOverlay());
}


		/*------------------------------------------*\
		 |              Private Methods             |
		\*------------------------------------------*/



/*----------------------------------------------------------*\
 |                      Static Section                      |
\*----------------------------------------------------------*/

// --------------------------------------------------
PrintConsole* Viewer::console[2];
// --------------------------------------------------


// --------------------------------------------------
Result Viewer::startMainLoop(Viewer* viewer)
// --------------------------------------------------
{
	sf2d_init();

	// PrintConsole top, bot;
	// consoleInit(GFX_TOP, NULL);
	// consoleInit(GFX_BOTTOM, NULL);
	// consoleSelect(&top);

	// Viewer::console[0] = &top;
	// Viewer::console[1] = &bot;

	viewer->initialize();
	
	u32 kDown, kHeld, kUp;
	touchPosition touch;
	while(viewer->isRunning() && aptMainLoop())
	{
		hidScanInput();
		kDown = hidKeysDown();
		kHeld = hidKeysHeld();
		kUp = hidKeysUp();
		hidTouchRead(&touch);

		if (kHeld & KEY_L && kHeld & KEY_R && kHeld & KEY_A && kHeld & KEY_B && kHeld & KEY_X && kHeld & KEY_Y) break;

		// sf2d_start_frame(GFX_TOP, GFX_LEFT);
		// 	viewer->drawTopScreen();
		// sf2d_end_frame();
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			viewer->drawBotScreen();
		sf2d_end_frame();
		sf2d_swapbuffers();

		viewer->updateControls(kDown, kHeld, kUp, &touch);
	}

	printf("Fini ?!\n");
	sf2d_fini();
	return viewer->state();
}
