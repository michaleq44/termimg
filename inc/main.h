#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <termcap.h>

#include <stb_image.h>
#include <stb_image_resize.h>

#define min(x, y) (((x) < (y)) ? (x) : (y))

typedef struct {
	int width, height, channels;
	uint8_t* data;
} Image;

typedef struct {
	uint8_t r, g, b;
	uint8_t a;
} RGBA;

// #############
// # CONSTANTS #
// #############

const wchar_t ALPHA_CHARS[] = L" ░▒▓█";
//const wchar_t ALPHA_CHARS[] = L"ABCDE";
const uint8_t ALPHA_CHARS_SIZE = 5;

// DARK
const RGBA D_BLACK = {0, 0, 0, 255};
const RGBA D_RED = {128, 0, 0, 255};
const RGBA D_GREEN = {0, 128, 0, 255};
const RGBA D_YELLOW = {128, 128, 0, 255};
const RGBA D_BLUE = {0, 0, 128, 255};
const RGBA D_MAGENTA = {128, 0, 128, 255};
const RGBA D_CYAN  = {0, 128, 128, 255};
const RGBA D_WHITE = {192, 192, 192, 255};

// BRIGHT
const RGBA B_BLACK = {128, 128, 128, 255};
const RGBA B_RED = {255, 0, 0, 255};
const RGBA B_GREEN = {0, 255, 0, 255};
const RGBA B_YELLOW = {255, 255, 0, 255};
const RGBA B_BLUE = {0, 0, 255, 255};
const RGBA B_MAGENTA = {255, 0, 255, 255};
const RGBA B_CYAN = {0, 255, 255, 255};
const RGBA B_WHITE = {255, 255, 255, 255};

const RGBA TERM_COLORS[] = {D_BLACK, D_RED, D_GREEN, D_YELLOW, D_BLUE, D_MAGENTA, D_CYAN, D_WHITE,	B_BLACK, B_RED, B_GREEN, B_YELLOW, B_BLUE, B_MAGENTA, B_CYAN, B_WHITE};


bool image_load(Image *img, const char* filename);
void image_free(Image *img);
RGBA rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
RGBA rgb(uint8_t r, uint8_t g, uint8_t b);
uint8_t rgb_to_term(RGBA rgb);
RGBA* image_to_rgba(Image img);

int newline();
void print_rgba_image(RGBA* data, Image img);
void print_image(Image img);
void rgb_set_color(RGBA color);
void rgb_to_term_set_color(RGBA color);
void i_set_color(uint8_t color);
void reset_color();
wchar_t alpha_to_char(uint8_t alpha);
int resize_image(Image img1, Image *img2);

// UNCOLORED
int print_rgba_uncolored(RGBA color);

// COLORED
int print_rgba_colored(RGBA color);

// ##########
// MACRO DEFS
// ##########
#ifdef COLORED

#define print_rgba print_rgba_colored

#else

#define print_rgba print_rgba_uncolored

#endif
