#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <3ds.h>

/* ---------- Macros ---------- */

// #define NULL nullptr

#define cdelete(x) {delete x; x = NULL;}
#define adelete(x) {delete [] x; x = NULL;}

#define SAVEFROMSDROOT

void waitKey(u32 keyWait);


#endif // MAIN_HPP_INCLUDED