SRC = src/main.c src/stb_image.c src/buf.c
INC = -Iinc
CFLAGS = --std=c2x
LDFLAGS = -lm -lcurses

WINDOWS_LDFLAGS = -static

BIN = termimg
BINDIR = bin/

DEBUG_CFLAGS = -g
RELEASE_CFLAGS = -O3

debug:
	mkdir -p $(BINDIR)
ifeq ($(windows),1)
	x86_64-w64-mingw32-gcc $(SRC) $(INC) $(CFLAGS) $(DEBUG_CFLAGS) $(WINDOWS_LDFLAGS) -o $(BINDIR)/$(BIN).exe
else
	gcc $(SRC) $(INC) $(CFLAGS) $(DEBUG_CFLAGS) $(LDFLAGS) -o $(BINDIR)/$(BIN)
	chmod +x $(BINDIR)/$(BIN)
endif

release:
	mkdir -p $(BINDIR)/release
ifeq ($(windows),1)
	x86_64-w64-mingw32-gcc $(SRC) $(INC) $(CFLAGS) $(RELEASE_CFLAGS) $(WINDOWS_LDFLAGS) -o $(BINDIR)/release/$(BIN).exe
else
	gcc $(SRC) $(INC) $(CFLAGS) $(RELEASE_CFLAGS) $(LDFLAGS) -o $(BINDIR)/release/$(BIN)
	chmod +x $(BINDIR)/release/$(BIN)
endif

run: debug
ifeq ($(windows),1)
	rm -f $(BIN).exe
	cp -f $(BINDIR)\$(BIN).exe .
else
	rm -f ./$(BIN)
	cp -f $(BINDIR)/$(BIN) ./
	chmod +x ./$(BIN)
endif

run-release: release
ifeq ($(windows),1)
	rm -f $(BIN).exe
	cp -f $(BINDIR)\release\$(BIN).exe .
else
	rm -f ./$(BIN)
	cp -f $(BINDIR)/release/$(BIN) ./
	chmod +x ./$(BIN)
endif

install:
	install $(BINDIR)/release/$(BIN) $(DESTDIR)/usr/bin/