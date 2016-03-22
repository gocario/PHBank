#include "texture_manager.hpp"

#include <sfil.h>
#include <stdio.h>

#define ROMFS "/pk/romfs/"

static bool threadMainLoop;

static const int rowBackball[4][2] = {
	{24,   0},
	{16, +24},
	{ 8, +48},
};

void _loadingScreen(void* arg)
{
	TextureManager* that = (TextureManager*) arg;

	sf2d_set_clear_color(RGBA8(0xF8,0xF8,0xF8,0xFF));
	sf2d_set_vblank_wait(false);
	while (threadMainLoop)
	{
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
			that->drawLoadingTopScreen();
		sf2d_end_frame();
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			that->drawLoadingBottomScreen();
		sf2d_end_frame();
		sf2d_swapbuffers();
	}
	sf2d_set_vblank_wait(true);
}

void _loadTextures(void* arg)
{
	TextureManager* that = (TextureManager*) arg;

	that->loadTextures();

	threadMainLoop = false;
}

TextureManager::TextureManager(void)
{
	ballLoadingScreen = NULL;
	pkmIcons = NULL;
	pkmShinyIcons = NULL;
	pkmFormIcons = NULL;
	pkmShinyFormIcons = NULL;
	itemIcons = NULL;
	ballIcons = NULL;
	types = NULL;
	boxTiles = NULL;
	boxBackgrounds = NULL;
	resumeBackground = NULL;
}

TextureManager::~TextureManager(void)
{
	sf2d_free_texture(ballLoadingScreen);
	sf2d_free_texture(pkmIcons);
	sf2d_free_texture(pkmShinyIcons);
	sf2d_free_texture(pkmFormIcons);
	sf2d_free_texture(pkmShinyFormIcons);
	sf2d_free_texture(itemIcons);
	sf2d_free_texture(ballIcons);
	sf2d_free_texture(types);
	sf2d_free_texture(boxTiles);
	sf2d_free_texture(boxBackgrounds);
	sf2d_free_texture(resumeBackground);
}

Result TextureManager::load(void)
{
	printf("Loading ballLoadingScreen: %p\n", (ballLoadingScreen = sfil_load_PNG_file(ROMFS "ball_loading_screen.png", SF2D_PLACE_RAM)));

	if (!ballLoadingScreen) return -5;

	drawStaticLoadingScreen();

	// s32 prio = 0;
	// threadMainLoop = true;
	// svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
	// threadCreate(_loadingScreen, (void*) this, 4*1024, prio-1, -2, true);

	Result ret = (loadTextures() ? 0 : -5);

	sf2d_texture_set_params(this->boxTiles, GPU_TEXTURE_MAG_FILTER(GPU_LINEAR) | GPU_TEXTURE_MIN_FILTER(GPU_LINEAR));

	// threadMainLoop = false;

	return ret;
}

bool TextureManager::loadTextures()
{
	printf("Loading pkmIcons: %p\n", (this->pkmIcons = sfil_load_PNG_file(ROMFS "pokemon_icons_spritesheet.png", SF2D_PLACE_RAM)));
	printf("Loading pkmShinyIcons: %p\n", (this->pkmShinyIcons = sfil_load_PNG_file(ROMFS "pokemon_shiny_icons_spritesheet.png", SF2D_PLACE_RAM)));
	printf("Loading pkmFormIcons: %p\n", (this->pkmFormIcons = sfil_load_PNG_file(ROMFS "pokemon_form_icons_spritesheet.png", SF2D_PLACE_RAM)));
	printf("Loading pkmShinyFormIcons: %p\n", (this->pkmShinyFormIcons = sfil_load_PNG_file(ROMFS "pokemon_shiny_form_icons_spritesheet.png", SF2D_PLACE_RAM)));
	printf("Loading itemIcons: %p\n", (this->itemIcons = sfil_load_PNG_file(ROMFS "item_icons_spritesheet.png", SF2D_PLACE_RAM)));
	printf("Loading ballIcons: %p\n", (this->ballIcons = sfil_load_PNG_file(ROMFS "ball_icons_spritesheet.png", SF2D_PLACE_RAM)));
	printf("Loading types: %p\n", (this->types = sfil_load_PNG_file(ROMFS "types_lang.png", SF2D_PLACE_RAM)));
	printf("Loading boxTiles: %p\n", (this->boxTiles = sfil_load_PNG_file(ROMFS "box_tiles.png", SF2D_PLACE_RAM)));
	printf("Loading boxBackgrounds: %p\n", (this->boxBackgrounds = sfil_load_PNG_file(ROMFS "box_backgrounds.png", SF2D_PLACE_RAM)));
	printf("Loading resumeBackground: %p\n", (this->resumeBackground = sfil_load_PNG_file(ROMFS "resume_background.png", SF2D_PLACE_RAM)));

	return (pkmIcons && pkmShinyIcons && pkmFormIcons && pkmShinyFormIcons && itemIcons && ballIcons && types && boxTiles && boxBackgrounds && resumeBackground);
}

void TextureManager::drawStaticLoadingScreen(void)
{
	// Set the clear color to a false white.
	sf2d_set_clear_color(RGBA8(0xF8,0xF8,0xF8,0xFF));

	// Need to be called twice, probably a bug.
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
	sf2d_end_frame();

	// Render a blank top screen.
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
		drawLoadingTopScreen();
	sf2d_end_frame();

	// Render the loading screen on the bottom screen.
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		drawLoadingBottomScreen();
	sf2d_end_frame();

	// Swap the buffers
	sf2d_swapbuffers();
}

void TextureManager::drawLoadingTopScreen()
{
	// TOOD: Render a loading screen here too.
}

void TextureManager::drawLoadingBottomScreen()
{
	u64 tick = svcGetSystemTick() / 10000000;

	for (u8 i = 0; i < 17; i++) // row
	for (u8 j = 0; j < 15; j++) // col
	{
		drawLoadingBackball(
			rowBackball[i%3][0] + j * 24 - 32 + tick % 16,
			(i/3) * 64  + rowBackball[i%3][1] - j * 8 - 16 + tick % 16
		);
	}

	// for (u8 i = 0; i < 9; i++) // row
	// for (u8 j = 0; j < 8; j++) // col
	// {
	// 	drawLoadingPokeball(
	// 		rowBackball[(i*2)%3][0] + (j*2) * 24 - 32 + tick % 64,
	// 		((i*2)/3) * 64  + rowBackball[(i*2)%3][1] - (j*2) * 8 - 16 + tick % 64
	// 	);
	// }

	drawLoadingText(300, 220);
}

void TextureManager::drawLoadingPokeball(int x, int y)
{
	sf2d_draw_texture_part_scale(this->ballLoadingScreen, x, y,  0,  0, 16, 16, 2.0f, 2.0f);
}

void TextureManager::drawLoadingGreatball(int x, int y)
{
	sf2d_draw_texture_part_scale(this->ballLoadingScreen, x, y, 16,  0, 16, 16, 2.0f, 2.0f);
}

void TextureManager::drawLoadingUltraball(int x, int y)
{
	sf2d_draw_texture_part_scale(this->ballLoadingScreen, x, y, 32, 0, 16, 16, 2.0f, 2.0f);
}

void TextureManager::drawLoadingBackball(int x, int y)
{
	sf2d_draw_texture_part(this->ballLoadingScreen, x, y, 48, 0, 16, 16);
}

void TextureManager::drawLoadingText(int rx, int ry)
{
	sf2d_draw_texture_part_scale(this->ballLoadingScreen, rx - 64 * 4, ry -  8 * 4, 0, 16, 64, 8, 4.0f, 4.0f);
}
