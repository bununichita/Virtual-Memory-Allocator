#include "vma.h"

list_t
*dll_create(unsigned int data_size)
{
	list_t *list;
	list = malloc(sizeof(list_t));
	list->head = NULL;
	list->tail = NULL;
	list->data_size = data_size;
	list->size = 0;
	return list;
}

int block_inters(arena_t *arena, const uint64_t addr, const uint64_t size)
{
	// Functie care intoarce 1 daca noul block se intersecteaza
	// cu unul deja stocat si 0 in caz contrar
	if (!arena->block_list->size)
		return 0;
	node_t *curr;
	curr = arena->block_list->head;
	for (uint64_t i = 0; i < arena->block_list->size; i++) {
		uint64_t start_old, stop_old, start_new, stop_new;
		start_old = ((block_t *)curr->data)->start_addr;
		stop_old = start_old + ((block_t *)curr->data)->size - 1;
		start_new = addr;
		stop_new = addr + size - 1;
		if (start_new >= start_old && start_new <= stop_old)
			return 1;
		if (stop_new >= start_old && stop_new <= stop_old)
			return 1;
		if (start_new < start_old && stop_new > stop_old)
			return 1;
		curr = curr->next;
	}
	return 0;
}

node_t
*merg_l(arena_t *arena, const uint64_t addr)
{
	// Functie care intoarce adresa nodului cu care se va lipi la stanga
	// sau NULL daca nu este nevoie de acest lucru
	if (!arena->block_list->size)
		return NULL;
	node_t *curr;
	curr = arena->block_list->head;
	if (addr < ((block_t *)curr->data)->start_addr)
		return NULL;
	for (uint64_t i = 0; i < arena->block_list->size; i++) {
		uint64_t start_addr = ((block_t *)curr->data)->start_addr;
		uint64_t block_size = ((block_t *)curr->data)->size;
		if (start_addr + block_size > addr)
			return NULL;
		if (start_addr + block_size == addr)
			return curr;
		curr = curr->next;
	}
	return NULL;
}

node_t
*merg_r(arena_t *arena, node_t *left, const uint64_t addr, const uint64_t size)
{
	// Functie care intoarce adresa nodului cu care se va lipi la dreapta
	// sau NULL daca nu este nevoie de acest lucru
	if (!arena->block_list->size)
		return NULL;
	node_t *curr;
	// In caz ca avem nod lipit la stanga stim sigur ca trebuie sa
	// verificam left->next
	if (left) {
		if (left->next) {
			curr = left->next;
			if (addr + size == ((block_t *)curr->data)->start_addr)
				return curr;
			else
				return NULL;
		} else {
			return NULL;
		}
	}
	curr = arena->block_list->tail;
	for (uint64_t i = 0; i < arena->block_list->size; i++) {
		if (addr + size > ((block_t *)curr->data)->start_addr)
			return NULL;
		if (addr + size == ((block_t *)curr->data)->start_addr)
			return curr;
		curr = curr->prev;
	}
	return NULL;
}

arena_t
*alloc_arena(const uint64_t size)
{
	arena_t *arena;
	arena = malloc(sizeof(arena_t));
	arena->arena_size = size;
	arena->block_list = dll_create(sizeof(block_t));
	return arena;
}

void
free_1_1(arena_t *arena, node_t *curr, uint64_t i, uint64_t addr)
{
	node_t *curr_mini;
	curr_mini = ((list_t *)((block_t *)curr->data)->mini_list)->head;
	if (((block_t *)curr->data)->start_addr == addr) {
		free(((miniblock_t *)curr_mini->data)->rw_buffer);
		free(curr_mini->data);
		free(curr_mini);
		free(((block_t *)curr->data)->mini_list);
		free(curr->data);
		if (arena->block_list->size == 1) {
			// Cazul in care block-ul este singur in lista
			free(curr);
			arena->block_list->size = 0;
			arena->block_list->head = NULL;
			arena->block_list->tail = NULL;
			return;
		}
		if (i == 0) {
			// Cazul in care block-ul este primul in lista
			arena->block_list->size--;
			arena->block_list->head = arena->block_list->head->next;
			arena->block_list->head->prev = NULL;
			free(curr);
			return;
		}
		if (i == arena->block_list->size - 1) {
			arena->block_list->size--;
			arena->block_list->tail = arena->block_list->tail->prev;
			arena->block_list->tail->next = NULL;
			free(curr);
			return;
		}
		// Cazul in care block-ul este in interiorul listei de block-uri
		arena->block_list->size--;
		curr->prev->next = curr->next;
		curr->next->prev = curr->prev;
		free(curr);
		return;
	}
	printf("Invalid address for free.\n");
}

