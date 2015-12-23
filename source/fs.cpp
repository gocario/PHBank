#include <3ds.h>
// #include <3ds/services/fs.h>
// #include <3ds/result.h>
// #include <3ds/srv.h>
// #include <3ds/svc.h>
#include <stdio.h>
#include <string.h>

#include "fs.h"

// #define DEBUG_FS

Result fsInitialized = UNINITIALIZED;
Handle sdmcHandle;
FS_Archive sdmcArchive;
Handle saveHandle;
FS_Archive saveArchive;


Result _srvGetServiceHandle(Handle* out, const char* name)
{
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = 0x50100; // IPC_MakeHeader(0x5, 4, 0);
	strncpy((char*) &cmdbuf[1], name, 8);
	cmdbuf[3] = strlen(name);
	cmdbuf[4] = 0x0;

	Result rc = svcSendSyncRequest(*srvGetSessionHandle());
	if (R_FAILED(rc)) return rc;

	*out = cmdbuf[3];
	return cmdbuf[1];
}


Result FSUSER_ControlArchive(FS_Archive archive)
{
	u32* cmdbuf = getThreadCommandBuffer();

	u32 b1 = 0, b2 = 0;

	cmdbuf[0] = 0x080d0144; // IPC_MakeHeader(0x80D, 5, 4);
	cmdbuf[1] = (u32) archive.handle;
	cmdbuf[2] = (u32) (archive.handle >> 32);
	cmdbuf[3] = 0x0;
	cmdbuf[4] = 0x1; //buffer1 size
	cmdbuf[5] = 0x1; //buffer2 size
	cmdbuf[6] = 0x1a; // IPC_Desc_Buffer(0x1, IPC_BUFFER_R)
	cmdbuf[7] = (u32)&(b1);
	cmdbuf[8] = 0x1c; // IPC_Desc_Buffer(0x1, IPC_BUFFER_W)
	cmdbuf[9] = (u32)&(b2);

	Result rc = svcSendSyncRequest(archive.id);
	if (R_FAILED(rc)) return rc;

	return cmdbuf[1];
}


Result FS_ReadFile(char* path, void* dst, FS_Archive* archive, u64 maxSize, u32* bytesRead)
{
	if (!path || !dst || !archive) return -1;

	Result ret;
	u64 size;
	Handle fileHandle;

#ifdef DEBUG_FS
	printf("FS_ReadFile:\n");
#endif

	ret = FSUSER_OpenFile(&fileHandle, *archive, fsMakePath(PATH_ASCII, path), FS_OPEN_READ, FS_ATTRIBUTE_DIRECTORY);
#ifdef DEBUG_FS
	printf(" > FSUSER_OpenFile: %li\n", ret);
#endif

	if (R_SUCCEEDED(ret))
	{
		ret = FSFILE_GetSize(fileHandle, &size);
#ifdef DEBUG_FS
		printf(" > FSFILE_GetSize: %li\n", ret);
#endif
		if (R_FAILED(ret) || size > maxSize) ret = -2;
	}

	if (R_SUCCEEDED(ret))
	{
		ret = FSFILE_Read(fileHandle, bytesRead, 0, dst, size);
#ifdef DEBUG_FS
		printf(" > FSFILE_Read: %li\n", ret);
#endif
		if (R_FAILED(ret) || *bytesRead < size) ret = -3;
	}

	FSFILE_Close(fileHandle);
#ifdef DEBUG_FS
	printf(" > FSFILE_Close: %li\n", ret);
#endif

	return ret;
}


Result FS_WriteFile(char* path, void* src, u64 size, FS_Archive* archive, u32* bytesWritten)
{
	if (!path || !src || !archive) return -1;

	Result ret;
	Handle fileHandle;

#ifdef DEBUG_FS
	printf("FS_WriteFile:\n");
#endif

	ret = FSUSER_OpenFile(&fileHandle, *archive, fsMakePath(PATH_ASCII, path), FS_OPEN_WRITE | FS_OPEN_CREATE, FS_ATTRIBUTE_DIRECTORY);
#ifdef DEBUG_FS
	printf(" > FSUSER_OpenFile: %li\n", ret);
#endif

	if (R_SUCCEEDED(ret))
	{
		ret = FSFILE_Write(fileHandle, bytesWritten, 0L, src, size, FS_WRITE_FLUSH);
#ifdef DEBUG_FS
		printf(" > FSFILE_Write: %li\n", ret);
#endif
		if (R_FAILED(ret)) return ret;
	}

	FSFILE_Close(fileHandle);
#ifdef DEBUG_FS
	printf(" > FSFILE_Close: %li\n", ret);
#endif

	return ret;
}


