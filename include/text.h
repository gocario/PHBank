#pragma once
/**
 * @file text.h
 */
#ifndef TEXT_H
#define TEXT_H

#include <sftd.h>

extern sftd_font* font;
extern sftd_font* font_bold;

#ifdef __cplusplus
extern "C" {
#endif

void sftd_draw_text_white(uint16_t x, uint16_t y, const char* text, ...);
void sftd_draw_text_black(uint16_t x, uint16_t y, const char* text, ...);
// void sftd_draw_text_pkm(uint16_t x, uint16_t y, const char* text, ...);

void sftd_draw_wtext_white(uint16_t x, uint16_t y, const uint32_t* text);
void sftd_draw_wtext_black(uint16_t x, uint16_t y, const uint32_t* text);
void sftd_draw_wtext_pkm(uint16_t x, uint16_t y, const uint32_t* text);

void sftd_draw_wtextf_white(uint16_t x, uint16_t y, const wchar_t* text, ...);
void sftd_draw_wtextf_black(uint16_t x, uint16_t y, const wchar_t* text, ...);
void sftd_draw_wtextf_pkm(uint16_t x, uint16_t y, const wchar_t* text, ...);

#ifdef __cplusplus
}
#endif

#endif // TEXT_H
