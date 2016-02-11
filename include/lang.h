#pragma once
/**
 * @file lang.h
 */
#ifndef LANG_H
#define LANG_H

typedef enum {
	LANG_JPN,	///< Japan
	LANG_ENG,	///< English
	LANG_FRE,	///< French
	LANG_ITA,	///< Italian
	LANG_GER,	///< German
	LANG_SPA,	///< Spanish
	LANG_KOR,	///< Korean
} Lang;

extern Lang userlang;

#endif // LANG_H
