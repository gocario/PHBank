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

Result FS_loadFile(char* path, void* dst, FS_archive* fsArchive, Handle* fsHandle, u64 maxSize, u32* bytesRead)
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

Result FS_saveFile(char* path, void* src, u64 size, FS_archive* fsArchive, Handle* fsHandle, u32* bytesWritten)
{
	if (!path || !src || !fsArchive) return -1;

	Result ret;
	Handle fileHandle;

	ret = FSUSER_OpenFile(fsHandle, &fileHandle, *fsArchive, FS_makePath(PATH_CHAR, path), FS_OPEN_WRITE | FS_OPEN_CREATE, FS_ATTRIBUTE_NONE);
	if (ret) return ret;

	ret = FSFILE_Write(fileHandle, bytesWritten, 0x0, src, size, FS_WRITE_NOFLUSH);
	if (ret) return ret;

	FSFILE_Close(fileHandle);
	return ret;
}

Result FS_saveSFile(char* path, void* src, u64 size, FS_archive* fsArchive, Handle* fsHandle, u32* bytesWritten)
{
	Result ret;

	ret = FS_saveFile(path, src, size, fsArchive, fsHandle, bytesWritten);
	if (ret) return ret;

	ret = FSUSER_ControlArchive(*fsHandle, *fsArchive);
	return ret;
}

Result FS_deleteFile(char* path, Handle* fsHandle, FS_archive* fsArchive)
{
	if (!path || !fsHandle || !fsArchive) return -1;

	Result ret;

	ret = FSUSER_DeleteFile(fsHandle, *fsArchive, FS_makePath(PATH_CHAR, path));

	return ret;
}


Result FS_createDirectory(char* path, Handle* fsHandle, FS_archive* fsArchive)
{
	if (!path || !fsHandle || !fsArchive) return -1;

	Result ret;

	ret = FSUSER_CreateDirectory(fsHandle, *fsArchive, FS_makePath(PATH_CHAR, path));

	return ret;
}


Result FSUSER_ControlArchive(Handle handle, FS_archive archive)
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

	Result ret = 0;
	if ((ret=svcSendSyncRequest(handle))) return ret;

	return cmdbuf[1];
}

Result FS_filesysInit(Handle* sdHandle, Handle* saveHandle, FS_archive* sdArchive, FS_archive* saveArchive)
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

Result FS_filesysExit(Handle* sdHandle, Handle* saveHandle, FS_archive* sdArchive, FS_archive* saveArchive)
{
	FSUSER_CloseArchive(saveHandle, saveArchive);
	FSUSER_CloseArchive(sdHandle, sdArchive);
	svcCloseHandle(*saveHandle);
	svcCloseHandle(*sdHandle);
	return 0;
}