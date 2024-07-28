#include <mm/pmm.h>
#include <lib/libc.h>
#include <lib/panic.h>
#include <lib/printf.h>
#include <lib/bitmap.h>
#include <lib/multiboot.h>

extern void *end;

uint8_t *pmm_bitmap = NULL;
uint32_t pmm_last_page = 0;
uint32_t pmm_used_pages = 0;
uint32_t pmm_page_count = 0;

/*
 * pmm_install - sets up the physical memory manager (PMM)
 */
void pmm_install(struct multiboot_info_t *mbd) {
    /* Check bit 6 to see if we have a valid memory map */
    if(!(mbd->flags >> 6 & 0x1)) {
        panic("invalid memory map given by GRUB bootloader");
    }

    printf("Displaying memory map below:\n\n");

    uint32_t higher_address;
    uint32_t top_address;
    struct multiboot_memory_map_t *kernel_mmmt = NULL;

    /* Loop through the memory map and display the values */
    for(uint32_t i = 0; i < mbd->mmap_length;
        i += sizeof(struct multiboot_memory_map_t))
    {
        struct multiboot_memory_map_t* mmmt = 
            (struct multiboot_memory_map_t*) (mbd->mmap_addr + i);

        printf("Start Addr: %x | Length: %x | Size: %x | Type: %d\n",
            mmmt->addr_low, mmmt->len_low, mmmt->size, mmmt->type);

        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
            /* 
             * Do something with this memory block!
             * BE WARNED that some of memory shown as availiable is actually 
             * actively being used by the kernel! You'll need to take that
             * into account before writing to memory!
             */

            if (mmmt->addr_low >= 0x100000 && kernel_mmmt == NULL) {
                kernel_mmmt = mmmt;
            }

            top_address = mmmt->addr_low + mmmt->len_low;
            if (top_address > higher_address) {
                higher_address = top_address;
            }
        }
    }

    pmm_bitmap = &end;

    kernel_mmmt->len_low -= (uint32_t)pmm_bitmap - kernel_mmmt->addr_low;
    kernel_mmmt->addr_low = (uint32_t)pmm_bitmap;

    pmm_page_count = kernel_mmmt->len_low / PAGE_SIZE;
    uint32_t bitmap_size = ALIGN_UP(pmm_page_count / 8, PAGE_SIZE);
    memset(pmm_bitmap, 0xFF, bitmap_size);

    for(uint32_t i = 0; i < mbd->mmap_length;
        i += sizeof(struct multiboot_memory_map_t))
    {
        struct multiboot_memory_map_t* mmmt = 
            (struct multiboot_memory_map_t*) (mbd->mmap_addr + i);

        for (uint32_t o = 0; o < mmmt->len_low; o += PAGE_SIZE) {
            if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
                bitmap_clear(pmm_bitmap, (mmmt->addr_low + o) / PAGE_SIZE);
            }
        }
    }

    printf("Displaying kernel memory map entry below:\n\n");

    printf("Start Addr: %x | Length: %x | Size: %x | Type: %d\n",
        kernel_mmmt->addr_low, kernel_mmmt->len_low, kernel_mmmt->size, kernel_mmmt->type);

    dprintf("pmm_install: PMM bitmap address: 0x%x\n", (uint32_t)pmm_bitmap);
    dprintf("pmm_install: initialized PMM\n");
}

uint32_t pmm_find_pages(uint32_t page_count) {
    uint32_t pages = 0;
    uint32_t first_page = pmm_last_page;

    while (pages < page_count) {
        if (pmm_last_page == pmm_page_count) {
            panic("out of memory");
        }

        if (!bitmap_get(pmm_bitmap, pmm_last_page + pages)) {
            /* Found a free page! */
            pages++;
            if (pages == page_count) {
                /* Found enough pages! */
                for (uint32_t i = 0; i < pages; i++) {
                    bitmap_set(pmm_bitmap, pmm_last_page + 1);
                }

                pmm_last_page += pages;
                return first_page;
            }
        } else {
            /* Didn't find a free page */
            pmm_last_page += (pages == 0 ? 1 : pages);
            first_page = pmm_last_page;
            pages = 0;
        }
    }
    return 0;
}

void *pmm_alloc(size_t page_count) {
    
}