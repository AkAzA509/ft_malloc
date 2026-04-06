#pragma once

#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>

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
** Align the memory zone on a power of 2 to use it easier
*/
#define ALIGN_TO_PAGE(x) (((x) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

/*
** Compute the zone size with the mmory alignement in one pass
** Only the SMALL and TINY are compute because the LARGE doesn't have a explicit limit size
*/
#define TINY_ZONE_SIZE ALIGN_TO_PAGE((TINY_MAX  + sizeof(t_block)) * 100)
#define SMALL_ZONE_SIZE ALIGN_TO_PAGE((SMALL_MAX + sizeof(t_block)) * 100)

/*
** Alignement mémoire
** Toute adresse retournée à l'utilisateur doit être alignée sur 8 octets
** On arrondit la taille demandée au multiple de 8 supérieur
*/
#define ALIGN8(x) (((x) + 7) & ~7)

/*
** Header de bloc
** Placé juste avant la zone data retournée à l'utilisateur
**
** En mémoire :
**   [ t_block | ...data... ]
**               ↑ ptr retourné par malloc = (void*)(block + 1)
**
** Pour retrouver le header depuis un ptr utilisateur :
**   t_block *block = (t_block*)ptr - 1;
**
** next/prev sont utilisés uniquement quand le bloc est libre
** Quand le bloc est occupé, ils sont ignorés (mais présents)
*/
typedef struct s_block {
	struct s_block	*next;
	struct s_block	*prev;
	size_t			size;
	bool			is_free;
}					t_block;

/*
** Header de zone
** Placé au tout début de chaque mmap()
**
** En mémoire :
**   [ t_zone | t_block | data | t_block | data | ... ]
**              ↑ premier bloc = (t_block*)(zone + 1)
**
** free_blocks permet de savoir en O(1) si la zone est entièrement
** libre (auquel cas on peut munmap())
*/
typedef struct s_zone {
	struct s_zone	*next;
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

void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);
void	show_alloc_mem(void);