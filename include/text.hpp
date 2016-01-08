#pragma once
/**
 * @file text.hpp
 */
#ifndef TEXT_HPP
#define TEXT_HPP

#include <3ds/types.h>

void sftd_draw_text_pkm(const u16 x, const u16 y, const char* text, ... );
void sftd_draw_text_white(const u16 x, const u16 y, const char* text, ... );
void sftd_draw_text_black(const u16 x, const u16 y, const char* text, ... );

#endif // TEXT_HPP
