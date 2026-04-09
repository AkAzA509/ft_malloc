#include "malloc.h"
#include <stdio.h>

/*
* The ft_memcpy() function copies n bytes from memory area src to memory area dest.
* The memory areas must not overlap.
*/
static void	*ft_memcpy(void *dst, const void *src, size_t len) {
	unsigned char	*d;
	unsigned char	*s;

	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	while (len > 0) {
		*d = *s;
		d++;
		s++;
		len--;
	}
	return (dst);
}

/*
* Try to merge adjacent bloc to his right to grab free memory,
* if it work, try to split the block
*/
static bool	merge_block(t_block *block, size_t size, t_zone *zone) {
	while (block && block->next && block->next->is_free)
		block = merge_with_next(block, zone);

	if (block->size >= size) {
		split_block(block, size);
		return true;
	}
	return false;
}

/*
* TINY or SMALL zones:
* if the ptr need to be shrink, ft_realloc() try to split the bloc if it failed free and recreate the ptr with size.
* if the ptr need to be growth, ft_realloc() try to merge adjacent block to grap memory (if it work try to split if the blog is really to large),
* else free and recreate the ptr too.
*/
void	*realloc_other(t_block *block, t_zone **zone_head, void *ptr, size_t size) {
	t_zone	*zone = block->owner;
	t_zone	**zone_match = find_zone_link(zone_head, zone);
	
	if (!*zone_match)
		return NULL;
	
	if (block->size > size) {
		if (!split_block(block, size)) {
			void *new_ptr = ft_malloc(size);
			if (!new_ptr)
				return NULL;
			
			size_t copy_size = block->size > size ? size : block->size;
			new_ptr = ft_memcpy(new_ptr, ptr, copy_size);
			ft_free(ptr);
			return new_ptr;
		}
	}
	else {
		if (!merge_block(block, size, zone)) {
			void *new_ptr = ft_malloc(size);
			if (!new_ptr)
				return NULL;
			
			size_t copy_size = block->size > size ? size : block->size;
			new_ptr = ft_memcpy(new_ptr, ptr, copy_size);
			ft_free(ptr);
			return new_ptr;
		}
	}
	return ptr;
}

/*
* LARGE type zone: free the zone and remalloc a new ptr.
*/
void	*realloc_large(t_block *block, void *ptr, size_t size) {
	void *new_ptr = ft_malloc(size);
	if (!new_ptr)
		return NULL;
	
	size_t copy_size = block->size > size ? size : block->size;
	new_ptr = ft_memcpy(new_ptr, ptr, copy_size);
	ft_free(ptr);
	return new_ptr;
}

/*
* If a reallocation became too large or too small within is origin type zone
* ft_realloc() free the ptr en recreate it with ft_malloc() to have the right type zone
*/
static void	*change_class(void *ptr, size_t new_size, size_t old_size) {
	void *new_ptr = ft_malloc(new_size);
	if (!new_ptr)
		return NULL;
	
	size_t copy_size = old_size > new_size ? new_size : old_size;
	new_ptr = ft_memcpy(new_ptr, ptr, copy_size);
	ft_free(ptr);
	return new_ptr;
}

/*
* The ft_realloc() function changes the size of the memory block pointed to by ptr to size bytes.
* The contents will be unchanged in the range from the start of the region up to the minimum of the old and new sizes.
* If the new size is larger than the old size, the added memory will not be initialized.
* If ptr is NULL, then the call is equivalent to ft_malloc(size).
* For all values of size; if size is equal to zero, and ptr is not NULL, then the call is equivalent to ft_free(ptr).
* If ptr is NULL and size equal to zero, then NULL is return.
*/
void	*ft_realloc(void *ptr, size_t new_size) {
	if (new_size <= (size_t)0 && ptr)
		return ft_free(ptr), NULL;
	else if (!ptr && new_size > (size_t)0)
		return ptr = ft_malloc(new_size);
	else if (!ptr && new_size == (size_t)0)
		return NULL;

	size_t size = ALIGN8(new_size);

	t_block	*block = ((t_block *)ptr) - 1;

	if (block->size == size)
		return ptr;

	if((block->kind == TINY && size > TINY_MAX) || (block->kind == SMALL && (size > SMALL_MAX || size <= TINY_MAX)) || (block->kind == LARGE && size <= SMALL_MAX))
		return ptr = change_class(ptr, size, block->size);

	switch (block->kind) {
		case TINY:
			ptr = realloc_other(block, &g_alloc.tiny, ptr, size);
			break;

		case SMALL:
			ptr = realloc_other(block, &g_alloc.small, ptr, size);
			break;

		case LARGE:
			ptr = realloc_large(block, ptr, size);
			break;
		
		default:
			break;
	}

	return ptr;
}