#include "view/scene.hpp"

Scene::Scene(Scene* parent)
{
	this->parent = parent;
	this->child = NULL;

	if (this->parent)
	{
		this->parent->child = this;
	}
}

Scene::Scene(SceneType sType, Scene* parent) : Scene(parent)
{
	this->sType = sType;
}

Scene::~Scene()
{
	if (this->parent)
	{
		this->parent->child = NULL;
		this->parent = NULL;
	}

	if (this->child)
	{
		delete this->child;
	}
}


Result Scene::initialize()
{
	return SUCCESS_STEP;
}

Result Scene::drawTopScreen()
{
	return SUCCESS_STEP;
}

Result Scene::drawBotScreen()
{
	return SUCCESS_STEP;
}

Result Scene::update()
{
	return SUCCESS_STEP;
}

bool Scene::isParent() { return this->child; }
bool Scene::isChild() { return this->parent; }
bool Scene::hasParent() { return this->parent; }
bool Scene::hasChild() { return this->child; }
bool Scene::hasRegularChild() { return this->hasChild() && child->isRegular(); }
bool Scene::hasOverlayChild() { return this->hasChild() && child->isOverlay(); }
bool Scene::isRegular() { return sType == SceneType::Regular; }
bool Scene::isOverlay() { return sType == SceneType::Overlay; }
bool Scene::isRunning() { return sState == SceneState::Running || sState == SceneState::Continuing; }

void Scene::setType(SceneType sType)
{
	this->sType = sType;
}

void Scene::setState(SceneState sState)
{
	this->sState = sState;
}

SceneState Scene::state()
{
	return this->sState;
}

SceneState Scene::close()
{
	SceneState state = this->state();
	if (this->isChild()) delete this;
	return state;
}


bool Scene::touchWithin(s16 px, s16 py, s16 x, s16 y, s16 w, s16 h)
{
	return (px >= x && py >= y) && (px <= x + w && py <= y + h);
}

u32 Scene::kDown;
u32 Scene::kHeld;
u32 Scene::kUp;
touchPosition Scene::touch;
touchPosition Scene::oldTouch;

SceneState Scene::startMainLoop(Scene* scene)
{
	while (scene->parent) scene = scene->parent;

	scene->initialize();

	while (scene->isRunning() && aptMainLoop())
	{
		hidScanInput();
		kDown = hidKeysDown();
		kHeld = hidKeysHeld();
		kUp = hidKeysUp();
		if ((kDown | kHeld) & KEY_TOUCH)
		{
			oldTouch = touch;
			hidTouchRead(&touch);
		}

#ifndef DEBUG_TOP_CONSOLE
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			scene->drawTopScreen();
		sf2d_end_frame();
#endif
#ifndef DEBUG_BOT_CONSOLE
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			scene->drawBotScreen();
		sf2d_end_frame();
#endif
		sf2d_swapbuffers();

		scene->update();
	}

	return scene->state();
}
