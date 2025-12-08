SRC = src/main.c src/stb_image.c
INC = -Iinc
CFLAGS = --std=c23
LDFLAGS = -lm -lcurses
BIN = termimg
BINDIR = bin/

DEBUG_CFLAGS = -g
RELEASE_CFLAGS = -O3

compile:
	gcc $(SRC) $(INC) $(CFLAGS) $(DEBUG_CFLAGS) $(LDFLAGS) -o $(BINDIR)/$(BIN)
	rm -f ./$(BIN)
	cp -f $(BINDIR)/$(BIN) ./
	chmod +x ./$(BIN)

release:
	mkdir -p $(BINDIR)/release
	gcc $(SRC) $(INC) $(CFLAGS) $(RELEASE_CFLAGS) $(LDFLAGS) -o $(BINDIR)/release/$(BIN)
	rm -f ./$(BIN)
	cp -f $(BINDIR)/release/$(BIN) ./
	chmod +x ./$(BIN)
