#include "ts.h"
#include "key.h"

#include <sf2d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define TS_DEBUG

// #define r(fmt, args ...) printf(fmt, ##args)
// #define debug_print(fmt, args ...) printf(fmt, ##args)
#define r(fmt, args ...)
#define debug_print(fmt, args ...)

AM_TitleMediaEntry titleEntry;

static u32 titleCount;
static s32 titleCurrent;
static AM_TitleMediaEntry* titleList;
static sf2d_texture** titleIcons;

/**
 * @brief Updates the output title enty.
 */
static void TS_Select(void)
{
	titleEntry = titleList[titleCurrent];
}

/**
 * @brief Initializes the title module.
 */
static void TS_Init(void)
{
	Result ret;

	titleCount = 0;
	titleCurrent = 0;

	debug_print("TS_Init()");

	amInit();
	debug_print(" > amInit\n");

	ret = AM_GetPokemonTitleEntryList(&titleList, &titleCount);
	r(" > AM_GetPokemonTitleEntryList: %lx\n", ret);

	debug_print("Got: %li titles\n", titleCount);

	titleIcons = (sf2d_texture**) malloc(titleCount * sizeof(sf2d_texture*));

	for (u32 i = 0; i < titleCount; i++)
	{
		debug_print("Texturing %li\n", i);
		titleIcons[i] = sf2d_create_texture(48, 48, TEXFMT_RGB565, SF2D_PLACE_RAM);
		u16* dst = (u16*)(titleIcons[i]->data + 64 * 8 * 2 * sizeof(u16));
		u16* src = (u16*)(titleList[i].smdh->bigIconData);
		for (u8 j = 0; j < 48; j += 8)
		{
			memcpy(dst, src, 48 * 8 * sizeof(u16));
			src += 48 * 8;
			dst += 64 * 8;
		}
	}

	debug_print("Textured\n");

	if (titleCount > 0)
	{
		TS_Select();
	}

	ret++;
}

/**
 * @brief Exits the title module.
 */
static void TS_Exit(void)
{
	AM_FreePokemonTitleEntryList(titleList, titleCount);
	free(titleIcons);

	amExit();
}
/**
 * @brief Selects the previous title.
 */
static void TS_Prev(void)
{
	titleCurrent--;
	if (titleCurrent < 0)
		titleCurrent = titleCount-1;
	TS_Select();
}

/**
 * @brief Selects the next title.
 */
static void TS_Next(void)
{
	titleCurrent++;
	if (titleCurrent > titleCount-1)
		titleCurrent = 0;
	TS_Select();
}

#ifdef TS_DEBUG
/**
 * @brief Draws the title selector state.
 */
static void TS_Draw(void)
{
	// Draw stuff here!
	consoleClear();

	printf("Title Selector:\n\n");

	for (u32 i = 0; i < titleCount; i++)
	{
		if (i == titleCurrent)
			printf(" > ");
		else
			printf("   ");

		printf("%s [%u]\n", AM_GetPokemonTitleName(titleList[i].titleid), titleList[i].mediatype);
	}
}
#endif

