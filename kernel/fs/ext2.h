#ifndef __EXT2_H
#define __EXT2_H

#include <stdint.h>

struct ext2_sb {
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
    uint32_t last_write_time;
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
    uint16_t sb_block_group;
    uint32_t optional_features;
    uint32_t required_features;
    uint32_t features_for_rw;
    uint8_t  fs_id[16];
    uint8_t  vol_name[16];
    uint8_t  last_mount_point[64];
    uint32_t compression_algorithms_used;
    uint8_t  preallocated_blocks_for_files;
    uint8_t  preallocated_blocks_for_dirs;
    uint16_t unused;
    uint8_t  journal_id[16];
    uint32_t journal_inode;
    uint32_t journal_device;
    uint32_t head_of_oprphan_inode_list;
};

struct ext2_bgd {
    uint32_t bitmap_block;
    uint32_t bitmap_inode;
    uint32_t inode_table;
    uint16_t free_blocks;
    uint16_t free_inodes;
    uint16_t directories;
    uint8_t  unused[14];
};

// finally we're doing ext2 stuff, i n o d e s
// inode_modules
// lmao
struct ext2_inode {
    uint16_t type_permissions;
    uint16_t user_id;
    uint32_t size;
    uint32_t last_access_time;
    uint32_t creation_time;
    uint32_t last_modify_time;
    uint32_t deletion_time;
    uint16_t group_id;
    uint16_t hard_links;
    uint32_t sectors;
    uint32_t flags;
    uint32_t os_specific;
    uint32_t direct_block_ptr[12];
    uint32_t singly_block_ptr;
    uint32_t doubly_block_ptr;
    uint32_t triply_block_ptr;
    uint32_t generation;
    uint32_t file_acl;
    uint32_t dir_acl;
    uint32_t fragment_addr;
    uint32_t os_specific_2[3];
};

struct ext2_dir_entry {
    uint32_t inode;
    uint16_t size;
    uint8_t  name_length;
    uint8_t  type_indicator;
    uint8_t *name;
};

struct ext2_fs {
    struct ext2_sb *sb;
    struct ext2_bgd *bgd_table;
    struct ext2_inode *root_inode;
    uint32_t block_size;
    uint32_t bgd_count;
    uint32_t bgd_block;
    uint32_t inode_size;
};

uint8_t ext2_init(void);

#endif