#ifndef IMAGE_MANAGER_HPP
#define IMAGE_MANAGER_HPP


struct BoxBackgroundImageGIMP
{
	unsigned int width;
	unsigned int height;
	unsigned int bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
	unsigned char pixel_data[220 * 210 * 4 + 1];
};

struct BoxIconsImageGIMP
{
	unsigned int width;
	unsigned int height;
	unsigned int bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
	unsigned char pixel_data[1000 * 870 * 4 + 1];
};

struct BoxTilesImageGIMP
{
	unsigned int width;
	unsigned int height;
	unsigned int bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
	unsigned char pixel_data[192 * 88 * 4 + 1];
};

struct PokemonResumeBackgroundGIMP
{
	unsigned int width;
	unsigned int height;
	unsigned int bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
	unsigned char pixel_data[400 * 240 * 4 + 1];
};

namespace ImageManager
{
	extern const BoxBackgroundImageGIMP boxBackground23o_img;
	extern const BoxIconsImageGIMP boxIcons_img;
	extern const BoxTilesImageGIMP boxTiles_img;
	extern const PokemonResumeBackgroundGIMP pokemonResumeBackground_img;
}


#endif // IMAGE_MANAGER_HPP