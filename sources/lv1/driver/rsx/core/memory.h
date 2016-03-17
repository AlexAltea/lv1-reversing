/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// RSX memory regions settings
struct rsx_mem_reg_setting_t {
    S32 id;                   // 0x00: region ID
    S32 unk_04;               // 0x04: ?
    S64 type;                 // 0x08: type of segment
    S64 offset;               // 0x10: offset, added to base address
    S64 size;                 // 0x18: size of region
};

// RSX core memory object, size 0x190
struct rsx_core_mem_obj_t {
    S64 core_id;                 // 0x000: RSX device core object ID
    S64 bar_1_addr;              // 0x008: BAR1(VRAM) start address, 0x28080000000
    S64 ddr_lpar_addr;           // 0x010: LPAR start address of mapped DDR memory, 0x7000B0000000
    S64 bar_1_size;              // 0x018: BAR1/BAR2 size, 0x10000000(256 MB)
    S64 ddr_page_size;           // 0x020: DDR memory page size, 0x100000(1 MB)
    S64 ddr_sys_reserv;          // 0x028: size of system reserved DDR memory, 0x200000(2 MB)
    //-------------------------------------------------------------------------------------------------------------------------------
    S32 unk_030;                 // 0x030: ? 
    S32 unk_034;                 // 0x034: ? 
    //-------------------------------------------------------------------------------------------------------------------------------
    S32 unk_038;                 // 0x038: ? 
    S32 unk_03C;                 // 0x03C: ?
    //-------------------------------------------------------------------------------------------------------------------------------
    S32 unk_040;                 // 0x040: ?
    S32 DDR_MB_CTags;            // 0x044: DDR_MB_CTags(BAR0: 0x28000100320), 
    S32 unk_048;                 // 0x048: ?
    //-------------------------------------------------------------------------------------------------------------------------------
    S32 unk_04C;                 // 0x04C: ? 
    S64 unk_050;                 // 0x050: ?
    S64 unk_058;                 // 0x058: ?
    //-------------------------------------------------------------------------------------------------------------------------------
    S64 driver_info_addr;        // 0x060: LV1 driver info base LV1 process EA
    S64 driver_info_lpar;        // 0x068: mapped driver info LPAR address
    S64 driver_info_total_size;  // 0x070: driver info total size, 0xC000(48 KB), (memory region 11 size)
    S64 driver_info_size;        // 0x078: driver info size, 0x4000(16 KB), for one of the 3 RSX contexts
    //-------------------------------------------------------------------------------------------------------------------------------
    S64 unk_080;                 // 0x080: ? IO address 0x28002000000
    S32 reports_size_total;      // 0x088: reports size total, 0x40000(256 KB), (memory region 10 size)
    S32 unk_08C;                 // 0x08C: ? 
    S64 reports_lpar_addr;       // 0x090: mapped reports LPAR address
    S64 reports_addr;            // 0x098: reports base IO address, 0x2808FE00000, (memory region 10 start address)
    S64 reports_size;            // 0x0A0: reports size, 0x10000(64 KB), 4 posible areas(remains from older versions with max 4 contexts?)
    //-------------------------------------------------------------------------------------------------------------------------------
    S64 unk_0A8;                 // 0x0A8: ? 0
    S64 unk_0B0;                 // 0x0B0: ? 0x300000
    S64 unk_0B8;                 // 0x0B8: ? 0x1000
    //--- RSX bitmap objects --------------------------------------------------------------------------------------------------------
    //                                      items  value_1          value_2    description
    S64* bm_reports;              // 0x0C0: 4      0x480000340000   0x10000    4 report areas of 0x10000(64 KB) size
    S64* bm_local_mem;            // 0x0C8: 254    0x7000B0000000   0x100000   254 local memory pages of 1MB size
    S64* bm_driver_info;          // 0x0D0: 3      0x590000         0x4000     3 driver info areas of 0x4000(16 KB) size
    S64* bm_class_nv_sw;          // 0x0D8: 128    0x28002050000    0x20       128 class(nv, sw) objects of 0x20 byte size
    S64* bm_unk_00;               // 0x0E0: 512    0                1          ? DDR_MB_CTags related
    S64* bm_unk_01;               // 0x0E8: 768    0                1          ?
    S64* bm_unk_02;               // 0x0F0: 15     0                1          ?
    S64* bm_unk_03;               // 0x0F8: 8      0                1          ?
    S64* bm_ctx_dma;              // 0x100: 256    0x28002040000    0x10       256 context DMA objects of 0x10 byte size
    //-------------------------------------------------------------------------------------------------------------------------------
    S64* rsx_mem_ctx[16];         // 0x108: RSX memory context object array, 0 to 15
    S64* unk_188;                 // 0x188: ?
} rsx_core_mem_obj;

S32 rsx_core_memory_get_BAR1_offset_by_address(rsx_core_mem_obj_t* core_mem, S64 addr);
rsx_mem_ctx_obj_t* rsx_core_memory_get_memory_context_by_id(rsx_core_mem_obj_t* core_mem, U32 mem_ctx_id);
rsx_core_mem_obj_t* rsx_core_memory_allocate_memory_context(rsx_core_mem_obj_t* core_mem, S32 local_size, S64 arg1, S64 arg2, S64 arg3, S64 arg4);
S32 rsx_core_memory_value_div_by_16(rsx_core_mem_obj_t* core_mem, S32 offset);
S32 rsx_core_memory_get_BAR2_offset_by_address(rsx_core_mem_obj_t* core_mem, S64 addr);
S64 rsx_core_memory_get_mem_reg_addr_by_id(rsx_core_mem_obj_t* core_mem, S32 mem_region_id);
S32 rsx_core_memory_get_mem_reg_size_by_id(rsx_core_mem_obj_t* core_mem, S32 mem_region_id);
rsx_core_mem_obj_t* rsx_core_memory_ctor(S64 rsx_core_id);
