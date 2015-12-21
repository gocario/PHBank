#pragma once
#ifndef PHBANK_HPP
#define PHBANK_HPP

#include "save_manager.hpp"
#include "data_manager.hpp"
#include "font_manager.hpp"
/*
class PHBank
{
	public:
		explicit PHBank();
		~PHBank();

		static PHBank* pHBank();
		static PKBank* pKBank();
		static sftd_font* font();

		static Result load();
		static Result destroy();

	private:
		static PHBank _pHBank;
		static PKBank _pKBank;
		static sftd_font* _font;
};
*/

namespace PHBanku
{
	extern SaveManager* save;
	extern DataManager* data;
	extern FontManager* font;
}


#endif // PHBANK_HPP
