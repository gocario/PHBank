#include "bank_updater.hpp"

#include <stdio.h>

#define MATCH_VER(check, version) ((check & 0xFFFFFFF0) == (version & 0xFFFFFFF0))

namespace BankUpdater
{
	/// DEBUG_FUNCTION
	static inline void write(u8* buf, u32 off, u32 count)
	{
		printf("[@0x%lu](%lu):\n", off, count);
		for (u32 i = 0; i < count; i++)
		{
			printf("%02x ", buf[off+i]);
			if (i % 0x10 == 0xF) printf("\n");
		}
		printf("\n");
	}

	static inline uint32_t MakeMagic(char a, char b, char c, char d)
	{
		return (a | b << 8 | c << 16 | d << 24);
	}

	bool updateBank(u8* bank, u32 bytesRead)
	{
		// It can't even read the magic/version
		if (bytesRead < 0x10) return false;

		u32 version = *(u32*)(bank + 0x4);

		printf("Bank code version: %8x\n", VERSION);
		printf("Bank file version: %8lx\n", version);

		// If the version is already the current
		if (version == VERSION) return true;

		// Before the versioning starts or blank file
		if (version == 0x00000000)
		{
			printf("\a0x00000000->0x020000B0\n");

			*(u32*)(bank + 0x00) = MakeMagic('B', 'A', 'N', 'K');
			printf("\r"); // It is really needed else it will freeze

			// bank[0] = 'B';
			// bank[1] = 'A';
			// bank[2] = 'N';
			// bank[3] = 'K';

			*(u32*)(bank + 0x04) = version = 0x020000B0;
			*(u32*)(bank + 0x20) = 0x00000100;	///< Box data offset (NEW)
			*(u32*)(bank + 0x24) = 0x000A9FC0;	///< Box name offset (NEW)
			*(u32*)(bank + 0x28) = 0x000AACF8;	///< Box background offset (NEW)
			*(u32*)(bank + 0x2C) = 0x00000000;	///< ??? offset (NEW)
		}

		// Before the wonderbox
		if (MATCH_VER(version, 0x020000B0))
		{
			printf("\a0x020000B0->0x020001B0\n");

			*(u32*)(bank + 0x04) = version = 0x020001B0;
			*(u32*)(bank + 0x30) = 0x000AAE00;	///< Wonder box data offset (NEW)
		}

		// Current version
		if (MATCH_VER(version, 0x020001B0))
		{
			printf("\a0x020001B0->0x020101B0\n");

			*(u32*)(bank + 0x04) = version = 0x020101B0;
			*(u32*)(bank + 0x34) = 0x000AC930;	///< Trash box data offset (NEW)
		}

		if (MATCH_VER(version, 0x020101B0))
		{
			printf("\a->0x020001B0\n");

			*(u32*)(bank + 0x04) = version = 0x020200B0;

			return true;
		}

		// Current real version
		return version == VERSION;
	}

	/**
	 * @brief data[@offset](#size)
	 *
	 * Box data[@0x00000100](#0xA9EC0)
	 * Box name[@0x000A9FC0](#0xD48)
	 * Box background[@0x000AACF8](#0x64)
	 * Wonder box data[@0x000AAE00](#0x1B30)
	 * Trash box data[@0x000AC930](#0x1B30)
	 **/
}
