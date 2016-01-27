#pragma once
/**
 * @file texture_manager.hpp
 * @brief Texture Manager
 */
#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <3ds/types.h>
#include <sf2d.h>

class TextureManager
{
	public:
		sf2d_texture* pkmIcons;
		sf2d_texture* pkmShinyIcons;
		sf2d_texture* ballIcons;

		sf2d_texture* resumeBackground;
		
	public:
		TextureManager();
		~TextureManager();

		Result load();
};

#endif // TEXTURE_MANAGER_HPP
