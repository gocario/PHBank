#pragma once
#ifndef ULTRA_BOX_VIEWER_HPP
#define ULTRA_BOX_VIEWER_HPP

#include "viewer.hpp"

#include "save.hpp"

struct CursorUBox_s
{
	bool inBank = false;
	int16_t slot = 0;
	int16_t row = 0;
	int16_t col = 0;
};

void computeSlot(CursorUBox_s* cursorUBox);

class UltraBoxViewer : public Viewer
{
	public:
		UltraBoxViewer(Viewer* parent = NULL);
		UltraBoxViewer(StateView_e state, Viewer* parent = NULL);
		~UltraBoxViewer();

		Result initialize() override;
		Result drawTopScreen() override;
		Result drawBotScreen() override;
		Result updateControls(const u32& kDown = 0, const u32& kHeld = 0, const u32& kUp = 0, const touchPosition* touch = NULL) override;

		bool selectViewBox(uint16_t boxID, bool inBank);

	protected:
		sf2d_texture* tiles = NULL;

		CursorUBox_s cursorUBox;
		touchPosition touch;
		touchPosition originalTouch;
		box_s* vBox = NULL;
		uint16_t boxCount;
		uint16_t rowCount;
		uint16_t colCount;

		int16_t marginBottom = 40;
		int16_t offsetTop = 0;
		int16_t offsetLeft = 0;
		int16_t originalOffsetTop = 0;
		int16_t originalOffsetLeft = 0;

		bool selectViewBox();
		bool selectMoveBox();
		int16_t currentColCount(int16_t row);
		Result closeViewer(bool save = false);
};

#endif // ULTRA_BOX_VIEWER_HPP
