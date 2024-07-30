#include <cpu/io.h>
#include <lib/printf.h>
#include <drivers/vga.h>
#include <drivers/ata.h>

uint16_t ata_base;
uint8_t ata_type;
char ata_drive_name[41];

/*
 * ata_400ns - reads the status register enough times for a 400ns delay to ensure proper data
 */
void ata_400ns(void) {
    for (int i = 0; i < 4; i++) {
        inb(ata_base + 7);
    }
}

/*
 * ata_poll - polls the drive
 */
uint8_t ata_poll() {
    ata_400ns();

    uint8_t status = 0;
    for (;;) {
        status = inb(ata_base + 7);
        if (!(status & 0x80)) {
            break;
        } 
        if (status & 0x08) {
            break;
        }
        if (status & 0x01) {
            return ATA_DISK_ERR;
        }
    }

    return ATA_OK;
}

/*
 * ata_identify - identifies a drive and sets the drive label
 */
uint8_t ata_identify(uint16_t base, uint8_t type) {
    ata_base = base;
    ata_type = type;

    outb(base + 6, type); /* select master/slave */
    for (uint16_t i = 0x1F2; i != 0x1F5; i++) {
        outb(i, 0);
    }
    outb(base + 7, ATA_IDENTIFY); /* send identify command */

    uint8_t status = inb(base + 7);
    if (!status) {
        return ATA_NO_DRIVES;
    }
    if (ata_poll() != ATA_OK) {
        return ATA_DISK_ERR;
    }

    uint8_t ident_buf[512];
    ata_read(0, ident_buf, 1);

    /* reverse the buffer & store the drive name */
    for (uint8_t i = 0; i < 40; i += 2) {
        ata_drive_name[i] = ident_buf[54 + i + 1];
        ata_drive_name[i + 1] = ident_buf[54 + i];
    }

    /* add null terminator to the drive name */
    for (uint8_t i = 39; i > 0; i--) {
        if (ata_drive_name[i] != ' ') {
            ata_drive_name[i + 1] = 0;
            break;
        }
    }

    ata_400ns();
    return ATA_OK;
}

/*
 * ata_read - reads drive.
 */
__attribute__((no_sanitize("undefined")))
uint8_t ata_read(uint32_t lba, uint8_t *buffer, uint32_t sectors) {
    outb(ata_base + 6, (ata_type == ATA_MASTER ? 0xE0 : 0xF0) | ((lba >> 24) & 0x0F));
    outb(ata_base + 1, ATA_WAIT);
    outb(ata_base + 2, sectors);
    outb(ata_base + 3, (uint8_t)lba);
    outb(ata_base + 4, (uint8_t)(lba >> 8));
    outb(ata_base + 5, (uint8_t)(lba >> 16));
    outb(ata_base + 7, ATA_READ);

    uint16_t *buf = (uint16_t *)buffer;
    for (uint32_t i = 0; i < sectors * 256; i++) {
        if (ata_poll() != ATA_OK) {
            return ATA_DISK_ERR;
        }
        buf[i] = inw(ata_base);
    }

    ata_400ns();
    return ATA_OK;
}

/*
 * ata_write - writes drive.
 */
__attribute__((no_sanitize("undefined")))
uint8_t ata_write(uint32_t lba, uint8_t *buffer, uint32_t sectors) {
    outb(ata_base + 6, (ata_type == ATA_MASTER ? 0xE0 : 0xF0) | ((lba >> 24) & 0x0F));
    outb(ata_base + 1, ATA_WAIT);
    outb(ata_base + 2, sectors);
    outb(ata_base + 3, (uint8_t)lba);
    outb(ata_base + 4, (uint8_t)(lba >> 8));
    outb(ata_base + 5, (uint8_t)(lba >> 16));
    outb(ata_base + 7, ATA_WRITE);

    uint16_t *buf = (uint16_t *)buffer;
    for (uint32_t i = 0; i < sectors * 256; i++) {
        if (ata_poll() != ATA_OK) {
            return ATA_DISK_ERR;
        }
        outw(ata_base, buf[i]);
    }

    ata_400ns();
    return ATA_OK;
}

/*
 * ata_install - inits the primary master drive.
 */
void ata_install(void) {
    dprintf("\033[92m * \033[0minitializing disks... ");
    if (ata_identify(ATA_PRIMARY, ATA_MASTER)) {
        dprintf("\033[94m[\033[91mfail\033[94m]\033[0m\n");
    } else {
        dprintf("\033[94m[\033[92mok\033[94m]\033[0m\n");
    }
}