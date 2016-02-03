#include "texture_manager.hpp"

#include <sfil.h>
#include <stdio.h>

#define ROMFS "/pk/romfs/"


#define STACKSIZE (4*1024)

static volatile bool threadMainLoop;
static volatile TextureManager* texture;

static const int rowBackball[4][2] = {
	{24,   0},
	{16, +24},
	{ 8, +48},
};

static inline void drawLoadingPokeball(int x, int y)
{
	sf2d_draw_texture_part_scale(texture->ballLoadingScreen, x, y,  0,  0, 16, 16, 2.0f, 2.0f);
}

static inline void drawLoadingGreatball(int x, int y)
{
	sf2d_draw_texture_part_scale(texture->ballLoadingScreen, x, y, 16,  0, 16, 16, 2.0f, 2.0f);
}

static inline void drawLoadingUltraball(int x, int y)
{
	sf2d_draw_texture_part_scale(texture->ballLoadingScreen, x, y, 32, 0, 16, 16, 2.0f, 2.0f);
}

static inline void drawLoadingBackball(int x, int y)
{
	sf2d_draw_texture_part(texture->ballLoadingScreen, x, y, 48, 0, 16, 16);
}

static inline void drawLoadingText(int rx, int ry)
{
	sf2d_draw_texture_part_scale(texture->ballLoadingScreen,
		rx - 64 * 4,
		ry -  8 * 4,
		0, 16, 64, 8, 4.0f, 4.0f);
}

static void drawLoadingScreen()
{
	u64 tick = (svcGetSystemTick() / 10000000);
	for (u8 i = 0; i < 17; i++) // row
	for (u8 j = 0; j < 15; j++) // col
	{
		drawLoadingBackball(
			rowBackball[i%3][0] + j * 24 - 32 + tick % 16,
			(i/3) * 64  + rowBackball[i%3][1] - j * 8 - 16 + tick % 16
		);
	}

	// for (u8 i = 0; i < 9; i++) // row
	// for (u8 j = 0; j < 8; j++) // row
	// {
	// 	drawLoadingPokeball(
	// 		rowBackball[(i*2)%3][0] + (j*2) * 24 - 32 + tick % 64,
	// 		((i*2)/3) * 64  + rowBackball[(i*2)%3][1] - (j*2) * 8 - 16 + tick % 64
	// 	);
	// }

	drawLoadingText(300, 220);
}

static void loading_screen(void* arg)
{
	texture = (TextureManager*) arg;
	sf2d_set_clear_color(RGBA8(0xF8, 0xF8, 0xF8, 0xFF));
	sf2d_set_vblank_wait(false);
	while (threadMainLoop)
	{
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			drawLoadingScreen();
		sf2d_end_frame();
		sf2d_swapbuffers();
	}
	sf2d_set_vblank_wait(true);
}


TextureManager::TextureManager(void)
{

}

TextureManager::~TextureManager(void)
{
	sf2d_free_texture(ballLoadingScreen);
	sf2d_free_texture(pkmSprites);
	sf2d_free_texture(pkmShinySprites);
	sf2d_free_texture(pkmIcons);
	sf2d_free_texture(pkmShinyIcons);
	sf2d_free_texture(itemIcons);
	sf2d_free_texture(ballIcons);
	sf2d_free_texture(types);
	sf2d_free_texture(boxTiles);
	sf2d_free_texture(boxBackground);
	sf2d_free_texture(resumeBackground);
}

Result TextureManager::load(void)
{
	// s32 prio = 0;
	// Thread loadingThread;

	printf("Loading ballLoadingScreen: %p\n", (ballLoadingScreen = sfil_load_PNG_file(ROMFS "ball_loading_screen.png", SF2D_PLACE_RAM)));
	
	// For call
	// if (!ballLoadingScreen) return -(!ballLoadingScreen);
	// threadMainLoop = true;
	// loading_screen((void*)this);

	// For thread
	// threadMainLoop = true;
	// svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
	// loadingThread = threadCreate(loading_screen, (void*)this, STACKSIZE, prio-1, 1, false);

	printf("Loading pkmSprites: %p\n", (pkmSprites = sfil_load_PNG_file(ROMFS "pokemon_spritesheet.png", SF2D_PLACE_RAM)));
	printf("Loading pkmShinySprites: %p\n", (pkmShinySprites = sfil_load_PNG_file(ROMFS "pokemon_shiny_spritesheet.png", SF2D_PLACE_RAM)));
	printf("Loading pkmIcons: %p\n", (pkmIcons = sfil_load_PNG_file(ROMFS "pokemon_icons_spritesheet.png", SF2D_PLACE_RAM)));
	printf("Loading pkmShinyIcons: %p\n", (pkmShinyIcons = sfil_load_PNG_file(ROMFS "pokemon_shiny_icons_spritesheet.png", SF2D_PLACE_RAM)));
	printf("Loading itemIcons: %p\n", (itemIcons = sfil_load_PNG_file(ROMFS "item_icons_spritesheet.png", SF2D_PLACE_RAM)));
	printf("Loading ballIcons: %p\n", (ballIcons = sfil_load_PNG_file(ROMFS "ball_icons_spritesheet.png", SF2D_PLACE_RAM)));
	printf("Loading types: %p\n", (types = sfil_load_PNG_file(ROMFS "types_lang.png", SF2D_PLACE_RAM)));
	printf("Loading boxTiles: %p\n", (boxTiles = sfil_load_PNG_file(ROMFS "box_tiles.png", SF2D_PLACE_RAM)));
	printf("Loading boxBackground: %p\n", (boxBackground = sfil_load_PNG_file(ROMFS "box_wp23o.png", SF2D_PLACE_RAM)));
	printf("Loading resumeBackground: %p\n", (resumeBackground = sfil_load_PNG_file(ROMFS "resume_background.png", SF2D_PLACE_RAM)));

	// threadMainLoop = false;
	// threadJoin(loadingThread, U64_MAX);
	// threadFree(loadingThread);

	return -(!pkmSprites || !pkmShinySprites || !pkmIcons || !pkmShinyIcons || !itemIcons || !ballIcons || !resumeBackground || !types || !boxTiles || !boxBackground);
}
