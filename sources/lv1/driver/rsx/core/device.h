/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

#include "lv1/driver/rsx/device/clock.h"

// LV1 RSX device core object, size 0x1C8
struct rsx_core_device_t {
    S32 unk_000;                 // 0x000: ?
    S32 *unk_004;                // 0x004: ? address
    S64 device_id;               // 0x008: RSX device ID
    S64 *ioif0;                  // 0x010: RSX IO interface 0 object
    S64 core_id;                 // 0x018: RSX device core object ID  
    //S32 unk_01C;               // 0x01C: sometime RSX device core object ID as 32bit
    S64 *core_mem_obj;           // 0x020: RSX core memory object
    //--------------------------------------------------------------------
    S64 *rsx_ctx[3];             // 0x028: RSX context 0(ID: 0x55555555), 1(ID: 0x55555554) and 3(ID: 0x55555553)
    //--------------------------------------------------------------------
    S64 *unk_040;                // 0x040: ? removed object
    //--------------------------------------------------------------------
    S64 dev_lpar_addr[MAX_DEV];  // 0x048: logical partition addresses of asserted devices, 0 to 15.
                                     //        E.g. idx 1 = RSX audio device,
                                     //        E.g. idx 8 = RSX device 8, our GPU.
    //--- objects --------------------------------------------------------
    S64 *dev_clock_1_obj;        // 0x0C8: RSX device clock object 1, "nvclk"
    S64 *dev_clock_5_obj;        // 0x0D0: RSX device clock object 5, "display clk"
    S64 *eic_obj;                // 0x0D8: RSX external interrupt controller object
    S64 *dev_fb_obj;             // 0x0E0: RSX device framebuffer object
    S64 *dev_graph_obj;          // 0x0E8: RSX device graph object
    S64 *unk_0F0;                // 0x0F0: ?
    S64 *dev_fifo_obj;           // 0x0F8: RSX device fifo object
    S64 *dev_audio_obj;          // 0x100: RSX device audio object
    S64 *obj_hash_tbl_obj;       // 0x108: RSX object hash table object
    S64 *obj_video_rsx_obj;      // 0x110: RSX object video_rsx object
    S64 *obj_vfb_obj;            // 0x118: RSX object video framebuffer, vfb object ever zero, seams to be the 2D stuff for otherOS(no 3D)
    S64 *bm_obj_channels;        // 0x120: a RSX utils bitmap object, 4 items, the max(4) RSX channels
    //--------------------------------------------------------------------
    // ??? tb values, for mesure things like total time used for interrupts, and other things
    S64 unk_128;                 // 0x128: ?
    S64 unk_130;                 // 0x130: ?
    S64 unk_138;                 // 0x138: ?
    S64 unk_140;                 // 0x140: ?
    S64 unk_148;                 // 0x148: ?
    //------------------------------------------
    S64 unk_150;                 // 0x150: ?
    S64 unk_158;                 // 0x158: ?
    S64 unk_160;                 // 0x160: ?
    //------------------------------------------
    S64 unk_168;                 // 0x168: ?
    S64 unk_170;                 // 0x170: ?
    S64 unk_178;                 // 0x178: ?
    S64 unk_180;                 // 0x180: ?
    S64 unk_188;                 // 0x188: ?
    //------------------------------------------
    S64 unk_190;                 // 0x190: ?
    S64 unk_198;                 // 0x198: ?
    S64 unk_1A0;                 // 0x1A0: ?
    S64 unk_1A8;                 // 0x1A8: ?
    S32 unk_1B0;                 // 0x1B0: ?
    S32 unk_1B4;                 // 0x1B4: ?
    S32 unk_1B8;                 // 0x1B8: ?
    S32 unk_1BC;                 // 0x1BC: ?
    S64 unk_1C0;                 // 0x1C0: ?
};


S32 rsx_core_device_map_device(rsx_core_device_t* core, S32 device_id, S64 *dev_lpar_addr, S64 *dev_lpar_size);
void rsx_core_device_finalize(rsx_core_device_t* core);
S32 rsx_core_device_close_device(S32 dev_core_id);

rsx_core_device_t* rsx_core_device_get_core_object(void);
S32 rsx_core_device_get_rsx_enable_flag(void);
rsx_core_device_t* rsx_core_device_get_core_object_by_id(S32 dev_core_id);

S32 rsx_core_device_open(S32 arg1, S32 dev_core_id);

extern S32 g_rsx_core_id;