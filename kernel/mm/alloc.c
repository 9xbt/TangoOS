#include <mm/heap.h>
#include <mm/alloc.h>
#include <lib/printf.h>

struct heap *kernel_heap;

void alloc_init() {
    kernel_heap = heap_create();
    printf("heap: created kernel heap\n");
}

void *kmalloc(size_t n) {
    return heap_alloc(kernel_heap, n);
}

void kfree(void *ptr) {
    heap_free(ptr);
}