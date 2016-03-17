/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// RSX hash table object, size 0x18
struct rsx_object_hash_table_t {
    S32 unk_00;             // 0x00: 
    S32 size_0;             // 0x04: RSX hash table size, 0x8000(32 KB)
    S64 io_addr;            // 0x08: RSX hash table IO address, 0x28002010000
    S32 size_1;             // 0x10: RSX hash table size, 0x8000(32 KB)
    S32 bar0_offset;        // 0x14: offset into BAR0

    // Methods
    void init();
    void finalize();
    void create_entry(S32 channel_id, U32 class_type, S32 offset, S32 arg1);
    void invalidate_entry(S32 channel_id, U32 class_type);
};
