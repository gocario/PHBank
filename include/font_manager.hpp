#pragma once
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
