#include "texture_manager.hpp"

#include <sfil.h>
#include <stdio.h>

#define ROMFS "/pk/romfs/"

TextureManager::TextureManager()
{

}


TextureManager::~TextureManager()
{
	sf2d_free_texture(pkmIcons);
	sf2d_free_texture(pkmShinyIcons);
	sf2d_free_texture(ballIcons);
	sf2d_free_texture(resumeBackground);
}


Result TextureManager::load()
{
	printf("Loading pkmIcons: %p\n", (pkmIcons = sfil_load_PNG_file(ROMFS "pokemon_icons_spritesheet.png", SF2D_PLACE_RAM)));
	printf("Loading pkmShinyIcons: %p\n", (pkmShinyIcons = sfil_load_PNG_file(ROMFS "pokemon_shiny_icons_spritesheet.png", SF2D_PLACE_RAM)));
	printf("Loading ballIcons: %p\n", (ballIcons = sfil_load_PNG_file(ROMFS "ball_icons_spritesheet.png", SF2D_PLACE_RAM)));
	printf("Loading resumeBackground: %p\n", (resumeBackground = sfil_load_PNG_file(ROMFS "resume_background.png", SF2D_PLACE_RAM)));

	return !(pkmIcons && pkmShinyIcons && ballIcons && resumeBackground);
}
