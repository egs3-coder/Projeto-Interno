CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -pedantic -Isrc
LDFLAGS=-lm

CONSOLE_SRC=src/main.c src/cards.c src/history.c src/analysis.c src/pif_guess.c src/quiz.c src/shop.c src/game.c

all: build/jogo

build:
	mkdir -p build

build/jogo: $(CONSOLE_SRC) | build
	$(CC) $(CFLAGS) $(CONSOLE_SRC) $(LDFLAGS) -o build/jogo

raylib: build/jogo_raylib

build/jogo_raylib: src/raylib_main.c src/cards.c src/shop.c src/player_data.h src/raylib_question_bank.h | build
	$(CC) $(CFLAGS) src/raylib_main.c src/cards.c src/shop.c -lraylib $(LDFLAGS) -o build/jogo_raylib

clean:
	rm -rf build history.txt

.PHONY: all clean raylib
