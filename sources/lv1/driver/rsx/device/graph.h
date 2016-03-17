/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// RSX device graph object, size 0x198
struct rsx_graph_obj_t {
    S64 graph_addr;            // 0x000: RSX graph address 0x28002060000 (memory region 6 start address)
    S32 graph_offset;          // 0x008: RSX graph offset (0x60000), from 0x28002000000
    S32 graph_size;            // 0x00C: RSX graph size 0x80000(512 KB) (memory region 6 size)
    //---------------------------------------------------------------------------------------
    S64 channel_addr[16];      // 0x010: RSX channel address into GRAPH, 0x28002060000
    //---------------------------------------------------------------------------------------
    S32 max_channels;          // 0x090: 4, the max RSX channels
    S32 unk_094;               // 0x094: 
    S32 unk_098;               // 0x098: ? offset into graph memory
    S32 unk_09C;               // 0x09C: 
    S32 unk_0A0;               // 0x0A0: ? offset into graph memory
    S32 unk_0A4;               // 0x0A4: 
    S32 unk_0A8;               // 0x0A8: ? offset into graph memory
    S32 unk_0AC;               // 0x0AC: 
    //---------------------------------------------------------------------------------------
    S64 *vtab;                 // 0x0B0: vtab
    //---------------------------------------------------------------------------------------
    S64 unk_0B8;               // 0x0B8: 
    S64 unk_0C0;               // 0x0C0: 
    S64 unk_0C8;               // 0x0C8: 
    S64 unk_0D0;               // 0x0D0: 
    S64 unk_0D8;               // 0x0D8: 
    S64 unk_0E0;               // 0x0E0: 
    S64 unk_0E8;               // 0x0E8: 
    S64 unk_0F0;               // 0x0F0: 
    S64 unk_0F8;               // 0x0F8: 
    S64 unk_100;               // 0x100: 
    S64 unk_108;               // 0x108: 
    S64 unk_110;               // 0x110: 
    S64 unk_118;               // 0x118: 
    S64 unk_120;               // 0x120: 
    S64 unk_128;               // 0x128: 
    S64 unk_130;               // 0x130: 
    S64 unk_138;               // 0x138: 
    S64 unk_140;               // 0x140: 
    S64 unk_148;               // 0x148: 
    S64 unk_150;               // 0x150: 
    S64 unk_158;               // 0x158: 
    S64 unk_160;               // 0x160: 
    S64 unk_168;               // 0x168: 
    S64 unk_170;               // 0x170: 
    S64 unk_178;               // 0x178: 
    S64 unk_180;               // 0x180: 
    
    S32 unk_188;               // 0x188: 
    S32 unk_18C;               // 0x18C: 
    S32 unk_190;               // 0x190: 
    S32 unk_194;               // 0x194: 

    // Methods
    S64  get_graph_channel_address_by_id(S32 channel_id);
    S32  sub21D224(S32 arg1);
    void sub21D054();
    void sub21D038();
    void sub21D01C();
} rsx_graph_obj;

//--- init -------------------------------------------------------------
void rsx_device_graph_21E9CC(S64 *vtab);
void rsx_device_graph_21D2DC(rsx_graph_obj_t* graph);
void rsx_device_graph_21D350(rsx_graph_obj_t* graph);
S32 rsx_device_graph_21EF60(S64 *vtab);
void rsx_device_graph_21ED38(S64 *vtab, S32 offset, S32 arg1, S32 arg2);
void rsx_device_graph_21DA04(rsx_graph_obj_t* graph, S32 idx);
S32 rsx_device_graph_21E950(void);
S32 rsx_device_graph_21D6C0(void);
void rsx_device_graph_21E95C(S64 *vtab, S32 *arg1, S32 *arg2, S32 *arg3, S32 *arg4, S32 *arg5);
void rsx_device_graph_21DD9C(rsx_graph_obj_t* graph);
void rsx_device_graph_21E93C(S64 *vtab, S64 *arg1, S32 *arg2);
void rsx_device_graph_21D7A0(rsx_graph_obj_t* graph);
void rsx_device_graph_init(rsx_graph_obj_t* graph);
