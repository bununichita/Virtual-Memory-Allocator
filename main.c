#include "vma.h"

int main(void) {
	arena_t *arena;
	while(1) {
		char command[50];
		scanf("%s", command);
		if (strcmp(command, "ALLOC_ARENA") == 0){
			uint64_t size;
			scanf("%lu", &size);
			arena = alloc_arena(size);
		} else if (strcmp(command, "DEALLOC_ARENA") == 0){
			dealloc_arena(arena);
			return 0;
		} else if (strcmp(command, "ALLOC_BLOCK") == 0){
			uint64_t addr, size;
			scanf("%lu %lu", &addr, &size);
			alloc_block(arena, (const uint64_t) addr, (const uint64_t) size);
		} else if (strcmp(command, "FREE_BLOCK") == 0){
			uint64_t addr;
			scanf("%lu", &addr);
			free_block(arena, (const uint64_t) addr);
		} else if (strcmp(command, "READ") == 0){
			uint64_t addr, size;
			scanf("%lu %lu", &addr, &size);
			read(arena, (const uint64_t) addr, (const uint64_t) size);
		} else if (strcmp(command, "WRITE") == 0){
			uint64_t addr, size;
			int8_t *data;
			scanf("%lu %lu", &addr, &size);
			getc(stdin);
			data = malloc(size * sizeof(int8_t));
			for (uint64_t i = 0; i < size; i++) {
				data[i] = getc(stdin);
			}
			write(arena, (const uint64_t) addr, (const uint64_t) size, data);
			free(data);
		} else if (strcmp(command, "PMAP") == 0){
			pmap(arena);
		} else if (strcmp(command, "MPROTECT") == 0) {
            uint64_t address;
            scanf("%lu ", &address);
            char s[50];
            fgets(s, 50, stdin);
			int8_t perm;
			perm = perm_pars(s);
			mprotect(arena, address, &perm);
		} else {
			printf("Invalid command. Please try again.\n");
		}
	}
	return 0;
}
