#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "phbank.hpp"
#include "personal.hpp"
#include "view/box_browser.hpp"

#include "fs.h"
#include "key.h"
#include "error.h"
#include "version.h"

#ifdef __cia
#include "ts.h"
#endif

/// A very bad implementation for consoleExit, only for debug.
PrintConsole* consoleExit(gfxScreen_t screen, PrintConsole* console)
{
	// TODO Future implementation!
	extern PrintConsole* currentConsole;
	*currentConsole = *consoleGetDefault();
	gfxSetScreenFormat(screen, GSP_BGR8_OES);
	gfxSetDoubleBuffering(screen, true);
	gspWaitForVBlank();
	return console;
}

void sizeof_report()
{
#define _SIZE_OF(x) printf(#x ": 0x%X\n", sizeof(x))
	printf("Sizeof report:\n");
	printf("\n");
	_SIZE_OF(EK6_t);
	_SIZE_OF(PK6_t);
	_SIZE_OF(PK6_s);
	_SIZE_OF(BattlePK6_s);
	printf("\n");
	_SIZE_OF(TrainerCard_s);
	_SIZE_OF(PCLayout_s);
	_SIZE_OF(HallOfFamePokemon_s);
	_SIZE_OF(HallOfFameEntry_s);
	_SIZE_OF(HallOfFame_s);
	_SIZE_OF(BattleMansion_s);
	_SIZE_OF(PC_s);
	printf("\n");
	_SIZE_OF(Pokedex_XY_s);
	_SIZE_OF(DayCare_XY_s);
	_SIZE_OF(SAV6_XY_s);
	printf("\n");
	_SIZE_OF(Pokedex_ORAS_s);
	_SIZE_OF(DayCare_ORAS_s);
	_SIZE_OF(SAV6_ORAS_s);
	printf("\n");
	_SIZE_OF(BK_s);
	_SIZE_OF(BKLayout_s);
	_SIZE_OF(BKExtra_s);
	_SIZE_OF(BNK6_s);
	printf("\n");
	_SIZE_OF(PersonalInfo);
	printf("\n");
	printf("End of report!\n");
#undef _SIZE_OF
}

int main(void)
{
	Result ret = 0, error = 0;

	sf2d_init();
	sftd_init();
	romfsInit();

	srand(osGetTime());

	// consoleInit(GFX_TOP, NULL); // TODO: Comment it!
	// consoleInit(GFX_BOTTOM, NULL); // TODO: Comment it!

	// sizeof_report();
	// waitKey(KEY_ANY);

#ifdef DEBUG_TOP_CONSOLE
	consoleInit(GFX_TOP, NULL);
#endif
#ifdef DEBUG_BOT_CONSOLE
	consoleInit(GFX_BOTTOM, NULL);
#endif

	printf("> Loading texture manager\n");
	PHBank::Texture = new TextureManager();
	ret = PHBank::Texture->load();
	if (R_FAILED(ret))
	{
		// Graphics
		error |= ERR_GRAPHICS;
	}

	printf("> Loading font manager\n");
	PHBank::Font = new FontManager();
	ret = PHBank::Font->load();
	if (R_FAILED(ret))
	{
		// Font
		error |= ERR_FONT;
	}

	printf("> Loading data manager\n");
	PHBank::Data = new DataManager();
	ret = PHBank::Data->load();
	if (R_FAILED(ret))
	{
		// Data
		error |= ERR_DATA;
	}

#ifdef __cia
	printf("> Starting title selector\n");
	while (!error && TS_Loop())
	{

	// Draw the static loading screen again because of ts.h
	PHBank::Texture->drawStaticLoadingScreen();

	printf("> Loading filesystem services\n");
	ret = FSCIA_Init(titleEntry.titleid, titleEntry.mediatype);
	if (R_FAILED(ret))
	{
		// Filesystem
		error |= ERR_FILESYSTEM;
	}
#else // __3dsx
	printf("> Loading filesystem services\n");
	ret = FS_Init();
	if (R_FAILED(ret))
	{
		// Filesystem
		error |= ERR_FILESYSTEM;
	}
#endif


	printf("> Loading save manager\n");
	PHBank::Save = new SaveManager();
	ret = PHBank::Save->load();
	if (R_FAILED(ret))
	{
		// Save
		error |= ERR_SAVE;
	}

	if (!error)
	{
		printf("Loading successful\n");
		Scene* scene = new BoxBrowser();

		SceneState state = Scene::startMainLoop(scene);

		if (state == SceneState::Saving)
		{
			// TODO Remove when better save display!
			consoleInit(GFX_TOP, NULL);
			printf("Saving...\n");
			// ^

			PHBank::Save->save();
		}
		else
		{
			// TODO Remove when better exit display!
			consoleInit(GFX_TOP, NULL);
			printf("Exiting...\n");
			// ^
		}

		delete scene;
	}

	printf("> Deleting save manager\n");
	delete PHBank::Save;

#ifdef __cia
	printf("> Exiting filesystem services\n");
	FSCIA_Exit();
	consoleExit(GFX_TOP, NULL);
	break; // TODO Remove! The app crash itself after the 2nd ts, unknown cause.
	} // while (TS_LOOP())
#else // __3dsx
	printf("> Exiting filesystem services\n");
	FS_Exit();
#endif

	if (error)
	{
		// TODO Remove when better error display!
		consoleInit(GFX_TOP, NULL);
		// ^

		printf("\nProblem happened: 0x%lx\n", error);
		if (error & ERR_SAVE) printf(" \a Save\n");
		if (error & ERR_DATA) printf(" \a Data\n");
		if (error & ERR_FONT) printf(" \a Font\n");
		if (error & ERR_GRAPHICS) printf(" \a Graphics\n");
		if (error & ERR_FILESYSTEM) printf(" \a Filesystem\n");
		printf("PHBank version: %08x\n", VERSION);
		printf("Can't start the viewer.\n");
		printf("Press any key to exit\n");
		waitKey(KEY_ANY);
	}
	else
	{
		// TODO Remove when better exit display!
		consoleInit(GFX_BOTTOM, NULL);
		// ^

		printf("\nThe app execution ended!\n");
		printf("Thanks for being awesome!\n");
	}

	printf("> Deleting data manager\n");
	delete PHBank::Data;
	printf("> Deleting font manager\n");
	delete PHBank::Font;
	printf("> Deleting texture manager\n");
	delete PHBank::Texture;

	romfsExit();
	sftd_fini();
	sf2d_fini();
	return 0;
}
