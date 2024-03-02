# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wpedantic -std=c99 -O0 -g

# define targets
TARGETS=kNN mk

#define object-files
OBJ=mk.o kNN.o utils.o trie.o

build: $(TARGETS)

mk: mk.o utils.o trie.o
	$(CC) $(CFLAGS) $^ -o $@

kNN: kNN.o utils.o trie.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

pack:
	zip -FSr 315CA_TeodorStamatin_Tema3.zip README.md Makefile *.c *.h

clean:
	rm -f $(TARGETS) $(OBJ)

.PHONY: pack clean
