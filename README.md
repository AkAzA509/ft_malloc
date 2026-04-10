# ft_malloc

`ft_malloc` is a custom memory allocator written in C for Linux. It provides a shared library that can replace the standard `malloc`, `free`, and `realloc` functions at runtime through `LD_PRELOAD`.

The project is built around a zone-based allocator with three allocation classes:

- `TINY` for allocations from 1 to 128 bytes
- `SMALL` for allocations from 129 to 1024 bytes
- `LARGE` for allocations above 1024 bytes

The code also includes a custom `ft_printf` implementation used for allocator diagnostics and debug output.

## Features

- Custom implementations of `malloc`, `free`, and `realloc`
- Thread-safe allocation paths protected by a global mutex
- 8-byte aligned allocations
- Zone splitting and block merging to reduce fragmentation
- Optional memory perturbation for debugging with pseudo-random filling
- Optional error logging controlled through environment variables
- Memory visualisation helpers with detailed zone and block output
- Extended memory dump with fragmentation statistics
- Built-in test programs for single-threaded and multi-threaded validation

## Public API

The shared library exposes the following functions:

```c
void *ft_malloc(size_t size);
void ft_free(void *ptr);
void *ft_realloc(void *ptr, size_t size);
void show_alloc_mem(void);
void show_alloc_mem_ex(void);
```

## How It Works

The allocator manages memory using `mmap` and stores metadata for each zone and block.

- `TINY` and `SMALL` allocations are served from reusable pre-allocated zones.
- `LARGE` allocations are handled with a dedicated zone per request.
- Free blocks can be merged when they are adjacent in memory.
- Reallocations try to grow or shrink in place when possible before falling back to allocate-copy-free behavior.

The implementation is Linux-specific and relies on `sysconf(_SC_PAGESIZE)`, `mmap`, and `munmap`.

## Environment Variables

The allocator reads the following environment variables when an allocation function is first called:

- `MALLOC_LOG_`
  - Enables colored error logging when set to a non-zero numeric value.
- `MALLOC_PERTURB_`
  - Fills allocated blocks with pseudo-random data and inverts the pattern on free.
- `MALLOC_MMAP_THRESHOLD_`
  - Forces allocations greater than or equal to the given size to use the `LARGE` path.
- `MALLOC_MMAP_MAX_`
  - Limits the number of large allocations handled through `mmap`.

Values must be valid positive integers. A value of `0` disables the option.

Example:

```bash
export MALLOC_LOG_=1
export MALLOC_PERTURB_=45
export MALLOC_MMAP_THRESHOLD_=700
```

## Build

```bash
make
```

This builds a shared library named `libft_malloc_<HOSTTYPE>.so` and creates a symlink called `libft_malloc.so`.

Additional targets:

```bash
make test   # build the test binaries
make clean  # remove object files
make fclean # remove objects, shared library, symlink, and test binaries
make re     # rebuild everything from scratch
```

## Usage

To preload the allocator into another program:

```bash
LD_PRELOAD=./libft_malloc.so ./your_program
```

You can also link against the shared library directly from a test or another program using the public header in `include/malloc.h`.

## Tests

Running `make test` builds two executables:

- `test_normal`, a functional test suite that checks basic allocations, reallocations, edge cases, stress cases, and allocator dumps
- `test_thread`, a multi-threaded stress test that performs concurrent allocations, reallocations, and frees across several worker threads

Both tests are linked with an `rpath` so they can find `libft_malloc.so` in the local directory.

## Notes

- This project is intended for Linux only.
- The allocator is designed as a learning and experimentation project.
- Debug output is intentionally verbose to help inspect internal allocation state.