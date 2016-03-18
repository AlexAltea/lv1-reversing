/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

#include "lv1/driver/rsx/core/device.h"
#include "lv1/driver/rsx/core/memory.h"
#include "lv1/driver/rsx/object/channel.h"
#include "lv1/driver/rsx/object/context_dma.h"
#include "lv1/driver/rsx/object/nv_class.h"
#include "lv1/driver/rsx/object/sw_class.h"

enum {
    L1GPU_SYSTEM_MODE_SMALL_IO_PAGES  = (1 <<  1),  // Use 64 KB pages for GART memory mapping (otherwise 1 MB)
    L1GPU_SYSTEM_MODE_GSEMU           = (1 <<  2),  // Create DMA objects: 0xFEED0003, 0xFEED0004
    L1GPU_SYSTEM_MODE_SYSTEM_SEMA     = (1 <<  3),  // System semaphore
    L1GPU_SYSTEM_MODE_LOCAL_PB        = (1 <<  4),  // Local PB
    L1GPU_SYSTEM_MODE_UNK20           = (1 <<  5),  // Create DMA objects: 0xBAD68000
    L1GPU_SYSTEM_MODE_UNK400          = (1 << 10),  // Use 512 MB of VRAM IO address space (otherwise 256 MB)
    L1GPU_SYSTEM_MODE_UNK800          = (1 << 11),  // Set IRQ mask to 0x00000000 (otherwise 0xFFFFFFFF)
};

// LV1 RSX context display buffer object, size 0x28
struct rsx_ctx_dbuf_obj_t {
    S08 unk_00;                  // 0x00: flag
    S08 pad0[3];                 // 0x01: 
    S32 width;                  // 0x04: display buffer width in pixels
    S32 height;                 // 0x08: display buffer height in pixels
    S32 pitch;                  // 0x0C: display buffer pitch size in byte
    S64 unk_10;                 // 0x10: 
    S64 bar1_addr;              // 0x18: BAR1(VRAM) address of display buffer
    S64 dma_obj;                // 0x20: DMA object, e.g. 0xDAC10001
};

// LV1 RSX context object, size 0x340
class rsx_core_context_t {
    S64 core_id;                // 0x000: RSX core object ID
    U32 id;                     // 0x008: RSX context ID, (index XOR 0x55555555)
    S32* unk_00C;               // 0x00C: ? ptr
    S64* mem_ctx;               // 0x010: RSX memory context
    U64 system_mode;               // 0x018: system mode flags, [27:27]("flag local pb"), [28:28]("flag system sema"), [29:29]("flag gsemu ctx"), [30:30]("small io page size")
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
    rsx_object_context_dma_t* dma_098;               // 0x098: dma class object, 0xFEED0000 or 0xFEED0001
    //--------------------------------------------------------------------
    rsx_object_context_dma_t* dma_0A0;               // 0x0A0: dma class object, 0xFEED0000
    rsx_object_context_dma_t* dma_0A8;               // 0x0A8: dma class object, 0xFEED0001
    //--------------------------------------------------------------------
    S64 unk_0B0;                // 0x0B0: 
    //--- if "flag gsemu ctx" --------------------------------------------
    rsx_object_context_dma_t* dma_0B8;               // 0x0B8: dma class object, 0xFEED0003
    rsx_object_context_dma_t* dma_0C0;               // 0x0C0: dma class object, 0xFEED0004
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
    rsx_object_context_dma_t* dma_array_1[8];        // 0x140: dma class objects, 0x66604208 to 0x6660420F
    //--------------------------------------------------------------------
    rsx_object_context_dma_t* dma_180;               // 0x180: dma class object, 0x66606660
    rsx_object_context_dma_t* dma_188;               // 0x188: dma class object, 0x66616661
    rsx_object_context_dma_t* dma_190;               // 0x190: dma class object, 0x66626660
    rsx_object_context_dma_t* dma_198;               // 0x198: dma class object, 0xBAD68000
    rsx_object_context_dma_t* dma_1A0;               // 0x1A0: dma class object, 0x13378086
    rsx_object_context_dma_t* dma_1A8;               // 0x1A8: dma class object, 0x13378080
    rsx_object_context_dma_t* dma_1B0;               // 0x1B0: dma class object, 0x56616660
    rsx_object_context_dma_t* dma_1B8;               // 0x1B8: dma class object, 0x56616661
    //-------------------------------------------------------------------- 
    rsx_object_channel_t* ch_obj;  // 0x1C0: RSX channel object
    S64 unk_1C8;                // 0x1C8: BAR1 address: ctx 0(0x2808FE01000), ctx 1(0x2808FE11000)
    S64 unk_1D0;                // 0x1D0: BAR1 address: ctx 0(0x2808FE00000), ctx 1(0x2808FE10000)
    S64 unk_1D8;                // 0x1D8: BAR1 address: ctx 0(0x2808FE01400), ctx 1(0x2808FE11400)
    //--------------------------------------------------------------------
    S08 db_flag[8];             // 0x1E0: display buffer flag, 0 to 7
    rsx_ctx_dbuf_obj_t d_buf[8];// 0x1E8: display buffer objects, 0 to 7
    //--------------------------------------------------------------------
    S64 driver_info_addr;       // 0x328: driver info address
    S64 driver_info_lpar;       // 0x330: driver info LPAR address
    S64 driver_info_addr_0;     // 0x338: driver info address too ?

    // Methods
    rsx_object_context_dma_t* get_dma_object_by_index(S32 index)

    // Related with initialization
    void sub212F78();
    void sub213614();
    void sub2136CC();
    void sub214040();
    void sub2143E0();
    void sub2146F4();
    void sub214C58();

public:

    // Methods
    rsx_core_context_t(S64 core_id, rsx_memory_context_t* mem_ctx, U64 system_mode);

    void init(U64 *out, S64 core_id, rsx_memory_context_t* mem_ctx, U64 system_mode, S32 idx);
    S32 iomap(U64 ea, S64 lpar_addr, S32 size, U64 flags);
    S64 get_dma_control_lpar_address();
    S64 get_driver_info_lpar_addr();
    U32 get_rsx_context_id();
    S32 get_size_of_reports(void);
};
