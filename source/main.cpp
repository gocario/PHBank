#include <3ds.h> // Useless --v
#include <sf2d.h>
#include <sftd.h>
#include <stdio.h>
#include <stdlib.h>

#include "fs.h"
#include "key.h"
#include "error.h"
#include "version.h"
#include "phbank.hpp"
#include "box_viewer.hpp"

#ifdef __cia
#include "ts.h"
#endif

extern PrintConsole* currentConsole;
/// A very bad implementation for consoleExit, only for debug.
PrintConsole* consoleExit(gfxScreen_t screen, PrintConsole* console)
{
	// TODO Future implementation!
	*currentConsole = *consoleGetDefault();
	gfxSetScreenFormat(screen, GSP_BGR8_OES);
	gfxSetDoubleBuffering(screen, true);
	gspWaitForVBlank();
	return console;
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

	printf("> Loading texture manager\n");
	PHBanku::texture = new TextureManager();
	ret = PHBanku::texture->load();
	if (R_FAILED(ret))
	{
		// Graphics
		error |= ERR_GRAPHICS;
	}

	printf("> Loading font manager\n");
	PHBanku::font = new FontManager();
	ret = PHBanku::font->load();
	if (R_FAILED(ret))
	{
		// Font
		error |= ERR_FONT;
	}

	printf("> Loading data manager\n");
	PHBanku::data = new DataManager();
	ret = PHBanku::data->load();
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
	PHBanku::texture->drawStaticLoadingScreen();

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
	PHBanku::save = new SaveManager();
	ret = PHBanku::save->load();
	if (R_FAILED(ret))
	{
		// Save
		error |= ERR_SAVE;
	}

	if (!error)
	{
		printf("> Starting box viewer...\n");
		Viewer* viewer = new BoxViewer();

		ViewState state = Viewer::startMainLoop(viewer);

		if (state == ViewState::Saving)
		{
			// TODO Remove when better save display!
			consoleInit(GFX_TOP, NULL);
			printf("Saving...\n");
			// ^

			PHBanku::save->save();
		}
		else
		{
			// TODO Remove when better exit display!
			consoleInit(GFX_TOP, NULL);
			printf("Exiting...\n");
			// ^
		}

		delete viewer;
	}

	delete PHBanku::save;

#ifdef __cia
	FSCIA_Exit();
	consoleExit(GFX_TOP, NULL);
	break; // TODO Remove! The app crash itself after the 2nd ts, unknown cause.
	} // while (TS_LOOP())
#else // __3dsx
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

	delete PHBanku::data;
	delete PHBanku::font;
	delete PHBanku::texture;

	romfsExit();
	sftd_fini();
	sf2d_fini();
	return 0;
}
