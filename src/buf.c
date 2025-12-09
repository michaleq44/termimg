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
	ret.height = console_info.dwSize.Y;
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
