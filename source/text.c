#include "text.h"

#include <sf2d.h>
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

sftd_font* font;
sftd_font* font_bold;

void sftd_draw_text_white(uint16_t x, uint16_t y, const char* text, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, text);
	vsnprintf(buffer, 256, text, args);
	sftd_draw_text(font, x, y, RGBA8(0xFF,0xFF,0xFF,0xFF), 12, buffer);
	va_end(args);
}

void sftd_draw_text_black(uint16_t x, uint16_t y, const char* text, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, text);
	vsnprintf(buffer, 256, text, args);
	sftd_draw_text(font, x, y, RGBA8(0x00, 0x00,0x00,0xFF), 12, buffer);
	va_end(args);
}


void sftd_draw_wtext_white(uint16_t x, uint16_t y, const uint32_t* text)
{
	sftd_draw_wtext(font, x, y, RGBA8(0xFF,0xFF,0xFF,0xFF), 12, (wchar_t*) text);
}

void sftd_draw_wtext_black(uint16_t x, uint16_t y, const uint32_t* text)
{
	sftd_draw_wtext(font, x, y, RGBA8(0x00,0x00,0x00,0xFF), 12, (wchar_t*) text);
}

void sftd_draw_wtext_pkm(uint16_t x, uint16_t y, const uint32_t* text)
{
	sftd_draw_wtext(font, x+1, y+1, RGBA8(0x00,0x00,0x00,0xAF), 12, (wchar_t*) text);
	sftd_draw_wtext(font, x, y, RGBA8(0xFF,0xFF,0xFF,0xFF), 12, (wchar_t*) text);
}


void sftd_draw_wtextf_white(uint16_t x, uint16_t y, const wchar_t* text, ...)
{
	wchar_t buffer[256];
	va_list args;
	va_start(args, text);
	vswprintf(buffer, 256, text, args);
	sftd_draw_wtext(font, x, y, RGBA8(0xFF,0xFF,0xFF,0xFF), 12, buffer);
	va_end(args);
}

void sftd_draw_wtextf_black(uint16_t x, uint16_t y, const wchar_t* text, ...)
{
	wchar_t buffer[256];
	va_list args;
	va_start(args, text);
	vswprintf(buffer, 256, text, args);
	sftd_draw_wtext(font, x, y, RGBA8(0x00,0x00,0x00,0xFF), 12, buffer);
	va_end(args);
}

void sftd_draw_wtextf_pkm(uint16_t x, uint16_t y, const wchar_t* text, ...)
{
	wchar_t buffer[256];
	va_list args;
	va_start(args, text);
	vswprintf(buffer, 256, text, args);
	sftd_draw_wtext(font, x+1, y+1, RGBA8(0x00,0x00,0x00,0xAF), 12, buffer);
	sftd_draw_wtext(font, x, y, RGBA8(0xFF,0xFF,0xFF,0xFF), 12, buffer);
	va_end(args);
}
