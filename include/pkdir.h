#pragma once
/**
 * @file pkdir.h
 * @brief Constants of pkbrew.
 */
#ifndef PKDIR_H
#define PKDIR_H

/**
 * @brief Brew files.
 */
#define PK_SAVE_FILE "main"
#define PK_BANK_FILE "bank" 

/**
 * @brief Shared folders.
 */
#define PK_ROOT_FOLDER "/"
#define PK_BASE_FOLDER PK_ROOT_FOLDER "pk/"
#define PK_BACKUP_FOLDER PK_ROOT_FOLDER "pk/backup/"
#define PK_DATA_FOLDER PK_ROOT_FOLDER "pk/data/"
#define PK_ROMFS_FOLDER PK_ROOT_FOLDER "pk/romfs/"

/**
 * @brief Brew folders.
 */
#define PK_BANK_FOLDER PK_ROOT_FOLDER "pk/bank/"
#define PK_PCHEXPP_FOLDER PK_ROOT_FOLDER "pk/PCHex++/"

#endif // PKDIR_H
