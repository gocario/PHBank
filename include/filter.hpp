#pragma once
/**
 * @file filter.hpp
 * @brief Filter module
 */
#ifndef FILTER_HPP
#define FILTER_HPP

#include "save.hpp"

namespace Filter
{
	/**
	 * @brief Runs a filter to see if the Pokémon can be transfered from Bank to XY
	 * @param pkm The Pokémon to filter.
	 * @return Wether the Pokémon runs through the filter.
	 */
	bool filterToXY(pkm_s* pkm);

	/**
	 * @brief Runs a filter to see if the Pokémon can be transfered from the Bank to ORAS
	 * @param pkm The Pokémon to filter.
	 * @return Wether the Pokémon runs through the filter.
	 */
	bool filterFromXY(pkm_s* pkm);

	/**
	 * @brief Runs a filter to see if the Pokémon can be transfered from the Bank to ORAS
	 * @param pkm The Pokémon to filter.
	 * @return Wether the Pokémon runs through the filter.
	 */
	bool filterToORAS(pkm_s* pkm);

	/**
	 * @brief Runs a filter to see if the Pokémon can be transfered from ORAS to the Bank
	 * @param pkm The Pokémon to filter.
	 * @return Wether the Pokémon runs through the filter.
	 */
	bool filterFromORAS(pkm_s* pkm);
};

#endif // FILTER_HPP
