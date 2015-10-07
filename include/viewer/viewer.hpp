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
	/*-- Viewer Destructor --*/
		virtual ~Viewer();
		/*-----------------------*/

		/*-- Viewer Methods --*/
		virtual Result initialize();
		virtual Result drawTopScreen();
		virtual Result drawBotScreen();
		virtual Result updateControls(const u32& kDown = 0, const u32& kHeld = 0, const u32& kUp = 0, const touchPosition* touch = NULL);
		/*--------------------*/

		/*-- Viewer Hierarchy --*/
		bool hasParent();
		bool hasChild();
		bool isChild();
		bool isParent();
		bool hasRegularChild();
		bool hasOverlayChild();
		/*----------------------*/

		/*-- Viewer State --*/
		bool isRegular();
		bool isOverlay();
		virtual void setStateView(StateView_e state);
		/*------------------*/

		/*-- Viewer L.State --*/
		virtual Result close();
		virtual Result state();
		virtual bool isRunning();
		virtual void setLStateView(LStateView_e state);
		/*--------------------*/

		/*-- Utils --*/
		bool touchWithin(u16 px, u16 py, s16 x, s16 y, s16 w, s16 h);
		/*-----------*/

		/*-- Viewer Starter --*/
		static PrintConsole console[2];
		static Result startMainLoop(Viewer* viewer);
		/*--------------------*/
		
	protected:
		/*-- Viewer Constructors --*/
		Viewer(Viewer* parent = NULL);
		Viewer(StateView_e state, Viewer* parent = NULL);
		/*-------------------------*/

		/*-- Viewer Hierarchy --*/
		Viewer* parent = NULL;
		Viewer* child = NULL;
		/*----------------------*/

		/*-- Viewer State --*/
		StateView_e stateView = StateView::Regular;
		/*------------------*/

		/*-- Viewer L.State --*/
		LStateView_e lStateView = StateView::Running;
		/*--------------------*/

	private:
};


#endif // VIEWER_HPP