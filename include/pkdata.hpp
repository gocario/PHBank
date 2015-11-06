#ifndef PKDATA_HPP
#define PKDATA_HPP

#include "main.hpp"
#include "filesystem.hpp"


#define LANG_EN 1
#define LANG_FR 2

#define PERSONAL_COUNT 826
#define PERSONAL_LENGTH 14

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
		static u8* personal(u32 species, u32 form);
		static u8* personal(u32 species);
		static u8* abilities(u32 ability);
		static u8* items(u32 item);
		static u8* moves(u32 move);
		static u8* natures(u32 nature);
		static u8* species(u32 species);

		static const u8* HPTypes(u8 hiddenPower);
	private:

		static u8 _lang;
		static u8 _personal[PERSONAL_COUNT][PERSONAL_LENGTH];
		static u8 _abilities[DEX_ABILITIES_COUNT][DEX_ABILITIES_LENGTH];
		static u8 _items[DEX_ITEMS_COUNT][DEX_ITEMS_LENGTH];
		static u8 _moves[DEX_MOVES_COUNT][DEX_MOVES_LENGTH];
		static u8 _natures[DEX_NATURES_COUNT][DEX_NATURES_LENGTH];
		static u8 _species[DEX_SPECIES_COUNT][DEX_SPECIES_LENGTH];
};

#endif // PKDATA_HPP
