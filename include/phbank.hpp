#ifndef PHBANK_HPP
#define PHBANK_HPP

#include "pkbank.hpp"
#include "pkdata.hpp"

class PHBank
{
	public:
		explicit PHBank();
		~PHBank();

		static PHBank* pHBank();
		static PKBank* pKBank();

		static void destroy();

	private:
		static PHBank _pHBank;
		static PKBank _pKBank;
};

#endif // PHBANK_HPP