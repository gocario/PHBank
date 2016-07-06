#pragma once
/**
 * @file sav/save.hpp
 * @author Gocario
 */

#include <3ds/types.h>

namespace Game
{
	enum Version
	{
		Unknown = 0,

		// Single Game
		S = 1,		// Sapphire
		R = 2,		// Ruby
		E = 3,		// Emerald
		FR = 4,		// Fire Red
		LG = 5,		// Leaf Green
		// 6
		HG = 7,		// Heart Gold
		SS = 8,		// Soul Silver
		// 9
		D = 10,		// Diamond
		P = 11,		// Perl
		Pt = 12,	// Platinum
		// 13
		// 14
		CXD = 15,	// ?
		// 16
		// 17
		// 18
		// 19
		W = 20,		// White
		B = 21,		// Black
		W2 = 22,	// White 2
		B2 = 23,	// Black 2
		X = 24,		// X
		Y = 25,		// Y
		OR = 26,	// Omega Ruby
		AS = 27,	// Alpha Sapphire
		// S = 28,		// Sun
		// M = 29,		// Moon

		// Group Games
		RSE = 10001,
		FRLG = 10002,
		HGSS = 1003,
		DPP = 1004,
		BW = 1005,
		BW2 = 1006,
		XY = 1007,
		ORAS = 1008,
		// SM = 1009,

		// Generation
		Gen3 = 2001,
		Gen4 = 2002,
		Gen5 = 2003,
		Gen6 = 2004,
		// Gen7 = 2005,
	};

	typedef u32 SaveConst;

	namespace Const
	{
		const SaveConst XY_size = 0x65600;
		const SaveConst ORAS_size = 0x76000;
		// const SaveConst SM_size = 0x1;

		const SaveConst BANK_size = 0xAEA00;
	}

	inline bool is(Version ver, Version gen)
	{
		switch (gen)
		{
			// Unknown
			case Unknown: return ver == Unknown;

			// By Serie
			case BW: return ver == W || ver == B || ver == BW;
			case BW2: return ver == W2 || ver == B2 || ver == BW2;
			case XY: return ver == X || ver == Y || ver == XY;
			case ORAS: return ver == OR || ver == AS || ver == ORAS;
			// case SM: return ver == S || ver == M || ver == SM;

			// By Generation
			case Gen3: return is(ver, Game::RSE) || is(ver, Game::FRLG);
			case Gen4: return is(ver, Game::HGSS) || is(ver, Game::DPP);
			case Gen5: return is(ver, Game::BW) || is(ver, Game::BW2);
			case Gen6: return is(ver, Game::XY) || is(ver, Game::ORAS);
			// case Gen6: return is(ver, Game::SM);

			// By Default
			default: return ver == gen;
		}
	}
};
