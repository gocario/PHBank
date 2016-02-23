#pragma once
/**
 * @file pokedex.hpp
 */
#ifndef POKEDEX_HPP
#define POKEDEX_HPP

#include "save.hpp"

namespace Pokedex
{
	/**
	 * @brief Registers a Pokémon into a Pokédex.
	 * @param version The version of the game.
	 * @param sav The save buffer of the Pokédex.
	 * @param pkm The Pokémon to register.
	 */
	void importToGame(GameVersion version, savebuffer_t sav, pkm_s* pkm);
}

#endif // POKEDEX_HPP
