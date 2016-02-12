#include "smdh.h"
#include "utils.h"

// #include <string.h>

// Shamelessly stolen from bch2obj.py
u8 tileOrder[] = {0,1,8,9,2,3,10,11,16,17,24,25,18,19,26,27,4,5,12,13,6,7,14,15,20,21,28,29,22,23,30,31,32,33,40,41,34,35,42,43,48,49,56,57,50,51,58,59,36,37,44,45,38,39,46,47,52,53,60,61,54,55,62,63};

// Converts RGB565 to RGB8
static inline void putPixel565(u8* dst, u8 x, u8 y, u16 v)
{
	dst[((47-y)+x*48)*3+0] = (v & 0x1F) << 3;
	dst[((47-y)+x*48)*3+1] = ((v >> 5) & 0x3F) << 2;
	dst[((47-y)+x*48)*3+2] = ((v >> 11) & 0x1F) << 3;
}

Result smdhExtractData(smdh_s* smdh, char* name, char* desc, char* auth, u8* iconData)
{
	if (!smdh) return -1;
	if (smdh->header.magic != SMDH_HEADER_MAGIC) return -2;

	// strcpy(name, "pokémon Ω");
	// strcpy(desc, "world");
	// strcpy(auth, "there");

	if (name) unicodeToChar(name, smdh->applicationTitles[1].shortDescription, 0x40);
	if (desc) unicodeToChar(desc, smdh->applicationTitles[1].longDescription, 0x80);
	if (auth) unicodeToChar(auth, smdh->applicationTitles[1].publisher, 0x40);

	if (iconData)
	{
		u8 i, j, k, x, y;
		u16* data = smdh->bigIconData;

		// Convert RGB565 to RGB24
		for (j = 0; j < 48; j += 8) // Col
		{
			for (i = 0; i < 48; i += 8) // Row
			{
				for (k = 0; k < 64; k++) // Tile
				{
					x = tileOrder[k] & 0x7;
					y = tileOrder[k] >> 3;
					putPixel565(iconData, i + x, j + y, *(data++));
				}
			}
		}
	}

	return 0;
}
