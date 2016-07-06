#include "font_manager.hpp"

#include "text.h"
#include "NotoSans_Regular_ttf.h"
// #include "NotoSans_Bold_ttf.h"

#include <stdio.h>

FontManager::FontManager(void)
{

}

FontManager::~FontManager(void)
{
	if (font) sftd_free_font(font);
	// if (font_bold) sftd_free_font(font_bold);
}

Result FontManager::load(void)
{
	Result ret = (loadFonts() ? 0 : -5);
	if (R_FAILED(ret)) return ret;
	
	return ret;
}

bool FontManager::loadFonts(void)
{
	font = sftd_load_font_mem(NotoSans_Regular_ttf, NotoSans_Regular_ttf_size);
	printf("Loading font: @%p\n", font);
	if (!font) return false;

	// font_bold = sftd_load_font_mem(NotoSans_Bold_ttf, NotoSans_Bold_ttf_size);
	// printf("Loading font_bold: %p\n", font_bold);
	// if (!font_bold) return false;

	return true;
}
