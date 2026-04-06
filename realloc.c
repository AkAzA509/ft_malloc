#include "malloc.h"
#include <stdio.h>

void	*realloc(void *ptr, size_t size) {
	if (size <= (size_t)0)
return ptr;
}