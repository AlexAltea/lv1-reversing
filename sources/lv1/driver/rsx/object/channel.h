/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

#include "lv1/driver/rsx/object/context_dma.h"
#include "lv1/driver/rsx/object/nv_class.h"
#include "lv1/driver/rsx/object/sw_class.h"

// LV1 RSX object channel object, size 0x28
struct rsx_object_channel_t {
    S32 id;                     // 0x00: RSX channel object ID, 0 to 3
    S32 unk_04;                 // 0x04: ? ptr
    S64 *dma_obj;               // 0x08: DMA object
    S64 fc1_addr;               // 0x10: channel fc1 address, e.g. 0(0x28002020000), 1(0x28002020080), ... size 0x80
    S64 fc2_addr;               // 0x18: channel fc2 address, e.g. 0(0x28002030000), 1(0x28002031000), ... size 0x1000
    S64 graph_addr;             // 0x20: channel graph address, 0x28002060000

    // Methods
    rsx_object_channel_t(rsx_object_context_dma_t* dma_obj);

    S64 get_dma_control_lpar_address();
    S32 get_id();
    void create_sw_object_hash_table_entry(rsx_object_sw_class_t* sw_obj);
    void create_nv_object_hash_table_entry(rsx_object_nv_class_t* nv_obj);
    void create_dma_object_hash_table_entry(rsx_object_context_dma_t* dma_obj);
    void sub220D0C(S32 channel_id, rsx_object_context_dma_t* dma_obj);
} rsx_channel_obj;

//rsx_object_channel_destroy_sw_object_hash_table_entry