void
free_1_m_first(node_t *curr, node_t *curr_mini)
{
	// Cazul in care miniblock-ul sters este primul in lista de miniblock-uri
	((block_t *)curr->data)->size -= ((miniblock_t *)curr_mini->data)->size;
	uint64_t statr_addr = ((miniblock_t *)curr_mini->next->data)->start_addr;
	((block_t *)curr->data)->start_addr = statr_addr;
	curr_mini->next->prev = NULL;
	((list_t *)((block_t *)curr->data)->mini_list)->head = curr_mini->next;
	((list_t *)((block_t *)curr->data)->mini_list)->size--;
	free(((miniblock_t *)curr_mini->data)->rw_buffer);
	free(curr_mini->data);
	free(curr_mini);
}

void
free_1_m_last(node_t *curr, node_t *curr_mini)
{
	// Cazul in care miniblock-ul este ultimul in lista de miniblock-uri
	((block_t *)curr->data)->size -= ((miniblock_t *)curr_mini->data)->size;
	curr_mini->prev->next = NULL;
	((list_t *)((block_t *)curr->data)->mini_list)->tail = curr_mini->prev;
	((list_t *)((block_t *)curr->data)->mini_list)->size--;
	free(((miniblock_t *)curr_mini->data)->rw_buffer);
	free(curr_mini->data);
	free(curr_mini);
}

void
free_1_m(arena_t *arena, node_t *curr, node_t *curr_mini, uint64_t addr)
{
	int valid_addr = 0;
	size_t new_block_size = 0;
	// nmn = new miniblock number
	uint64_t nmn = 0;
	size_t size = ((list_t *)((block_t *)curr->data)->mini_list)->size;
	for (uint64_t j = 0; j < size; j++) {
		if (((miniblock_t *)curr_mini->data)->start_addr == addr) {
			valid_addr = 1;
			if (j == 0) {
				// Cazul in care miniblock-ul sters este primul in lista
				// de miniblock - uri
				free_1_m_first(curr, curr_mini);
				return;
			}
			if (j == ((list_t *)((block_t *)curr->data)->mini_list)->size - 1) {
				// Cazul in care miniblock-ul este ultimul in lista
				// de miniblock - uri
				free_1_m_last(curr, curr_mini);
				return;
			}
			// Cazul in care miniblock-ul este la in interiorul listei
			// de miniblock - uri
			node_t *new;
			new = malloc(sizeof(node_t));
			new->next = NULL;
			new->prev = NULL;
			new->data = malloc(sizeof(block_t));
			uint64_t b_size, m_size;
			b_size = ((block_t *)curr->data)->size;
			m_size = ((miniblock_t *)curr_mini->data)->size;
			((block_t *)new->data)->size = b_size - new_block_size - m_size;
			uint64_t m_start_addr;
			m_start_addr = ((miniblock_t *)curr_mini->next->data)->start_addr;
			((block_t *)new->data)->start_addr = m_start_addr;
			((block_t *)new->data)->mini_list = dll_create(sizeof(miniblock_t));
			node_t *next = curr_mini->next;
			((list_t *)((block_t *)new->data)->mini_list)->head = next;
			node_t *tail = ((list_t *)((block_t *)curr->data)->mini_list)->tail;
			((list_t *)((block_t *)new->data)->mini_list)->tail = tail;
			size_t siz = ((list_t *)((block_t *)curr->data)->mini_list)->size;
			((list_t *)((block_t *)new->data)->mini_list)->size = siz - nmn - 1;
			if (arena->block_list->tail == curr) {
				curr->next = new;
				new->prev = curr;
				arena->block_list->tail = new;
				arena->block_list->size++;
			} else {
				new->next = curr->next;
				new->prev = curr;
				curr->next->prev = new;
				curr->next = new;
				arena->block_list->size++;
			}
			((block_t *)curr->data)->size = new_block_size;
			((list_t *)((block_t *)curr->data)->mini_list)->size = nmn;
			node_t *prev = curr_mini->prev;
			((list_t *)((block_t *)curr->data)->mini_list)->tail = prev;
			((list_t *)((block_t *)curr->data)->mini_list)->tail->next = NULL;
			free(((miniblock_t *)curr_mini->data)->rw_buffer);
			free(curr_mini->data);
			free(curr_mini);
			return;
		}
		new_block_size += ((miniblock_t *)curr_mini->data)->size;
		nmn++;
		curr_mini = curr_mini->next;
	}
	if (!valid_addr) {
		printf("Invalid address for free.\n");
		return;
	}
}

