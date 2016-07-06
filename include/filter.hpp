#pragma once
/**
 * @file filter.hpp
 * @brief Filter module
 * @author Gocario
 */

#include "pkm/pk6.hpp"

namespace Filter
{
	/**
	 * @brief Runs a filter to see if the Pokémon can be transfered from Bank to XY
	 * @param pk6 The Pokémon to filter.
	 * @return Wether the Pokémon runs through the filter.
	 */
	bool filterToXY(const PK6_s* pk6);

	/**
	 * @brief Runs a filter to see if the Pokémon can be transfered from the Bank to ORAS
	 * @param pk6 The Pokémon to filter.
	 * @return Wether the Pokémon runs through the filter.
	 */
	bool filterFromXY(const PK6_s* pk6);

	/**
	 * @brief Runs a filter to see if the Pokémon can be transfered from the Bank to ORAS
	 * @param pk6 The Pokémon to filter.
	 * @return Wether the Pokémon runs through the filter.
	 */
	bool filterToORAS(const PK6_s* pk6);

	/**
	 * @brief Runs a filter to see if the Pokémon can be transfered from ORAS to the Bank
	 * @param pk6 The Pokémon to filter.
	 * @return Wether the Pokémon runs through the filter.
	 */
	bool filterFromORAS(const PK6_s* pk6);

	/**
	 * @brief Runs a filter to see if the Pokémon can be transfered from the Bank to SM
	 * @param pk6 The Pokémon to filter.
	 * @return Wether the Pokémon runs through the filter.
	 * @todo Implement!
	 */
	bool filterToSM(const PK6_s* pk6);

	/**
	 * @brief Runs a filter to see if the Pokémon can be transfered from SM to the Bank
	 * @param pk6 The Pokémon to filter.
	 * @return Wether the Pokémon runs through the filter.
	 * @todo Implement!
	 */
	bool filterFromSM(const PK6_s* pk6);
};

// FILTER_HPP
