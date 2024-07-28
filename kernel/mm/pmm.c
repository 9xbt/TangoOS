#include <mm/pmm.h>
#include <lib/libc.h>
#include <lib/panic.h>
#include <lib/printf.h>
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

    struct multiboot_memory_map_t *kernel_mmmt = NULL;

    /* Loop through the memory map and display the values */
    for(int i = 0; i < mbd->mmap_length;
        i += sizeof(struct multiboot_memory_map_t))
    {
        struct multiboot_memory_map_t* mmmt = 
            (struct multiboot_memory_map_t*) (mbd->mmap_addr + i);

        printf("Start Addr: %x | Length: %x | Size: %x | Type: %d\n",
            mmmt->addr_low, mmmt->len_low, mmmt->size, mmmt->type);

        if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE &&
            mmmt->addr_low >= 0x100000 && kernel_mmmt == NULL) {
            /* 
             * Do something with this memory block!
             * BE WARNED that some of memory shown as availiable is actually 
             * actively being used by the kernel! You'll need to take that
             * into account before writing to memory!
             */
            kernel_mmmt = mmmt;
        }
    }

    pmm_bitmap = &end;
    pmm_page_count = kernel_mmmt->len_low / PAGE_SIZE;
    uint32_t bitmap_size = ALIGN_UP(pmm_page_count / 8, PAGE_SIZE);

    memset(pmm_bitmap, 0xFF, bitmap_size);

    dprintf("pmm_install: pmm bitmap: 0x%x\n", (uint32_t)pmm_bitmap);
    dprintf("pmm_install: initialized PMM\n");
}