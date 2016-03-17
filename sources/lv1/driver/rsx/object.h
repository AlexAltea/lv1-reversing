/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#define MAX_RSX_CORE_ID     16         // max rsx core object count
#define MAX_DEV             16         // max RSX devices



////////////////////////////////////////////////////////////////////////
// LV1 RSX objects


// LV1 RSX context display buffer object, size 0x28
struct rsx_ctx_dbuf_obj_t {
    S8 unk_00;                  // 0x00: flag
    S8 pad0[3];                 // 0x01: 
    S32 width;                  // 0x04: display buffer width in pixels
    S32 height;                 // 0x08: display buffer height in pixels
    S32 pitch;                  // 0x0C: display buffer pitch size in byte
    S64 unk_10;                 // 0x10: 
    S64 bar1_addr;              // 0x18: BAR1(VRAM) address of display buffer
    S64 dma_obj;                // 0x20: DMA object, e.g. 0xDAC10001
} rsx_ctx_dbuf_obj;


// LV1 RSX object v framebuffer object, size 0x40
struct rsx_obj_vfb_obj_t {
    S64 unk_00;                 // 0x00: 
    S64 unk_08;                 // 0x08: 
    S64 unk_10;                 // 0x10: 
    S32 unk_18;                 // 0x18: 
    S8 unk_1C;                  // 0x1C: 
    S8 unk_1D[3];               // 0x1D: padding
    S64 unk_20;                 // 0x20: 
    S64 unk_28;                 // 0x28: 
    S64 unk_30;                 // 0x30: 
    S64 unk_38;                 // 0x38: 
} rsx_obj_vfb_obj;

// LV1 RSX object video RSX object, size 0xC0
struct rsx_obj_video_rsx_t {
    S8 unk_00;                   // 0x00: RSX LV1 debug level, 0(off) to 3(max debug output)
    S8 unk_01[3];                // 0x00: padding
    //--------------------------------------------------------------------
    S32 unk_04;                  // 0x04: ?
    S32 unk_08;                  // 0x08: ?
    S32 unk_0C;                  // 0x0C: ?
    S32 unk_10;                  // 0x10: ?
    S32 unk_14;                  // 0x14: ?
    S32 unk_18;                  // 0x18: ?
    S32 unk_1C;                  // 0x1C: ?
    S32 unk_20;                  // 0x20: ?
    //--------------------------------------------------------------------
    S32 unk_24[2];               // 0x24: ?
    //--------------------------------------------------------------------
    S32 unk_2C[3];               // 0x2C: [0], 
    // S32 unk_30;               // 0x30: [1], 
    // S32 unk_34;               // 0x34: [2], 
    //--------------------------------------------------------------------
    S64 unk_38;                  // 0x38: 
    S64 unk_40;                  // 0x40: 
    //--------------------------------------------------------------------
    S32 unk_48;                  // 0x48: ? init with 0x20200
    S32 unk_4C;                  // 0x4C: ? init with 0x20200
    //--------------------------------------------------------------------
    S8 unk_50;                   // 0x50: ?
    S8 unk_51;                   // 0x51: ?
    S8 unk_52;                   // 0x52: ?
    S8 unk_53;                   // 0x53: ?
    S8 unk_54;                   // 0x54: ?
    S8 unk_55;                   // 0x55: ?
    S8 unk_56;                   // 0x56: ?
    S8 unk_57;                   // 0x57: ?
    //--------------------------------------------------------------------
    S32 unk_58;                  // 0x58: ?
    S32 unk_5C;                  // 0x5C: ?
    S32 unk_60;                  // 0x60: ?
    S32 unk_64;                  // 0x64: ?
    S32 unk_68;                  // 0x68: ?
    S32 unk_6C;                  // 0x6C: ?
    S32 unk_70;                  // 0x70: ?
    S32 unk_74;                  // 0x74: ?
    //--------------------------------------------------------------------
    S8 unk_78[2];                // 0x78: ? 
    S8 unk_7A[2];                // 0x7A: padding
    S32 unk_7C[2];               // 0x7C: ?
    S32 unk_84[2];               // 0x84: ?
    //--------------------------------------------------------------------
    S32 unk_8C;                  // 0x8C: ?
    //--------------------------------------------------------------------
    S64 *unk_90;                 // 0x90: prt, unk lv1 obj
    S64 *unk_98;                 // 0x98: prt, unk lv1 obj
    //--------------------------------------------------------------------
    S64 unk_A0[2];               // 0xA0: ?
    //--------------------------------------------------------------------
    S64 unk_B0[2];               // 0xB0: ? clock
} rsx_obj_video_rsx;

// RSX hash table object, size 0x18
struct rsx_hash_tbl_obj_t {
    S32 unk_00;             // 0x00: 
    S32 size_0;             // 0x04: RSX hash table size, 0x8000(32 KB)
    S64 io_addr;            // 0x08: RSX hash table IO address, 0x28002010000
    S32 size_1;             // 0x10: RSX hash table size, 0x8000(32 KB)
    S32 bar0_offset;        // 0x14: offset into BAR0
} rsx_hash_tbl_obj;

// RSX external interrupt controller object, DUMMY, TODO
struct rsx_eic_obj_t {
    S32 unk_000;              // 0x00: 
    // ...
} rsx_eic_obj;

// RSX device clock setting
struct rsx_dev_clock_setting_t {
    S32 unk_00;               // 0x00: 
    S32 unk_04;               // 0x04: 
    S32 offset_0;             // 0x08: 
    S32 offset_1;             // 0x0C: 
    S32 unk_10;               // 0x10: 
} rsx_dev_clock_setting;

// LV1 RSX debug object
struct rsx_dbg_obj_t {
    S64 unk_00;                  // 0x00: ? init 0
    S32 unk_08;                  // 0x08: ?
    S32 unk_0C;                  // 0x0C: ?
    S64 unk_10;                  // 0x10: init 0xF4240(1000000)
    S64 unk_18;                  // 0x18: init creation-time tb value + unk_10
    S64 unk_20;                  // 0x20: ?
} rsx_dbg_obj;
