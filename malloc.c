#include "malloc.h"
#include <stdio.h>


// void    show_alloc_mem() {
    
// }

// void    *malloc(size_t size) {
//     if (size <= (size_t)0)
//         return NULL;

    
// }

int main() {
    char *test = malloc(sizeof(char *));
    if (!test)
        return 0;
    printf("addresse de la stringL %ld", test - test);
}