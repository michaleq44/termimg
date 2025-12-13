#include "buf.h"

Image getBufferSize() {
	Image ret;
#ifdef _WIN32
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hout == INVALID_HANDLE_VALUE) {
		ret.width = -1, ret.height = -1;
		return ret;
	}

	CONSOLE_SCREEN_BUFFER_INFO console_info;
	if (!GetConsoleScreenBufferInfo(hout, &console_info)) {
		ret.width = -1, ret.height = -1;
		return ret;
	}
	ret.width = console_info.dwSize.X;
	ret.height = console_info.srWindow.Bottom - console_info.srWindow.Top;
#else
	char *termtype = getenv("TERM");
	char termbuf[2048];
	if (tgetent(termbuf, termtype) < 0) {
		ret.width = -1, ret.height = -1;
		return ret;
	}
	ret.width = tgetnum("co");
	ret.height = tgetnum("li");
#endif
	return ret;
}

void waitForKeypress() {
#ifdef _WIN32
	getch();
#else
	struct termios old, new;
	if (tcgetattr(STDIN_FILENO, &old) == -1) return;
	new.c_lflag &= ~(ECHO | ICANON);
	new.c_cc[VMIN] = 1;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &new) == -1) return;
	getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &old);
#endif
}

ExecInfo getExecConfig() {
	ExecInfo ret;
	ret.use_term_colors = false;
	ret.fit_type = FIT_WIDTH;
	char confpath[1024];

#ifdef _WIN32
	snprintf(confpath, sizeof(confpath), "%s%s", getenv("localappdata"), "\\termimg\\termimg.conf");
#else
	snprintf(confpath, sizeof(confpath), "%s%s", getenv("HOME"), "/.config/termimg/termimg.conf");
#endif
	printf("info: reading config from %s\n", confpath);
	
	FILE *f = fopen(confpath, "r");
	if (f == NULL) {
		printf("info: can't open config file or it doesn't exist. check termimg -h\n");
		return ret;
	}
	char buf[1024];

	while (fgets(buf, 1024, f)) {
		if (strcmp(buf, "norgb\n") == 0) {
			ret.use_term_colors = true;
		} else if (strcmp(buf, "fh\n") == 0) {
			ret.fit_type = FIT_HEIGHT;
		} else if (strcmp(buf, "fb\n") == 0) {
			ret.fit_type = FIT_WHOLE;
		} else if (strcmp(buf, "fn\n") == 0) {
			ret.fit_type = NO_FIT;
		} else if (strcmp(buf, "fw\n") == 0) {
			ret.fit_type = FIT_WIDTH;
		}
	}

	fclose(f);

	return ret;
}
