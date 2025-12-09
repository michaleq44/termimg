#pragma once

#include <stdint.h>

#ifndef min
#define min(x, y) (((x) < (y)) ? (x) : (y))
#endif

typedef struct {
	int width, height, channels;
	uint8_t* data;
} Image;

typedef struct {
	uint8_t r, g, b;
	uint8_t a;
} RGBA;

enum FitType {
	FIT_HEIGHT, FIT_WIDTH, FIT_WHOLE, NO_FIT
};
