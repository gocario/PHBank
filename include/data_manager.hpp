#pragma once
/**
 * @file data_manager.hpp
 * @brief Data Manager
 */
#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include <3ds/types.h>

#define DEX_ABILITIES_COUNT (192) // 0: None
#define DEX_ABILITIES_LENGTH (16)
#define DEX_ITEMS_COUNT (776) // 0: None
#define DEX_ITEMS_LENGTH (20)
#define DEX_MOVES_COUNT (622) // 0: None
#define DEX_MOVES_LENGTH (20)
#define DEX_NATURES_COUNT (25)
#define DEX_NATURES_LENGTH (10)
#define DEX_SPECIES_COUNT (722) // 0: Egg
#define DEX_SPECIES_LENGTH (20)

class DataManager
{
	private:
		u8 _abilities[DEX_ABILITIES_COUNT][DEX_ABILITIES_LENGTH];
		u8 _items[DEX_ITEMS_COUNT][DEX_ITEMS_LENGTH];
		u8 _moves[DEX_MOVES_COUNT][DEX_MOVES_LENGTH];
		u8 _natures[DEX_NATURES_COUNT][DEX_NATURES_LENGTH];
		u8 _species[DEX_SPECIES_COUNT][DEX_SPECIES_LENGTH];

	public:
		DataManager(void);
		~DataManager(void);

		Result load(void);

		const char* lang(void);
		const u8* abilities(u32 ability);
		const u8* items(u32 item);
		const u8* moves(u32 move);
		const u8* natures(u32 nature);
		const u8* species(u32 species);
		const u8* HPTypes(u8 hiddenPower);

	private:
		Result loadDataFile(const char* path, u8* dest, u32 lineMaxLength, u32 lineCount);
		Result loadDataLines(const u8* src, u8* dst, u32 lineMaxLength, u32 lineCount);
};

#endif // DATA_MANAGER_HPP
