#pragma once
#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* TODO : add your implementation for doubly-linked list */
// typedef struct { } list_t;

typedef struct node_t
{
	void* data;
	struct node_t *prev;
	struct node_t *next;
} node_t;

typedef struct list_t
{
	node_t* head;
	uint64_t data_size;
	uint64_t size;
	node_t* tail;
} list_t;

typedef struct {
	uint64_t start_address;
	size_t size;
	void *miniblock_list;
} block_t;

typedef struct {
	uint64_t start_address;
	size_t size;
	uint8_t perm;
	void *rw_buffer;
} miniblock_t;

typedef struct {
	uint64_t arena_size;
	list_t *block_list;
} arena_t;

list_t* dll_create(unsigned int data_size);
node_t* dll_get_nth_node(list_t* list, unsigned int n);
void dll_add_nth_node(list_t* list, unsigned int n, const void* data);
int find_block_poz(list_t* list, uint64_t new_poz);
node_t* dll_remove_nth_node(list_t* list, unsigned int n);
int dll_intersectie_block(arena_t* arena, const uint64_t address, const uint64_t size);

arena_t *alloc_arena(const uint64_t size);
void dealloc_arena(arena_t *arena);

void alloc_block(arena_t *arena, const uint64_t address, const uint64_t size);
void free_block(arena_t *arena, const uint64_t address);

void read(arena_t *arena, uint64_t address, uint64_t size);
void write(arena_t *arena, const uint64_t address,  const uint64_t size, int8_t *data);
void pmap(const arena_t *arena);
void mprotect(arena_t *arena, uint64_t address, int8_t *permission);