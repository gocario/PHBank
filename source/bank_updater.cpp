#include "bank_updater.hpp"

#include <stdio.h>

namespace BankUpdater
{
	/// DEBUG_FUNCTION
	inline void write(u8* buf, u32 off, u32 count)
	{
		for (u32 i = 0; i < count; i++)
		{
			printf("%02x ", buf[off+i]);
			if (i % 0xF == 0) printf("\n");
		}
		printf("\n");
	}

	bool updateBank(bankbuffer_t bankbuffer, u32 bytesRead)
	{
		// It can't even read the magic/version
		if (bytesRead < 0x10) return false;

		u32 version = *(u32*)(bankbuffer + 0x4);

		printf("Bank code version: %8x\n", VERSION);
		printf("Bank file version: %8lx\n", version);

		// If the version is already the current
		if (version == VERSION) return true;

		// Before the versioning starts
		if (version == 0x00000000)
		{
			*(u32*)(bankbuffer + 0x00) = MakeMagic('B', 'A', 'N', 'K');
			*(u32*)(bankbuffer + 0x04) = version = 0x020000B0;
			*(u32*)(bankbuffer + 0x20) = 0x00000100;	///< Box data offset
			*(u32*)(bankbuffer + 0x24) = 0x000A9FC0;	///< Box name offset
			*(u32*)(bankbuffer + 0x28) = 0x000AACF8;	///< Box background offset
			*(u32*)(bankbuffer + 0x2C) = 0x00000000;	///< ??? offset
		}

		// Current version
		if (version == 0x020000B0)
		{
			return true;
		}

		// It didn't reach the current version
		return false;
	}
}