void
free_block(arena_t *arena, const uint64_t addr)
{
	if (!arena->block_list->size) {
		printf("Invalid address for free.\n");
		return;
	}
	node_t *curr;
	curr = arena->block_list->head;
	for (uint64_t i = 0; i < arena->block_list->size; i++) {
		uint64_t start_block, size_block;
		start_block = ((block_t *)curr->data)->start_addr;
		size_block = ((block_t *)curr->data)->size;
		if (start_block <= addr && addr < start_block + size_block) {
			node_t *curr_mini;
			curr_mini = ((list_t *)((block_t *)curr->data)->mini_list)->head;
			if (((list_t *)((block_t *)curr->data)->mini_list)->size == 1) {
				// Cazul in care miniblock-ul este singurul din block
				free_1_1(arena, curr, i, addr);
				return;
			}
			free_1_m(arena, curr, curr_mini, addr);
			return;
		}
		curr = curr->next;
	}
	printf("Invalid address for free.\n");
}

void
dealloc_arena(arena_t *arena)
{
	if (!arena->block_list->size) {
		free(arena->block_list);
		free(arena);
		return;
	}

	uint64_t b_l_s = arena->block_list->size;

	for (uint64_t i = 0; i < b_l_s; i++) {
		node_t *curr;
		curr = arena->block_list->head;
		node_t *next_b;
		if (i != b_l_s - 1)
			next_b = curr->next;
		node_t *curr_mini;
		curr_mini = ((list_t *)((block_t *)curr->data)->mini_list)->head;
		uint64_t m_l_s = ((list_t *)((block_t *)curr->data)->mini_list)->size;
		for (uint64_t j = 0; j < m_l_s; j++) {
			node_t *next_m;
			if (j != m_l_s - 1)
				next_m = curr_mini->next;
			free_block(arena, ((miniblock_t *)curr_mini->data)->start_addr);
			if (j != m_l_s - 1)
				curr_mini = next_m;
		}
		if (i != b_l_s - 1)
			curr = next_b;
	}
	free(arena->block_list);
	free(arena);
}

void
alloc_block_simple(arena_t *arena, const uint64_t addr, const uint64_t size)
{
	if (!arena->block_list->size) {
		node_t *new, *new2;
		new = malloc(sizeof(node_t));
		new->next = NULL;
		new->prev = NULL;
		new->data = malloc(sizeof(block_t));
		((block_t *)new->data)->size = size;
		((block_t *)new->data)->start_addr = addr;
		((block_t *)new->data)->mini_list = dll_create(sizeof(miniblock_t));
		new2 = malloc(sizeof(node_t));
		new2->next = NULL;
		new2->prev = NULL;
		new2->data = malloc(sizeof(miniblock_t));
		((miniblock_t *)new2->data)->perm = 6;
		((miniblock_t *)new2->data)->size = size;
		((miniblock_t *)new2->data)->start_addr = addr;
		((miniblock_t *)new2->data)->rw_buffer = malloc(1);
		((list_t *)((block_t *)new->data)->mini_list)->head = new2;
		((list_t *)((block_t *)new->data)->mini_list)->tail = new2;
		((list_t *)((block_t *)new->data)->mini_list)->size++;
		arena->block_list->head = new;
		arena->block_list->tail = new;
		arena->block_list->size++;
		return;
	}
	node_t *new, *new2;
	new = malloc(sizeof(node_t));
	new->next = NULL;
	new->prev = NULL;
	new->data = malloc(sizeof(block_t));
	((block_t *)new->data)->size = size;
	((block_t *)new->data)->start_addr = addr;
	((block_t *)new->data)->mini_list = dll_create(sizeof(miniblock_t));
	new2 = malloc(sizeof(node_t));
	new2->next = NULL;
	new2->prev = NULL;
	new2->data = malloc(sizeof(miniblock_t));
	((miniblock_t *)new2->data)->perm = 6;
	((miniblock_t *)new2->data)->size = size;
	((miniblock_t *)new2->data)->start_addr = addr;
	((miniblock_t *)new2->data)->rw_buffer = malloc(1);
	((list_t *)((block_t *)new->data)->mini_list)->head = new2;
	((list_t *)((block_t *)new->data)->mini_list)->tail = new2;
	((list_t *)((block_t *)new->data)->mini_list)->size++;
	node_t *curr, *previous;
	previous = arena->block_list->tail;
	curr = arena->block_list->head;
	for (uint64_t i = 0; i < arena->block_list->size; i++) {
		if (addr < ((block_t *)curr->data)->start_addr) {
			previous = curr->prev;
			break;
		}
		curr = curr->next;
	}
	if (!previous) {
		new->next = arena->block_list->head;
		arena->block_list->head->prev = new;
		arena->block_list->head = new;
		arena->block_list->size++;
	} else if (previous == arena->block_list->tail) {
		new->prev = previous;
		previous->next = new;
		arena->block_list->tail = new;
		arena->block_list->size++;
	} else {
		new->next = previous->next;
		new->prev = previous;
		previous->next = new;
		new->next->prev = new;
		arena->block_list->size++;
	}
}

