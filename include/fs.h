#pragma once
/**
 * @file fs.h
 * @brief Filesystem Services
 */
#ifndef FS_H
#define FS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <3ds/services/fs.h>

/**
 * @brief The data of a user savedata archive's lowpath.
 */
typedef struct
{
	u32 mediatype; 	///< The mediatype of the FS_Path
	u32 lowid; 		///< The lower word of the saveid
	u32 highid; 	///< The upper word of the saveid
} FS_UserSaveData_LowPathData;

extern FS_Archive sdmcArchive;
extern FS_Archive saveArchive;

/**
 * @brief Returns true if FS is initialized.
 * @return Whether FS is initialized.
 */
bool FS_IsInitialized(void);

/**
 * @brief Returns true if the archive is initialized.
 * @return Whether the archive id is initialized.
 */
bool FS_IsArchiveInitialized(FS_Archive* archive);

/**
 * @brief Reads a file (path) to dst.
 * @param path The path of the file to read.
 * @param dst The destination buffer.
 * @param archive The archive where the file is located.
 * @param maxSize The maximum size of the file.
 * @param bytesRead The total of read bytes.
 * @return The error encountered.
 */
Result FS_ReadFile(char* path, void* dst, FS_Archive* archive, u64 maxSize, u32* bytesRead);

/**
 * @brief Writes src to a file (path).
 * @param path The path of the file to write.
 * @param src The source buffer.
 * @param size The size of bytes to write.
 * @param archive The archive where the file is located.
 * @param bytesWritten The total of written bytes.
 * @return The error encountered.
 */
Result FS_WriteFile(char* path, void* src, u64 size, FS_Archive* archive, u32* bytesWritten);

/**
 * @brief Deletes a file (path).
 * @param path The path of the file to delete.
 * @param archive The archive where the file is located.
 * @return The error encountered.
 */
Result FS_DeleteFile(char* path, FS_Archive* archive);

/**
 * @brief Creates a directory (path).
 * @param path The path of the directory to create.
 * @param archive The archive where the directory will be located.
 * @return The error encountered.
 */
Result FS_CreateDirectory(char* path, FS_Archive* archive);

/**
 * @brief Commits an archive.
 * @param The archive to commit.
 * @return The error encountered.
 */
Result FS_CommitArchive(FS_Archive* archive);

/**
 * @brief Initializes the filesystem service.
 * @return The error encountered.
 */
Result FS_FilesysInit(void);

/**
 * @brief Exits the filesystem service.
 * @return The error encountered.
 */
Result FS_FilesysExit(void);

#ifdef __cplusplus
}
#endif

#endif // FS_H
