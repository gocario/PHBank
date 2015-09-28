///
/// From smealum/3ds_hb_menu
/// Modified
///

#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <stdio.h>
#include <string.h>
#include <3ds.h>

Result saveFile(char* path, void* src, u64 size, FS_archive* archive, Handle* fsHandle, u32* bytesWritten);
Result loadFile(char* path, void* dst, FS_archive* fsarch, Handle* fshdl, u64 maxSize, u32* bytesRead);
Result deleteFile(char* path, Handle* fshdl, FS_archive* fsarch);

Result createDirectory(char* path, Handle* fsHandle, FS_archive* fsArchive);

Result filesysInit(Handle* sd, Handle* saveHandle, FS_archive* sdArchive, FS_archive* saveArchive);
Result filesysExit(Handle* sd, Handle* saveHandle, FS_archive* sdArchive, FS_archive* saveArchive);

#endif // FILESYSTEM_HPP
