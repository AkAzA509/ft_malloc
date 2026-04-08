#include "malloc.h"
#include <stdio.h>

static void	print_hex_byte(unsigned char byte)
{
	const char	*digits;

	digits = "0123456789abcdef";
	ft_putchar(digits[byte >> 4]);
	ft_putchar(digits[byte & 0x0F]);
	ft_putchar(' ');
}

static size_t print_alloc(t_zone *zone, char *type) {
	size_t	total_mem = 0;
	t_block	*block;

	block = zone ? zone->block_list : NULL;
	if(block) {
		ft_printf("%s : %p\n", type, zone);
		while (block) {
			ft_printf("Block : %p -> %p = %d bytes, the block is %s\n", block, (char *)(block + 1) + block->size, block->size, block->is_free == true ? "free" : "in use");
			total_mem += block->size;
			block = block->next;
		}
	}
	else 
		ft_printf("\nNo %s block\n", type);
	return total_mem;
}

void	show_alloc_mem(void) {
	t_zone	*tiny = g_alloc.tiny;
	t_zone	*small = g_alloc.small;
	t_zone	*large = g_alloc.large;
	size_t	total_mem = 0;

	total_mem += print_alloc(tiny, "TINY");
	total_mem += print_alloc(small, "SMALL");
	total_mem += print_alloc(large, "LARGE");

	ft_printf("\nTOTAL : %d\n", total_mem);
}

void print_hex_info(t_zone *zone, char *type) {
	t_block	*block;

	block = zone ? zone->block_list : NULL;
	if(block) {
		ft_printf(BLD_RED"%s ZONE : %p\n"RESET, type, zone);
		ft_printf(BLD_WHITE"------------------------------------------------------------------------------------\n"RESET);
		while (block) {
			if (block->size < 5000) {
				ft_printf(BLD_BLUE"Owner zone: %s  -  bloc %s  -  size: %d\n\n"RESET, type, block->is_free == true ? "free" : "in use", block->size);
				unsigned char *p = (unsigned char *)(block + 1);
				for (size_t i = 0; i < block->size; i += 16) {

					ft_printf(BLD_WHITE"%p : "BLD_GREEN, (char *)(block + 1) + i);

					for (size_t j = 0; j < 16; j++) {
						if (i + j < block->size)
							print_hex_byte(p[i + j]);
						else
							ft_printf("   ");
					}

					ft_printf(BLD_WHITE" | ");
					for (size_t j = 0; j < 16 && i + j < block->size; j++) {
						unsigned char c = p[i + j];
						ft_printf("%c", (c >= 32 && c <= 126) ? c : '.');
					}
					ft_printf(RESET"\n");
				}
				ft_printf(BLD_WHITE"\n------------------------------------------------------------------------------------\n"RESET);
			}
			block = block->next;
		}
	}
}

void	show_alloc_mem_ex(void) {
	t_zone	*tiny = g_alloc.tiny;
	t_zone	*small = g_alloc.small;
	t_zone	*large = g_alloc.large;

	print_hex_info(tiny, "TINY");
	print_hex_info(small, "SMALL");
	print_hex_info(large, "LARGE");
}
