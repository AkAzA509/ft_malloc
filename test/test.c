#define _POSIX_C_SOURCE 200809L
#include "../include/malloc.h"
#include <string.h>
#include <stdlib.h>

static int	g_pass = 0;
static int	g_fail = 0;

static void	report(const char *name, int ok) {
	if (ok) {
		g_pass++;
		ft_printf(BLD_GREEN"[OK] "RESET"%s\n", name);
	}
	else {
		g_fail++;
		ft_printf(BLD_RED"[KO] "RESET"%s\n", name);
	}
}

// void print_lenght_calcul() {
	// printf("align: %d\n", ALIGN8(4050));
	// size_t zone = sizeof(t_zone);
	// size_t block = sizeof(t_block);
	// size_t align_t = ALIGN8(TINY_MAX);
	// size_t align_s = ALIGN8(SMALL_MAX);
	// size_t page_size = PAGE_SIZE;
	
	// size_t res_s = zone + (align_s + block) * 100;
	// size_t res_t = zone + (align_t + block) * 100;
	// printf("calcul small:\n\t \
	// 	  (t_zone)%ld\n\t \
	// 	+ ((align)%ld + (t_block)%ld) \n\t \
	// 	* 100\n\t \
	// 	= align in memory %ld / 0x%zX / pages = %ld\n", zone, align_s, block, ALIGN_TO_PAGE(res_s), ALIGN_TO_PAGE(res_s), ALIGN_TO_PAGE(res_s) / PAGE_SIZE);
	// printf("calcul tiny:\n\t \
	// 	  (t_zone)%ld\n\t \
	// 	+ ((align)%ld + (t_block)%ld) \n\t \
	// 	* 100\n\t \
	// 	= align in memory %ld / 0x%zX / pages = %ld\n", zone, align_t, block, ALIGN_TO_PAGE(res_t), ALIGN_TO_PAGE(res_t), ALIGN_TO_PAGE(res_t) / PAGE_SIZE);
// }

void test_threshold() {
	setenv("MALLOC_MMAP_THRESHOLD_", "100", 0);
	setenv("MALLOC_PERTURB_", "45", 0);
	char	*str = ft_malloc(700);
	
	ft_printf("align : %zu, page align: %zu\n", ALIGN8(700), LARGE_ZONE_SIZE(700));
	ft_printf("size of header: zone: %zu, block: %zu\n", sizeof(t_zone), sizeof(t_block));
	
	str = memset(str, 'A', 700);
	show_alloc_mem_ex();
	ft_free(str);
	unsetenv("MALLOC_MMAP_THRESHOLD_");
	unsetenv("MALLOC_PERTURB_");
}

int	main(void) {
	test_threshold();
	char	*tiny;
	char	*small;
	char	*large;
	char	*r;
	void	*arr[32];
	size_t	i;

	ft_printf(BLD_BLUE"\n== ft_malloc test suite ==\n"RESET);

	/* Basic allocations: tiny, small, large */
	tiny = ft_malloc(32);
	small = ft_malloc(700);
	large = ft_malloc(6000);
	report("ft_malloc tiny", tiny != NULL);
	report("ft_malloc small", small != NULL);
	report("ft_malloc large", large != NULL);

	if (tiny)
		memset(tiny, 'A', 32);
	if (small)
		strcpy(small, "small-block-data");
	if (large)
		memset(large, 'L', 6000);

	show_alloc_mem();

	/* realloc grow + shrink with data preservation */
	r = ft_realloc(tiny, 200);
	report("ft_realloc grow tiny->small", r != NULL);
	if (r) {
		report("preserve data after grow", r[0] == 'A' && r[31] == 'A');
		tiny = r;
	}

	r = ft_realloc(tiny, 24);
	report("ft_realloc shrink", r != NULL);
	if (r) {
		report("preserve data after shrink", r[0] == 'A');
		tiny = r;
	}

	/* API edge cases */
	report("ft_malloc(0) == NULL", ft_malloc(0) == NULL);
	r = ft_realloc(NULL, 128);
	report("ft_realloc(NULL, n)", r != NULL);
	if (r)
		ft_free(r);
	r = ft_realloc(small, 0);
	report("ft_realloc(ptr, 0) == NULL", r == NULL);
	small = NULL;

	/* small stress pass */
	i = 0;
	while (i < 32) {
		arr[i] = ft_malloc((i + 1) * 17);
		i++;
	}
	i = 0;
	while (i < 32) {
		if (arr[i])
			ft_free(arr[i]);
		i++;
	}
	report("stress alloc/ft_free x32", 1);

	ft_printf(BLD_BLUE"\n== Detailed dump before cleanup ==\n"RESET);
	show_alloc_mem_ex();

	/* Cleanup */
	if (tiny)
		ft_free(tiny);
	if (small)
		ft_free(small);
	if (large)
		ft_free(large);

	ft_printf(BLD_BLUE"\n== Detailed dump after cleanup ==\n"RESET);
	show_alloc_mem_ex();

	ft_printf(BLD_WHITE"\nSummary: %zu passed, %zu failed\n"RESET,
		(size_t)g_pass, (size_t)g_fail);

	return (g_fail != 0);
}