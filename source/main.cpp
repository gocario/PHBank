#include <3ds.h> // Useless --v
#include <sf2d.h>
#include <sftd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "fs.h"
#include "key.h"
#include "phbank.hpp"
#include "box_viewer.hpp"

PrintConsole* consoleExit(gfxScreen_t screen, PrintConsole* console)
{
	// TODO Future implementation!
	gspWaitForVBlank();
	return console;
}

int main(int argc, char* argv[])
{
	sf2d_init();
	sf2d_set_clear_color(RGBA8(0x10, 0x10, 0x10, 0xFF));
	sftd_init();

	FS_fsInit();

	srand(osGetTime());

	// Initialize console;
	consoleInit(GFX_TOP, NULL);
	// consoleInit(GFX_BOTTOM, NULL);

	// TODO Add a loading screen here!

	// Initialize managers instance

	PHBanku::save = new SaveManager();
	PHBanku::data = new DataManager();
	PHBanku::font = new FontManager();
	PHBanku::texture = new TextureManager();

	// Results values

	Result ret = 0, error = 0;

	// Load managers data

	printf("> Loading save manager\n");
	ret = PHBanku::save->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Save Manager,\nplease check the previous logs\n");
		error |= -BIT(2);
	}

	printf("> Loading data manager\n");
	ret = PHBanku::data->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Data Manager,\nplease check the previous logs\n");
		error |= -BIT(3);
	}

	printf("> Loading font manager\n");
	ret = PHBanku::font->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Font Manager,\nplease check the previous logs\n");
		error |= -BIT(4);
	}

	printf("> Loading texture manager\n");
	ret = PHBanku::texture->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Texture Manager,\nplease check the previous logs\n");
		error |= -BIT(5);
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

		printf("Deleting viewer...\n");
		delete viewer;
	}
	else
	{
		printf("\nProblem happened: %li\nCan't start the viewer.\n", error);
		printf("Press any key to exit\n");
		waitKey(KEY_ANY);
	}

	printf("Deleting Managers...\n");
	delete PHBanku::save;
	delete PHBanku::data;
	delete PHBanku::font;
	delete PHBanku::texture;

	FS_fsExit();

	sftd_fini();
	sf2d_fini();
	return 0;
}
