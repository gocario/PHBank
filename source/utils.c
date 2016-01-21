#include "utils.h"

#include <3ds/util/utf.h>

void readUTF16(void* _dst, void* _src, u16 length)
{
	u8* dst = (u8*) _dst;
	u16* src = (u16*) _src;
	dst[0] = '\0';
	for (u16 i = 0; i < length; i++)
		dst[i] = src[i] & 0xFF;
	utf16_to_utf8(dst, src, length); // Hmmm..?
	dst[length] = '\0';
}


void readUnicode(void* _dst, void* _src, u16 length)
{
	u8* dst = (u8*) _dst;
	u16* src = (u16*) _src;
	dst[0] = '\0';
	for (u16 i = 0; i < length; i++)
		dst[i] = src[i] & 0xFF;
	dst[length] = '\0';
}