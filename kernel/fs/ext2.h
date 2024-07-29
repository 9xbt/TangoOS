#ifndef __EXT2_H
#define __EXT2_H

#include <stdint.h>

struct ext2_superblock {
    uint32_t inodes;
    uint32_t blocks;
    uint32_t reserved_blocks;
    uint32_t free_blocks;
    uint32_t free_inodes;
    uint32_t block_num;
    uint32_t log2_block;
    uint32_t log2_fragment;
    uint32_t blocks_per_group;
    uint32_t fragments_per_group;
    uint32_t inodes_per_group;
    uint32_t last_mount_time;
    uint32_t laast_write_time;
    uint16_t mount_times_check;
    uint16_t mount_times_allowed;
    uint16_t signature;
    uint16_t state;
    uint16_t err_handler;
    uint16_t minor_ver;
    uint32_t last_consistency_check;
    uint32_t consistency_interval;
    uint32_t os_id;
    uint32_t major_ver;
    uint16_t reserved_blocks_uid;
    uint16_t reserved_blocks_gid;

    uint32_t first_inode;
    uint16_t inode_size;
    uint16_t superblock_block_group;
    uint32_t optional_features;
    uint32_t required_features;
    uint32_t features_for_rw;
    uint8_t  fs_id[16];
    uint8_t  vol_name[16];
    uint8_t  mount_point[64];
    uint32_t compression_algorithms_used;
    uint8_t  preallocated_blocks_for_files;
    uint8_t  preallocated_blocks_for_dirs;
    uint16_t unused;
    uint8_t  journal_id[16];
    uint32_t journal_inode;
    uint32_t journal_device;
    uint32_t head_of_oprphan_inode_list;
} __attribute__((packed));

void ext2_init(void);

#endif