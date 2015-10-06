#ifndef BOX_VIEWER_HPP
#define BOX_VIEWER_HPP

#include "phbank.hpp"
#include "viewer.hpp"

#include <sf2d.h>
#include <3ds.h>

#define BOX_ROW_PKMCOUNT 5
#define BOX_COL_PKMCOUNT 6
#define BOX_PKMCOUNT BOX_ROW_PKMCOUNT * BOX_COL_PKMCOUNT
#define PC_BOXCOUNT 31 // 0x1f
#define BANK_BOXCOUNT 100 // 0x64
#define PKM_COUNT 721

#define PKM_SIZE 0xe8
#define BOX_SIZE PKM_SIZE * BOX_PKMCOUNT
#define PC_SIZE BOX_SIZE * PC_BOXCOUNT
#define BANK_SIZE BOX_SIZE * BANK_BOXCOUNT

// struct pkm_t { uint16_t speciesID; };
// struct box_t { pkm_t slot[30]; };
typedef struct CursorBox_t { bool inBank = false; int16_t slot = 0; int16_t inslot = 0; int16_t boxPC = 0; int16_t boxBK = 0; int16_t* box = NULL; int16_t row = 0; int16_t col = 0; } CursorBox_t;
namespace CursorType { typedef enum cursorType_e { SingleSelect = 0, QuickSelect = 1, MultipleSelect = 2, } CursorType_e; } typedef CursorType::CursorType_e CursorType_e;

void computeSlot(CursorBox_t* cursorBox);


class BoxViewer : public Viewer
{
	public:
		BoxViewer(Viewer* parent = NULL);
		BoxViewer(StateView_e state, Viewer* parent = NULL);
		~BoxViewer();

		Result initialize() override;
		Result drawTopScreen() override;
		Result drawBotScreen() override;
		Result updateControls(const u32& kDown = 0, const u32& kHeld = 0, const u32& kUp = 0, const touchPosition* touch = NULL) override;

	private:
		sf2d_texture* background = NULL;
		sf2d_texture* icons = NULL;
		sf2d_texture* tiles = NULL;

		CursorBox_t cursorBox;
		CursorType_e cursorType = CursorType::SingleSelect;
		pkm_t* sPkm = NULL;
		pkm_t* vPkm = NULL;
		box_t* vPCBox = NULL;
		box_t* vBKBox = NULL;

		void selectCursorType(CursorType_e cursorType);
		void switchCursorType();
		void selectViewBox();
		void selectViewPokemon();
		void selectMovePokemon();
		void cancelMovePokemon();
		void pasteMovePokemon();
};


#endif // BOX_VIEWER_HPP