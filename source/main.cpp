#include <3ds.h> // Useless --v
#include <sf2d.h>
#include <sftd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "key.hpp"
#include "phbank.hpp"
#include "box_viewer.hpp"


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

	Result ret, error = 0;

	// Load managers data

	ret = PHBanku::save->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Save Manager,\nplease check the previous logs\n");
		error |= -BIT(2);
	}

	ret = PHBanku::data->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Data Manager,\nplease check the previous logs\n");
		error |= -BIT(3);
	}

	ret = PHBanku::font->load();
	if (R_FAILED(ret))
	{
		printf("\n\nProblem with the Font Manager,\nplease check the previous logs\n");
		error |= -BIT(4);
	}

	if (R_SUCCEEDED(error) || error == -1)
	{
		printf("Newing viewer...\n");
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
		printf("\nProblem happened: %li\nCan't start the viewer.\n", error);
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
