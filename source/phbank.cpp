#include "phbank.hpp"

#include "PokemonGB_ttf.h"

PHBank::PHBank() {}
PHBank::~PHBank() {}

PHBank PHBank::_pHBank;
PKBank PHBank::_pKBank;
sftd_font* PHBank::_font = NULL;

PHBank* PHBank::pHBank()
{
	return &(PHBank::_pHBank);
}

PKBank* PHBank::pKBank()
{
	return &(PHBank::_pKBank);
}

sftd_font* PHBank::font()
{
	return (PHBank::_font);
}


void PHBank::load()
{
	PHBank::_font = sftd_load_font_mem(PokemonGB_ttf, PokemonGB_ttf_size);
}

void PHBank::destroy()
{
	// delete PHBank::_pHBank;
	// delete PHBank::_pKBank;
	printf("Deleting font...");
	if (_font)
		sftd_free_font(PHBank::_font);
	printf(" OK!\n");
}