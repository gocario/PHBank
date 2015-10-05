#include "pkdata.hpp"

uint8_t PKData::_lang = LANG_EN;
uint8_t PKData::_species[DEX_SPECIES_COUNT][DEX_SPECIES_LENGTH]; // 11~13
uint8_t PKData::_items[DEX_ITEMS_COUNT][DEX_ITEMS_LENGTH]; // 17

const char* PKData::lang()
{
	switch (_lang)
	{
		case LANG_FR:
			return "fr";
		case LANG_EN:
		default:
			return "en";
	}
}

uint8_t* PKData::species(uint32_t species)
{
	return (u8*)(((u8*)&(PKData::_species)) + 20 * species);
}


uint8_t* PKData::items(uint32_t item)
{
	return (u8*)(((u8*)&(PKData::_items)) + 20 * item);
}


Result PKData::load(Handle *sdHandle, FS_archive *sdArchive)
{
	// species_en: 6 074 octets #0x17ba
	// species_fr: 6 635 octets #0x19eb
	// items_en: 8 150 octets #0x1fd6
	// items_fr: 8 634 octets #0x21ba

	Result ret;
	ret = PKData::loadData(sdHandle, sdArchive, 0x17ba, (char*)("/pkbank/data/en/species_en.txt"), (u8*)&(PKData::_species), DEX_SPECIES_LENGTH, DEX_SPECIES_COUNT);
	ret = PKData::loadData(sdHandle, sdArchive, 0x1fd6, (char*)("/pkbank/data/en/items_en.txt"), (u8*)&(PKData::_items), DEX_ITEMS_LENGTH, DEX_ITEMS_COUNT);
	return ret;
}


Result PKData::loadData(Handle *sdHandle, FS_archive *sdArchive, u32 maxSize, char* path, u8* dest, u32 lineLength, u32 lineCount)
{
	uint32_t bytesRead;
	u8* buffer = new u8[maxSize];

	Result ret;
	ret = FS_loadFile(path, buffer, sdArchive, sdHandle, maxSize, &bytesRead);
	if (ret) { printf(" ERROR...\n"); return ret; }
	printf(" OK!\n");

	PKData::loadDataLine(buffer, dest, lineLength, lineCount);
	if (ret) return ret;


	// for (u32 c = 0; c < 0x100; c++)
	// {
	// 	u8 chr = *(u8*)(((u8*)&(PKData::_species)) + c);
	// 	if (chr > 0x10)
	// 		printf("%c", chr);
	// 	else
	// 		printf("#");
	// }
	// printf("|\n");


	adelete(buffer);
	return ret;
}


Result PKData::loadDataLine(u8* src, u8* dst, u32 lineLength, u32 lineCount)
{
	u32 count = 0;
	u32 lineOffset = 0;
	u32 sourceOffset = 3;
	while(count < lineCount)
	{
		lineOffset = 0;
		// printf("> %lu ", count);
		// printf("[@%p] ", (void*)(dst + lineLength * count));
		while (src[sourceOffset] != '\n' && src[sourceOffset] != 0)
		{
			// printf("%c", *(u8*) (src + sourceOffset));
			*(u8*)(dst + lineLength * count + lineOffset) = *(u8*)(src + sourceOffset);
			lineOffset++;
			sourceOffset++;
		}
		while (src[sourceOffset] == '\n' || src[sourceOffset] == 0) sourceOffset++;
		
		dst[lineLength * count + lineOffset] = '\0';

		// if (count < 10)
		// {
		// printf(" -> ");
		// for (u32 i = 0; i < 15; i++)
		// {
		// 	if (*(u8*)(dst + lineLength * count + i) > 0x10)
		// 		printf("%c", *(u8*)(dst + lineLength * count + i));
		// 	else
		// 		printf("#");
		// }
		// printf("|\n");
		// }
		// if (count == 10) waitKey(KEY_A);
		// if (hidKeysHeld() & KEY_R) return 0;

		count++;

	}
	return 0;
}

