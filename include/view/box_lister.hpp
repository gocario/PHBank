#pragma once
/**
 * @file view/box_lister.hpp
 */

#include "phbank.hpp"
// #include "save.hpp"

#include "view/scene.hpp"
#include "view/box_browser.hpp"

void computeListerInbox(CursorInbox_s* inbox);

class BoxLister : public Scene
{
	public:
		BoxLister(Scene* parent = NULL);
		BoxLister(SceneType sType, Scene* parent = NULL);
		~BoxLister();

		Result initialize() override;
		Result drawTopScreen() override;
		Result drawBotScreen() override;
		Result update() override;

		bool selectViewBox(uint16_t boxID, bool inBank);

	protected:
		CursorInbox_s cBox;
		touchPosition originalTouch;
		Box_s* vBox = NULL;
		u16 boxCount;
		u16 rowCount;
		u16 colCount;

		s16 marginBottom = 40;
		s16 offsetTop = 0;
		s16 offsetLeft = 0;
		s16 originalOffsetTop = 0;
		s16 originalOffsetLeft = 0;

		bool selectViewBox();
		bool selectMoveBox();
		s16 currentColCount(int16_t row);
		SceneState closeViewer(bool save = false);
};

// BOX_LISTER_HPP
