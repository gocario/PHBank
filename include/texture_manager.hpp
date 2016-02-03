#pragma once
/**
 * @file texture_manager.hpp
 * @brief Texture Manager
 */
#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <sf2d.h>

class TextureManager
{
	public:
		sf2d_texture* ballLoadingScreen;
		sf2d_texture* pkmSprites;		///< The sprite sheet for Pokémon 2D art.
		sf2d_texture* pkmShinySprites;	///< The sprite sheet for shiny Pokémon 2D art.
		sf2d_texture* pkmIcons;			///< The sprite sheet for Pokémon icons.
		sf2d_texture* pkmShinyIcons;	///< The sprite sheet for shiny Pokémon icons.
		sf2d_texture* itemIcons;		///< The sprite sheet for item icons.
		sf2d_texture* ballIcons;		///< The sprite sheet for ball icons.
		sf2d_texture* types;			///< The sprite sheet for lang types images.
		sf2d_texture* boxTiles;			///< The sheet for box sprites (cursor, button, etc).
		sf2d_texture* boxBackground;	///< The background of the box (TODO: Multiple background).
		sf2d_texture* resumeBackground;	///< The resume background for top screen.
		
	public:
		TextureManager(void);
		~TextureManager(void);

		Result load(void);
};

#endif // TEXTURE_MANAGER_HPP
