#ifndef PKDATA_HPP
#define PKDATA_HPP

#include "main.hpp"
#include "filesystem.hpp"


#define LANG_EN 1
#define LANG_FR 2

#define DEX_ABILITIES_COUNT 192
#define DEX_ABILITIES_LENGTH 16
#define DEX_ITEMS_COUNT 776
#define DEX_ITEMS_LENGTH 20
#define DEX_MOVES_COUNT 622
#define DEX_MOVES_LENGTH 20
#define DEX_NATURES_COUNT 25
#define DEX_NATURES_LENGTH 10
#define DEX_SPECIES_COUNT 722
#define DEX_SPECIES_LENGTH 20


class PKData
{
	public:
		static Result load(Handle *sdHandle, FS_archive *sdArchive);
		static Result loadData(Handle *sdHandle, FS_archive *sdArchive, u32 maxSize, char* path, u8* dest, u32 lineLength, u32 lineCount);
		static Result loadDataLine(u8* src, u8* dst, u32 lineLength, u32 lineCount);

		static const char* lang();
		static uint8_t* abilities(uint32_t ability);
		static uint8_t* items(uint32_t item);
		static uint8_t* moves(uint32_t move);
		static uint8_t* natures(uint32_t nature);
		static uint8_t* species(uint32_t species);
	private:

		static uint8_t _lang;
		static uint8_t _abilities[DEX_ABILITIES_COUNT][DEX_ABILITIES_LENGTH];
		static uint8_t _items[DEX_ITEMS_COUNT][DEX_ITEMS_LENGTH];
		static uint8_t _moves[DEX_MOVES_COUNT][DEX_MOVES_LENGTH];
		static uint8_t _natures[DEX_NATURES_COUNT][DEX_NATURES_LENGTH];
		static uint8_t _species[DEX_SPECIES_COUNT][DEX_SPECIES_LENGTH];
};


#endif // PKDATA_HPP