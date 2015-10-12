#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>

/* ---------- Macros ---------- */

// #define NULL nullptr

#define cdelete(x) {delete x; x = NULL;}
#define adelete(x) {delete [] x; x = NULL;}

void waitKey(u32 keyWait);
void sftd_draw_text_pkm(const u16 x, const u16 y, const char* text, ... );

#endif // MAIN_HPP_INCLUDED