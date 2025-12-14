#define COLORED
#include "main.h"


bool image_load(Image *img, const char* filename) {
	img->data = stbi_load(filename, &img->width, &img->height, &img->channels, 0);
	if (img->data == NULL) return false;
	return true;
}

void image_free(Image *img) {
	stbi_image_free(img->data);
}

RGBA* image_to_rgba(Image img) {
	RGBA *ret = malloc(img.width*img.height*sizeof(RGBA));
	if (!ret) return NULL;
	const int imgsize = img.width*img.height*img.channels;
	switch (img.channels) {
		case 3:
			for (int i = 0; i < imgsize; i+=3) {
				const int idx = i/3;
				ret[idx].r = img.data[i];
				ret[idx].g = img.data[i+1];
				ret[idx].b = img.data[i+2];
				ret[idx].a = 255;
			}
			break;
		case 4:
			for (int i = 0; i < imgsize; i+=4) {
				const int idx = i/4;
				ret[idx].r = img.data[i];
				ret[idx].g = img.data[i+1];
				ret[idx].b = img.data[i+2];
				ret[idx].a = img.data[i+3];
			}
			break;
		default:
			return NULL;
	}
	return ret;
}

int newline() {
	return printf("\n");
}

void reset_color() {
	printf("\x1b[0m");
}

void rgb_set_color(RGBA color) {
	printf("\x1b[38;2;%d;%d;%dm", color.r, color.g, color.b);
}
void rgb_to_term_set_color(RGBA color) {
	i_set_color(rgb_to_term(color));
}
void i_set_color(uint8_t color) {
	if (color > 15) {
		printf("\x1b[%dm", color);
		return;
	}
	if (color < 8) {
		printf("\x1b[%dm", color+30);
		return;
	}
	printf("\x1b[%dm", color+82);
}

int print_rgba_uncolored(RGBA color) {
	return printf("%d %d %d %d", color.r, color.g, color.b, color.a);
}

int print_rgba_colored(RGBA color) {
	rgb_set_color(color);
	int ret = print_rgba_uncolored(color);
	reset_color();
	return ret;
}

void print_rgba_image(RGBA* data, Image img) {
	for (int y = 0; y < img.height; y++) {
		for (int x = 0; x < img.width-1; x++) {
			rgb_set_color(data[y*img.width+x]);
			//printf("%d", data[y*img.width+x].a);
			printf("%s", alpha_to_char(data[y*img.width+x].a));
			reset_color();
			//printf("|");
		}
		rgb_set_color(data[y*img.width+img.width-1]);
		printf("%s", alpha_to_char(data[y*img.width+img.width-1].a));
		reset_color();
		newline();
	}
}

void print_image(Image img) {
	for (int y = 0; y < img.height-1; y++) {
		for (int x = 0; x < img.width; x++) {
			rgb_set_color(rgb(img.data[y*img.width*img.channels+x*img.channels],
					img.data[y*img.width*img.channels+x*img.channels+1],
					img.data[y*img.width*img.channels+x*img.channels+2]));
			if (img.channels == 4) printf("%s", alpha_to_char(img.data[y*img.width*img.channels+x*img.channels+3]));
			else printf("%s", ALPHA_CHARS[ALPHA_CHARS_SIZE-1]);
			reset_color();
		}
		reset_color();
		newline();
	}
	int y = img.height-1;
	for (int x = 0; x < img.width; x++) {
		rgb_set_color(rgb(img.data[y*img.width*img.channels+x*img.channels],
				img.data[y*img.width*img.channels+x*img.channels+1],
				img.data[y*img.width*img.channels+x*img.channels+2]));
		if (img.channels == 4) printf("%s", alpha_to_char(img.data[y*img.width*img.channels+x*img.channels+3]));
		else printf("%s", ALPHA_CHARS[ALPHA_CHARS_SIZE-1]);
		reset_color();
	}
	reset_color();
}	

RGBA rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	RGBA ret = {r, g, b, a};
	return ret;
}

RGBA rgb(uint8_t r, uint8_t g, uint8_t b) {
	RGBA ret = {r, g, b, 255};
	return ret;
}

