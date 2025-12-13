SRC = src/main.c src/stb_image.c src/buf.c
INC = -Iinc
CFLAGS = --std=c23
LDFLAGS = -lm -lcurses

WINDOWS_LDFLAGS = -static

BIN = termimg
BINDIR = bin/

DEBUG_CFLAGS = -g
RELEASE_CFLAGS = -O3

debug:
ifeq ($(windows),1)
	x86_64-w64-mingw32-gcc $(SRC) $(INC) $(CFLAGS) $(DEBUG_CFLAGS) $(WINDOWS_LDFLAGS) -o $(BINDIR)/$(BIN).exe
else
	gcc $(SRC) $(INC) $(CFLAGS) $(DEBUG_CFLAGS) $(LDFLAGS) -o $(BINDIR)/$(BIN)
endif

release:
	mkdir -p $(BINDIR)/release
ifeq ($(windows),1)
	x86_64-w64-mingw32-gcc $(SRC) $(INC) $(CFLAGS) $(RELEASE_CFLAGS) $(WINDOWS_LDFLAGS) -o $(BINDIR)/release/$(BIN).exe
else
	gcc $(SRC) $(INC) $(CFLAGS) $(RELEASE_CFLAGS) $(LDFLAGS) -o $(BINDIR)/release/$(BIN)
endif

run: debug
	rm -f ./$(BIN)
	cp -f $(BINDIR)/$(BIN) ./
	chmod +x ./$(BIN)

run-release: release
	rm -f ./$(BIN)
	cp -f $(BINDIR)/release/$(BIN) ./
	chmod +x ./$(BIN)