bool TS_Loop(void)
{
	TS_Init();

	// If no title, cancel it.
	if (titleCount == 0)
	{
		TS_Exit();
		return false;
	}
	// If only one title, select it.
	// else if (titleCount == 1)
	// {
	// 	TS_Select();
	// 	TS_Exit();
	// 	return true;
	// }

	TS_Select();

#ifdef TS_DEBUG
	consoleInit(GFX_BOTTOM, NULL);
	TS_Draw();
#endif

	bool tsReturn = false;

	while (aptMainLoop())
	{
		hidScanInput();

		u32 kState = hidKeysDown();
		if (kState & (KEY_UP | KEY_LEFT))
		{
			TS_Prev();
		}
		if (kState & (KEY_DOWN | KEY_RIGHT))
		{
			TS_Next();
		}

		if (kState & KEY_A)
		{
			tsReturn = true;
			break;
		}

		if (kState & (KEY_B | KEY_START))
		{
			tsReturn = false;
			break;
		}

		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		{
			// sf2d_draw_rectangle(151, 63, titleIcons[titleCurrent]->width*2+2, titleIcons[titleCurrent]->height*2+2, RGBA8(0xFF,0xAA,0x55,0xFF));
			// sf2d_draw_rectangle(152, 64, titleIcons[titleCurrent]->width*2, titleIcons[titleCurrent]->height*2, RGBA8(0x55,0xAA,0xFF,0xFF));

			// sf2d_draw_rectangle( 63, 95, titleIcons[titleCurrent]->width+2, titleIcons[titleCurrent]->height+2, RGBA8(0xAA,0x88,0x55,0xFF));
			// sf2d_draw_rectangle( 64, 96, titleIcons[titleCurrent]->width, titleIcons[titleCurrent]->height, RGBA8(0x55,0x88,0xAA,0xFF));

			// sf2d_draw_rectangle(287, 95, titleIcons[titleCurrent]->width+2, titleIcons[titleCurrent]->height+2, RGBA8(0xAA,0x88,0x55,0xFF));
			// sf2d_draw_rectangle(288, 96, titleIcons[titleCurrent]->width, titleIcons[titleCurrent]->height, RGBA8(0x55,0x88,0xAA,0xFF));

			if (titleCount > 0)
			{
				sf2d_draw_texture_scale(titleIcons[titleCurrent], 248, 160, -2.0f, -2.0f);

				// sf2d_draw_texture_scale(titleIcons[titleCurrent], 152, 64, 2.0f, 2.0f);

				if (titleList[titleCurrent].mediatype == MEDIATYPE_GAME_CARD)
				{
					sf2d_draw_rectangle(248, 64, 8, 16, RGBA8(0xFF,0xFF,0xFF,0xFF));
				}
			}

			if (titleCurrent > 0)
			{
				sf2d_draw_texture_scale(titleIcons[titleCurrent-1], 112, 144, -1.0f, -1.0f);

				// sf2d_draw_texture(titleIcons[titleCurrent-1], 64, 96);

				if (titleList[titleCurrent-1].mediatype == MEDIATYPE_GAME_CARD)
				{
					sf2d_draw_rectangle(112, 96, 4, 8, RGBA8(0xFF,0xFF,0xFF,0xFF));
				}
			}
			else if (titleCurrent < titleCount-1)
			{
				sf2d_draw_texture_scale(titleIcons[titleCount-1], 112, 144, -1.0f, -1.0f);

				if (titleList[titleCount-1].mediatype == MEDIATYPE_GAME_CARD)
				{
					sf2d_draw_rectangle(112, 96, 4, 8, RGBA8(0xFF,0xFF,0xFF,0xFF));
				}
			}

			if (titleCurrent < titleCount-1)
			{
				sf2d_draw_texture_scale(titleIcons[titleCurrent+1], 336, 144, -1.0f, -1.0f);

				// sf2d_draw_texture(titleIcons[titleCurrent+1], 288, 96);

				if (titleList[titleCurrent+1].mediatype == MEDIATYPE_GAME_CARD)
				{
					sf2d_draw_rectangle(336, 96, 4, 8, RGBA8(0xFF,0xFF,0xFF,0xFF));
				}
			}
			else if (titleCurrent > 0)
			{
				sf2d_draw_texture_scale(titleIcons[0], 336, 144, -1.0f, -1.0f);

				if (titleList[0].mediatype == MEDIATYPE_GAME_CARD)
				{
					sf2d_draw_rectangle(336, 96, 4, 8, RGBA8(0xFF,0xFF,0xFF,0xFF));
				}
			}
		}
		sf2d_end_frame();
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_end_frame();
		sf2d_swapbuffers();
	}

	TS_Exit();
	return tsReturn;
}
