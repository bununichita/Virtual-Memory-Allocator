#include "vma.h"

// list_t*
// dll_create(unsigned int data_size)
// {
// 	/* TODO */
//     list_t *p;
// 	p = NULL;
// 	p = malloc(sizeof(list_t));
// 	(p->head) = malloc(sizeof(node_t*));
// 	p->tail = p->head;
// 	(p->data_size) = data_size;
// 	(p->size) = 0;
// 	return p;
// }

// node_t*
// dll_get_nth_node(list_t* list, unsigned int n)
// {
// 	/* TODO */
//     n %= (list->size);
// 	node_t* current;
// 	current = (list->head);
// 	for (int i = 0; i < n; i++) {
// 		current = (current->next);
// 	}
// 	return current;
// }

// void
// dll_add_nth_node(list_t* list, unsigned int n, const void* data)
// {
// 	/* TODO */
//     // if (*(int *)(data) == 477534464 && list->data_size == sizeof(int)) {
// 	// 	*(int *)(data) = 0;
// 	// }
// 	if ((int)n < 0) {
// 		// printf("test<\n");
// 		return;
// 	}
// 	else if (n < (list->size)) {
// 		if (n == 0) {
// 			//printf("test0\n");
// 			node_t *new;
// 			new = malloc(sizeof(node_t));
// 			(new->data) = malloc((list->data_size));
// 			memcpy((new->data), (void*)data, (list->data_size));
// 			//(new->data) = (void*)new_data;
// 			(new->next) = (list->head);
// 			(list->head->prev) = new;
// 			(new->prev) = NULL;
// 			(list->head) = new;
// 			(list->size)++;
// 		} else {
// 			node_t *current;
// 			current = (list->head);
// 			for (int i = 0; i < n - 1; i++) {
// 				current = (current->next);
// 			}
// 			node_t *new;
// 			new = malloc(sizeof(node_t));
// 			(new->data) = malloc((list->data_size));
// 			memcpy((new->data), (void*)data, (list->data_size));
// 			// (new->data) = (void*)new_data;
// 			(new->next) = (current->next);
// 			(current->next->prev) = new;
// 			(new->prev) = current;
// 			(current->next) = new;
// 			(list->size)++;
// 		}		
// 	} else {
// 		if ((list->size) == 0) {
// 		    if (!(list->head)) {
// 				(list->head) = malloc(sizeof(node_t));
// 			}
// 			// printf("test1\n");
// 			(list->head->data) = malloc((list->data_size));
// 			memcpy((list->head->data), (void*)data, (list->data_size));
// 			// (list->head->data) = (void*)new_data;
// 			(list->head->next) = NULL;
// 			(list->head->prev) = NULL;
// 			list->tail = list->head;
// 			//printf("%s\n", *(char *)(list->head->data));
// 			(list->size)++;
// 		} else {
// 			node_t *current;
// 			current = (list->head);
// 			for (int i = 0; i < (list->size) - 1; i++) {
// 				current = (current->next);
// 			}
// 			node_t *new;
// 			new = malloc(sizeof(node_t));
// 			(new->data) = malloc((list->data_size));
// 			memcpy((new->data), (void*)data, (list->data_size));
// 			// (new->data) = (void*)new_data;
// 			(new->next) = NULL;
// 			(new->prev) = current;
// 			(current->next) = new;
// 			list->tail = current->next;
// 			(list->size)++;
// 		}
// 	}
// }

// // void add_block(list_t* arena, uint64_t poz, void* data, uint64_t address, uint64_t size)
// // {

// // }

// int find_block_poz(list_t* list, uint64_t new_poz)
// {
// 	node_t* curr;
// 	curr = list->head;
// 	if (list->size == 0) {
// 		return 0;
// 	}
// 	if (((block_t*)curr)->start_address > new_poz) {
// 		return 0;
// 	}
// 	int return_poz = 1;
// 	while (curr->next) {
// 		if (((block_t*)curr->next)->start_address > new_poz) {
// 			return return_poz;
// 		}
// 		return_poz++;
// 		curr = curr->next;
// 	}
// 	return list->size;
// }

