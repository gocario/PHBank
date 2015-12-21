#pragma once
#ifndef SAVEXIT_VIEWER_HPP
#define SAVEXIT_VIEWER_HPP

#include "viewer.hpp"

class SavexitViewer : public Viewer
{
	public:
		SavexitViewer(Viewer* parent = NULL);
		SavexitViewer(StateView_e state, Viewer* parent = NULL);
		~SavexitViewer();

		Result initialize() override;
		Result drawTopScreen() override;
		Result drawBotScreen() override;
		Result updateControls(const u32& kDown = 0, const u32& kHeld = 0, const u32& kUp = 0, const touchPosition* touch = NULL) override;
};

#endif // SAVEXIT_VIEWER_HPP