void
alloc_block_left(const uint64_t addr, const uint64_t size, node_t *left)
{
	((block_t *)left->data)->size += size;
	((list_t *)((block_t *)left->data)->mini_list)->size++;
	node_t *new;
	new = malloc(sizeof(node_t));
	new->data = malloc(sizeof(miniblock_t));
	((miniblock_t *)new->data)->perm = 6;
	((miniblock_t *)new->data)->size = size;
	((miniblock_t *)new->data)->start_addr = addr;
	((miniblock_t *)new->data)->rw_buffer = malloc(1);
	new->prev = ((list_t *)((block_t *)left->data)->mini_list)->tail;
	new->next = NULL;
	((list_t *)((block_t *)left->data)->mini_list)->tail->next = new;
	((list_t *)((block_t *)left->data)->mini_list)->tail = new;
}

void
alloc_block_right(const uint64_t addr, const uint64_t size, node_t *right)
{
	((block_t *)right->data)->size += size;
	((block_t *)right->data)->start_addr = addr;
	((list_t *)((block_t *)right->data)->mini_list)->size++;
	node_t *new;
	new = malloc(sizeof(node_t));
	new->data = malloc(sizeof(miniblock_t));
	((miniblock_t *)new->data)->perm = 6;
	((miniblock_t *)new->data)->size = size;
	((miniblock_t *)new->data)->start_addr = addr;
	((miniblock_t *)new->data)->rw_buffer = malloc(1);
	new->prev = NULL;
	new->next = ((list_t *)((block_t *)right->data)->mini_list)->head;
	((list_t *)((block_t *)right->data)->mini_list)->head->prev = new;
	((list_t *)((block_t *)right->data)->mini_list)->head = new;
}

void
ablr(arena_t *arena, const uint64_t addr, const uint64_t size, node_t *left)
{
	// alloc_block_left_right
	node_t *right = left->next;
	arena->block_list->size--;
	((block_t *)left->data)->size += size;
	((block_t *)left->data)->size += ((block_t *)right->data)->size;
	((list_t *)((block_t *)left->data)->mini_list)->size++;
	uint64_t r_size = ((list_t *)((block_t *)right->data)->mini_list)->size;
	((list_t *)((block_t *)left->data)->mini_list)->size += r_size;
	node_t *new;
	new = malloc(sizeof(node_t));
	new->data = malloc(sizeof(miniblock_t));
	((miniblock_t *)new->data)->perm = 6;
	((miniblock_t *)new->data)->size = size;
	((miniblock_t *)new->data)->start_addr = addr;
	((miniblock_t *)new->data)->rw_buffer = malloc(1);
	new->prev = ((list_t *)((block_t *)left->data)->mini_list)->tail;
	new->next = ((list_t *)((block_t *)right->data)->mini_list)->head;
	((list_t *)((block_t *)left->data)->mini_list)->tail->next = new;
	node_t *r_tail;
	r_tail = ((list_t *)((block_t *)right->data)->mini_list)->tail;
	((list_t *)((block_t *)left->data)->mini_list)->tail = r_tail;
	((list_t *)((block_t *)right->data)->mini_list)->head->prev = new;
	((list_t *)((block_t *)right->data)->mini_list)->head = NULL;
	((list_t *)((block_t *)right->data)->mini_list)->tail = NULL;
	free(((block_t *)right->data)->mini_list);
	free(right->data);
	if (right == arena->block_list->tail) {
		arena->block_list->tail = left;
		left->next = NULL;
		free(right);
	} else {
		right->next->prev = left;
		left->next = right->next;
		free(right);
	}
}

