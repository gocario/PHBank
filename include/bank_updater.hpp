#pragma once
/**
 * @file bank_updater.hpp
 * @author Gocario
 */
#ifndef BANK_UPDATER_HPP
#define BANK_UPDATER_HPP

#include "version.h"

#include <3ds/types.h>

namespace BankUpdater
{
	/**
	 * @brief Updates a bankbuffer to the latest version.
	 * @param bankbuffer The bank buffer of the bank file.
	 * @param bytesRead The number of read bytes.
	 * @return Whether the buffer is up to date.
	 * @see VERSION
	 */
	bool updateBank(u8* bank, u32 bytesRead);

	/**
	 * @todo
	 */
	bool convertBankToGen7(u8* bank, u32 bytesRead);
}

#endif // BANK_UPDATER_HPP
