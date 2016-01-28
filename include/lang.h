#pragma once
/**
 * @file lang.h
 */
#ifndef LANG_H
#define LANG_H

typedef enum {
	LANG_JPN,
	LANG_ENG,
	LANG_FRE,
	LANG_ITA,
	LANG_GER,
	LANG_SPA,
	LANG_KOR,
} Lang;

extern Lang userlang;

#endif
