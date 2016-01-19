#include <3ds.h> // Useless --v
#include <sf2d.h>
#include <sftd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "main.hpp"
#include "phbank.hpp"
#include "box_viewer.hpp"

void waitKey(u32 keyWait)
{
	while (aptMainLoop())
	{
		hidScanInput();

		if (hidKeysDown() & keyWait || (hidKeysHeld() & KEY_L && hidKeysHeld() & KEY_R && hidKeysHeld() & KEY_A)) break;

		gfxFlushBuffers();
		gspWaitForVBlank();
	}
}


int main(int argc, char* argv[])
{
	sf2d_init();
	sf2d_set_clear_color(RGBA8(0x10, 0x10, 0x10, 0xFF));
	sftd_init();

	srand(osGetTime());

	// Initialize console;
	// consoleInit(GFX_TOP, NULL);
	// consoleInit(GFX_BOTTOM, NULL);

	// Initialize managers instance

	PHBanku::save = new SaveManager();
	PHBanku::data = new DataManager();
	PHBanku::font = new FontManager();

	// Results values

	Result error = 0;
	Result ret;

	// Load managers data

	ret = PHBanku::save->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Save Manager,\ncheck the previous logs and press A\n");
		waitKey(KEY_A);
		error &= -BIT(2);
	}

	ret = PHBanku::data->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Data Manager,\ncheck the previous logs and press A\n");
		waitKey(KEY_A);
		error &= -BIT(3);
	}

	ret = PHBanku::font->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Font Manager,\ncheck the previous logs and press A\n");
		waitKey(KEY_A);
		error &= -BIT(4);
	}

	if (R_SUCCEEDED(error) || error == -1)
	{
		Viewer* viewer = new BoxViewer();

		Result ret = Viewer::startMainLoop(viewer);

		if (ret == StateView::Saving)
		{
			PHBanku::save->save();
		}

		printf("Deleting viewer...\n");
		delete viewer;
	}
	else
	{
		printf("Problem happened: %li\nCan't start the viewer.\n", error);
		printf("Press A to exit\n");
		waitKey(KEY_A);
	}

	printf("Deleting Managers...\n");
	delete PHBanku::save;
	delete PHBanku::data;
	delete PHBanku::font;

	sftd_fini();
	sf2d_fini();
	return 0;
}
