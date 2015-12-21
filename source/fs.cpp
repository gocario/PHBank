// #include <3ds.h>
#include <3ds/services/fs.h>
#include <3ds/result.h>
#include <3ds/srv.h>
#include <3ds/svc.h>
#include <stdio.h>
#include <string.h>

#include "fs.h"

Result fsInitialized;
Handle sdmcHandle;
FS_Archive sdmcArchive;
Handle saveHandle;
FS_Archive saveArchive;

Result _srvGetServiceHandle(Handle* out, const char* name)
{
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = 0x50100;
	strcpy((char*) &cmdbuf[1], name);
	cmdbuf[3] = strlen(name);
	cmdbuf[4] = 0x0;

	Result ret  = 0;

	ret = svcSendSyncRequest(*srvGetSessionHandle());
	if (R_FAILED(ret)) return ret;

	*out = cmdbuf[3];
	return cmdbuf[1];
}


Result FSUSER_ControlArchive(FS_Archive archive)
{
	u32* cmdbuf = getThreadCommandBuffer();

	u32 b1 = 0, b2 = 0;

	cmdbuf[0] = 0x080d0144;
	cmdbuf[1] = archive.handleLow;
	cmdbuf[2] = archive.handleHigh;
	cmdbuf[3] = 0x0;
	cmdbuf[4] = 0x1; //buffer1 size
	cmdbuf[5] = 0x1; //buffer1 size
	cmdbuf[6] = 0x1a;
	cmdbuf[7] = (u32)&(b1);
	cmdbuf[8] = 0x1c;
	cmdbuf[9] = (u32)&(b2);

	Result ret;

	ret = svcSendSyncRequest(archive.id);
	if (R_FAILED(ret)) return ret;

	return cmdbuf[1];
}


Result FS_ReadFile(char* path, void* dst, FS_Archive* archive, u64 maxSize, u32* bytesRead)
{
	if (!path || !dst || !archive) return -1;

	Result ret;
	u64 size;
	Handle fileHandle;

	// printf("FS_ReadFile:\n");

	ret = FSUSER_OpenFile(&fileHandle, *archive, fsMakePath(PATH_ASCII, path), FS_OPEN_READ, FS_ATTRIBUTE_DIRECTORY);
	// printf(" > FSUSER_OpenFile: %li\n", ret);

	if (R_SUCCEEDED(ret))
	{
		ret = FSFILE_GetSize(fileHandle, &size);
		// printf(" > FSFILE_GetSize: %li\n", ret);
		if (R_FAILED(ret) || size > maxSize) ret = -2;
	}

	if (R_SUCCEEDED(ret))
	{
		ret = FSFILE_Read(fileHandle, bytesRead, 0, dst, size);
		// printf(" > FSFILE_Read: %li\n", ret);
		if (R_FAILED(ret) || *bytesRead < size) ret = -3;
	}

	FSFILE_Close(fileHandle);
	// printf(" > FSFILE_Close: %li\n", ret);

	return ret;
}


Result FS_WriteFile(char* path, void* src, u64 size, FS_Archive* archive, u32* bytesWritten)
{
	if (!path || !src || !archive) return -1;

	Result ret;
	Handle fileHandle;

	// printf("FS_WriteFile:\n");

	ret = FSUSER_OpenFile(&fileHandle, *archive, fsMakePath(PATH_ASCII, path), FS_OPEN_WRITE | FS_OPEN_CREATE, FS_ATTRIBUTE_DIRECTORY);
	// printf(" > FSUSER_OpenFile: %li\n", ret);

	if (R_SUCCEEDED(ret))
	{
		ret = FSFILE_Write(fileHandle, bytesWritten, 0L, src, size, FS_WRITE_FLUSH);
		// printf(" > FSFILE_Write: %li\n", ret);
		if (R_FAILED(ret)) return ret;
	}

	FSFILE_Close(fileHandle);
	// printf(" > FSFILE_Close: %li\n", ret);

	return ret;
}


Result FS_WriteSFile(char* path, void* src, u64 size, FS_Archive* archive, u32* bytesWritten)
{
	Result ret;

	// printf("FS_WriteSFile:\n");

	ret = FS_WriteFile(path, src, size, archive, bytesWritten);
	// printf(" > FS_saveFile: %li\n", ret);
	if (R_FAILED(ret)) return ret;

	ret = FSUSER_ControlArchive(*archive);
	// printf(" > FSUSER_ControlArchive: %li\n", ret);

	return ret;
}


Result FS_DeleteFile(char* path, FS_Archive* archive)
{
	if (!path || !archive) return -1;

	Result ret;

	// printf("FS_DeleteFile:\n");

	ret = FSUSER_DeleteFile(*archive, fsMakePath(PATH_ASCII, path));
	// printf(" > FSUSER_DeleteFile: %li\n", ret);

	return ret;
}


Result FS_CreateDirectory(char* path, FS_Archive* archive)
{
	if (!path || !archive) return -1;

	Result ret;

	// printf("FS_CreateDirectory:\n");

	ret = FSUSER_CreateDirectory(*archive, fsMakePath(PATH_ASCII, path), FS_ATTRIBUTE_DIRECTORY);
	// printf(" > FSUSER_CreateDirectory: %li\n", ret);

	return ret;
}


Result FS_filesysInit()
{
	Result ret = 0;

	// printf("FS_filesysInit:\n");

	if (fsInitialized == 0)
	{

		ret = srvGetServiceHandle(&sdmcHandle, "fs:USER");
		// printf(" > srvGetServiceHandle: %li\n", ret);
		if (R_FAILED(ret)) return ret;

		sdmcArchive = (FS_Archive) { ARCHIVE_SDMC, (FS_Path) { PATH_EMPTY, 1, (u8*) "" }, sdmcHandle };
		ret = FSUSER_OpenArchive(&sdmcArchive);
		// printf(" > FSUSER_OpenArchive: %li\n", ret);
		if (R_FAILED(ret)) return ret;

		ret = _srvGetServiceHandle(&saveHandle, "fs:USER");
		// printf(" > _srvGetServiceHandle: %li\n", ret);
		if (R_FAILED(ret)) return ret;

		ret = FSUSER_Initialize(saveHandle);
		// printf(" > FSUSER_Initialize: %li\n", ret);
		if (R_FAILED(ret)) return ret;

		saveArchive = (FS_Archive) { ARCHIVE_SAVEDATA, (FS_Path) { PATH_EMPTY, 0, NULL }, saveHandle };
		ret = FSUSER_OpenArchive(&saveArchive);
		// printf(" > FSUSER_OpenArchive: %li\n", ret);
		if (R_FAILED(ret)) return ret;

		fsInitialized = 1;
	}

	return ret;
}


Result FS_filesysExit()
{
	Result ret = 0;

	// printf("FS_filesysExit:\n");

	if (fsInitialized == 1)
	{
		ret = FSUSER_CloseArchive(&sdmcArchive);
		ret = svcCloseHandle(sdmcHandle);

		ret = FSUSER_CloseArchive(&saveArchive);
		ret = svcCloseHandle(saveHandle);

		fsInitialized = 0;
	}

	return ret;
}
