/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

#include "lv1/driver/rsx/object/channel.h"
#include "lv1/driver/rsx/object/context_dma.h"
#include "lv1/driver/rsx/object/nv_class.h"
#include "lv1/driver/rsx/object/sw_class.h"

// LV1 RSX context object, size 0x340
struct rsx_core_context_t {
    S64 core_id;                // 0x000: RSX core object ID
    U32 id;                     // 0x008: RSX context ID, (index XOR 0x55555555)
    S32* unk_00C;               // 0x00C: ? ptr
    S64* mem_ctx;               // 0x010: RSX memory context
    U64 sys_mode;               // 0x018: system mode flags, [27:27]("flag local pb"), [28:28]("flag system sema"), [29:29]("flag gsemu ctx"), [30:30]("small io page size")
    S64 unk_020;                // 0x020: ? eic, ctx 0(0x38), ctx 1(0x39)
    S32 index;                  // 0x028: RSX context object index, 0, 1 or 2
    S32 unk_02C;                // 0x02C: ?
    S64 io_offset;              // 0x030: IO offset, ctx 0(0x80000000), ctx 1(0x90000000)
    S64 unk_038;                // 0x038: ?
    S64 io_size;                // 0x040: IO size, 0x10000000(256 MB)
    S64 page_size;              // 0x048: page size, based on system mode[29:29]: 0(1 MB), 1(64 KB)
    //--------------------------------------------------------------------
    rsx_object_nv_class_t* nv_obj[8];             // 0x050: nv class objects
    //--------------------------------------------------------------------
    rsx_object_sw_class_t* sw_obj;                // 0x090: sw class object, 0xCAFEBABE
    //--------------------------------------------------------------------
    S64* dma_098;               // 0x098: dma class object, 0xFEED0000 or 0xFEED0001
    //--------------------------------------------------------------------
    S64* dma_0A0;               // 0x0A0: dma class object, 0xFEED0000
    S64* dma_0A8;               // 0x0A8: dma class object, 0xFEED0001
    //--------------------------------------------------------------------
    S64 unk_0B0;                // 0x0B0: 
    //--- if "flag gsemu ctx" --------------------------------------------
    S64* dma_0B8;               // 0x0B8: dma class object, 0xFEED0003
    S64* dma_0C0;               // 0x0C0: dma class object, 0xFEED0004
    //--------------------------------------------------------------------
    S64 reports_lpar_addr;      // 0x0C8: 
    S64 reports_addr;           // 0x0D0: BAR1 reports address, ctx 0(0x2808FE00000), ctx 1(0x2808FE10000)
    S64 reports_size;           // 0x0D8: size of reports, 0x10000(64 KB)
    S64 unk_0E0;                // 0x0E0: BAR1 address: ctx 0(0x2808FE01000), ctx 1(0x2808FE11000)
    S64 unk_0E8;                // 0x0E8: BAR1 address: ctx 0(0x2808FE00000), ctx 1(0x2808FE10000)
    S64 unk_0F0;                // 0x0F0: BAR1 address: ctx 0(0x2808FF10000), ctx 1(0x2808FF10000)
    S64 unk_0F8;                // 0x0F8: BAR1 address: ctx 0(0x2808FE01400), ctx 1(0x2808FE11400)
    //--------------------------------------------------------------------
    rsx_object_context_dma_t* dma_array_0[8];        // 0x100: dma class objects, 0x66604200 to 0x66604207
    //--------------------------------------------------------------------
    rsx_object_context_dma_t* dma_array_1[8];        // 0x140: dma class objects, 0x66604208 to 0x6660420F
    //--------------------------------------------------------------------
    S64* dma_180;               // 0x180: dma class object, 0x66606660
    S64* dma_188;               // 0x188: dma class object, 0x66616661
    //-------------------------------------------------------------------- 
    S64* dma_190;               // 0x190: dma class object, 0x66626660
    S64* dma_198;               // 0x198: dma class object, 0xBAD68000
    //--------------------------------------------------------------------
    S64* dma_1A0;               // 0x1A0: dma class object, 0x13378086
    S64* dma_1A8;               // 0x1A8: dma class object, 0x13378080
    //-------------------------------------------------------------------- 
    S64* dma_1B0;               // 0x1B0: dma class object, 0x56616660
    S64* dma_1B8;               // 0x1B8: dma class object, 0x56616661
    //-------------------------------------------------------------------- 
    rsx_object_channel_t* ch_obj;  // 0x1C0: RSX channel object
    S64 unk_1C8;                // 0x1C8: BAR1 address: ctx 0(0x2808FE01000), ctx 1(0x2808FE11000)
    S64 unk_1D0;                // 0x1D0: BAR1 address: ctx 0(0x2808FE00000), ctx 1(0x2808FE10000)
    S64 unk_1D8;                // 0x1D8: BAR1 address: ctx 0(0x2808FE01400), ctx 1(0x2808FE11400)
    //--------------------------------------------------------------------
    S08 db_flag[8];             // 0x1E0: display buffer flag, 0 to 7
    rsx_ctx_dbuf_obj_t d_buf[8];    // 0x1E8: display buffer objects, 0 to 7
    //--------------------------------------------------------------------
    S64 driver_info_addr;       // 0x328: driver info address
    S64 driver_info_lpar;       // 0x330: driver info LPAR address
    S64 driver_info_addr_0;     // 0x338: driver info address too ?

    // Methods
    rsx_core_context_t(S64 core_id, rsx_mem_ctx_obj_t* mem_ctx, U64 system_mode);

    void init(U64 *out, S64 core_id, rsx_mem_ctx_obj_t* mem_ctx, U64 system_mode, S32 idx);
    S32 iomap(U64 ea, S64 lpar_addr, S32 size, U64 flags);
    S64 get_dma_control_lpar_address();
    rsx_object_context_dma_t* get_dma_object_by_index(S32 index);
    S64 get_driver_info_lpar_addr();
    U32 get_rsx_context_id();
    S32 get_size_of_reports(void);
    void sub213614();
    void sub2136CC();
};
