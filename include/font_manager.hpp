#pragma once
/**
 * @file font_manager.hpp
 * @author Gocario
 */

#include <sftd.h>

class FontManager
{
	public:
		FontManager();
		~FontManager();

		Result load();

	private:
		bool loadFonts();
};

// FONT_MANAGER_HPP
