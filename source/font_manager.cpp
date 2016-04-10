#include "font_manager.hpp"

#include "NotoSans_Regular_ttf.h"
#include "text.h"

#include <stdio.h>

FontManager::FontManager(void)
{

}

FontManager::~FontManager(void)
{
	if (font) sftd_free_font(font);
}

Result FontManager::load(void)
{
	Result ret = (loadFonts() ? 0 : -5);
	if (R_FAILED(ret)) return ret;
	
	return ret;
}

bool FontManager::loadFonts(void)
{
	printf("Loading font: %p\n", (font = sftd_load_font_mem(NotoSans_Regular_ttf, NotoSans_Regular_ttf_size)));
	font = sftd_load_font_mem(NotoSans_Regular_ttf, NotoSans_Regular_ttf_size);
	printf("Loading font: @%p\n", font);
	if (!font) return false;

	return (font);
	return true;
}
