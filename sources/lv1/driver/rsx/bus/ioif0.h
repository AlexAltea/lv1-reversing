/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// LV1 RSX IOIF 0 object, size 0x40
struct rsx_bus_ioif0_t {
    S64 vtab;                 // 0x00: pointer -> virtual table
    S32 chip_revision;        // 0x08: RSX chip revision
    S08 init;                 // 0x0C: init flag
    S08 pad1[3];              // 0x0D: padding 
    S64 base_addr;            // 0x10: interface base address, 0x28000000000
    S64 bar0_addr;            // 0x18: BAR0 start address, 0x28000000000
    S64 bar1_addr;            // 0x20: BAR1(VRAM) start address, 0x28080000000
    S64 bar2_addr;            // 0x28: BAR2(PRAMIN) start address, 0x28002000000
    S32 bar0_size;            // 0x30: BAR0 size, 0x2000000(32 MB)
    S32 bar1_2_size;          // 0x34: BAR1/BAR2 size, 0x10000000(256 MB)
    S32 unk_38;               // 0x38: ?
    S32 unk_3C;               // 0x3C: ?

    inline void enable_interrupt(U32 bits);

    // Get members
    S32 get_init_flag();
    S32 get_chip_revision();
    S64 get_bar0_addr();
    S32 get_bar0_size();
    S64 get_bar1_addr();
    S32 get_bar1_size();
    S64 get_bar2_addr();
    S32 get_bar2_size();
    S32 get_unk_38();

    // Access BAR0
    void bar0_write(S32 offset, S32 value);
    S32 bar0_read(S64 offset);

    void set_chip_revision();
    void sub219018();
    void sub217814();
} rsx_bus_ioif0_obj;

const char* rsx_bus_ioif0_get_class_name_string(void);

rsx_bus_ioif0_t* rsx_bus_ioif0_init(void);
