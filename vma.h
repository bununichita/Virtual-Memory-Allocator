#pragma once
#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* TODO : add your implementation for doubly-linked list */
// typedef struct { } list_t;

typedef struct node_t {
	void *data;
	struct node_t *prev;
	struct node_t *next;
} node_t;

typedef struct list_t {
	node_t *head;
	uint64_t data_size;
	uint64_t size;
	node_t *tail;
} list_t;

typedef struct {
	uint64_t start_addr;
	size_t size;
	void *mini_list;
} block_t;

typedef struct {
	uint64_t start_addr;
	size_t size;
	uint8_t perm;
	void *rw_buffer;
} miniblock_t;

typedef struct {
	uint64_t arena_size;
	list_t *block_list;
} arena_t;

list_t
*dll_create(unsigned int data_size);
int
block_inters(arena_t *arena, const uint64_t addr, const uint64_t size);
node_t
*merg_l(arena_t *arena, const uint64_t addr);
node_t
*merg_r(arena_t *arena, node_t *left, const uint64_t addr, const uint64_t size);
arena_t
*alloc_arena(const uint64_t size);
void
free_block(arena_t *arena, const uint64_t addr);
void
dealloc_arena(arena_t *arena);
void
alloc_block_simple(arena_t *arena, const uint64_t addr, const uint64_t size);
void
alloc_block_left(const uint64_t addr, const uint64_t size, node_t *left);
void
alloc_block_right(const uint64_t addr, const uint64_t size, node_t *right);
void
ablr(arena_t *arena, const uint64_t addr, const uint64_t size, node_t *left);
void
alloc_block(arena_t *arena, const uint64_t addr, const uint64_t size);
int
read_protected(node_t *miniblock);
int
mprotect_read(arena_t *arena, uint64_t addr, uint64_t size);
void
read(arena_t *arena, uint64_t addr, uint64_t size);
int
write_protected(node_t *miniblock);
int
mprotect_write(arena_t *arena, uint64_t addr, uint64_t size);
void
write(arena_t *arena, const uint64_t addr, const uint64_t size, int8_t *data);
void
pmap(const arena_t *arena);
int8_t
perm_pars(char s[50]);
void
mprotect(arena_t *arena, uint64_t addr, int8_t *permission);
