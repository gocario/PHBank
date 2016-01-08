#pragma once
/**
 * @file font_manager.hpp
 * @brief Font Manager
 */
#ifndef FONT_MANAGER_HPP
#define FONT_MANAGER_HPP

#include <3ds/types.h>
#include <sftd.h>

class FontManager
{
	public:
		sftd_font* font;
		
	public:
		FontManager();
		~FontManager();

		Result load();
};

#endif // FONT_MANAGER_HPP
