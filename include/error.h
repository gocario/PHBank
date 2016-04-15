#pragma once

#define ERR_SAVE (1 << 2)
#define ERR_DATA (1 << 3)
#define ERR_FONT (1 << 4)
#define ERR_GRAPHICS (1 << 5)
#define ERR_FILESYSTEM (1 << 7)

#define ERR_MODULE_MASK (0xFF000000)
#define ERR_DESCRIPTION_MASK (0x00FFFFFF)

