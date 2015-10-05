#ifndef PKDATA_HPP
#define PKDATA_HPP

#include "main.hpp"
#include "filesystem.hpp"


#define LANG_EN 1
#define LANG_FR 2

#define DEX_SPECIES_COUNT 722
#define DEX_SPECIES_LENGTH 20
#define DEX_ITEMS_COUNT 776
#define DEX_ITEMS_LENGTH 20


struct sprite_t;
typedef uint32_t species_t;

sprite_t getSpriteFromSpecies(species_t species);


class PKData
{
	public:
		static Result load(Handle *sdHandle, FS_archive *sdArchive);
		static Result loadData(Handle *sdHandle, FS_archive *sdArchive, u32 maxSize, char* path, u8* dest, u32 lineLength, u32 lineCount);
		static Result loadDataLine(u8* src, u8* dst, u32 lineLength, u32 lineCount);

		static const char* lang();
		static uint8_t* species(uint32_t species);
		static uint8_t* items(uint32_t item);
	private:

		static uint8_t _lang;
		static uint8_t _species[DEX_SPECIES_COUNT][DEX_SPECIES_LENGTH]; // 11~13
		static uint8_t _items[DEX_ITEMS_COUNT][DEX_ITEMS_LENGTH]; // 17
};


#endif // PKDATA_HPP