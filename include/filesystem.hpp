///
/// From smealum/3ds_hb_menu
/// Modified v2.4
///

#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <3ds.h>
// #include <3ds/services/fs.h>
// #include <3ds/srv.h>
// #include <3ds/svc.h>

Result FS_loadFile(char* path, void* dst, FS_archive* fsarch, Handle* fshdl, u64 maxSize, u32* bytesRead);
Result FS_saveFile(char* path, void* src, u64 size, FS_archive* archive, Handle* fsHandle, u32* bytesWritten);
Result FS_saveSFile(char* path, void* src, u64 size, FS_archive* archive, Handle* fsHandle, u32* bytesWritten);
Result FS_deleteFile(char* path, Handle* fshdl, FS_archive* fsarch);

Result FS_createDirectory(char* path, Handle* fsHandle, FS_archive* fsArchive);

Result FSUSER_ControlArchive(Handle handle, FS_archive archive);
Result FS_filesysInit(Handle* sd, Handle* saveHandle, FS_archive* sdArchive, FS_archive* saveArchive);
Result FS_filesysExit(Handle* sd, Handle* saveHandle, FS_archive* sdArchive, FS_archive* saveArchive);

#endif // FILESYSTEM_HPP
