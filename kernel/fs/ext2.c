#include <stdint.h>
#include <fs/ext2.h>
#include <mm/alloc.h>
#include <lib/libc.h>
#include <lib/printf.h>
#include <drivers/ata.h>

char *os_ids[] = {
    "Linux",
    "GNU HURD",
    "MASIX",
    "FreeBSD",
    "Other"
};

char *error_handling[] = {
    "",
    "Ignore",
    "Remount as RO",
    "Kernel panic"
};

struct ext2_fs *root_fs = NULL;

/*
 * ext2_read_block - reads a block from the partition.
 */
void ext2_read_block(struct ext2_fs *fs, uint32_t block, void *buffer) {
    ata_read(block * (fs->block_size / 512), buffer, (fs->block_size / 512));
}

/*
 * ext2_init - inits the ext2 driver.
 */
uint8_t ext2_init(void) {
    dprintf("\033[92m * \033[0minitializing filesystem... ");

    uint8_t *buf = kmalloc(1024);
    ata_read(2, buf, 2);
    struct ext2_sb *sb = (struct ext2_sb *)kmalloc(sizeof(struct ext2_sb));
    memcpy(sb, buf, sizeof(struct ext2_sb));

    if (sb->signature != 0xef53) {
        dprintf("ext2: not an ext2 partition\n");

        kfree(buf);
        kfree(sb);
        return 1;
    } else {
        dprintf("ext2: found ext2 partition\n");
    }
    kfree(buf);

    root_fs = (struct ext2_fs *)kmalloc(sizeof(struct ext2_fs));
    root_fs->sb = sb;
    root_fs->block_size = 1024 << sb->log2_block;
    root_fs->inode_size = sb->inode_size;

    /* base superblock fields */
    dprintf("ext2: inodes: %d\n", sb->inodes);
    dprintf("ext2: blocks: %d\n", sb->blocks);
    dprintf("ext2: reserved blocks: %d\n", sb->reserved_blocks);
    dprintf("ext2: free blocks: %d\n", sb->free_blocks);
    dprintf("ext2: free inodes: %d\n", sb->free_inodes);
    dprintf("ext2: starting block: %d\n", sb->block_num);
    dprintf("ext2: block size: %d\n", root_fs->block_size);
    dprintf("ext2: fragment size: %d\n", 1024 << sb->log2_fragment);
    dprintf("ext2: blocks per group: %d\n", sb->blocks_per_group);
    dprintf("ext2: fragments per group: %d\n", sb->fragments_per_group);
    dprintf("ext2: inodes per group: %d\n", sb->inodes_per_group);
    dprintf("ext2: last mount: %d\n", sb->last_mount_time);
    dprintf("ext2: last write: %d\n", sb->last_write_time);
    dprintf("ext2: mounts since last fsck: %d\n", sb->mount_times_check);
    dprintf("ext2: max amount of mounts before fsck: %d\n", sb->mount_times_allowed);
    dprintf("ext2: partition state: %s\n", sb->state == 1 ? "clean" : "has errors");
    dprintf("ext2: error detection handling: %s\n", error_handling[sb->err_handler]);
    dprintf("ext2: revision %d.%d\n", sb->major_ver, sb->minor_ver);
    dprintf("ext2: last fsck: %d\n", sb->last_consistency_check);
    dprintf("ext2: fsck force interval: %d\n", sb->consistency_interval);
    dprintf("ext2: partition created on %s\n", os_ids[sb->os_id]);
    dprintf("ext2: reserved block UID: %d\n", sb->reserved_blocks_uid);
    dprintf("ext2: reserved block GID: %d\n", sb->reserved_blocks_gid);
    /* extended superblock fields */
    dprintf("ext2: first inode: %d\n", sb->first_inode);
    dprintf("ext2: inode size: %d\n", sb->inode_size);
    dprintf("ext2: superblock block group: %d\n", sb->sb_block_group);
    dprintf("ext2: optional features: %s%s%s%s%s%s\n",
        (sb->optional_features & 0x01) ? "has preallocation, " : "",
        (sb->optional_features & 0x02) ? "AFS server inodes, " : "",
        (sb->optional_features & 0x04) ? "is ext3, " : "",
        (sb->optional_features & 0x08) ? "extended attributing, " : "",
        (sb->optional_features & 0x10) ? "resizable fs, " : "",
        (sb->optional_features & 0x20) ? "hash indexing" : "");
    dprintf("ext2: required features: %s%s%s%s\n",
        (sb->required_features & 0x1) ? "compressed, " : "",
        (sb->required_features & 0x2) ? "dirs have type field, " : "",
        (sb->required_features & 0x4) ? "fs needs journal replay, " : "",
        (sb->required_features & 0x8) ? "fs uses journal dev" : "");
    dprintf("ext2: features required for R/W: %s%s%s\n",
        (sb->required_features & 0x1) ? "sparse sb & gdt, " : "",
        (sb->required_features & 0x2) ? "64-bit, " : "",
        (sb->required_features & 0x4) ? "binary tree dir contents" : "");
    dprintf("ext2: filesystem uuid: %2x%2x%2x%2x-%2x%2x-%2x%2x-%2x%2x-%2x%2x%2x%2x%2x%2x\n",
        sb->fs_uuid[0],  sb->fs_uuid[1],  sb->fs_uuid[2],  sb->fs_uuid[3],
        sb->fs_uuid[4],  sb->fs_uuid[5],  sb->fs_uuid[6],  sb->fs_uuid[7],
        sb->fs_uuid[8],  sb->fs_uuid[9],  sb->fs_uuid[10], sb->fs_uuid[11],
        sb->fs_uuid[12], sb->fs_uuid[13], sb->fs_uuid[14], sb->fs_uuid[15]);
    dprintf("ext2: volume label: %s\n", sb->vol_name);
    dprintf("ext2: last mount point: %s\n", sb->last_mount_point);
    dprintf("ext2: compression algorithms used: %x\n", sb->compression_algorithms_used);
    dprintf("ext2: preallocated blocks for files: %d\n", sb->preallocated_blocks_for_files);
    dprintf("ext2: preallocated blocks for dirs: %d\n", sb->preallocated_blocks_for_dirs);
    dprintf("ext2: journal uuid: %2x%2x%2x%2x-%2x%2x-%2x%2x-%2x%2x-%2x%2x%2x%2x%2x%2x\n",
        sb->journal_id[0],  sb->journal_id[1],  sb->journal_id[2],  sb->journal_id[3],
        sb->journal_id[4],  sb->journal_id[5],  sb->journal_id[6],  sb->journal_id[7],
        sb->journal_id[8],  sb->journal_id[9],  sb->journal_id[10], sb->journal_id[11],
        sb->journal_id[12], sb->journal_id[13], sb->journal_id[14], sb->journal_id[15]);
    dprintf("ext2: journal inode: %d\n", sb->journal_inode);
    dprintf("ext2: journal device: %d\n", sb->journal_device);
    dprintf("ext2: head of oprphan inode list: %d\n", sb->head_of_oprphan_inode_list);

    /* TODO: test ext2_read_block */

    dprintf("\033[94m[\033[92mok\033[94m]\033[0m\n");
    return 0;
}