#include <3ds.h> // Useless --v
#include <sf2d.h>
#include <sftd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "fs.h"
#include "key.h"
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

	srand(osGetTime());

	// Initialize console;
	// consoleInit(GFX_TOP, NULL); // TODO: Comment it!
	// consoleInit(GFX_BOTTOM, NULL); // TODO: Comment it!

	printf("> Loading texture manager\n");
	PHBanku::texture = new TextureManager();
	ret = PHBanku::texture->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Texture Manager: %lx,\n", ret);
		error |= BIT(5);
	}

	printf("> Loading font manager\n");
	PHBanku::font = new FontManager();
	ret = PHBanku::font->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Font Manager: %lx,\n", ret);
		error |= BIT(4);
	}

	printf("> Loading data manager\n");
	PHBanku::data = new DataManager();
	ret = PHBanku::data->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Data Manager: %lx,\n", ret);
		error |= BIT(3);
	}

	printf("> Loading filesystem services\n");

#ifdef __cia
	while (!error && TS_Loop())
	{

	// Draw the static loading screen again because of ts.h
	PHBanku::texture->drawStaticLoadingScreen();

	ret = FSCIA_Init(titleEntry.titleid, titleEntry.mediatype);
#else
	ret = FS_Init();
#endif

	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Filesystem services : %lx,\n", ret);
		error |= BIT(7);
	}

	// Initialize managers instance & load managers data

	printf("> Loading save manager\n");
	PHBanku::save = new SaveManager();
	ret = PHBanku::save->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Save Manager: %lx,\n", ret);
		error |= BIT(2);
	}

	if (!error)
	{
		printf("Newing viewer...\n");
		Viewer* viewer = new BoxViewer();

		// consoleExit(GFX_TOP, NULL); // TODO: Comment it!

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

		printf("Deleting viewer...\n");
		delete viewer;
	}

	printf("Deleting Save Manager...\n");
	delete PHBanku::save;

#ifdef __cia
	FSCIA_Exit();
	consoleExit(GFX_TOP, NULL);
	break;
	} // while (TS_LOOP())

	if (!error)
	{
		// TODO Remove when better exit display!
		consoleInit(GFX_BOTTOM, NULL);
		// ^

		printf("\nThe app execution ended!\n");
		printf("It may crash but that is normal.\n");
	}
#else
	FS_Exit();
#endif

	if (error)
	{
		// TODO Remove when better error display!
		consoleInit(GFX_TOP, NULL);
		// ^

		printf("\nProblem happened: %lx\n", error);
		printf("PHBank version: %08x\n", VERSION);
		printf("Can't start the viewer.\n");
		printf("Press any key to exit\n");
		waitKey(KEY_ANY);
	}

	printf("Deleting Managers...\n");
	delete PHBanku::data;
	delete PHBanku::font;
	delete PHBanku::texture;

	sftd_fini();
	sf2d_fini();
	return 0;
}
