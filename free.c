#include "malloc.h"
#include <stdio.h>

t_zone	**find_zone_link(t_zone **head, t_zone *zone) {
	t_zone	**link = head;

	while (*link && *link != zone)
		link = &(*link)->next;
	return link;
}

static bool	is_adjacent(t_block *left, t_block *right) {
	return ((char *)(left + 1) + left->size == (char *)right);
}

t_block	*merge_with_next(t_block *block, t_zone *zone) {
	t_block	*next = block->next;

	if (!next || !next->is_free || !is_adjacent(block, next))
		return block;

	block->size += sizeof(t_block) + next->size;
	block->next = next->next;

	if (block->next)
		block->next->prev = block;

	zone->free_blocks--;
	zone->total_blocks--;
	return block;
}

static void	free_other(t_block *block, t_zone **zone_head) {
	t_zone	*zone = block->owner;
	t_zone	**zone_link = find_zone_link(zone_head, zone);

	if (!*zone_link)
		return;

	if (block->is_free)
		return;

	block->is_free = true;
	zone->free_blocks++;

	while (block->prev && block->prev->is_free && is_adjacent(block->prev, block))
		block = merge_with_next(block->prev, zone);

	while (block->next && block->next->is_free && is_adjacent(block, block->next))
		block = merge_with_next(block, zone);

	if (zone->total_blocks == 1) {
		*zone_link = zone->next;
		munmap(zone, zone->zone_size);
	}
}

static void	free_large(t_block *block) {
	t_zone	*zone = block->owner;
	t_zone	**zone_link = find_zone_link(&g_alloc.large, zone);

	if (*zone_link)
		*zone_link = (*zone_link)->next;
	munmap(zone, zone->zone_size);
}

void	ft_free(void *ptr) {
	if (!ptr)
		return;

	t_block	*block = ((t_block *)ptr) - 1;

	switch (block->kind) {
		case TINY:
			free_other(block, &g_alloc.tiny);
			break;

		case SMALL:
			free_other(block, &g_alloc.small);
			break;

		case LARGE:
			free_large(block);
			break;
		
		default:
			break;
	}
}