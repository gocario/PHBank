#include <3ds.h>
#include <sf2d.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "main.hpp"
#include "phbank.hpp"
#include "filesystem.hpp"
#include "ui.hpp"

void waitKey(u32 keyWait)
{
	while (aptMainLoop())
	{
		hidScanInput();

		if (hidKeysDown() & keyWait) break;

		gfxFlushBuffers();
		gspWaitForVBlank();
	}
}

int main(int argc, char* argv[])
{
	sf2d_init();
	sf2d_set_clear_color(RGBA8(0x10, 0x10, 0x10, 0xFF));


	//Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one
	PrintConsole top, bot;
	consoleInit(GFX_TOP, &top);
	consoleInit(GFX_BOTTOM, &bot);
	consoleSelect(&top);


	Handle sdHandle, saveHandle;
	FS_archive sdArchive, saveArchive;
	printf("Initializing FileSystem\n");
	Result fs = FS_filesysInit(&sdHandle, &saveHandle, &sdArchive, &saveArchive);
	if (fs) printf("Init FS Failed\n");
	else printf("Init FS OK\n");



	PKData::load(&sdHandle, &sdArchive);
	PHBank::pKBank()->load(fs, &sdHandle, &saveHandle, &sdArchive, &saveArchive);
	Result ret = mainLoop(PHBank::pKBank(), &top, &bot);
	if (ret == STATE_SAVE)
	{
		consoleSelect(&top);
		PHBank::pKBank()->save(fs, &sdHandle, &saveHandle, &sdArchive, &saveArchive);
		printf("\n\nProgram terminated, press A\n");
		waitKey(KEY_A);
	}

	PHBank::destroy();
	FS_filesysExit(&sdHandle, &saveHandle, &sdArchive, &saveArchive);
	sf2d_fini();
	return 0;
}