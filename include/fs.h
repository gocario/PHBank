#pragma once
// #ifndef FS_H
// #define FS_H

// #include <3ds.h>
#include <3ds/services/fs.h>

typedef enum
{
	UNINITIALIZED = BIT(0),
	INITIALIZING = BIT(1),
	INITIALIZED = BIT(2),
	SDMC_INITIALIZED = BIT(4),
	SAVE_INITIALIZED = BIT(5),
} FS_State;

extern Result fsInitialized;
extern Handle sdmcHandle;
extern FS_Archive sdmcArchive;
extern Handle saveHandle;
extern FS_Archive saveArchive;

Result FS_ReadFile(char* path, void* dst, FS_Archive* archive, u64 maxSize, u32* bytesRead);
Result FS_WriteFile(char* path, void* src, u64 size, FS_Archive* archive, u32* bytesWritten);
Result FS_WriteSFile(char* path, void* src, u64 size, FS_Archive* archive, u32* bytesWritten);
Result FS_DeleteFile(char* path, FS_Archive* fsarch);
Result FS_DeleteSFile(char* path, FS_Archive* fsarch);

Result FS_CreateDirectory(char* path, FS_Archive* fsArchive);

Result FSUSER_ControlArchive(FS_Archive archive);

Result FS_FilesysInit();
Result FS_FilesysExit();

// #endif // FS_H