void
alloc_block(arena_t *arena, const uint64_t addr, const uint64_t size)
{
	if (addr >= arena->arena_size) {
		printf("The allocated address is outside the size of arena\n");
		return;
	}
	if (arena->arena_size < addr + size) {
		printf("The end address is past the size of the arena\n");
		return;
	}
	if (block_inters(arena, addr, size)) {
		printf("This zone was already allocated.\n");
		return;
	}
	node_t *left = merg_l(arena, addr);
	node_t *right = merg_r(arena, left, addr, size);
	if (!left && !right)
		alloc_block_simple(arena, addr, size);
	if (left && !right)
		alloc_block_left(addr, size, left);
	if (!left && right)
		alloc_block_right(addr, size, right);
	if (left && right)
		ablr(arena, addr, size, left);
}

int
read_protected(node_t *miniblock)
{
	int8_t perm;
	perm = ((miniblock_t *)miniblock->data)->perm;
	if (perm < 4)
		return 1;
	return 0;
}

int
mpr_aux2(uint64_t addr, uint64_t cut, uint64_t aux, node_t *curr_mini)
{
	if (addr + cut <= aux) {
		if (read_protected(curr_mini)) {
			printf("Invalid permissions for read.\n");
			return 1;
		}
		return 0;
	}
	uint64_t alloc_size;
	alloc_size = aux - addr;
	if (read_protected(curr_mini)) {
		printf("Invalid permissions for read.\n");
		return 1;
	}
	cut -= alloc_size;
	curr_mini = curr_mini->next;
	while (cut > 0) {
		if (cut > ((miniblock_t *)curr_mini->data)->size) {
			if (read_protected(curr_mini)) {
				printf("Invalid permissions for read.\n");
				return 1;
			}
			cut -= ((miniblock_t *)curr_mini->data)->size;
		} else {
			if (read_protected(curr_mini)) {
				printf("Invalid permissions for read.\n");
				return 1;
			}
			return 0;
		}
		curr_mini = curr_mini->next;
	}
}

int
mpr_aux1(node_t *curr, uint64_t cut, uint64_t addr)
{
	node_t *curr_mini;
	curr_mini = ((list_t *)((block_t *)curr->data)->mini_list)->head;
	uint64_t size = ((list_t *)((block_t *)curr->data)->mini_list)->size;
	for (uint64_t j = 0; j < size; j++) {
		uint64_t start_addr, size;
		start_addr = ((miniblock_t *)curr_mini->data)->start_addr;
		size = ((miniblock_t *)curr_mini->data)->size;
		if (start_addr <= addr && addr < start_addr + size) {
			uint64_t aux;
			aux = start_addr + size;
			return mpr_aux2(addr, cut, aux, curr_mini);
		}
		curr_mini = curr_mini->next;
	}
}

int
mprotect_read(arena_t *arena, uint64_t addr, uint64_t size)
{
	node_t *curr;
	curr = arena->block_list->head;
	for (uint64_t i = 0; i < arena->block_list->size; i++) {
		uint64_t start_block, size_block;
		start_block = ((block_t *)curr->data)->start_addr;
		size_block = ((block_t *)curr->data)->size;
		if (start_block <= addr && addr < start_block + size_block) {
			uint64_t cut;
			if (addr + size > start_block + size_block) {
				cut = start_block + size_block - addr;
				printf("Warning: size was bigger than the block size.");
				printf(" Reading %lu characters.\n", cut);
			} else {
				cut = size;
			}
			return mpr_aux1(curr, cut, addr);
		}
		curr = curr->next;
	}
	printf("Invalid address for read.\n");
	return 1;
}

void
read_aux2(node_t *curr_mini, uint64_t cut, uint64_t addr)
{
	uint64_t aux;
	uint64_t start_addr, size;
	start_addr = ((miniblock_t *)curr_mini->data)->start_addr;
	size = ((miniblock_t *)curr_mini->data)->size;
	aux = start_addr + size;
	if (addr + cut <= aux) {
		uint64_t start_read;
		start_read = addr - start_addr;
		void **buff;
		buff = &(((miniblock_t *)curr_mini->data)->rw_buffer);
		for (uint64_t count = 0; count < cut; count++)
			printf("%c", ((int8_t *)(*buff))[count + start_read]);
		printf("\n");
		return;
	}
	uint64_t alloc_size;
	alloc_size = aux - addr;
	uint64_t start_read;
	start_read = addr - ((miniblock_t *)curr_mini->data)->start_addr;
	void **buff;
	buff = &(((miniblock_t *)curr_mini->data)->rw_buffer);
	for (uint64_t count = 0; count < alloc_size; count++)
		printf("%c", ((int8_t *)(*buff))[count + start_read]);
	cut -= alloc_size;
	curr_mini = curr_mini->next;
	while (cut > 0) {
		uint64_t size;
		size = ((miniblock_t *)curr_mini->data)->size;
		void **buff;
		buff = &(((miniblock_t *)curr_mini->data)->rw_buffer);
		if (cut > size) {
			for (uint64_t count = 0; count < size; count++)
				printf("%c", ((int8_t *)(*buff))[count]);
			cut -= ((miniblock_t *)curr_mini->data)->size;
		} else {
			for (uint64_t count = 0; count < cut; count++)
				printf("%c", ((int8_t *)(*buff))[count]);
			printf("\n");
			return;
		}
		curr_mini = curr_mini->next;
	}
}

