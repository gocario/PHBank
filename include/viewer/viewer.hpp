#ifndef VIEWER_HPP
#define VIEWER_HPP

#include "main.hpp"

#include <sf2d.h>
#include <3ds.h>
#include <stdio.h>


#define SUCCESS_STEP 0
#define CHILD_STEP 1
#define PARENT_STEP 2


namespace StateView
{
	enum StateView_e
	{
		Regular = 0,
		Overlay = 1,
	};
	enum LStateView_e
	{
		Running = 0,
		Exiting = 1,
		Continuing = 2,
		Aborting = 3,
		Saving = 4,
	};
}
typedef StateView::StateView_e StateView_e;
typedef StateView::LStateView_e LStateView_e;


class Viewer
{
	public:
		virtual ~Viewer();
		virtual Result initialize();
		virtual Result drawTopScreen();
		virtual Result drawBotScreen();
		virtual Result updateControls(const u32& kDown = 0, const u32& kHeld = 0, const u32& kUp = 0, const touchPosition* touch = NULL);
		virtual Result exit();

		virtual Result state();

		static PrintConsole* console[2];
		static Result startMainLoop(Viewer* viewer);

		bool touchWithin(u16 px, u16 py, s16 x, s16 y, s16 w, s16 h);

		/*-- State View --*/
		bool hasParent();
		bool hasChild();
		bool isChild();
		bool isParent();
		
		bool isRegular();
		bool isOverlay();
		bool isRunning();

		bool isChildRegular();
		bool isChildOverlay();
		/*----------------*/
		
	protected:
		Viewer(Viewer* parent = NULL);
		Viewer(StateView_e state, Viewer* parent = NULL);
		Viewer* parent = NULL;
		Viewer* child = NULL;

		/*-- State View --*/
		StateView_e stateView = StateView::Regular;
		LStateView_e lStateView = StateView::Running;
		/*----------------*/

	private:
};


#endif // VIEWER_HPP