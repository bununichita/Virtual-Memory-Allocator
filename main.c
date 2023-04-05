#include "vma.h"

int main() {
	arena_t *arena;
	while(1) {
		char command[50];
		// long nr, pos;
		scanf("%s", command);
		if(strncmp(command, "ALLOC_ARENA", 11) == 0){
			uint64_t size;
			scanf("%lu", &size);
			// arena = malloc(sizeof(arena_t));
			// arena->arena_size = size;
			arena = alloc_arena(size);
		}
		if(strncmp(command, "DEALLOC_ARENA", 13) == 0){
			dealloc_arena(arena);
			return 0;
		}
		if(strncmp(command, "ALLOC_BLOCK", 11) == 0){
			uint64_t addr, size;
			scanf("%lu %lu", &addr, &size);
			alloc_block(arena, (const uint64_t) addr, (const uint64_t) size);
		}
		if(strncmp(command, "FREE_BLOCK", 10) == 0){
			uint64_t addr;
			scanf("%lu", &addr);
			free_block(arena, (const uint64_t) addr);
		}
		if(strncmp(command, "READ", 4) == 0){
			uint64_t addr, size;
			scanf("%lu %lu", &addr, &size);
			read(arena, (const uint64_t) addr, (const uint64_t) size);
		}
		if(strncmp(command, "WRITE", 5) == 0){
			uint64_t addr, size;
			int8_t *data;
			scanf("%lu %lu ", &addr, &size);
			data = malloc(size * sizeof(int8_t));
			for (int i = 0; i < size; i++) {
				data[i] = getc(stdin);
			}
			write(arena, (const uint64_t) addr, (const uint64_t) size, data);
			free(data);
		}
		if(strncmp(command, "PMAP", 4) == 0){
			pmap(arena);
		}
        if(strncmp(command, "MPROTECT", 8) == 0) {
            uint64_t address;
            scanf("%lu ", &address);
            char s[50];
            fgets(s, 50, stdin);
			int8_t perm;
			perm = perm_pars(s);
			mprotect(arena, address, &perm);
		}
	}
	return 0;
}