void
read_aux1(node_t *curr, uint64_t cut, uint64_t addr)
{
	node_t *curr_mini;
	curr_mini = ((list_t *)((block_t *)curr->data)->mini_list)->head;
	uint64_t size = ((list_t *)((block_t *)curr->data)->mini_list)->size;
	for (uint64_t j = 0; j < size; j++) {
		uint64_t start_addr, size;
		start_addr = ((miniblock_t *)curr_mini->data)->start_addr;
		size = ((miniblock_t *)curr_mini->data)->size;
		if (start_addr <= addr && addr < start_addr + size) {
			read_aux2(curr_mini, cut, addr);
			return;
		}
		curr_mini = curr_mini->next;
	}
}

void
read(arena_t *arena, uint64_t addr, uint64_t size)
{
	if (!arena->block_list->size) {
		printf("Invalid address for read.\n");
		return;
	}
	if (mprotect_read(arena, addr, size))
		return;
	node_t *curr;
	curr = arena->block_list->head;
	for (uint64_t i = 0; i < arena->block_list->size; i++) {
		uint64_t start_block, size_block;
		start_block = ((block_t *)curr->data)->start_addr;
		size_block = ((block_t *)curr->data)->size;
		if (start_block <= addr && addr < start_block + size_block) {
			uint64_t cut;
			if (addr + size > start_block + size_block)
				cut = start_block + size_block - addr;
			else
				cut = size;
			read_aux1(curr, cut, addr);
			return;
		}
		curr = curr->next;
	}
}

int
write_protected(node_t *miniblock)
{
	int8_t perm;
	perm = ((miniblock_t *)miniblock->data)->perm;
	if (1 < perm && perm < 4)
		return 0;
	if (5 < perm)
		return 0;
	return 1;
}

int
mpw2_aux(uint64_t addr, uint64_t cut, uint64_t aux, node_t *curr_mini)
{
	if (addr + cut <= aux) {
		if (write_protected(curr_mini)) {
			printf("Invalid permissions for write.\n");
			return 1;
		}
		return 0;
	}
	uint64_t alloc_size;
	alloc_size = aux - addr;
	if (write_protected(curr_mini)) {
		printf("Invalid permissions for write.\n");
		return 1;
	}
	cut -= alloc_size;
	curr_mini = curr_mini->next;
	while (cut > 0) {
		if (cut > ((miniblock_t *)curr_mini->data)->size) {
			if (write_protected(curr_mini)) {
				printf("Invalid permissions for write.\n");
				return 1;
			}
			cut -= ((miniblock_t *)curr_mini->data)->size;
		} else {
			if (write_protected(curr_mini)) {
				printf("Invalid permissions for write.\n");
				return 1;
			}
			return 0;
		}
		curr_mini = curr_mini->next;
	}
}

int
mpw1_aux(node_t *curr, uint64_t cut, uint64_t addr)
{
	node_t *curr_mini;
	curr_mini = ((list_t *)((block_t *)curr->data)->mini_list)->head;
	size_t size = ((list_t *)((block_t *)curr->data)->mini_list)->size;
	for (uint64_t j = 0; j < size; j++) {
		uint64_t start_addr, size;
		start_addr = ((miniblock_t *)curr_mini->data)->start_addr;
		size = ((miniblock_t *)curr_mini->data)->size;
		if (start_addr <= addr && addr < start_addr + size) {
			uint64_t aux;
			aux = start_addr + size;
			return mpw2_aux(addr, cut, aux, curr_mini);
		}
		curr_mini = curr_mini->next;
	}
}

int
mprotect_write(arena_t *arena, uint64_t addr, uint64_t size)
{
	node_t *curr;
	curr = arena->block_list->head;
	for (uint64_t i = 0; i < arena->block_list->size; i++) {
		uint64_t start_block, size_block;
		start_block = ((block_t *)curr->data)->start_addr;
		size_block = ((block_t *)curr->data)->size;
		if (start_block <= addr && addr < start_block + size_block) {
			uint64_t cut;
			if (addr + size > start_block + size_block) {
				cut = start_block + size_block - addr;
				printf("Warning: size was bigger than the block size.");
				printf(" Writing %lu characters.\n", cut);
			} else {
				cut = size;
			}
			node_t *curr_mini;
			return mpw1_aux(curr, cut, addr);
		}
		curr = curr->next;
	}
	printf("Invalid address for write.\n");
	return 1;
}

