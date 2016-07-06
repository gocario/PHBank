#include "save/sav.hpp"

Game::Version SAV::getGameVersion(u64 size)
{
	switch (size)
	{
		case Game::Const::XY_size: return Game::XY;
		case Game::Const::ORAS_size: return Game::ORAS;
		// case Game::Const::SM_size: return Game::SM;
		default: return Game::Unknown;
	}
}

u64 SAV::getGameSize(Game::Version version)
{
	if (Game::is(version, Game::XY))
		return Game::Const::XY_size;
	else if (Game::is(version, Game::ORAS))
		return Game::Const::ORAS_size;
	// else if (Game::is(version, Game::SM))
	//	return Game::Const::SM_size;
	else return 0;
}
