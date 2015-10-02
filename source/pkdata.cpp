#include "pkdata.hpp"

uint8_t PKData::_lang = LANG_EN;
uint8_t PKData::_species[722][15]; // 11~13
uint8_t PKData::_items[776][20]; // 17

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
	return PKData::_species[species];
}


uint8_t* PKData::items(uint32_t item)
{
	return _items[item];
}



void PKData::loadData(Handle *sdHandle, FS_archive *sdArchive)
{
	Result ret;

	const uint32_t maxSize = 0x4b0;
	const uint32_t pathSize = 0x20;
	uint32_t bytesRead;
	uint8_t* temp = new uint8_t[maxSize];
	char dir[] = "/pkbank/data/";
	char speciesPath[pathSize], itemPath[pathSize];

	printf("Loading species data...");
	sprintf(speciesPath, "%s%s/%s_%s.txt", dir, lang(), "species", lang());
	ret = FS_loadFile(speciesPath, temp, sdArchive, sdHandle, maxSize, &bytesRead);
	if (ret) { printf(" [%lx]%s\n", bytesRead, speciesPath); return; }
	loadDataLine(temp, _species[0], 15, bytesRead);
	printf(" OK\n");

	for (uint32_t i = 0; i < 0x16; i++)
	{
		printf("%s", _species[i]);
	}

	printf("Loading items data...");
	sprintf(itemPath, "%s%s/%s_%s.txt", dir, lang(), "items", lang());
	ret = FS_loadFile(itemPath, temp, sdArchive, sdHandle, maxSize, &bytesRead);
	if (ret) { printf(" Error\n"); return; }
	loadDataLine(temp, _items[0], 15, bytesRead);
	printf(" OK\n");

	adelete(temp);
}


void PKData::loadDataLine(uint8_t *src, uint8_t *dst, uint8_t lineLength, uint32_t size)
{
	uint16_t srcOffset = 0;
	uint8_t lineOffset;

	for (uint8_t line = 0; srcOffset < size; line++)
	{
		for (lineOffset = 0; srcOffset < size && src[srcOffset] != '\n'; lineOffset++)
		{
			printf("%c", src[srcOffset]);
			dst[line * lineLength + lineOffset] = src[srcOffset];
			srcOffset++;
		}
		dst[line * lineLength + lineOffset] = '\0';
		srcOffset++;
	}
}

