#pragma once
/**
 * @file phbank.hpp
 * @brief Master-manager
 */

#include "save_manager.hpp"
#include "data_manager.hpp"
#include "font_manager.hpp"
#include "texture_manager.hpp"

namespace PHBank
{
	extern SaveManager* Save;
	extern DataManager* Data;
	extern FontManager* Font;
	extern TextureManager* Texture;
}

// PHBANK_HPP
