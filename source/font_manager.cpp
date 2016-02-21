#include "font_manager.hpp"

// #include "FreeSans_ttf.h"
// #include "PokemonGB_ttf.h"
#include "Roboto_Regular_ttf.h"

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

	return ret;
}

bool FontManager::loadFonts(void)
{
	printf("Loading font: %p\n", (font = sftd_load_font_mem(Roboto_Regular_ttf, Roboto_Regular_ttf_size)));

	return (font);
}
