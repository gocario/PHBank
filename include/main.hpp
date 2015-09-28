#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <3ds.h>

/* ---------- Macros ---------- */

// #define NULL nullptr

#define cdelete(x) {delete x; x = NULL;}
#define adelete(x) {delete [] x; x = NULL;}


#endif // MAIN_HPP_INCLUDED