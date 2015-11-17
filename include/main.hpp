#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>

void waitKey(u32 keyWait);
void sftd_draw_text_pkm(const u16 x, const u16 y, const char* text, ... );
void sftd_draw_text_white(const u16 x, const u16 y, const char* text, ... );
void sftd_draw_text_black(const u16 x, const u16 y, const char* text, ... );

#endif // MAIN_HPP_INCLUDED
