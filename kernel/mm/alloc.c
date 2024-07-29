#include <mm/heap.h>
#include <mm/alloc.h>
#include <lib/printf.h>

struct heap *kernel_heap;

/*
 * alloc_init - inits the heap.
 */
void alloc_init() {
    kernel_heap = heap_create();
}

/*
 * kmalloc - allocs kernel memory, n = the amount of bytes you want.
 */
void *kmalloc(size_t n) {
    return heap_alloc(kernel_heap, n);
}

/*
 * kfree - frees kmalloc'd memory.
 */
void kfree(void *ptr) {
    heap_free(ptr);
}