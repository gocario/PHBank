#ifndef BOX_VIEWER_HPP
#define BOX_VIEWER_HPP

#include "phbank.hpp"
#include "viewer.hpp"

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

typedef struct vPkm_t {
	pkm_t* pkm = NULL;
	bool emptySlot;
	u8 NKName[0x1a / 2];
	u8 OTName[0x1a / 2];
	u8 HTName[0x1a / 2];
	const u8* species;
	const u8* item;
	const u8* nature;
	const u8* ability;
	const u8* moves[4];
	const u8* hiddenPower;
	u8 level;
	u16 stats[6];
	u16 ivs[6];
	u16 evs[6];
} vPkm_t;

typedef struct BoxSlot_t {
	bool inBank = false;
	int16_t slot = 0;
	int16_t inslot = 0;
	int16_t box = 0;
	int16_t row = 0;
	int16_t col = 0;
} BoxSlot_t;

typedef struct CursorBox_t {
	bool inBank = false;
	int16_t slot = 0;
	int16_t inslot = 0;
	int16_t boxPC = 0;
	int16_t boxBK = 0;
	int16_t* box = NULL;
	int16_t row = 0;
	int16_t col = 0;
} CursorBox_t;

namespace CursorType
{
	typedef enum CursorType_e {
		SingleSelect = 0,
		QuickSelect = 1,
		MultipleSelect = 2,
	} CursorType_e;
}
typedef CursorType::CursorType_e CursorType_e;

namespace CursorState
{
	typedef enum CursorState_e {
		SelectingTarget = 0,
		TargetFound = 1,
		DraggingTarget = 2,
	} CursorState_e;
}
typedef CursorState::CursorState_e CursorState_e;


void computeSlot(CursorBox_t* cursorBox);
void extractBoxSlot(CursorBox_t* cursorBox, BoxSlot_t* boxSlot);
void injectBoxSlot(CursorBox_t* cursorBox, BoxSlot_t* boxSlot);


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

		void selectViewBox(uint16_t boxID, bool inBank = false);
	private:
		sf2d_texture* backgroundBox = NULL;
		sf2d_texture* backgroundResume = NULL;
		sf2d_texture* icons = NULL;
		sf2d_texture* tiles = NULL;

		CursorBox_t cursorBox;
		CursorType_e cursorType = CursorType::SingleSelect;
		touchPosition touch;
		bool isPkmHeld = false;
		bool isPkmDragged = false;
		bool isMultiPkmDragged = false;
		BoxSlot_t sSlot;
		pkm_t* sPkm = NULL;
		vPkm_t vPkm;
		box_t* vPCBox = NULL;
		box_t* vBKBox = NULL;

		void selectCursorType(CursorType_e cursorType);
		void switchCursorType();
		void selectViewBox();
		void selectViewPokemon();
		void selectMovePokemon();
		void cancelMovePokemon();
		void populateVPkmData(vPkm_t* vPkm);
};


#endif // BOX_VIEWER_HPP