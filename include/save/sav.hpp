#pragma once
/**
 * @file save/sav.hpp
 * @author Gocario
 */

#include "save/save.hpp"
 
class SAV
{
	public:
		static Game::Version getGameVersion(u64 size);
		static u64 getGameSize(Game::Version version);
};

// SAV_HPP
