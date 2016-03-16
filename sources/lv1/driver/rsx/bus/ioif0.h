/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// LV1 RSX IOIF 0 object, size 0x40
struct rsx_bus_ioif0_obj_t {
    S64 vtab;                 // 0x00: pointer -> virtual table
    S32 chip_revision;        // 0x08: RSX chip revision
    S08 init;                 // 0x0C: init flag
    S08 pad1[3];              // 0x0D: padding 
    S64 base_addr;            // 0x10: interface base address, 0x28000000000
    S64 bar_0_addr;           // 0x18: BAR0 start address, 0x28000000000
    S64 bar_1_addr;           // 0x20: BAR1(VRAM) start address, 0x28080000000
    S64 bar_2_addr;           // 0x28: BAR2(PRAMIN) start address, 0x28002000000
    S32 bar_0_size;           // 0x30: BAR0 size, 0x2000000(32 MB)
    S32 bar_1_2_size;         // 0x34: BAR1/BAR2 size, 0x10000000(256 MB)
    S32 unk_38;               // 0x38: ?
    S32 unk_3C;               // 0x3C: ?
} rsx_bus_ioif0_obj;

const char *rsx_bus_ioif0_get_class_name_string(void);
S32 rsx_bus_ioif0_get_init_flag(rsx_bus_ioif0_obj_t* ioif0);
S32 rsx_bus_ioif0_get_chip_revision(rsx_bus_ioif0_obj_t* ioif0);
S64 rsx_bus_ioif0_get_BAR0_addr(rsx_bus_ioif0_obj_t* ioif0);
S32 rsx_bus_ioif0_get_BAR0_size(rsx_bus_ioif0_obj_t* ioif0);
S64 rsx_bus_ioif0_get_BAR1_addr(rsx_bus_ioif0_obj_t* ioif0);
S32 rsx_bus_ioif0_get_BAR1_size(rsx_bus_ioif0_obj_t* ioif0);
S64 rsx_bus_ioif0_get_BAR2_addr(rsx_bus_ioif0_obj_t* ioif0);
S32 rsx_bus_ioif0_get_unk_38(rsx_bus_ioif0_obj_t* ioif0);
void rsx_bus_ioif0_write_BAR0_by_offset(rsx_bus_ioif0_obj_t* ioif0, S32 offset, S32 value);
S32 rsx_bus_ioif0_read_BAR0_by_offset(rsx_bus_ioif0_obj_t* ioif0, S64 offset);


rsx_bus_ioif0_obj_t* rsx_bus_ioif0_init(void);
void rsx_bus_ioif0_set_chip_revision(rsx_bus_ioif0_obj_t* ioif0);
void rsx_bus_ioif0_219018(rsx_bus_ioif0_obj_t* ioif0);
void rsx_bus_ioif0_217814(rsx_bus_ioif0_obj_t* ioif0);
