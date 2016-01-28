#pragma once
/**
 * @file viewer.hpp
 */
#ifndef VIEWER_HPP
#define VIEWER_HPP

#include <3ds.h>
#include <sf2d.h>

#define SUCCESS_STEP 0
#define CHILD_STEP 1
#define PARENT_STEP 2

enum class ViewState : u8
{
	Running = 0,
	Exiting = 1,
	Continuing = 2,
	Aborting = 3,
	Saving = 4,
};

enum class ViewType : u8
{
	Regular = 0,
	Overlay = 1,
};

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

		/*-- Viewer Type --*/
		bool isRegular();
		bool isOverlay();
		virtual void setType(ViewType type);
		/*------------------*/

		/*-- Viewer State --*/
		virtual ViewState close();
		virtual ViewState state();
		virtual bool isRunning();
		virtual void setState(ViewState state);
		/*--------------------*/

		/*-- Utils --*/
		bool touchWithin(u16 px, u16 py, s16 x, s16 y, s16 w, s16 h);
		/*-----------*/

		/*-- Viewer Starter --*/
		static PrintConsole console[2];
		static ViewState startMainLoop(Viewer* viewer);
		/*--------------------*/
		
	protected:
		/*-- Viewer Constructors --*/
		Viewer(Viewer* parent = NULL);
		Viewer(ViewType type, Viewer* parent = NULL);
		/*-------------------------*/

		/*-- Viewer Hierarchy --*/
		Viewer* parent = NULL;
		Viewer* child = NULL;
		/*----------------------*/

		/*-- Viewer Type --*/
		ViewType vType = ViewType::Regular;
		/*------------------*/

		/*-- Viewer Type --*/
		ViewState vState = ViewState::Running;
		/*--------------------*/
};

#endif // VIEWER_HPP
