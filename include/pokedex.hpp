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
	 * @brief Registers a Pokémon into a Pokédex of a XY game.
	 * @param sav The XY save buffer of the Pokédex.
	 * @param pkm The Pokémon to register.
	 */
	void importToXY(savebuffer_t sav, pkm_s* pkm);

	/**
	 * @brief Registers a Pokémon into a Pokédex of an ORAS game.
	 * @param sav The ORAS save buffer of the Pokédex.
	 * @param pkm The Pokémon to register.
	 */
	void importToORAS(savebuffer_t sav, pkm_s* pkm);
}

#endif // POKEDEX_HPP
