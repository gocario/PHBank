#include "fs.h"

#include <3ds/services/fs.h>
#include <3ds/result.h>
#include <3ds/ipc.h>
#include <3ds/srv.h>
#include <3ds/svc.h>

#include <string.h>

// #define DEBUG_FS

#ifdef DEBUG_FS
#include <stdio.h>
#define debug_print(fmt, args ...) printf(fmt, ##args)
#else
#define debug_print(fmt, args ...)
#endif

typedef enum {
	STATE_UNINITIALIZED,
	STATE_UNINITIALIZING,
	STATE_INITIALIZING,
	STATE_INITIALIZED,
} FS_State;

static Handle fsHandle;
static FS_State fsState = STATE_UNINITIALIZED;
static bool saveInitialized = false;
FS_Archive saveArchive;


bool FS_IsInitialized(void)
{
	return (fsState == STATE_INITIALIZED);
}


Result FS_ReadFile(char* path, void* dst, FS_Archive* archive, u64 maxSize, u32* bytesRead)
{
	if (!path || !dst || !archive) return -1;
	
	Result ret;
	u64 size;
	Handle fileHandle;

	debug_print("FS_ReadFile:\n");

	ret = FSUSER_OpenFile(&fileHandle, *archive, fsMakePath(PATH_ASCII, path), FS_OPEN_READ, FS_ATTRIBUTE_NONE);
	debug_print(" > FSUSER_OpenFile: %lx\n", ret);
	if (R_FAILED(ret)) return ret;

	ret = FSFILE_GetSize(fileHandle, &size);
	debug_print(" > FSFILE_GetSize: %lx\n", ret);
	if (R_FAILED(ret) || size > maxSize) ret = -2;

	if (R_SUCCEEDED(ret))
	{
		ret = FSFILE_Read(fileHandle, bytesRead, 0x0, dst, size);
		debug_print(" > FSFILE_Read: %lx\n", ret);
		if (R_FAILED(ret) || *bytesRead < size) ret = -3;
	}

	ret = FSFILE_Close(fileHandle);
	debug_print(" > FSFILE_Close: %lx\n", ret);

	return ret;
}


Result FS_WriteFile(char* path, void* src, u64 size, FS_Archive* archive, u32* bytesWritten)
{
	if (!path || !src || !archive) return -1;

	Result ret;
	Handle fileHandle;

	debug_print("FS_WriteFile:\n");

	ret = FSUSER_OpenFile(&fileHandle, *archive, fsMakePath(PATH_ASCII, path), FS_OPEN_WRITE | FS_OPEN_CREATE, FS_ATTRIBUTE_NONE);
	debug_print(" > FSUSER_OpenFile: %lx\n", ret);
	if (R_FAILED(ret)) return ret;

	if (R_SUCCEEDED(ret))
	{
		ret = FSFILE_Write(fileHandle, bytesWritten, 0L, src, size, FS_WRITE_FLUSH);
		debug_print(" > FSFILE_Write: %lx\n", ret);
		if (R_FAILED(ret) || *bytesWritten != size) ret = -2;
	}

	ret = FSFILE_Close(fileHandle);
	debug_print(" > FSFILE_Close: %lx\n", ret);

	return ret;
}


Result FS_DeleteFile(char* path, FS_Archive* archive)
{
	if (!path || !archive) return -1;
	
	Result ret;

	debug_print("FS_DeleteFile:\n");

	ret = FSUSER_DeleteFile(*archive, fsMakePath(PATH_ASCII, path));
	debug_print(" > FSUSER_DeleteFile: %lx\n", ret);

	return ret;
}


Result FS_CreateDirectory(char* path, FS_Archive* archive)
{
	if (!path || !archive) return -1;
	
	Result ret;

	debug_print("FS_CreateDirectory:\n");

	ret = FSUSER_CreateDirectory(*archive, fsMakePath(PATH_ASCII, path), FS_ATTRIBUTE_DIRECTORY);
	debug_print(" > FSUSER_CreateDirectory: %lx\n", ret);

	return ret;
}


Result FS_CommitArchive(FS_Archive* archive)
{
	Result ret = 0;

	debug_print("FS_CommitArchive:\n");

	if (saveInitialized)
	{
		ret = FSUSER_ControlArchive(*archive, ARCHIVE_ACTION_COMMIT_SAVE_DATA, NULL, 0, NULL, 0);
		debug_print(" > FSUSER_ControlArchive: %lx\n", ret);
	}

	return ret;
}


Result FS_fsInit(void)
{
	Result ret = 0;
	fsState = STATE_INITIALIZING;

	debug_print("FS_fsInit:\n");

	ret = srvGetServiceHandleDirect(&fsHandle, "fs:USER");
	debug_print(" > _srvGetServiceHandle: %lx\n", ret);
	if (R_FAILED(ret)) return ret;

	ret = FSUSER_Initialize(fsHandle);
	debug_print(" > FSUSER_Initialize: %lx\n", ret);
	if (R_FAILED(ret)) return ret;

	fsUseSession(fsHandle, false);
	debug_print(" > fsUseSession\n");

	if (!saveInitialized)
	{
		saveArchive = (FS_Archive) { ARCHIVE_SAVEDATA, fsMakePath(PATH_EMPTY, NULL), fsHandle };

		ret = FSUSER_OpenArchive(&saveArchive);
		debug_print(" > FSUSER_OpenArchive: %lx\n", ret);

		saveInitialized = R_SUCCEEDED(ret);
	}

	if (saveInitialized)
	{
		fsState = STATE_INITIALIZED;
	}

	return ret;
}


Result FS_fsExit(void)
{
	Result ret = 0;
	fsState = STATE_UNINITIALIZING;
	
	debug_print("FS_fsExit:\n");

	if (saveInitialized)
	{
		ret = FS_CommitArchive(&saveArchive);
		debug_print(" > FS_CommitArchive: %lx\n", ret);

		ret = FSUSER_CloseArchive(&saveArchive);
		debug_print(" > FSUSER_CloseArchive: %lx\n", ret);
		saveInitialized = false;
	}

	fsEndUseSession();
	debug_print(" > fsEndUseSession\n");

	ret = svcCloseHandle(fsHandle);
	debug_print(" > _srvGetServiceHandle: %lx\n", ret);

	if (!saveInitialized)
	{
		fsState = STATE_UNINITIALIZED;
	}

	return ret;
}
