#include "../include/malloc.h"
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define free ft_free
#define malloc ft_malloc
#define realloc ft_realloc

#define THREAD_COUNT 6
#define SLOT_COUNT 64
#define ITERATIONS 4000

typedef struct s_worker {
	int id;
	unsigned int seed;
	size_t alloc_count;
	size_t free_count;
} t_worker;

static size_t next_size(unsigned int *state) {
	unsigned int bucket;

	bucket = rand_r(state) % 10u;
	if (bucket < 6u)
		return (size_t)(1u + (rand_r(state) % 128u));
	if (bucket < 9u)
		return (size_t)(129u + (rand_r(state) % 896u));
	return (size_t)(1500u + (rand_r(state) % 8000u));
}

static void *worker_main(void *arg) {
	t_worker *w = (t_worker *)arg;
	void *slots[SLOT_COUNT];
	size_t i;

	i = 0;
	while (i < SLOT_COUNT) {
		slots[i] = NULL;
		i++;
	}

	i = 0;
	while (i < ITERATIONS) {
		size_t idx = (size_t)(rand_r(&w->seed) % SLOT_COUNT);
		unsigned int op = rand_r(&w->seed) % 3u;

		if (op == 0u) {
			size_t s = next_size(&w->seed);
			if (!slots[idx]) {
				slots[idx] = malloc(s);
				if (slots[idx]) {
					memset(slots[idx], (unsigned char)('A' + (w->id % 26)), s);
					w->alloc_count++;
				}
			}
		}
		else if (op == 1u) {
			if (slots[idx]) {
				size_t s = next_size(&w->seed);
				void *p = realloc(slots[idx], s);
				if (p)
					slots[idx] = p;
			}
		}
		else {
			if (slots[idx]) {
				free(slots[idx]);
				slots[idx] = NULL;
				w->free_count++;
			}
		}
		i++;
	}

	i = 0;
	while (i < SLOT_COUNT) {
		if (slots[i]) {
			free(slots[i]);
			slots[i] = NULL;
			w->free_count++;
		}
		i++;
	}
	return NULL;
}

int main(void) {
	pthread_t threads[THREAD_COUNT];
	t_worker workers[THREAD_COUNT];
	size_t i;
	int ok;

	ft_printf(BLD_BLUE"\n== ft_malloc thread test ==\n"RESET);

	i = 0;
	while (i < THREAD_COUNT) {
		workers[i].id = (int)i;
		workers[i].seed = (unsigned int)(12345u + (unsigned int)i * 100u);
		workers[i].alloc_count = 0;
		workers[i].free_count = 0;
		if (pthread_create(&threads[i], NULL, worker_main, &workers[i]) != 0) {
			ft_printf(BLD_RED"[KO] pthread_create failed at worker %zu\n"RESET, i);
			return 1;
		}
		i++;
	}

	ok = 1;
	i = 0;
	while (i < THREAD_COUNT) {
		if (pthread_join(threads[i], NULL) != 0) {
			ok = 0;
			ft_printf(BLD_RED"[KO] pthread_join failed at worker %zu\n"RESET, i);
		}
		i++;
	}

	if (ok)
		ft_printf(BLD_GREEN"[OK] all threads joined successfully\n"RESET);

	i = 0;
	while (i < THREAD_COUNT) {
		ft_printf(BLD_WHITE"worker %zu: alloc=%zu free=%zu\n"RESET,
			i, workers[i].alloc_count, workers[i].free_count);
		i++;
	}

	show_alloc_mem();
	show_alloc_mem_ex();
	return ok ? 0 : 1;
}
