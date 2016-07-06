#pragma once
/**
 * @file am.h
 * @brief Title Selector Module for Pokémon titles
 * @author Gocario
 */
#ifndef TS_H
#define TS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am.h"

extern AM_TitleMediaEntry titleEntry;

/**
 * @brief Starts the loop of the title selector.
 * @return Whether the title selector has selected a title.
 */
bool TS_Loop(void);

#ifdef __cplusplus
}
#endif

#endif // TS_H
