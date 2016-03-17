/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// LV1 RSX utils bitmap object, size 0x38
struct rsx_utils_bitmap_t {
    S64 value1;                 // 0x00: base, e.g. a memory start address
    S64 item_total;             // 0x08: count of items in bitmap
    S64 value2;                 // 0x10: size one item, e.g. a memory page size
    S32 seg_base;               // 0x18: 0x40, bits per segment(one u64)
    S32 unk_1C;                 // 0x1C: ?
    S64 unk_20;                 // 0x20: ?
    S32 seg_count;              // 0x28: segment count of bitmap
    S32 unk_2C;                 // 0x2C: ?
    S64 *bitmap;                // 0x30: address of bitmap

    // Methods
    S32  get_item_total();
    void free_map();
    S32  dealloc(S64 start, S32 count);
    S32  allocate(S32 count, S64 *out);
    void set_bit(S32 idx, S32 value);
    void create(S64 val_1, S32 item_total, S64 val_2);
};
