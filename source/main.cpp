#include <3ds.h>
#include <sf2d.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "main.hpp"
#include "phbank.hpp"
#include "filesystem.hpp"
#include "box_viewer.hpp"
#include "viewer.hpp"

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

int main(int argc, char* argv[])
{
	sf2d_init();
	sf2d_set_clear_color(RGBA8(0x10, 0x10, 0x10, 0xFF));

	PrintConsole top;
	consoleInit(GFX_TOP, &top);
	// consoleInit(GFX_BOTTOM, NULL);
	consoleSetWindow(&top, 0, 22, 40, 7);

	Handle sdHandle, saveHandle;
	FS_archive sdArchive, saveArchive;
	printf("Initializing FileSystem\n");
	Result fs = FS_filesysInit(&sdHandle, &saveHandle, &sdArchive, &saveArchive);
	if (fs) printf("Init FS Failed\n");
	else printf("Init FS OK\n");



	PKData::load(&sdHandle, &sdArchive);
	PHBank::pKBank()->load(fs, &sdHandle, &saveHandle, &sdArchive, &saveArchive);
	
	consoleSetWindow(&top, 0, 0, 50, 30);
	printf("\x1b[2J");
	Result ret = Viewer::startMainLoop(new BoxViewer());

	if (ret == StateView::Saving)
	{
		PHBank::pKBank()->save(fs, &sdHandle, &saveHandle, &sdArchive, &saveArchive);
		printf("\n\nProgram terminated, press A\n");
		waitKey(KEY_A);
	}

	PHBank::destroy();
	FS_filesysExit(&sdHandle, &saveHandle, &sdArchive, &saveArchive);
	sf2d_fini();
	return 0;
}