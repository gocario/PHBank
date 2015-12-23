#include <3ds.h> // Useless --v
#include <sf2d.h> // Useless --v
#include <sftd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "fs.h"
#include "main.hpp"
#include "phbank.hpp"
#include "box_viewer.hpp"
// #include "pkdata.hpp"

void waitKey(u32 keyWait)
{
	while (aptMainLoop())
	{
		hidScanInput();

		if (hidKeysDown() & keyWait || (hidKeysHeld() & KEY_L && hidKeysHeld() & KEY_R & hidKeysHeld() & KEY_A)) break;

		gfxFlushBuffers();
		gspWaitForVBlank();
	}
}


void sftd_draw_text_pkm(const u16 x, const u16 y, const char* text, ... )
{
	char buffer[256];

	va_list args;
	va_start(args, text);
	vsnprintf(buffer, 256, text, args);
	sftd_draw_text(PHBanku::font->font, x+1, y+1, RGBA8(0x00, 0x00, 0x00, 0xAF), 12, buffer);
	sftd_draw_text(PHBanku::font->font, x, y, RGBA8(0xFF, 0xFF, 0xFF, 0xFF), 12, buffer);
	va_end(args);
}

void sftd_draw_text_white(const u16 x, const u16 y, const char* text, ... )
{
	char buffer[256];

	va_list args;
	va_start(args, text);
	vsnprintf(buffer, 256, text, args);
	sftd_draw_text(PHBanku::font->font, x, y, RGBA8(0xFF, 0xFF, 0xFF, 0xFF), 12, buffer);
	va_end(args);
}

void sftd_draw_text_black(const u16 x, const u16 y, const char* text, ... )
{
	char buffer[256];

	va_list args;
	va_start(args, text);
	vsnprintf(buffer, 256, text, args);
	sftd_draw_text(PHBanku::font->font, x, y, RGBA8(0x00, 0x00, 0x00, 0xFF), 12, buffer);
	va_end(args);
}

// void sftd_draw_wtext_white(const u16 x, const u16 y, const wchar_t* text, ... )
// {
// 	wchar_t buffer[256];

// 	va_list args;
// 	va_start(args, text);
// 	vswprintf(buffer, 256, text, args);
// 	sftd_draw_wtext(PHBanku::font->font, x, y, RGBA8(0xFF, 0xFF, 0xFF, 0xFF), 12, buffer);
// 	va_end(args);
// }



int main(int argc, char* argv[])
{
	sf2d_init();
	sf2d_set_clear_color(RGBA8(0x10, 0x10, 0x10, 0xFF));
	sftd_init();

	srand(time(NULL));

	// PrintConsole top;
	// consoleInit(GFX_TOP, NULL);
	// consoleInit(GFX_BOTTOM, NULL);
	// consoleSetWindow(&top, 0, 22, 40, 7);
	// consoleSetWindow(&top, 0, 0, 50, 30);

	printf("Press A to start\n");
	waitKey(KEY_A);

	PHBanku::save = new SaveManager();
	PHBanku::data = new DataManager();
	PHBanku::font = new FontManager();

	Result error = 0;

	printf("Initializing FileSystem\n");
	if (R_FAILED(FS_FilesysInit()))
	{
		printf("Init FS Failed\n");
		error &= -BIT(2);
	}
	else
	{
		printf("Init FS OK\n");
	}

	if (PHBanku::save->load())
	{
		printf("\n\nProblem with the Save Manager,\ncheck the previous logs and press A\n");
		waitKey(KEY_A);
		error &= -BIT(2);
	}

	if (PHBanku::data->load())
	{
		printf("\n\nProblem with the Data Manager,\ncheck the previous logs and press A\n");
		waitKey(KEY_A);
		error &= -BIT(3);
	}

	if (PHBanku::font->load())
	{
		printf("\n\nProblem with the Font Manager,\ncheck the previous logs and press A\n");
		waitKey(KEY_A);
		error &= -BIT(4);
	}


	if (R_SUCCEEDED(error))
	{
		Viewer* viewer = new BoxViewer();

		// consoleClear();
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
		printf("Error: %li\n", error);
	}
	
	// printf("\n\nProgram terminated, press A\n");
	// waitKey(KEY_A);

	delete PHBanku::save;
	delete PHBanku::data;
	delete PHBanku::font;
	
	printf("Deleting FS...\n");
	FS_FilesysExit();
	
	sftd_fini();
	sf2d_fini();
	return 0;
}
