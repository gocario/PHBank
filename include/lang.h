#pragma once
/**
 * @file lang.h
 */
#ifndef LANG_H
#define LANG_H

typedef enum
{
	LANGUAGE_JP,	///< Japan
	LANGUAGE_EN,	///< English
	LANGUAGE_FR,	///< French
	LANGUAGE_IT,	///< Italian
	LANGUAGE_DE,	///< German
	LANGUAGE_x6,	///< 0x6
	LANGUAGE_ES,	///< Spanish
	LANGUAGE_KR,	///< Korean
} Language;

extern Language userlang;

#ifdef __cplusplus
extern "C" {
#endif

void updateSystemLanguage(void);

#ifdef __cplusplus
}
#endif

#endif // LANG_H
