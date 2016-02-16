#pragma once
/**
 * @file bank_updater.hpp
 */
#ifndef BANK_UPDATER_HPP
#define BANK_UPDATER_HPP

#include "version.h"
#include "save.hpp"

namespace BankUpdater
{
	/**
	 * @brief Updates a bankbuffer to the latest version.
	 * @param bankbuffer The bank buffer of the bank file.
	 * @param bytesRead The number of read bytes.
	 * @return Whether the buffer is up to date.
	 * @see VERSION
	 */
	bool updateBank(bankbuffer_t bankbuffer, u32 bytesRead);
}

#endif // BANK_UPDATER_HPP