Result FS_WriteSFile(char* path, void* src, u64 size, FS_Archive* archive, u32* bytesWritten)
{
	Result ret;

#ifdef DEBUG_FS
	printf("FS_WriteSFile:\n");
#endif

	ret = FS_WriteFile(path, src, size, archive, bytesWritten);
#ifdef DEBUG_FS
	printf(" > FS_WriteFile: %li\n", ret);
#endif
	if (R_FAILED(ret)) return ret;

	ret = FSUSER_ControlArchive(*archive);
#ifdef DEBUG_FS
	printf(" > FSUSER_ControlArchive: %li\n", ret);
#endif

	return ret;
}


Result FS_DeleteFile(char* path, FS_Archive* archive)
{
	if (!path || !archive) return -1;

	Result ret;

#ifdef DEBUG_FS
	printf("FS_DeleteFile:\n");
#endif

	ret = FSUSER_DeleteFile(*archive, fsMakePath(PATH_ASCII, path));
#ifdef DEBUG_FS
	printf(" > FSUSER_DeleteFile: %li\n", ret);
#endif

	return ret;
}


Result FS_DeleteSFile(char* path, FS_Archive* archive)
{
	if (!path || !archive) return -1;

	Result ret;

#ifdef DEBUG_FS
	printf("FS_DeleteSFile:\n");
#endif

	ret = FS_DeleteFile(path, archive);
#ifdef DEBUG_FS
	printf(" > FS_DeleteFile: %li\n", ret);
#endif
	if (R_FAILED(ret)) return ret;

	ret = FSUSER_ControlArchive(*archive);
#ifdef DEBUG_FS
	printf(" > FSUSER_ControlArchive: %li\n", ret);
#endif

	return ret;

}


Result FS_CreateDirectory(char* path, FS_Archive* archive)
{
	if (!path || !archive) return -1;

	Result ret;

#ifdef DEBUG_FS
	printf("FS_CreateDirectory:\n");
#endif

	ret = FSUSER_CreateDirectory(*archive, fsMakePath(PATH_ASCII, path), FS_ATTRIBUTE_DIRECTORY);
#ifdef DEBUG_FS
	printf(" > FSUSER_CreateDirectory: %li\n", ret);
#endif

	return ret;
}


Result FS_FilesysInit()
{
	Result ret = 0;

// #ifdef DEBUG_FS
	printf("FS_filesysInit:\n");
// #endif

	if (!(fsInitialized & INITIALIZED))
	{
		fsInitialized &= INITIALIZING;

		ret = srvGetServiceHandle(&sdmcHandle, "fs:USER");
// #ifdef DEBUG_FS
		printf(" > srvGetServiceHandle: %li\n", ret);
// #endif
		if (R_FAILED(ret)) return ret;

		sdmcArchive = (FS_Archive) { ARCHIVE_SDMC, (FS_Path) { PATH_EMPTY, 1, (u8*) "" }, sdmcHandle };
		ret = FSUSER_OpenArchive(&sdmcArchive);
// #ifdef DEBUG_FS
		printf(" > FSUSER_OpenArchive: %li\n", ret);
// #endif
		if (R_FAILED(ret)) return ret;

		fsInitialized &= SDMC_INITIALIZED;

		ret = _srvGetServiceHandle(&saveHandle, "fs:USER");
// #ifdef DEBUG_FS
		printf(" > _srvGetServiceHandle: %li\n", ret);
// #endif
		if (R_FAILED(ret)) return ret;

		ret = FSUSER_Initialize(saveHandle);
// #ifdef DEBUG_FS
		printf(" > FSUSER_Initialize: %li\n", ret);
// #endif
		if (R_FAILED(ret)) return ret;

		saveArchive = (FS_Archive) { ARCHIVE_SAVEDATA, (FS_Path) { PATH_EMPTY, 0, NULL }, saveHandle };
		ret = FSUSER_OpenArchive(&saveArchive);
// #ifdef DEBUG_FS
		printf(" > FSUSER_OpenArchive: %li\n", ret);
// #endif
		if (R_FAILED(ret)) return ret;

		fsInitialized &= SAVE_INITIALIZED;

		fsInitialized &= INITIALIZED;
		fsInitialized &= ~INITIALIZING;
	}

	return ret;
}


Result FS_FilesysExit()
{
	Result ret = 0;

#ifdef DEBUG_FS
	printf("FS_filesysExit:\n");
#endif

	if (fsInitialized != UNINITIALIZED)
	{
		ret = FSUSER_CloseArchive(&sdmcArchive);
		ret = svcCloseHandle(sdmcHandle);

		ret = FSUSER_CloseArchive(&saveArchive);
		ret = svcCloseHandle(saveHandle);

		fsInitialized = UNINITIALIZED;
	}

	return ret;
}
