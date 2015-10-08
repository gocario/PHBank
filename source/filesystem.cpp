///
/// From smealum/3ds_hb_menu
/// Modified v2.4
///

#include "filesystem.hpp"


// ==================================================
Result _srvGetServiceHandle(Handle* out, const char* name)
// --------------------------------------------------
{
	u32* cmdbuf = getThreadCommandBuffer();
	cmdbuf[0] = 0x50100;
	strcpy((char*) &cmdbuf[1], name);
	cmdbuf[3] = strlen(name);
	cmdbuf[4] = 0x0;

	Result ret  = 0;

	ret = svcSendSyncRequest(*srvGetSessionHandle());
	if (ret) return ret;

	*out = cmdbuf[3];
	return cmdbuf[1];
}


// ==================================================
Result FSUSER_ControlArchive(Handle handle, FS_archive archive)
// --------------------------------------------------
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

	ret = svcSendSyncRequest(handle);
	if (ret) return ret;

	return cmdbuf[1];
}


// ==================================================
Result FS_loadFile(char* path, void* dst, FS_archive* fsArchive, Handle* fsHandle, u64 maxSize, u32* bytesRead)
// --------------------------------------------------
{
	if (!path || !dst || !fsArchive) return -1;

	Result ret = 0;
	u64 size;
	Handle fileHandle;

	if (!ret)
	{
		ret = FSUSER_OpenFile(fsHandle, &fileHandle, *fsArchive, FS_makePath(PATH_CHAR, path), FS_OPEN_READ, FS_ATTRIBUTE_NONE);
		if (ret) return ret;
	}

	if (!ret)
	{
		ret = FSFILE_GetSize(fileHandle, &size);
		if (ret || size > maxSize) ret = -2;
	}

	if (!ret)
	{
		ret = FSFILE_Read(fileHandle, bytesRead, 0, dst, size);
		if (ret || *bytesRead < size) ret=-3;
	}

	FSFILE_Close(fileHandle);
	return ret;
}


// ==================================================
Result FS_saveFile(char* path, void* src, u64 size, FS_archive* fsArchive, Handle* fsHandle, u32* bytesWritten)
// --------------------------------------------------
{
	if (!path || !src || !fsArchive) return -1;

	Result ret;
	Handle fileHandle;

	ret = FSUSER_OpenFile(fsHandle, &fileHandle, *fsArchive, FS_makePath(PATH_CHAR, path), FS_OPEN_WRITE | FS_OPEN_CREATE, FS_ATTRIBUTE_NONE);
	if (ret) return ret;

	ret = FSFILE_Write(fileHandle, bytesWritten, 0, src, size, FS_WRITE_NOFLUSH);
	if (ret) return ret;

	FSFILE_Close(fileHandle);
	return ret;
}


// ==================================================
Result FS_saveSFile(char* path, void* src, u64 size, FS_archive* fsArchive, Handle* fsHandle, u32* bytesWritten)
// --------------------------------------------------
{
	Result ret;

	ret = FS_saveFile(path, src, size, fsArchive, fsHandle, bytesWritten);
	if (ret) return ret;

	ret = FSUSER_ControlArchive(*fsHandle, *fsArchive);
	return ret;
}


// ==================================================
Result FS_deleteFile(char* path, Handle* fsHandle, FS_archive* fsArchive)
// --------------------------------------------------
{
	if (!path || !fsHandle || !fsArchive) return -1;

	Result ret;

	ret = FSUSER_DeleteFile(fsHandle, *fsArchive, FS_makePath(PATH_CHAR, path));

	return ret;
}


// ==================================================
Result FS_createDirectory(char* path, Handle* fsHandle, FS_archive* fsArchive)
// --------------------------------------------------
{
	if (!path || !fsHandle || !fsArchive) return -1;

	Result ret;

	ret = FSUSER_CreateDirectory(fsHandle, *fsArchive, FS_makePath(PATH_CHAR, path));

	return ret;
}


// ==================================================
Result FS_filesysInit(Handle* sdHandle, Handle* saveHandle, FS_archive* sdArchive, FS_archive* saveArchive)
// --------------------------------------------------
{
	Result ret;
	printf("  Getting SD Card handle\n");
	ret = srvGetServiceHandle(sdHandle, "fs:USER");
	if (ret) return ret;

	printf("  Opening SD Card archive\n");
	*sdArchive = (FS_archive){ARCH_SDMC, (FS_path){PATH_EMPTY, 1, (u8*)""}, 0, 0};
	ret = FSUSER_OpenArchive(sdHandle, sdArchive);
	if (ret) return ret;

	printf("  Getting save handle\n");
	ret = _srvGetServiceHandle(saveHandle, "fs:USER");
	if (ret) return ret;

	printf("  Initializing save handle\n");
	ret = FSUSER_Initialize(saveHandle);
	if (ret) return ret;

	printf("  Opening save archive\n");
	*saveArchive = (FS_archive){ARCH_SAVEDATA, (FS_path){PATH_EMPTY, 0, NULL}, 0, 0};
	ret = FSUSER_OpenArchive(saveHandle, saveArchive);
	return ret;
}


// ==================================================
Result FS_filesysExit(Handle* sdHandle, Handle* saveHandle, FS_archive* sdArchive, FS_archive* saveArchive)
// --------------------------------------------------
{
	FSUSER_CloseArchive(saveHandle, saveArchive);
	FSUSER_CloseArchive(sdHandle, sdArchive);
	svcCloseHandle(*saveHandle);
	svcCloseHandle(*sdHandle);
	return 0;
}