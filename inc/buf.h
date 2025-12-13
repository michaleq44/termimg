#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termcap.h>
#include <termios.h>
#include <unistd.h>
#endif

#include "types.h"

Image getBufferSize();
void waitForKeypress();
ExecInfo getExecConfig();
