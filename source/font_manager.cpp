#include "font_manager.hpp"

#include "FreeSans_ttf.h"
// #include "PokemonGB_ttf.h"
#include "RobotoRegular_ttf.h"

FontManager::FontManager()
{

}


FontManager::~FontManager()
{
	if (font) sftd_free_font(font);
}


Result FontManager::load()
{
	font = sftd_load_font_mem(RobotoRegular_ttf, RobotoRegular_ttf_size);

	return 0;
}
