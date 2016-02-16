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
#define r(fmt, args ...) printf(fmt, ##args)
#else
#define debug_print(fmt, args ...)
#define r(fmt, args ...)
#endif

static bool saveInitialized = false;
FS_Archive saveArchive;

Result FS_ReadFile(const char* path, void* dst, const FS_Archive* archive, u64 maxSize, u32* bytesRead)
{
	if (!path || !dst || !archive || !bytesRead) return -1;

	Result ret;
	u64 size;
	Handle fileHandle;

	debug_print("FS_ReadFile:\n");

	ret = FSUSER_OpenFile(&fileHandle, *archive, fsMakePath(PATH_ASCII, path), FS_OPEN_READ, FS_ATTRIBUTE_NONE);
	r(" > FSUSER_OpenFile: %lx\n", ret);
	if (R_FAILED(ret)) return ret;

	ret = FSFILE_GetSize(fileHandle, &size);
	r(" > FSFILE_GetSize: %lx\n", ret);
	if (R_FAILED(ret) || size > maxSize) ret = -2;

	if (R_SUCCEEDED(ret))
	{
		ret = FSFILE_Read(fileHandle, bytesRead, 0x0, dst, size);
		r(" > FSFILE_Read: %lx\n", ret);
		if (R_FAILED(ret) || *bytesRead < size) ret = -3;
	}

	ret = FSFILE_Close(fileHandle);
	r(" > FSFILE_Close: %lx\n", ret);

	return ret;
}

Result FS_WriteFile(const char* path, const void* src, u64 size, const FS_Archive* archive, u32* bytesWritten)
{
	if (!path || !src || !archive || !bytesWritten) return -1;

	Result ret;
	Handle fileHandle;

	debug_print("FS_WriteFile:\n");

	ret = FSUSER_OpenFile(&fileHandle, *archive, fsMakePath(PATH_ASCII, path), FS_OPEN_WRITE | FS_OPEN_CREATE, FS_ATTRIBUTE_NONE);
	r(" > FSUSER_OpenFile: %lx\n", ret);
	if (R_FAILED(ret)) return ret;

	if (R_SUCCEEDED(ret))
	{
		ret = FSFILE_Write(fileHandle, bytesWritten, 0L, src, size, FS_WRITE_FLUSH);
		r(" > FSFILE_Write: %lx\n", ret);
		if (R_FAILED(ret) || *bytesWritten != size) ret = -2;
	}

	ret = FSFILE_Close(fileHandle);
	r(" > FSFILE_Close: %lx\n", ret);

	return ret;
}

Result FS_DeleteFile(const char* path, const FS_Archive* archive)
{
	if (!path || !archive) return -1;

	Result ret;

	debug_print("FS_DeleteFile:\n");

	ret = FSUSER_DeleteFile(*archive, fsMakePath(PATH_ASCII, path));
	r(" > FSUSER_DeleteFile: %lx\n", ret);

	return ret;
}

Result FS_CreateDirectory(const char* path, const FS_Archive* archive)
{
	if (!path || !archive) return -1;

	Result ret;

	debug_print("FS_CreateDirectory:\n");

	ret = FSUSER_CreateDirectory(*archive, fsMakePath(PATH_ASCII, path), FS_ATTRIBUTE_DIRECTORY);
	r(" > FSUSER_CreateDirectory: %lx\n", ret);

	return ret;
}

Result FS_CommitArchive(const FS_Archive* archive)
{
	if (!archive) return -1;

	Result ret;

	debug_print("FS_CommitArchive:\n");

	ret = FSUSER_ControlArchive(*archive, ARCHIVE_ACTION_COMMIT_SAVE_DATA, NULL, 0, NULL, 0);
	r(" > FSUSER_ControlArchive: %lx\n", ret);

	return ret;
}

#ifdef __cia

static u32 lowPath[3];

Result FSCIA_Init(u64 titleid, FS_MediaType mediatype)
{
	Result ret = 1;

	debug_print("FSCIA_Init:\n");

	if (!saveInitialized)
	{
		lowPath[0] = mediatype;
		lowPath[1] = titleid; /// titleid & 0xFFFFFFFF
		lowPath[2] = titleid >> 32 // (titleid >> 32) & 0xFFFFFFFF

		debug_print(" > [0]: 0x%016lx\n", lowPath[0]);
		debug_print(" > [1]: 0x%016lx\n", lowPath[1]);
		debug_print(" > [2]: 0x%016lx\n", lowPath[2]);

		saveArchive = (FS_Archive) { ARCHIVE_USER_SAVEDATA, (FS_Path) { PATH_BINARY, 12, lowPath } };

		ret = FSUSER_OpenArchive(&saveArchive);
		r(" > FSUSER_OpenArchive: %lx\n", ret);

		saveInitialized = R_SUCCEEDED(ret);
	}

	return ret;
}

Result FSCIA_Exit(void)
{
	Result ret = 1;

	debug_print("FSCIA_Exit:\n");

	if (saveInitialized)
	{
		ret = FS_CommitArchive(&saveArchive);
		r(" > FS_CommitArchive: %lx\n", ret);

		ret = FSUSER_CloseArchive(&saveArchive);
		r(" > FSUSER_CloseArchive: %lx\n", ret);

		saveInitialized = !R_SUCCEEDED(ret);
	}

	return ret;
}

#else

static Handle fsHandle;

Result FS_Init(void)
{
	Result ret = 1;

	debug_print("FS_Init:\n");

	ret = srvGetServiceHandleDirect(&fsHandle, "fs:USER");
	r(" > srvGetServiceHandleDirect: %lx\n", ret);
	if (R_FAILED(ret)) return ret;

	ret = FSUSER_Initialize(fsHandle);
	r(" > FSUSER_Initialize: %lx\n", ret);
	if (R_FAILED(ret)) return ret;

	fsUseSession(fsHandle, false);
	debug_print(" > fsUseSession\n");

	if (!saveInitialized)
	{
		saveArchive = (FS_Archive) { ARCHIVE_SAVEDATA, fsMakePath(PATH_EMPTY, NULL) };

		ret = FSUSER_OpenArchive(&saveArchive);
		r(" > FSUSER_OpenArchive: %lx\n", ret);

		saveInitialized = R_SUCCEEDED(ret);
	}

	return ret;
}

Result FS_Exit(void)
{
	Result ret = 1;

	debug_print("FS_Exit:\n");

	if (saveInitialized)
	{
		ret = FS_CommitArchive(&saveArchive);
		r(" > FS_CommitArchive: %lx\n", ret);

		ret = FSUSER_CloseArchive(&saveArchive);
		r(" > FSUSER_CloseArchive: %lx\n", ret);

		saveInitialized = R_SUCCEEDED(ret);
	}

	fsEndUseSession();
	debug_print(" > fsEndUseSession\n");

	ret = svcCloseHandle(fsHandle);
	r(" > svcCloseHandle: %lx\n", ret);

	return ret;
}

#endif