void
write_aux2(node_t *curr_mini, uint64_t cut, uint64_t addr, int8_t *data)
{
	uint64_t aux, start_addr, size;
	start_addr = ((miniblock_t *)curr_mini->data)->start_addr;
	size = ((miniblock_t *)curr_mini->data)->size;
	aux = start_addr + size;
	if (addr + cut <= aux) {
		free(((miniblock_t *)curr_mini->data)->rw_buffer);
		void **buff;
		buff = &(((miniblock_t *)curr_mini->data)->rw_buffer);
		*buff = malloc(size * sizeof(int8_t));
		uint64_t start_write;
		start_write = addr - start_addr;
		for (uint64_t count = 0; count < cut; count++)
			((int8_t *)(*buff))[start_write + count] = data[count];
		return;
	}
	uint64_t alloc_size;
	alloc_size = aux - addr;
	uint64_t start_write;
	start_write = addr - ((miniblock_t *)curr_mini->data)->start_addr;
	free(((miniblock_t *)curr_mini->data)->rw_buffer);
	void **buff;
	buff = &(((miniblock_t *)curr_mini->data)->rw_buffer);
	*buff = malloc(size * sizeof(int8_t));
	for (uint64_t count = 0; count < alloc_size; count++)
		((int8_t *)(*buff))[start_write + count] = data[count];
	uint64_t next_count = alloc_size;
	cut -= alloc_size;
	curr_mini = curr_mini->next;
	while (cut > 0) {
		uint64_t aux, start_addr, size;
		start_addr = ((miniblock_t *)curr_mini->data)->start_addr;
		size = ((miniblock_t *)curr_mini->data)->size;
		void **buff;
		buff = &(((miniblock_t *)curr_mini->data)->rw_buffer);
		if (cut > size) {
			free(((miniblock_t *)curr_mini->data)->rw_buffer);
			*buff = malloc(size * sizeof(int8_t));
			for (uint64_t count = 0; count < size; count++) {
				((int8_t *)(*buff))[count] = data[next_count];
				next_count++;
			}
			cut -= ((miniblock_t *)curr_mini->data)->size;
		} else {
			free(((miniblock_t *)curr_mini->data)->rw_buffer);
			*buff = malloc(size * sizeof(int8_t));
			for (uint64_t count = 0; count < cut; count++) {
				((int8_t *)(*buff))[count] = data[next_count];
				next_count++;
			}
			return;
		}
		curr_mini = curr_mini->next;
	}
}

void
write_aux1(node_t *curr, int8_t *data, uint64_t cut, uint64_t addr)
{
	node_t *curr_mini;
	curr_mini = ((list_t *)((block_t *)curr->data)->mini_list)->head;
	size_t size = ((list_t *)((block_t *)curr->data)->mini_list)->size;
	for (uint64_t j = 0; j < size; j++) {
		uint64_t start_addr, min_size;
		start_addr = ((miniblock_t *)curr_mini->data)->start_addr;
		min_size = ((miniblock_t *)curr_mini->data)->size;
		if (start_addr <= addr && addr < start_addr + min_size) {
			write_aux2(curr_mini, cut, addr, data);
			return;
		}
		curr_mini = curr_mini->next;
	}
}

void
write(arena_t *arena, const uint64_t addr, const uint64_t size, int8_t *data)
{
	if (!arena->block_list->size) {
		printf("Invalid address for write.\n");
		return;
	}
	if (mprotect_write(arena, addr, size))
		return;

	node_t *curr;
	curr = arena->block_list->head;
	for (uint64_t i = 0; i < arena->block_list->size; i++) {
		uint64_t start_block, size_block;
		start_block = ((block_t *)curr->data)->start_addr;
		size_block = ((block_t *)curr->data)->size;
		if (start_block <= addr && addr < start_block + size_block) {
			uint64_t cut;
			if (addr + size > start_block + size_block)
				cut = start_block + size_block - addr;
			else
				cut = size;
			write_aux1(curr, data, cut, addr);
			return;
		}
		curr = curr->next;
	}
}

