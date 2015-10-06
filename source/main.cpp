#include <3ds.h>
#include <sf2d.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "main.hpp"
#include "pkbank.hpp"
#include "pkdata.hpp"
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



	PKBank* pkBank = NULL;

	pkBank = new PKBank();
	PKData::load(&sdHandle, &sdArchive);
	pkBank->load(fs, &sdHandle, &saveHandle, &sdArchive, &saveArchive);
	Result ret = mainLoop(pkBank, &top, &bot);
	if (ret == STATE_SAVE)
	{
		consoleSelect(&top);
		pkBank->save(fs, &sdHandle, &saveHandle, &sdArchive, &saveArchive);
		printf("\n\nProgram terminated, press A\n");
		waitKey(KEY_A);
	}

	cdelete(pkBank);
	FS_filesysExit(&sdHandle, &saveHandle, &sdArchive, &saveArchive);
	gfxExit();
	return 0;
}