///
/// From smealum/3ds_hb_menu
/// Modified
///

#include "filesystem.hpp"

Result _srvGetServiceHandle(Handle* out, const char* name)
{
	Result rc = 0;

	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = 0x50100;
	strcpy((char*) &cmdbuf[1], name);
	cmdbuf[3] = strlen(name);
	cmdbuf[4] = 0x0;

	if ((rc = svcSendSyncRequest(*srvGetSessionHandle()))) return rc;

	*out = cmdbuf[3];
	return cmdbuf[1];
}

Result loadFile(char* path, void* dst, FS_archive* fsArchive, Handle* fsHandle, u64 maxSize, u32* bytesRead)
{
	if (!path || !dst || !fsArchive) return -1;

	Result ret;
	u64 size;
	Handle fileHandle;

	ret = FSUSER_OpenFile(fsHandle, &fileHandle, *fsArchive, FS_makePath(PATH_CHAR, path), FS_OPEN_READ, FS_ATTRIBUTE_NONE);
	if (ret) return ret;

	ret = FSFILE_GetSize(fileHandle, &size);
	if (ret) goto loadFileExit;
	if (size > maxSize) { ret=-2; goto loadFileExit; }

	ret = FSFILE_Read(fileHandle, bytesRead, 0x0, dst, size);
	if (ret) goto loadFileExit;
	if (*bytesRead < size) { ret=-3; goto loadFileExit; }

	loadFileExit:
	FSFILE_Close(fileHandle);
	return ret;
}

Result saveFile(char* path, void* src, u64 size, FS_archive* fsArchive, Handle* fsHandle, u32* bytesWritten)
{
	if (!path || !src || !fsArchive) return -1;

	Result ret;
	Handle fileHandle;

	ret = FSUSER_OpenFile(fsHandle, &fileHandle, *fsArchive, FS_makePath(PATH_CHAR, path), FS_OPEN_WRITE | FS_OPEN_CREATE, FS_ATTRIBUTE_NONE);
	if (ret) return ret;

	ret = FSFILE_Write(fileHandle, bytesWritten, 0x0, src, size, FS_WRITE_NOFLUSH);

	FSFILE_Close(fileHandle);
	return ret;
}

Result deleteFile(char* path, Handle* fsHandle, FS_archive* fsArchive)
{
	if (!path || !fsHandle || !fsArchive) return -1;

	Result ret;

	ret = FSUSER_DeleteFile(fsHandle, *fsArchive, FS_makePath(PATH_CHAR, path));

	return ret;
}


Result createDirectory(char* path, Handle* fsHandle, FS_archive* fsArchive)
{
	if (!path || !fsHandle || !fsArchive) return -1;

	Result ret;

	ret = FSUSER_CreateDirectory(fsHandle, *fsArchive, FS_makePath(PATH_CHAR, path));

	return ret;
}


Result filesysInit(Handle* sdHandle, Handle* saveHandle, FS_archive* sdArchive, FS_archive* saveArchive)
{
	Result ret;
	printf("  Getting save handle\n");
	ret = _srvGetServiceHandle(saveHandle, "fs:USER");
	if (ret) return ret;

	printf("  Initializing save handle\n");
	ret = FSUSER_Initialize(saveHandle);
	if (ret) return ret;

	printf("  Opening save archive\n");
	*saveArchive = (FS_archive){0x4, (FS_path){PATH_EMPTY, 0, NULL}, 0, 0};
	ret = FSUSER_OpenArchive(saveHandle, saveArchive);
	if (ret) return ret;

	printf("  Getting SD Card handle\n");
	ret = srvGetServiceHandle(sdHandle, "fs:USER");
	if (ret) return ret;

	printf("  Opening SD Card archive\n");
	*sdArchive = (FS_archive){0x9, (FS_path){PATH_EMPTY, 1, (u8*)""}, 0, 0};
	ret = FSUSER_OpenArchive(sdHandle, sdArchive);
	return ret;
}

Result filesysExit(Handle* sdHandle, Handle* saveHandle, FS_archive* sdArchive, FS_archive* saveArchive)
{
	FSUSER_CloseArchive(saveHandle, saveArchive);
	FSUSER_CloseArchive(sdHandle, sdArchive);
	svcCloseHandle(*saveHandle);
	svcCloseHandle(*sdHandle);
	return 0;
}