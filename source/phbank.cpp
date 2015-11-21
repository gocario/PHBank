#include "phbank.hpp"

#include "FreeSans_ttf.h"
// #include "PokemonGB_ttf.h"
#include "RobotoRegular_ttf.h"

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


Result PHBank::load()
{
	PHBank::_font = sftd_load_font_mem(RobotoRegular_ttf, RobotoRegular_ttf_size);

	return 0;
}

Result PHBank::destroy()
{
	// delete PHBank::_pHBank;
	// delete PHBank::_pKBank;
	printf("Deleting font...");
	if (_font)
		sftd_free_font(PHBank::_font);
	printf(" OK!\n");

	return 0;
}
