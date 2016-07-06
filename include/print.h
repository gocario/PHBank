#pragma once

#include <3ds/types.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

inline void print(const u8* src, size_t len)
{
	for (size_t i = 0; i < len; i++)
	{
		printf("%02X", src[i]);
		if (i % 0x10 == 0xF) printf("\n");
	}
	printf("\n");
}

#ifdef __cplusplus
}
#endif
