#include <stdbool.h>
#include <mm/pmm.h>
#include <mm/heap.h>
#include <lib/libc.h>
#include <lib/printf.h>

/*
 * heap_create - creates heap.
 */
__attribute__((no_sanitize("undefined")))
struct heap *heap_create() {
    struct heap *h = (struct heap *)pmm_alloc(1);
    h->head = (struct heap_block *)pmm_alloc(1);
    h->head->next = h->head;
    h->head->prev = h->head;
    h->head->size = 0;
    h->head->used = true;
    h->head->magic = HEAP_MAGIC;

    dprintf("heap: created heap at address 0x%x\n", h->head->size, (uint32_t)h);
    return h;
}

/*
 * heap_alloc - allocates from the heap.
 */
__attribute__((no_sanitize("undefined")))
void *heap_alloc(struct heap *h, uint32_t n) {
    uint32_t pages = DIV_CEILING(sizeof(struct heap_block) + n, PAGE_SIZE);
    
    struct heap_block *block = (struct heap_block *)pmm_alloc(pages);
    block->next = h->head;
    block->prev = h->head->prev;
    block->size = n;
    block->used = true;
    block->magic = HEAP_MAGIC;

    dprintf("heap: allocated %d bytes at address %x\n", n, (uint32_t)block);
    return (void*)block + sizeof(struct heap_block);
}

/*
 * heap_free - frees alloc'd memory.
 */
__attribute__((no_sanitize("undefined")))
void heap_free(void *ptr) {
    struct heap_block *block = (struct heap_block *)(ptr - sizeof(struct heap_block));

    if (block->magic != HEAP_MAGIC) {
        printf("heap: bad block magic at address 0x%x\n", (uint32_t)block);
        return;
    }

    block->prev->next = block->next;
    block->next->prev = block->prev;
    uint32_t pages = DIV_CEILING(sizeof(struct heap_block) + block->size, PAGE_SIZE);

    dprintf("heap: freed %d bytes at address %x\n", block->size, (uint32_t)block);
    pmm_free(block, pages);
}