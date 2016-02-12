#include <3ds.h> // Useless --v
#include <sf2d.h>
#include <sftd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "fs.h"
#include "key.h"
#include "version.h"
#include "phbank.hpp"
#include "box_viewer.hpp"

#ifdef __cia
#include "ts.h"
#endif

/*
extern "C"
{
	void __attribute__((weak)) __appInit(void)
	{
		srvInit();
		aptInit();
		hidInit();

		fsInit();
		sdmcInit();
	}

	void __attribute__((weak)) __appExit(void)
	{
		sdmcExit();
		fsExit();
		
		hidExit();
		aptExit();
		srvExit();
	}
}
*/

extern PrintConsole currentCopy;
extern PrintConsole* currentConsole;

PrintConsole* consoleExit(gfxScreen_t screen, PrintConsole* console)
{
	// TODO Future implementation!
	currentConsole = &currentCopy;
	gfxSetScreenFormat(screen, GSP_BGR8_OES);
	gfxSetDoubleBuffering(screen, true);
	gspWaitForVBlank();
	return console;
}

int main(int argc, char* argv[])
{	
	Result ret = 0, error = 0;

	sf2d_init();
	sftd_init();

#ifdef __cia
	if (!TS_Loop())
	{
		consoleInit(GFX_TOP, NULL);
		goto eof_cia;
	}
	ret = FSCIA_Init(titleEntry.titleid, titleEntry.mediatype);
#else
	ret = FS_Init();
#endif

	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Filesystem services,\nplease check the previous logs\n");
		error |= -BIT(10);
	}
	
	srand(osGetTime());

	// Initialize console;
	// consoleInit(GFX_TOP, NULL);
	// consoleInit(GFX_BOTTOM, NULL);

	// TODO Export the loading screen here?

	// Initialize managers instance

	PHBanku::save = new SaveManager();
	PHBanku::data = new DataManager();
	PHBanku::font = new FontManager();
	PHBanku::texture = new TextureManager();

	// Results values

	// Load managers data

	printf("> Loading texture manager\n");
	ret = PHBanku::texture->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Texture Manager,\nplease check the previous logs\n");
		error |= -BIT(5);
	}

	printf("> Loading font manager\n");
	ret = PHBanku::font->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Font Manager,\nplease check the previous logs\n");
		error |= -BIT(4);
	}

	printf("> Loading data manager\n");
	ret = PHBanku::data->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Data Manager,\nplease check the previous logs\n");
		error |= -BIT(3);
	}

	printf("> Loading save manager\n");
	ret = PHBanku::save->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Save Manager,\nplease check the previous logs\n");
		error |= -BIT(2);
	}

	if (R_SUCCEEDED(error) || error == -1)
	{
		printf("Newing viewer...\n");
		Viewer* viewer = new BoxViewer();
	
		consoleExit(GFX_TOP, NULL);

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
			// TODO Remove when better save display!
			consoleInit(GFX_TOP, NULL);
			printf("Exiting...\n");
			// ^
		}

		printf("Deleting viewer...\n");
		delete viewer;
	}
	else
	{
		// TODO Remove when better error display!
		consoleInit(GFX_TOP, NULL);
		// ^
		
		printf("\nProblem happened: %li\n", error);
		printf("PHBank version: %x\n", VERSION);
		printf("Can't start the viewer.\n");
		printf("Press any key to exit\n");
		waitKey(KEY_ANY);
	}

	printf("Deleting Managers...\n");
	delete PHBanku::save;
	delete PHBanku::data;
	delete PHBanku::font;
	delete PHBanku::texture;

#ifdef __cia
	FSCIA_Exit();
eof_cia:
	printf("\nYou can close that app now.\n");
	printf("Pressing any key will crash the app!\n");
	waitKey(KEY_ANY);
#else
	FS_Exit();
#endif

	sftd_fini();
	sf2d_fini();
	return 0;
}
