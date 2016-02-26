#pragma once
/**
 * @file data_manager.hpp
 * @brief Data Manager
 */
#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include <3ds/types.h>

#define DEX_ABILITIES_COUNT (192) // 0: None
#define DEX_ITEMS_COUNT (776) // 0: None
#define DEX_MOVES_COUNT (622) // 0: None
#define DEX_NATURES_COUNT (25)
#define DEX_SPECIES_COUNT (722) // 0: Egg
#define DEX_TYPES_COUNT (18) // 0: Normal

class DataManager
{
	private:
		uint32_t* wAbilities[DEX_ABILITIES_COUNT];
		uint32_t* wItems[DEX_ITEMS_COUNT];
		uint32_t* wMoves[DEX_MOVES_COUNT];
		uint32_t* wNatures[DEX_NATURES_COUNT];
		uint32_t* wSpecies[DEX_SPECIES_COUNT];
		uint32_t* wTypes[DEX_TYPES_COUNT];

	public:
		DataManager(void);
		~DataManager(void);

		Result load(void);

		const char* lang(void);
		const uint32_t* abilities(u32 ability);
		const uint32_t* items(u32 item);
		const uint32_t* moves(u32 move);
		const uint32_t* natures(u32 nature);
		const uint32_t* species(u32 species);
		const uint32_t* types(u8 type);

	private:
		Result loadPersonal(const char* file, bool ao, u32 personalCount, u32 personalInfoSize);
		Result loadDataFile(const char* file, uint32_t** data, u32 lineCount);
		void loadDataLines(const u8* src, uint32_t** data, u32 srcSize, u32 lineCount);
		void freeDataLines(uint32_t** data, u32 lineCount);
};

#endif // DATA_MANAGER_HPP
