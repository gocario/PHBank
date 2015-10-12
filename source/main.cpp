#include <3ds.h> // Useless --v
#include <sf2d.h> // Useless --v
#include <sftd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "main.hpp"
#include "phbank.hpp"
#include "filesystem.hpp"
#include "box_viewer.hpp"
#include "pkdata.hpp"

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
	sftd_draw_text(PHBank::font(), x+1, y+1, RGBA8(0x00, 0x00, 0x00, 0xAF), 8, buffer);
	sftd_draw_text(PHBank::font(), x, y, RGBA8(0xFF, 0xFF, 0xFF, 0xFF), 8, buffer);
	va_end(args);
}


int main(int argc, char* argv[])
{
	sf2d_init();
	sf2d_set_clear_color(RGBA8(0x10, 0x10, 0x10, 0xFF));
	sftd_init();

	// PrintConsole top;
	// consoleInit(GFX_TOP, NULL);
	// consoleInit(GFX_BOTTOM, NULL);
	// consoleSetWindow(&top, 0, 22, 40, 7);
	// consoleSetWindow(&top, 0, 0, 50, 30);

	Handle sdHandle, saveHandle;
	FS_archive sdArchive, saveArchive;
	printf("Initializing FileSystem\n");
	Result fs = FS_filesysInit(&sdHandle, &saveHandle, &sdArchive, &saveArchive);
	if (fs) printf("Init FS Failed\n");
	else printf("Init FS OK\n");

	PHBank::load();

	srand(time(NULL));

	PKData::load(&sdHandle, &sdArchive);
	Viewer* viewer = new BoxViewer();
	if (!PHBank::pKBank()->load(fs, &sdHandle, &saveHandle, &sdArchive, &saveArchive))
	{
		consoleClear();
		Result ret = Viewer::startMainLoop(viewer);

		if (ret == StateView::Saving)
		{
			PHBank::pKBank()->save(fs, &sdHandle, &saveHandle, &sdArchive, &saveArchive);
		}
	}
	else
	{
		printf("Error, bad target?\n");
	}
	
	// printf("\n\nProgram terminated, press A\n");
	// waitKey(KEY_A);

	printf("Deleting view...\n");
	delete viewer;
	printf("Deleting PHBank...\n");
	PHBank::destroy();
	printf("Deleting FS...\n");
	FS_filesysExit(&sdHandle, &saveHandle, &sdArchive, &saveArchive);
	printf("Deleting SFTD...\n");
	sftd_fini();
	printf("Deleting SF2D...\n");
	sf2d_fini();
	return 0;
}