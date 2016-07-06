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
#define SAVE_FILE "main"
#define BANK_FILE "bank"

/**
 * @brief Devices.
 */
#define ROOT "/"
#define SDMC "sdmc:/"
#define ROMFS "romfs:/"
#define SAVE "pksav:/"

/**
 * @brief Shared folders.
 */
#define BASE_FOLDER "pk/"
#define BACKUP_FOLDER BASE_FOLDER "backup/"
#define DATA_FOLDER BASE_FOLDER "data/"
#define ROMFS_FOLDER BASE_FOLDER "romfs/"

/**
 * @brief Brew folders.
 */
#define BANK_FOLDER BASE_FOLDER "bank/"

#endif // PKDIR_H
