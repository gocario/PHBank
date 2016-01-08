#pragma once
/**
 * @file textures.h
 */
#ifndef TEXTURES_H
#define TEXTURES_H

typedef struct {
	unsigned int width;
	unsigned int height;
	unsigned int bytes_per_pixel;
	unsigned char pixel_data[];
} gimp_img_t;

extern const gimp_img_t boxBackground23o_img;
extern const gimp_img_t boxIcons_img;
extern const gimp_img_t boxTiles_img;
extern const gimp_img_t pokemonResumeBackground_img;

#endif // TEXTURES_H
