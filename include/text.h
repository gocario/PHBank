#pragma once
/**
 * @file text.hpp
 */
#ifndef TEXT_HPP
#define TEXT_HPP

#include <sftd.h>

#ifdef __cplusplus
extern "C" {
#endif

extern sftd_font* font;

void sftd_draw_text_pkm(const u16 x, const u16 y, const char* text, ...);
void sftd_draw_text_white(const u16 x, const u16 y, const char* text, ...);
void sftd_draw_text_black(const u16 x, const u16 y, const char* text, ...);

void sftd_draw_wtext_pkm(uint16_t x, uint16_t y, const uint32_t* text);
void sftd_draw_wtext_white(uint16_t x, uint16_t y, const uint32_t* text);
void sftd_draw_wtext_black(uint16_t x, uint16_t y, const uint32_t* text);

#ifdef __cplusplus
}
#endif

#endif // TEXT_HPP
