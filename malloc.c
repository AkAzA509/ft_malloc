#include "malloc.h"
#include <stdio.h>

t_allocator g_alloc = {0};

/*
* Allocate the zone and create the first block with all rest memory and initialize it.
*/
static t_zone	*create_zone(size_t zone_size, e_block_kind kind) {
	t_zone	*zone = mmap(0, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (zone == MAP_FAILED)
		return NULL;

	zone->zone_size = zone_size;
	zone->free_blocks = 1;
	zone->total_blocks = 1;
	zone->next = NULL;

	t_block	*block = (t_block *)(zone + 1);
	block->size = zone_size - sizeof(t_zone) - sizeof(t_block);
	block->is_free = true;
	block->next = NULL;
	block->prev = NULL;
	block->kind = kind;
	block->owner = zone;

	zone->block_list = block;
	return zone;
}

/*
* Splits a block into two blocks if there is enough remaining space.
*
* Before split :
*
*  block
*   |
*   v
*  [ t_block | . . . . . . . data (block->size bytes) . . . . . . . ]
*
* After split :
*
*  block                        new_block
*   |                                |
*   v                                v
*  [ t_block | data (align_mem) | t_block | data (remaining bytes) ]
*              ^                  ^
*              |                  (char *)(block + 1) + align_mem
*              user gets this ptr = (void *)(block + 1)
*
* The split only occurs if the remaining space after align_mem is large
* enough to hold a t_block header plus at least ALIGN8(1) bytes of data.
* new_block is inserted right after block in the free list.
*
* Returns true if the split was performed, false otherwise.
*/
bool	split_block(t_block *block, size_t align_mem) {
	t_block	*new_block;

	if (block->size < align_mem + sizeof(t_block) + ALIGN8(1))
		return false;
	new_block = (t_block *)((char *)(block + 1) + align_mem);
	new_block->is_free = true;
	new_block->next = block->next;
	new_block->prev = block;
	new_block->size = block->size - align_mem - sizeof(t_block);
	new_block->owner = block->owner;
	if (block->next)
		block->next->prev = new_block;
	block->next = new_block;
	block->size = align_mem;
	return true;
}

/*
* Create TINY or SMALL type zone
*/
static void	*allocate_block(size_t align_mem, size_t zone_size, e_block_kind kind, t_zone **zone_head) {
	t_zone	*zone = NULL;
	t_block	*block = NULL;
	bool	did_split = false;

	if (!*zone_head) {
		*zone_head = create_zone(zone_size, kind);
		if (!*zone_head)
			return NULL;
	}

	zone = *zone_head;
	while (zone) {
		if (zone->free_blocks > 0) {
			block = zone->block_list;
			while (block && (!block->is_free || block->size < align_mem))
				block = block->next;
			if (block)
				break;
		}
		zone = zone->next;
	}
	if (!block) {
		zone = create_zone(zone_size, kind);
		if (!zone)
			return NULL;
		zone->next = *zone_head;
		*zone_head = zone;
		block = zone->block_list;
	}
	did_split = split_block(block, align_mem);
	if (did_split)
		zone->total_blocks++;
	else
		zone->free_blocks--;
	block->is_free = false;
	return (void *)(block + 1);
}

/*
* Create specific LARGE type zone
*/
static void	*large(size_t zone_large_size, t_zone **zone_head) {
	t_zone	*zone;
	t_block	*block;

	zone = create_zone(zone_large_size, LARGE);
	if (!zone)
		return NULL;
	block = zone->block_list;
	block->is_free = false;
	zone->free_blocks = 0;
	zone->total_blocks = 1;
	zone->next = *zone_head;
	*zone_head = zone;
	return (void *)(block + 1);
}

/*
* The ft_malloc() function allocates size bytes and returns a pointer to the allocated memory.
* The memory is not initialized (Malloc environnement variable can change this behavior).
* If size is 0, then ft_malloc() returns NULL.
*/
void	*ft_malloc(size_t size) {
	if (size <= (size_t)0)
		return NULL;

	size_t	align_mem = ALIGN8(size);

	if (align_mem <= TINY_MAX)
		return allocate_block(align_mem, TINY_ZONE_SIZE, TINY, &g_alloc.tiny);
	else if (align_mem <= SMALL_MAX)
		return allocate_block(align_mem, SMALL_ZONE_SIZE, SMALL, &g_alloc.small);
	else
		return large(LARGE_ZONE_SIZE(size), &g_alloc.large);

	return NULL;
}