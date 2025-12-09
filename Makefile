SRC = src/main.c src/stb_image.c src/buf.c
INC = -Iinc
CFLAGS = --std=c23
LDFLAGS = -lm -lcurses
BIN = termimg
BINDIR = bin/

DEBUG_CFLAGS = -g
RELEASE_CFLAGS = -O3

compile:
	gcc $(SRC) $(INC) $(CFLAGS) $(DEBUG_CFLAGS) $(LDFLAGS) -o $(BINDIR)/$(BIN)
	x86_64-w64-mingw32-gcc $(SRC) $(INC) $(CFLAGS) $(DEBUG_CFLAGS) -static -o $(BINDIR)/$(BIN).exe
	rm -f ./$(BIN)
	cp -f $(BINDIR)/$(BIN) ./
	chmod +x ./$(BIN)

release:
	mkdir -p $(BINDIR)/release
	gcc $(SRC) $(INC) $(CFLAGS) $(RELEASE_CFLAGS) $(LDFLAGS) -o $(BINDIR)/release/$(BIN)
	x86_64-w64-mingw32-gcc $(SRC) $(INC) $(CFLAGS) $(RELEASE_CFLAGS) -static -o $(BINDIR)/release/$(BIN).exe
	rm -f ./$(BIN)
	cp -f $(BINDIR)/release/$(BIN) ./
	chmod +x ./$(BIN)
