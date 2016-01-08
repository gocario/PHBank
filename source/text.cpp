#include "phbank.hpp"

#include <sf2d.h>
#include <sftd.h>
#include <stdio.h>
#include <stdarg.h>

void sftd_draw_text_pkm(const u16 x, const u16 y, const char* text, ... )
{
	char buffer[256];

	va_list args;
	va_start(args, text);
	vsnprintf(buffer, 256, text, args);
	sftd_draw_text(PHBanku::font->font, x+1, y+1, RGBA8(0x00, 0x00, 0x00, 0xAF), 12, buffer);
	sftd_draw_text(PHBanku::font->font, x, y, RGBA8(0xFF, 0xFF, 0xFF, 0xFF), 12, buffer);
	va_end(args);
}


void sftd_draw_text_white(const u16 x, const u16 y, const char* text, ... )
{
	char buffer[256];

	va_list args;
	va_start(args, text);
	vsnprintf(buffer, 256, text, args);
	sftd_draw_text(PHBanku::font->font, x, y, RGBA8(0xFF, 0xFF, 0xFF, 0xFF), 12, buffer);
	va_end(args);
}


void sftd_draw_text_black(const u16 x, const u16 y, const char* text, ... )
{
	char buffer[256];

	va_list args;
	va_start(args, text);
	vsnprintf(buffer, 256, text, args);
	sftd_draw_text(PHBanku::font->font, x, y, RGBA8(0x00, 0x00, 0x00, 0xFF), 12, buffer);
	va_end(args);
}
