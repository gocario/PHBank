#pragma once
/**
 * @file data_manager.hpp
 * @brief Data Manager
 * @author Gocario
 */

#include <3ds/types.h>

#define BANK_TEXT_COUNT (29) // 0: Blank
#define DEX_ABILITIES_COUNT (192) // 0: None
#define DEX_ITEMS_COUNT (776) // 0: None
#define DEX_MOVES_COUNT (622) // 0: None
#define DEX_NATURES_COUNT (25)
#define DEX_SPECIES_COUNT (722) // 0: Egg
#define DEX_TYPES_COUNT (18) // 0: Normal

enum class BankText : u8
{
	Blank = 0,
	BOX_VIEWER = 1,
	GameTrainer = 2,
	Level = 3,
	DexNo = 4,
	OriginalTrainer = 5,
	Stat = 6,
	Value = 7,
	IV = 8,
	EV = 9,
	HitPoint = 10,
	Attack = 11,
	Defense = 12,
	SpAttack = 13,
	SpDefense = 14,
	Speed = 15,
	Nature = 16,
	Ability = 17,
	Item = 18,
	HiddenPower = 19,
	Moves = 20,
	Box = 21,

	SAVEXIT_VIEWER = 23,
	ExitMessage = 24,
	SaveExit = 25,
	Exit = 26,
	Backup = 27,
	Return = 28,
};

class DataManager
{
	private:
		uint32_t* wText[BANK_TEXT_COUNT];
		uint32_t* wAbilities[DEX_ABILITIES_COUNT];
		uint32_t* wItems[DEX_ITEMS_COUNT];
		uint32_t* wMoves[DEX_MOVES_COUNT];
		uint32_t* wNatures[DEX_NATURES_COUNT];
		uint32_t* wSpecies[DEX_SPECIES_COUNT];
		uint32_t* wTypes[DEX_TYPES_COUNT];

	public:
		DataManager();
		~DataManager();

		Result load();

		const char* lang();
		const uint32_t* text(BankText text);
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

// DATA_MANAGER_HPP
