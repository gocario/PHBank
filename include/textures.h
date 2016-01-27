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
extern const gimp_img_t boxTiles_img;
// extern const gimp_img_t boxPkmIcons_img;
extern const gimp_img_t itemBallIcons_img;
extern const gimp_img_t pkmResumeBackground_img;

#endif // TEXTURES_H
