#include "malloc.h"
#include <stdio.h>

g_alloc = 0;

// void    show_alloc_mem() {
    
// }

void	*tiny(size_t size) {
	if (!g_alloc.tiny) {
		g_alloc.tiny = mmap(0, TINY_ZONE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (!g_alloc.tiny)
			return NULL;
	}
	else {
		t_zone	*zone = g_alloc.tiny;
		if (zone->free_blocks > 0) {
			t_block *new = zone + 1;
			new->is_free = false;
			new->next = NULL;
			new->prev = NULL;
			new->size = TINY_ZONE_SIZE - sizeof(t_zone) - sizeof(t_block); // the first block take all the place we will split later for each memory query
		}
	}
}

// void	*small(size_t size) {

// }

// void	*large(size_t size) {

// }

void	*malloc(size_t size) {
	if (size <= (size_t)0)
		return NULL;

	if (size <= TINY_MAX)
		return tiny(size);
	else if (size <= SMALL_MAX)
		return small(size);
	else
		return large(size);

	return NULL;
}

int main() {

}