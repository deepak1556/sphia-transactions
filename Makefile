
BIN    ?= sphia-transaction
PREFIX ?= /usr/local
SRC     = $(wildcard deps/*/*.c src/*.c)
CFLAGS  = -std=c99 -Ideps -Wall -Wextra -fPIC
CFLAGS += -fvisibility=hidden -D_BSD_SOURCE
LDFLAGS = -pthread -lsophia

$(BIN): $(SRC)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $(BIN)

install: $(BIN)
	mkdir -p $(PREFIX)/bin
	cp $(BIN) $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/$(BIN)

.PHONY: install uninstall
