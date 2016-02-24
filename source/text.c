#include "text.h"

#include <sf2d.h>
#include <stdio.h>
#include <stdarg.h>

sftd_font* font;

void sftd_draw_text_pkm(u16 x, u16 y, const char* text, ...)
{
	char buffer[256];

	va_list args;
	va_start(args, text);
	vsnprintf(buffer, 256, text, args);
	sftd_draw_text(font, x+1, y+1, RGBA8(0x00, 0x00, 0x00, 0xAF), 12, buffer);
	sftd_draw_text(font, x, y, RGBA8(0xFF, 0xFF, 0xFF, 0xFF), 12, buffer);
	va_end(args);
}

void sftd_draw_text_white(u16 x, u16 y, const char* text, ...)
{
	char buffer[256];

	va_list args;
	va_start(args, text);
	vsnprintf(buffer, 256, text, args);
	sftd_draw_text(font, x, y, RGBA8(0xFF, 0xFF, 0xFF, 0xFF), 12, buffer);
	va_end(args);
}

void sftd_draw_text_black(u16 x, u16 y, const char* text, ...)
{
	char buffer[256];

	va_list args;
	va_start(args, text);
	vsnprintf(buffer, 256, text, args);
	sftd_draw_text(font, x, y, RGBA8(0x00, 0x00, 0x00, 0xFF), 12, buffer);
	va_end(args);
}

void sftd_draw_wtext_pkm(uint16_t x, uint16_t y, const uint32_t* text)
{
	sftd_draw_wtext(font, x+1, y+1, RGBA8(0x00,0x00,0x00,0xAF), 12, (wchar_t*) text);
	sftd_draw_wtext(font, x, y, RGBA8(0xFF,0xFF,0xFF,0xFF), 12, (wchar_t*) text);
}

void sftd_draw_wtext_white(uint16_t x, uint16_t y, const uint32_t* text)
{
	sftd_draw_wtext(font, x, y, RGBA8(0xFF,0xFF,0xFF,0xFF), 12, (wchar_t*) text);
}

void sftd_draw_wtext_black(uint16_t x, uint16_t y, const uint32_t* text)
{
	sftd_draw_wtext(font, x, y, RGBA8(0x00,0x00,0x00,0xFF), 12, (wchar_t*) text);
}
