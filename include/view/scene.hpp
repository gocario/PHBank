#pragma once
/**
 * @file scene.hpp
 */

#include <3ds.h>
#include <sf2d.h>

#define SUCCESS_STEP (0)
#define CHILD_STEP (1)
#define PARENT_STEP (2)

enum class SceneState : u8
{
	Running = 0,
	Exiting = 1,
	Continuing = 2,
	Aborting = 3,
	Saving = 4,
};

enum class SceneType : u8
{
	Regular = 0,
	Overlay = 1,
};

class Scene
{
	public:
		/*-- Scene Destructor --*/
		virtual ~Scene();
		/*-----------------------*/

		/*-- Scene Methods --*/
		virtual Result initialize();
		virtual Result drawTopScreen();
		virtual Result drawBotScreen();
		virtual Result update();
		/*--------------------*/

		/*-- Scene Hierarchy/Type/State --*/
		bool isChild();
		bool isParent();
		bool hasParent();
		bool hasChild();
		bool hasRegularChild();
		bool hasOverlayChild();
		bool isRegular();
		bool isOverlay();
		virtual bool isRunning();

		virtual void setType(SceneType type);
		virtual void setState(SceneState state);
		virtual SceneState state();
		virtual SceneState close();
		/*--------------------------------*/

		/*-- Utils --*/
		bool touchWithin(s16 px, s16 py, s16 x, s16 y, s16 w, s16 h);
		/*-----------*/

		/*-- Scene Starter --*/
		static u32 kDown;
		static u32 kHeld;
		static u32 kUp;
		static touchPosition touch;
		static touchPosition oldTouch;
		static SceneState startMainLoop(Scene* scene);
		/*--------------------*/

	protected:
		/*-- Scene Constructors --*/
		Scene(Scene* parent = NULL);
		Scene(SceneType type, Scene* parent = NULL);
		/*-------------------------*/

		/*-- Scene Hierarchy --*/
		Scene* parent = NULL;
		Scene* child = NULL;
		/*----------------------*/

		/*-- Scene Type --*/
		SceneType sType = SceneType::Regular;
		/*------------------*/

		/*-- Scene Type --*/
		SceneState sState = SceneState::Running;
		/*--------------------*/
};

// VIEWER_HPP
