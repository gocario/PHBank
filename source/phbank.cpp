#include "phbank.hpp"

PHBank::PHBank() {}
PHBank::~PHBank() {}

PHBank PHBank::_pHBank;
PKBank PHBank::_pKBank;

PHBank* PHBank::pHBank()
{
	return &(PHBank::_pHBank);
}

PKBank* PHBank::pKBank()
{
	return &(PHBank::_pKBank);
}


void PHBank::destroy()
{
	// delete PHBank::_pHBank;
	// delete PHBank::_pKBank;
}