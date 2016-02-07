#pragma once
/**
 * @file pokedex.hpp
 */
#ifndef POKEDEX_HPP
#define POKEDEX_HPP

#include "save.hpp"

namespace Pokedex
{
	void importToXY(savebuffer_t sav, pkm_s* pkm);
	void importToORAS(savebuffer_t sav, pkm_s* pkm);
}

#endif // POKEDEX_HPP