void
pmap(const arena_t *arena)
{
	printf("Total memory: 0x%lX bytes\n", arena->arena_size);
	uint64_t free_mem = 0;
	uint64_t miniblocks_number = 0;
	if (!arena->block_list->size) {
		free_mem = 0;
	} else {
		node_t *curr;
		curr = arena->block_list->head;
		for (uint64_t i = 0; i < arena->block_list->size; i++) {
			free_mem += ((block_t *)curr->data)->size;
			size_t size = ((list_t *)((block_t *)curr->data)->mini_list)->size;
			miniblocks_number += size;
			curr = curr->next;
		}
	}
	free_mem = arena->arena_size - free_mem;
	printf("Free memory: 0x%lX bytes\n", free_mem);
	printf("Number of allocated blocks: %lu\n", arena->block_list->size);
	printf("Number of allocated miniblocks: %lu\n", miniblocks_number);
	if (!arena->block_list->size)
		return;
	node_t *curr;
	curr = arena->block_list->head;
	for (uint64_t i = 1; i <= arena->block_list->size; i++) {
		printf("\nBlock %lu begin\n", i);
		uint64_t start_addr = ((block_t *)curr->data)->start_addr;
		uint64_t stop_addr = ((block_t *)curr->data)->start_addr;
		stop_addr += ((block_t *)curr->data)->size;
		printf("Zone: 0x%lX - 0x%lX\n", start_addr, stop_addr);
		node_t *curr_mini;
		curr_mini = ((list_t *)((block_t *)curr->data)->mini_list)->head;
		size_t size = ((list_t *)((block_t *)curr->data)->mini_list)->size;
		for (uint64_t j = 1; j <= size; j++) {
			uint64_t start, stop;
			start = ((miniblock_t *)curr_mini->data)->start_addr;
			stop = ((miniblock_t *)curr_mini->data)->start_addr;
			stop += ((miniblock_t *)curr_mini->data)->size;
			char permission[5];
			if (((miniblock_t *)curr_mini->data)->perm == 0)
				strcpy(permission, "---");
			else if (((miniblock_t *)curr_mini->data)->perm == 1)
				strcpy(permission, "--X");
			else if (((miniblock_t *)curr_mini->data)->perm == 2)
				strcpy(permission, "-W-");
			else if (((miniblock_t *)curr_mini->data)->perm == 3)
				strcpy(permission, "-WX");
			else if (((miniblock_t *)curr_mini->data)->perm == 4)
				strcpy(permission, "R--");
			else if (((miniblock_t *)curr_mini->data)->perm == 5)
				strcpy(permission, "R-X");
			else if (((miniblock_t *)curr_mini->data)->perm == 6)
				strcpy(permission, "RW-");
			else if (((miniblock_t *)curr_mini->data)->perm == 7)
				strcpy(permission, "RWX");
			printf("Miniblock %lu:\t\t0x%lX\t\t-", j, start);
			printf("\t\t0x%lX\t\t| %s\n", stop, permission);
			curr_mini = curr_mini->next;
		}
		printf("Block %lu end\n", i);
		curr = curr->next;
	}
}

int8_t
perm_pars(char s[50])
{
	char *p;
	int8_t perm = 0;
	p = strtok(s, " | ");
	while (p) {
		if (strncmp(p, "PROT_NONE", 9) == 0)
			return 0;
		if (strncmp(p, "PROT_READ", 9) == 0)
			perm += 4;
		if (strncmp(p, "PROT_WRITE", 10) == 0)
			perm += 2;
		if (strncmp(p, "PROT_EXEC", 9) == 0)
			perm += 1;
		p = strtok(NULL, " | ");
	}
	return perm;
}

void
mprotect(arena_t *arena, uint64_t addr, int8_t *permission)
{
	if (!arena->block_list->size) {
		printf("Invalid address for mprotect.\n");
		return;
	}
	node_t *curr;
	curr = arena->block_list->head;
	for (uint64_t i = 0; i < arena->block_list->size; i++) {
		uint64_t start_block, size_block;
		start_block = ((block_t *)curr->data)->start_addr;
		size_block = ((block_t *)curr->data)->size;
		if (start_block <= addr && addr < start_block + size_block) {
			node_t *curr_mini;
			curr_mini = ((list_t *)((block_t *)curr->data)->mini_list)->head;
			size_t size = ((list_t *)((block_t *)curr->data)->mini_list)->size;
			for (uint64_t j = 0; j < size; j++) {
				if (((miniblock_t *)curr_mini->data)->start_addr == addr) {
					((miniblock_t *)curr_mini->data)->perm = *permission;
					return;
				}
				curr_mini = curr_mini->next;
			}
		}
		curr = curr->next;
	}
	printf("Invalid address for mprotect.\n");
}
