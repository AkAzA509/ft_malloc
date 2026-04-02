#pragma once

#include <stdlib.h>

void    free(void *ptr);
void    *ft_malloc(size_t size);
void    *realloc(void *ptr, size_t size);

void    show_alloc_mem();