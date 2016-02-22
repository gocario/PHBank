#pragma once
/**
 * @file lang.h
 */
#ifndef LANG_H
#define LANG_H

typedef enum
{
	LANGUAGE_JP = 1,	///< Japan
	LANGUAGE_EN = 2,	///< English
	LANGUAGE_FR = 3,	///< French
	LANGUAGE_IT = 4,	///< Italian
	LANGUAGE_DE = 5,	///< German
	LANGUAGE_x6 = 6,	///< 0x6
	LANGUAGE_ES = 7,	///< Spanish
	LANGUAGE_KR = 8,	///< Korean
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
