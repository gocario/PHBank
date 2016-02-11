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

/// The data of a user savedata archive's lowpath.
typedef struct
{
	u32 mediatype; 	///< The mediatype of the FS_Path.
	u32 lowid; 		///< The lower word of the saveid.
	u32 highid; 	///< The upper word of the saveid.
} FS_UserSaveData_LowPathData;

/// Attribute flags extended.
enum
{
	FS_ATTRIBUTE_NONE = 0,  ///< None.
};

extern FS_Archive saveArchive;

/**
 * @brief Reads a file (path) to dst.
 * @param path The path of the file to read.
 * @param dst The destination buffer.
 * @param archive The archive where the file is located.
 * @param maxSize The maximum size of the file.
 * @param bytesRead The total of read bytes.
 */
Result FS_ReadFile(const char* path, void* dst, const FS_Archive* archive, u64 maxSize, u32* bytesRead);

/**
 * @brief Writes src to a file (path).
 * @param path The path of the file to write.
 * @param src The source buffer.
 * @param size The size of bytes to write.
 * @param archive The archive where the file is located.
 * @param bytesWritten The total of written bytes.
 */
Result FS_WriteFile(const char* path, const void* src, u64 size, const FS_Archive* archive, u32* bytesWritten);

/**
 * @brief Deletes a file (path).
 * @param path The path of the file to delete.
 * @param archive The archive where the file is located.
 */
Result FS_DeleteFile(const char* path, const FS_Archive* archive);

/**
 * @brief Creates a directory (path).
 * @param path The path of the directory to create.
 * @param archive The archive where the directory will be located.
 */
Result FS_CreateDirectory(const char* path, const FS_Archive* archive);

/**
 * @brief Commits an archive.
 * @param The archive to commit.
 */
Result FS_CommitArchive(const FS_Archive* archive);

#ifdef __cia

/**
 * @brief Initializes the filesystem service.
 */
Result FSCIA_Init(u64 titleid, FS_MediaType mediatype);

/**
 * @brief Exits the filesystem service.
 */
Result FSCIA_Exit(void);

#else

/**
 * @brief Initializes the filesystem service.
 */
Result FS_Init(void);

/**
 * @brief Exits the filesystem service.
 */
Result FS_Exit(void);

#endif

#ifdef __cplusplus
}
#endif

#endif // FS_H
