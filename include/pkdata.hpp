#ifndef PKDATA_HPP
#define PKDATA_HPP

#include <3ds.h>

#include "main.hpp"
#include "filesystem.hpp"


#define LANG_EN 1
#define LANG_FR 2


struct sprite_t;
typedef uint32_t species_t;

sprite_t getSpriteFromSpecies(species_t species);


class PKData
{
	public:

		static void loadData(Handle *sdHandle, FS_archive *sdArchive);
		static void loadDataLine(uint8_t *src, uint8_t *dest, uint8_t len, uint32_t size);

		static const char* lang();
		static uint8_t* species(uint32_t species);
		static uint8_t* items(uint32_t item);
	private:

		static uint8_t _lang;
		static uint8_t _species[722][15]; // 11~13
		static uint8_t _items[776][20]; // 17
};


#endif // PKDATA_HPP