// node_t*
// dll_remove_nth_node(list_t* list, unsigned int n)
// {
//     /* TODO */
// 	node_t *aux;
// 	if (n < 0)
// 		return NULL;
// 	node_t *current;
// 	if (n == 0) {
// 		current = (list->head);
// 		(list->head) = (list->head->next);
// 		(list->head->prev) = NULL;
// 		// for (int i = 0; i < (list->size) - 1; i++) {
// 		// 	current = (current->next);
// 		// }
// 		// aux = (list->head);
// 		//(list->head) = (list->head->next);
// 		(list->size)--;
// 	} else if (n < (list->size)) {
// 		current = (list->head);
// 		for (int i = 0; i < n - 1; i++) {
// 			current = (current->next);
// 		}
// 		aux = (current->next);
// 		(current->next) = (aux->next);
// 		(aux->next->prev) = current;
// 		current = aux;
// 		(list->size)--;
// 	} else {
// 		current = (list->head);
// 		for (int i = 0; i < (list->size) - 1; i++) {
// 			current = (current->next);
// 		}
// 		(current->prev->next) = NULL;
// 		list->tail = current->prev;
// 		(list->size)--;
// 	}
// 	return current;
// }

list_t *dll_create(unsigned int data_size)
{
    list_t* list;
    list = malloc(sizeof(list_t));

    list->head = NULL;
	list->tail = NULL;
    list->data_size = data_size;
    list->size = 0;

    return list;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, eroare.
 */
void dll_add_nth_node(list_t* list, unsigned int n, const void* new_data)
{
    
	if (!list->size) {
		node_t* new;
		new = malloc(sizeof(node_t));
		new->next = NULL;
		new->prev = NULL;
		new->data = malloc(list->data_size);
		memcpy(new->data, new_data, list->data_size);
		list->head = new;
		list->tail = new;
		list->size++;
	}

	if (n > list->size) {
        n = list->size;
    }

	if (!n) {
		node_t* new;
		new = malloc(sizeof(node_t));
		new->prev = NULL;
		new->next = list->head;
		new->data = malloc(list->data_size);
		memcpy(new->data, new_data, list->data_size);
		list->head = new;
		list->size++;
	}

	if (n < list->size) {
		node_t* curr;
		curr = list->head;
		for (int i = 1; i < n; i++) {
			curr = curr->next;
		}
		
		node_t* new;
		new = malloc(sizeof(node_t));
		new->next = curr->next;
		new->prev = curr;
		curr->next->prev = new;
		curr->next = new;
		new->data = malloc(list->data_size);
		memcpy(new->data, new_data, list->data_size);
		list->size++;
	}
	
	if (n == list->size) {
		node_t* new;
		new = malloc(sizeof(node_t));
		new->next = NULL;
		new->prev = list->tail;
		list->tail->next = new;
		new->data = malloc(list->data_size);
		memcpy(new->data, new_data, list->data_size);
		list->tail = new;
		list->size++;
    }
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, eroare. Functia intoarce un pointer spre acest
 * nod proaspat eliminat din lista. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
node_t *ll_remove_nth_node(list_t* list, unsigned int n)
{
    if (!list->size) {
		printf("list empty\n");
		return NULL;
	}

	if (list->size == 1) {
		node_t* removed;
		removed = list->head;
		list->head = NULL;
		list->tail = NULL;
		list->size--;
		return removed;
	}
	
	if (n > list->size - 1) {
        n = list->size - 1;
    }

	if (!n) {
		node_t* removed;
		removed = list->head;
		list->head = list->head->next;
		list->head->prev = NULL;
		list->size--;
		return removed;
	}

	if (n < list->size - 1) {
		node_t* curr;
		curr = list->head;
		for (int i = 0; i < n; i++) {
			curr = curr->next;
		}
		curr->prev->next = curr->next;
		curr->next->prev = curr->prev;
		list->size--;
		return curr;
	}

	if (n == list->size - 1) {
		node_t* removed;
		removed = list->tail;
		list->tail = list->tail->prev;
		list->tail->next = NULL;
		list->size--;
		return removed;
	}
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int ll_get_size(list_t* list)
{
	return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void dll_free(list_t** list)
{
    if (!list || !*list) {
        return;
    }
	
	node_t* curr;

    while ((*(list))->size) {
        curr = ll_remove_nth_node(*list, 0);
        free(curr->data);
        curr->data = NULL;
        free(curr);
        curr = NULL;
    }

    free(*list);
    *list = NULL;
}

int
dll_intersectie_block(arena_t* arena, const uint64_t address, const uint64_t size)
{
	// Verificam daca lista de block-uri este goala
	//printf("%lu\n", arena->block_list->size);
	if (!arena->block_list->size) {
		printf("Lista de block-uri este goala\n");
		return 0;
	}
	node_t* curr;
	curr = arena->block_list->head;

	for (int i = 0; i < arena->block_list->size; i++) {
		uint64_t start_old, stop_old, start_new, stop_new;
		start_old = ((block_t*)curr->data)->start_address;
		stop_old = ((block_t*)curr->data)->start_address + ((block_t*)curr)->size - 1;
		start_new = address;
		stop_new = address + size - 1;

		if (start_new >= start_old && start_new <= stop_old) {
			return 1;
		}

		if (stop_new >= start_old && stop_new <= stop_old) {
			return 1;
		}

		if (start_new < start_old && stop_new > stop_old) {
			return 1;
		}
		curr = curr->next;
	}
	return 0;
}

node_t* merge_left(arena_t* arena, const uint64_t address, const uint64_t size)
{
	// Functia intoarce adresa block-ului din stanga daca este necesara
	// alipirea noului block de cel din stanga
	if (!arena->block_list->size) {
		return NULL;
	}
	
	node_t* curr;
	curr = arena->block_list->head;
	if (address < ((block_t*)curr->data)->start_address) {
		return NULL;
	}
	for (int i = 0; i < arena->block_list->size; i++) {
		if (((block_t*)curr->data)->start_address + ((block_t*)curr->data)->size > address) {
			return NULL;
		}
		if (((block_t*)curr->data)->start_address + ((block_t*)curr->data)->size == address)
			return curr;
		curr = curr->next;
	}
	return NULL;
}

node_t* merge_right(arena_t* arena,node_t* left, const uint64_t address, const uint64_t size)
{
	if (!arena->block_list->size) {
		return NULL;
	}
	node_t* curr;
	if (left) {
		if (left->next) {
			curr = left->next;
			if (address + size == ((block_t*)curr->data)->start_address) {
				return curr;
			} else {
				return NULL;
			}
		} else {
			return NULL;
		}
	}
	curr = arena->block_list->tail;
	for (int i = 0; i < arena->block_list->size; i++) {
		if (address + size > ((block_t*)curr->data)->start_address) {
			return NULL;
		}
		if (address + size == ((block_t*)curr->data)->start_address) {
			return curr;
		}
		curr = curr->prev;
	}
	return NULL;

}

arena_t *alloc_arena(const uint64_t size)
{
    arena_t* arena;
	arena = malloc(sizeof(arena_t));
	arena->arena_size = size;
	arena->block_list = dll_create(sizeof(block_t));
	return arena;
}

void dealloc_arena(arena_t *arena)
{

}

// void alloc_block(arena_t *arena, const uint64_t address, const uint64_t size)
// {
// 	// Verificam daca dimensiunile noului block se
// 	// incadreaza in limitele arenei
// 	printf("test0\n");
// 	printf("%lu\n", address);
// 	printf("%lu\n", address + size);
// 	printf("%lu 	%lu		%lu\n", address, address + size, arena->arena_size);
// 	if (!(address >= 0 && (address + size) <= arena->arena_size)) {
// 		printf("Nu se incadreaza in limitele arenei\n");
// 		return;
// 	}
// 	printf("test1\n");
// 	// Cazul in care noul block intersecteaza un block 
// 	// deja existent
// 	if (dll_intersectie_block(arena, address, size)) {
// 		printf("Intersectie block-uri\n");
// 		return;
// 	}
// 	printf("test2\n");

// 	block_t* block;
// 	block = malloc(sizeof(block_t));
// 	block->size = size;
// 	block->start_address = address;
// 	block->miniblock_list = dll_create(sizeof(miniblock_t));
// 	printf("test3\n");
// 	// Adaugam un block in arena
// 	dll_add_nth_node(arena->block_list, find_block_poz(arena->block_list, address), block);
// 	printf("test4\n");
	

// 	// block_t* block;
// 	// block = malloc(sizeof(block_t));
// 	// block->start_address = address;
// 	// block->size = size;
// 	// block =
// }

void alloc_block_simple(arena_t *arena, const uint64_t address, const uint64_t size)
{
	if (!arena->arena_size) {
		node_t* new, *new2;		
		new = malloc(sizeof(node_t));
		new->next = NULL;
		new->prev = NULL;
		new->data = malloc(sizeof(block_t));
		((block_t*)new->data)->size = size;
		((block_t*)new->data)->start_address = address;
		((block_t*)new->data)->miniblock_list = dll_create(sizeof(miniblock_t));
		new2 = malloc(sizeof(node_t));
		new2->next = NULL;
		new2->prev = NULL;
		new2->data = malloc(sizeof(miniblock_t));
		((miniblock_t *)new2->data)->perm = 6;
		((miniblock_t *)new2->data)->size = size;
		((miniblock_t *)new2->data)->start_address = address;
		((miniblock_t *)new2->data)->rw_buffer = malloc(size);
		((list_t *)((block_t *)new->data)->miniblock_list)->head = new2;
		((list_t *)((block_t *)new->data)->miniblock_list)->tail = new2;
		((list_t *)((block_t *)new->data)->miniblock_list)->data_size = sizeof(miniblock_t);
		((list_t *)((block_t *)new->data)->miniblock_list)->size++;
		arena->block_list->head = new;
		arena->block_list->tail = new;
		arena->block_list->size++;
		return;
	}
	node_t* new, *new2;		
	new = malloc(sizeof(node_t));
	new->next = NULL;
	new->prev = NULL;
	new->data = malloc(sizeof(block_t));
	((block_t*)new->data)->size = size;
	((block_t*)new->data)->start_address = address;
	((block_t*)new->data)->miniblock_list = dll_create(sizeof(miniblock_t));
	new2 = malloc(sizeof(node_t));
	new2->next = NULL;
	new2->prev = NULL;
	new2->data = malloc(sizeof(miniblock_t));
	((miniblock_t *)new2->data)->perm = 6;
	((miniblock_t *)new2->data)->size = size;
	((miniblock_t *)new2->data)->start_address = address;
	((miniblock_t *)new2->data)->rw_buffer = malloc(size);
	((list_t *)((block_t *)new->data)->miniblock_list)->head = new2;
	((list_t *)((block_t *)new->data)->miniblock_list)->tail = new2;
	((list_t *)((block_t *)new->data)->miniblock_list)->data_size = sizeof(miniblock_t);
	((list_t *)((block_t *)new->data)->miniblock_list)->size++;

	// if (address < ((block_t*)arena->block_list->head->data)->start_address) {
	// 	// Noul block trebuie inserat pe prima pozitie
	// 	arena->block_list->head->prev = new;
	// 	new->next = arena->block_list->head;
	// 	arena->block_list->head = new;
	// }

	
	node_t* curr, *previous;
	previous = arena->block_list->tail;
	curr = arena->block_list->head;
	for (int i = 0; i < arena->block_list->size; i++) {
		if (address < ((block_t*)curr->data)->start_address) {
			previous = curr->prev;
			break;
		}
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

void alloc_block_left(arena_t *arena, const uint64_t address, const uint64_t size, node_t* left)
{
	((block_t*)left->data)->size += size;
	((list_t*)((block_t*)left->data)->miniblock_list)->size++;
	node_t* new;
	new = malloc(sizeof(node_t));
	new->data = malloc(sizeof(miniblock_t));
	((miniblock_t*)new->data)->perm = 6;
	((miniblock_t*)new->data)->size = size;
	((miniblock_t*)new->data)->start_address = address;
	((miniblock_t*)new->data)->rw_buffer = malloc(size);
	new->prev = ((list_t*)((block_t*)left->data)->miniblock_list)->tail;
	new->next = NULL;
	((list_t*)((block_t*)left->data)->miniblock_list)->tail->next = new;
	((list_t*)((block_t*)left->data)->miniblock_list)->tail = new;
}

void alloc_block_right(arena_t *arena, const uint64_t address, const uint64_t size, node_t* right)
{
	((block_t*)right->data)->size += size;
	((block_t*)right->data)->start_address = address;
	((list_t*)((block_t*)right->data)->miniblock_list)->size++;
	node_t* new;
	new = malloc(sizeof(node_t));
	new->data = malloc(sizeof(miniblock_t));
	((miniblock_t*)new->data)->perm = 6;
	((miniblock_t*)new->data)->size = size;
	((miniblock_t*)new->data)->start_address = address;
	((miniblock_t*)new->data)->rw_buffer = malloc(size);
	new->prev = NULL;
	new->next = ((list_t*)((block_t*)right->data)->miniblock_list)->head;
	((list_t*)((block_t*)right->data)->miniblock_list)->head->prev = new;
	((list_t*)((block_t*)right->data)->miniblock_list)->head = new;
}

void alloc_block_left_right(arena_t *arena, const uint64_t address, const uint64_t size, node_t* left, node_t* right)
{
	((block_t*)left->data)->size += size;
	((block_t*)left->data)->size += ((block_t*)right->data)->size;
	((list_t*)((block_t*)left->data)->miniblock_list)->size++;
	((list_t*)((block_t*)left->data)->miniblock_list)->size += ((list_t*)((block_t*)right->data)->miniblock_list)->size;
	node_t* new;
	new = malloc(sizeof(node_t));
	new->data = malloc(sizeof(miniblock_t));
	((miniblock_t*)new->data)->perm = 6;
	((miniblock_t*)new->data)->size = size;
	((miniblock_t*)new->data)->start_address = address;
	((miniblock_t*)new->data)->rw_buffer = malloc(size);
	new->prev = ((list_t*)((block_t*)left->data)->miniblock_list)->tail;
	new->next = ((list_t*)((block_t*)right->data)->miniblock_list)->head;
	((list_t*)((block_t*)left->data)->miniblock_list)->tail->next = new;
	((list_t*)((block_t*)left->data)->miniblock_list)->tail = ((list_t*)((block_t*)right->data)->miniblock_list)->tail;
	((list_t*)((block_t*)right->data)->miniblock_list)->head->prev = new;
	((list_t*)((block_t*)right->data)->miniblock_list)->head = NULL;
	((list_t*)((block_t*)right->data)->miniblock_list)->tail = NULL;
	free(((block_t*)right->data)->miniblock_list);
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

void alloc_block(arena_t *arena, const uint64_t address, const uint64_t size)
{
	if (address >= arena->arena_size) {
		printf("The allocated address is outside the size of arena\n");
		return;
	}
	if (arena->arena_size < address + size) {
		printf("The end address is past the size of the arena\n");
		return;
	}
	if (dll_intersectie_block(arena, address, size)) {
		printf("This zone was already allocated.\n");
		return;
	}
	//printf("%lu\n", arena->block_list->size);
	// int left = 0, right = 0;
	// node_t* curr;
	// curr = arena->block_list->head;
	// for (int i = 1; i < arena->block_list->size; i++) {
	// 	if ()
	// }
	node_t* left = merge_left(arena, address, size);
	node_t* right = merge_right(arena, left, address, size);
	if (!left && !right) {
		alloc_block_simple(arena, address, size);
	}
	if (left && !right) {
		alloc_block_left(arena, address, size, left);
	}
	if (!left && right) {
		alloc_block_right(arena, address, size, right);
	}
	if (left && right) {
		alloc_block_left_right(arena, address, size, left, right);
	}
}

void free_block(arena_t *arena, const uint64_t address)
{

}

void read(arena_t *arena, uint64_t address, uint64_t size)
{

}

void write(arena_t *arena, const uint64_t address, const uint64_t size, int8_t *data)
{

}

void pmap(const arena_t *arena)
{

}

void mprotect(arena_t *arena, uint64_t address, int8_t *permission)
{

}