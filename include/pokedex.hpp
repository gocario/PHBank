#pragma once
/**
 * @file pokedex.hpp
 * @author Gocario
 */

#include "sav/sav6.hpp"
#include "save/save.hpp"

#define PKM_COUNT (721)

namespace Pokedex
{
	/**
	 * @brief Registers a Pokémon into a Pokédex.
	 * @param version The version of the game.
	 * @param sav The save buffer of the Pokédex.
	 * @param pkm The Pokémon to register.
	 */
	void importToGame(Game::Version version, u8* sav, const PK6_s* pkm);
}

// POKEDEX_HPP
