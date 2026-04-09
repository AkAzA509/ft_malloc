#pragma once

#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>
#include "ft_printf/ft_printf.h"

#define GREEN "\033[92m"
#define WHITE "\033[97m"
#define BLUE "\033[94m"
#define RED "\033[91m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

#define BLD_WHITE BOLD WHITE
#define BLD_RED BOLD RED
#define BLD_BLUE BOLD BLUE
#define BLD_GREEN BOLD GREEN

/*
** Lenght max
** TINY  : 1 to TINY_MAX octets
** SMALL : TINY_MAX+1 to SMALL_MAX
** LARGE : SMALL_MAX+1 to space limits
*/
#define TINY_MAX	128
#define SMALL_MAX	1024

/*
** Get the page size on the current os
** _SC_PAGESIZE is not portable (this alocator is linux only implementation)
*/
#define PAGE_SIZE (size_t)sysconf(_SC_PAGESIZE)

/*
** Rounds x up to the nearest multiple of 8.
** Ensures every address returned to the user is 8-byte aligned.
**
**   + 7        ensures we round up and not down
**   & ~7       clears the 3 lower bits (the actual rounding)
**
** Example with x = 13 :
**   13 + 7 = 20
**   20 & ~7 = 16
*/
#define ALIGN8(x) (((x) + 7) & ~7)

/*
** Rounds x up to the nearest multiple of PAGE_SIZE.
**
** The trick works because PAGE_SIZE is a power of 2 :
**   + (PAGE_SIZE - 1)  ensures we round up and not down
**   & ~(PAGE_SIZE - 1) clears the lower bits (the actual rounding)
**
** Example with PAGE_SIZE = 4096 and x = 5000 :
**   5000 + 4095 = 9095
**   9095 & ~4095 = 8192  (2 pages)
*/
#define ALIGN_TO_PAGE(x) (((x) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

/*
** Compute the zone size with the mmory alignement in one pass
** Only the SMALL and TINY are compute because the LARGE doesn't have a explicit limit size
*/
#define TINY_ZONE_SIZE ALIGN_TO_PAGE(sizeof(t_zone) + (ALIGN8(TINY_MAX) + sizeof(t_block)) * 100)
#define SMALL_ZONE_SIZE ALIGN_TO_PAGE(sizeof(t_zone) + (ALIGN8(SMALL_MAX) + sizeof(t_block)) * 100)
#define LARGE_ZONE_SIZE(x) ALIGN_TO_PAGE(sizeof(t_zone) + (ALIGN8(x) + sizeof(t_block)))

typedef enum e_block_kind {
	TINY,
	SMALL,
	LARGE
}	e_block_kind;

typedef struct s_block {
	struct s_block	*next;
	struct s_block	*prev;
	struct s_zone	*owner;
	size_t			size;
	e_block_kind	kind;
	bool			is_free;
}					t_block;

typedef struct s_zone {
	struct s_zone	*next;
	struct s_block	*block_list;
	size_t			zone_size;
	size_t			free_blocks;
	size_t			total_blocks;
}					t_zone;

/*
** Global struct
** Store the 3 allocation linked list
*/
typedef struct s_allocator {
	t_zone			*tiny;
	t_zone			*small;
	t_zone			*large;
}					t_allocator;

extern t_allocator g_alloc;

// utils
t_zone	**find_zone_link(t_zone **head, t_zone *zone);
bool	split_block(t_block *block, size_t align_mem);
t_block	*merge_with_next(t_block *block, t_zone *zone);

// mandatory
void	*ft_malloc(size_t size);
void	ft_free(void *ptr);
void	*ft_realloc(void *ptr, size_t size);
void	show_alloc_mem(void);

// bonus
void	show_alloc_mem_ex(void);

// env var:
//		- MALLOC_PERTURB_
//		- MALLOC_MMAP_MAX_
//		- MALLOC_MMAP_THRESHOLD_
//		- MALLOC_LOG_ (custom)

// pour le multi thread:
// mutex et flag dans la struct
// realloc envoie le flag si c'est lui qui a lock avant et qui a besoins de malloc et free
// pour que malloc et free puisse faire les tache malgrer le lock
// si malloc et free sont appeler alors que mutex et lock et que pas de flag alors attente