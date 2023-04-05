# Copyright 2023 Nichita Adrian Bunu <bununichita@gmail.com>

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=vma

build: $(TARGETS)

vma: main.c vma.c
	$(CC) $(CFLAGS) *.c -o vma
	
run_vma:
	./vma

pack:
	zip -FSr 313CA_BunuNichita-Adrian_Tema1.zip README Makefile *.c *.h
	
clean:
	rm -f $(TARGETS)
	
.PHONY: pack clean