uint8_t rgb_to_term(RGBA rgb) {
	uint8_t match;
	uint16_t match_diff = 1024;
	for (uint8_t i = 0; i < 16; i++) {
		uint16_t diff = labs(rgb.r-TERM_COLORS[i].r)+labs(rgb.g-TERM_COLORS[i].g)+labs(rgb.b-TERM_COLORS[i].b);
		if (diff < match_diff) {
			match = i;
			match_diff = diff;
		}
	}
	return match;
}

const char* alpha_to_char(uint8_t alpha) {
	const uint8_t alpha_step = 255 / ALPHA_CHARS_SIZE;
	if (alpha > (255 - alpha_step / 2)) return ALPHA_CHARS[ALPHA_CHARS_SIZE-1];
	return ALPHA_CHARS[(alpha + alpha_step / 2) / alpha_step];
}

int resize_image(Image img1, Image *img2) {
	img2->channels = img1.channels;
	img2->data = malloc(img2->width * img2->height * img2->channels * sizeof(typeof(img2->data)));
	return stbir_resize_uint8(img1.data, img1.width, img1.height, 0,
			img2->data, img2->width, img2->height, 0,
			img1.channels);
}

Image img;
int lines, columns;
int imgindx = -1;
ExecInfo conf;

int main(int argc, char** argv) {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#elif __linux__
	fprintf("\033%G");
#endif

	conf = getExecConfig();

	for (int i = 1; i < argc; i++) if (strcmp(argv[i], "norgb") == 0) {
		printf("info: using terminal colors instead of rgb\n");
		conf.use_term_colors = true;
	} else if (strcmp(argv[i], "fh") == 0) {
		printf("info: fitting to buffer height. will look bad if you have line wrapping on and the image is wide enough\n");
		conf.fit_type = FIT_HEIGHT;
	} else if (strcmp(argv[i], "fb") == 0) {
		printf("info: fitting whole into buffer\n");
		conf.fit_type = FIT_WHOLE;
	} else if (strcmp(argv[i], "fn") == 0) {
		printf("warning: not fitting to buffer. will probably not render how you'd want it to\n");
		conf.fit_type = NO_FIT;
	} else if (strcmp(argv[i], "fw") == 0) {
		conf.fit_type = FIT_WIDTH;
	} else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "help") == 0 || strcmp(argv[i], "h") == 0 || strcmp(argv[i], "?") == 0) {
		printf(HELP_STRING);
		return 0;
	} else {
		imgindx = i;
	}
	if (conf.fit_type == FIT_WIDTH) printf("info: fitting to buffer width\n");
	if (conf.fit_type != NO_FIT) {
		Image termInfo = getBufferSize();
		if (termInfo.width < 0 || termInfo.height < 0) {
			printf("warning: failed to get terminal info. not applying fitting. will probably look bad\n");
			conf.fit_type = NO_FIT;
		} else {
			lines = termInfo.height;
			columns = termInfo.width;
			printf("buffer size: %dx%d\n", columns, lines);
		}
	}
		
	if (imgindx == -1) {
		printf(HELP_STRING);
		return 0;
	}
	
	if (!image_load(&img, argv[imgindx])) {
		printf("error: failed to load image");
		return 1;
	}
	printf("Image has %d color channels\n", img.channels);
	Image img2;
	if (conf.fit_type == FIT_WHOLE) {
		if (img.height * min(img.width, columns) / img.width / 2 > lines) conf.fit_type = FIT_HEIGHT;
		else conf.fit_type = FIT_WIDTH;
	}
	switch (conf.fit_type) {
		case FIT_WIDTH:
			img2.width = min(img.width, columns);
			img2.height = (img.height * img2.width) / img.width / 2;
			break;
		case FIT_HEIGHT:
			img2.height = min(img.height, lines);
			img2.width = (img.width * img2.height * 2) / img.height;
			break;
		case NO_FIT:
			img2.width = img.width;
			img2.height = img.height / 2;
			break;			
	}
	if (img2.height > img.height || img2.width > img.width) {
		img2.width = img.width;
		img2.height = img.width / 2;
	}
	printf("using size: %dx%d\n", img2.width, img2.height);
	resize_image(img, &img2);
	image_free(&img);
	
	print_image(img2);
	image_free(&img2);
	waitForKeypress();
	
	return 0;
}
