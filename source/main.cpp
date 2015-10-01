#include <3ds.h>
#include <stdio.h>
#include <string.h>

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
	PrintConsole top, bot;

	gfxInitDefault();
	gfxSetDoubleBuffering(GFX_BOTTOM, false);

	//Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one
	consoleInit(GFX_TOP, &top);
	consoleInit(GFX_BOTTOM, &bot);


	consoleSelect(&top);

	//Move the cursor to row 15 and column 19 and then prints "Hello World!"
	//To move the cursor you have to print "\x1b[r;cH", where r and c are respectively
	//the row and column where you want your cursor to move
	//The top screen has 30 rows and 50 columns
	//The bottom screen has 30 rows and 40 columns
	printf("PKBank:\n");
	printf(" > Press A to initialize\n");
	printf(" > Press B to start state\n");
	//printf(" > Press B delete manager\n");
	printf(" > Press X to load data\n");
	printf(" > Press Y to save data\n");
	printf(" > Press Select to clean console\n");
	printf(" > Press Start to exit\n");


	// PKBank* pkBank = new PKBank();
	// mainLoop(pkBank, &top, &bot);
	// cdelete(pkBank);
	// gfxExit();
	// return 0;


	Handle sdHandle, saveHandle;
	FS_archive sdArchive, saveArchive;
	printf("Initializing FileSystem\n");
	Result fs = FS_filesysInit(&sdHandle, &saveHandle, &sdArchive, &saveArchive);

	if (fs)
		printf("Init FS Failed\n");
	else
		printf("Init FS OK\n");


	// PKData::loadData(&sdHandle, &sdArchive);
	// printf("\n\nProgram terminated, press A\n");
	// waitKey(KEY_A);
	// FS_filesysExit(&sdHandle, &saveHandle, &sdArchive, &saveArchive);
	// gfxExit();
	// return 0;


	PKBank* pkBank = NULL;

	pkBank = new PKBank();
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





	u32 kDown;//, kHeld, kUp;
	//touchPosition touch;
	// Main loop
	while (aptMainLoop())
	{
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		kDown = hidKeysDown();
		//kHeld = hidKeysHeld();
		//kUp   = hidKeysUp();
		//hidTouchRead(&touch);
		

		if (kDown & KEY_START) break; // break in order to return to hbmenu

		if (kDown & KEY_SELECT)
		{
			consoleClear();
		}

		if (kDown & KEY_A)
		{
			printf("Creating PkBank...");
			cdelete(pkBank);
			pkBank = new PKBank();
			if (pkBank) printf(" OK\n");
			else printf(" Error\n");
		}

		if (kDown & KEY_B)
		{
			if (pkBank)
			{
				mainLoop(pkBank, &top, &bot);
				break;
			}

			/*
			printf("Destroying PkBank...");
			cdelete(pkBank);
			if (!pkBank) printf(" OK\n");
			else printf(" Error\n");
			*/
		}

		if (kDown & KEY_X)
		{
			if (pkBank)
			{
				printf("Loading PkBank...\n");
				if (pkBank->load(fs, &sdHandle, &saveHandle, &sdArchive, &saveArchive))
					printf("Loading PkBank... Error\n");
				else
					printf("Loading PkBank... OK\n");
			}
		}

		if (kDown & KEY_Y)
		{
			if (pkBank)
			{
				printf("Saving PkBank...\n");
				if (pkBank->save(fs, &sdHandle, &saveHandle, &sdArchive, &saveArchive))
					printf("Saving PkBank... Error\n");
				else
					printf("Saving PkBank... OK\n");
			}
		}

		// Flush and swap framebuffers
		gfxFlushBuffers();
		//gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
	}
	
	cdelete(pkBank);

	FS_filesysExit(&sdHandle, &saveHandle, &sdArchive, &saveArchive);
	gfxExit();
	return 0;
}