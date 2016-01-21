#pragma once
/**
 * @file key.hpp
 */
#ifndef KEY_HPP
#define KEY_HPP

#include <3ds/types.h>

inline void waitKey(u32 key)
{
	while (aptMainLoop())
	{
		hidScanInput();
		if (hidKeysDown() & key) break;
	}
}

#endif // KEY_HPP
