/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#define MAX_RSX_CORE_ID     16         // max rsx core object count
#define MAX_DEV             16         // max RSX devices



////////////////////////////////////////////////////////////////////////
// LV1 RSX objects






// LV1 RSX object channel object, size 0x28
struct rsx_channel_obj_t {
    S32 id;                     // 0x00: RSX channel object ID, 0 to 3
    S32 unk_04;                 // 0x04: ? ptr
    S64 *dma_obj;               // 0x08: DMA object
    S64 fc1_addr;               // 0x10: channel fc1 address, e.g. 0(0x28002020000), 1(0x28002020080), ... size 0x80
    S64 fc2_addr;               // 0x18: channel fc2 address, e.g. 0(0x28002030000), 1(0x28002031000), ... size 0x1000
    S64 graph_addr;             // 0x20: channel graph address, 0x28002060000
} rsx_channel_obj;

// LV1 RSX object sw object, size 0x18
struct rsx_sw_class_obj_t {
    S32 id;                     // 0x00: ID ?
    S32 class;                  // 0x04: 0xCAFEBABE
    S64 opd;                    // 0x08: OPD of rsx_object_sw_driver()
    S64 bar2_addr;              // 0x10: BAR2(PRAMIN) address
} rsx_sw_class_obj;

// LV1 RSX object nv object, size 0x10
struct rsx_nv_class_obj_t {
    S32 unk_00;                 // 0x00: 
    S32 class;                  // 0x04: 
    S64 bar2_addr;              // 0x08: BAR2(PRAMIN) address
} rsx_nv_class_obj;

// LV1 RSX object context DMA object, size 0x18
struct rsx_ctx_dma_obj_t {
    uS32 class;                 // 0x00: 
    S32 unk_04;                 // 0x04: 
    S64 bar2_addr;              // 0x08: BAR2(PRAMIN) address
    S64 bar1_offset;            // 0x10: BAR1(VRAM) offset
} rsx_ctx_dma_obj;

// LV1 RSX context display buffer object, size 0x28
struct rsx_ctx_dbuf_obj_t {
    int8_t unk_00;                  // 0x00: flag
    int8_t pad0[3];                 // 0x01: 
    S32 width;                  // 0x04: display buffer width in pixels
    S32 height;                 // 0x08: display buffer height in pixels
    S32 pitch;                  // 0x0C: display buffer pitch size in byte
    S64 unk_10;                 // 0x10: 
    S64 bar1_addr;              // 0x18: BAR1(VRAM) address of display buffer
    S64 dma_obj;                // 0x20: DMA object, e.g. 0xDAC10001
} rsx_ctx_dbuf_obj;



// LV1 RSX context object, size 0x340
struct rsx_ctx_obj_t {
    S64 core_id;                // 0x000: RSX core object ID
    uS32 id;                    // 0x008: RSX context ID, (index XOR 0x55555555)
    S32 *unk_00C;               // 0x00C: ? ptr
    S64 *mem_ctx;               // 0x010: RSX memory context
    uS64 sys_mode;              // 0x018: system mode flags, [27:27]("flag local pb"), [28:28]("flag system sema"), [29:29]("flag gsemu ctx"), [30:30]("small io page size")
    S64 unk_020;                // 0x020: ? eic, ctx 0(0x38), ctx 1(0x39)
    S32 idx;                    // 0x028: RSX context object index, 0, 1 or 2
    S32 unk_02C;                // 0x02C: ?
    S64 io_offset;              // 0x030: IO offset, ctx 0(0x80000000), ctx 1(0x90000000)
    S64 unk_038;                // 0x038: ?
    S64 io_size;                // 0x040: IO size, 0x10000000(256 MB)
    S64 page_size;              // 0x048: page size, based on system mode[29:29]: 0(1 MB), 1(64 KB)
    //--------------------------------------------------------------------
    S64 *nv_obj[8];             // 0x050: nv class objects
    //--------------------------------------------------------------------
    S64 *sw_obj;                // 0x090: sw class object, 0xCAFEBABE
    //--------------------------------------------------------------------
    S64 *dma_098;               // 0x098: dma class object, 0xFEED0000 or 0xFEED0001
    //--------------------------------------------------------------------
    S64 *dma_0A0;               // 0x0A0: dma class object, 0xFEED0000
    S64 *dma_0A8;               // 0x0A8: dma class object, 0xFEED0001
    //--------------------------------------------------------------------
    S64 unk_0B0;                // 0x0B0: 
    //--- if "flag gsemu ctx" --------------------------------------------
    S64 *dma_0B8;               // 0x0B8: dma class object, 0xFEED0003
    S64 *dma_0C0;               // 0x0C0: dma class object, 0xFEED0004
    //--------------------------------------------------------------------
    S64 reports_lpar_addr;      // 0x0C8: 
    S64 reports_addr;           // 0x0D0: BAR1 reports address, ctx 0(0x2808FE00000), ctx 1(0x2808FE10000)
    S64 reports_size;           // 0x0D8: size of reports, 0x10000(64 KB)
    S64 unk_0E0;                // 0x0E0: BAR1 address: ctx 0(0x2808FE01000), ctx 1(0x2808FE11000)
    S64 unk_0E8;                // 0x0E8: BAR1 address: ctx 0(0x2808FE00000), ctx 1(0x2808FE10000)
    S64 unk_0F0;                // 0x0F0: BAR1 address: ctx 0(0x2808FF10000), ctx 1(0x2808FF10000)
    S64 unk_0F8;                // 0x0F8: BAR1 address: ctx 0(0x2808FE01400), ctx 1(0x2808FE11400)
    //--------------------------------------------------------------------
    S64 *dma_array_0[8];        // 0x100: dma class objects, 0x66604200 to 0x66604207
  //--------------------------------------------------------------------
    S64 *dma_array_1[8];        // 0x140: dma class objects, 0x66604208 to 0x6660420F
    //--------------------------------------------------------------------
    S64 *dma_180;               // 0x180: dma class object, 0x66606660
    S64 *dma_188;               // 0x188: dma class object, 0x66616661
    //-------------------------------------------------------------------- 
    S64 *dma_190;               // 0x190: dma class object, 0x66626660
    S64 *dma_198;               // 0x198: dma class object, 0xBAD68000
    //--------------------------------------------------------------------
    S64 *dma_1A0;               // 0x1A0: dma class object, 0x13378086
    S64 *dma_1A8;               // 0x1A8: dma class object, 0x13378080
    //-------------------------------------------------------------------- 
    S64 *dma_1B0;               // 0x1B0: dma class object, 0x56616660
    S64 *dma_1B8;               // 0x1B8: dma class object, 0x56616661
    //-------------------------------------------------------------------- 
    S64 *ch_obj;                // 0x1C0: RSX channel object
    S64 unk_1C8;                // 0x1C8: BAR1 address: ctx 0(0x2808FE01000), ctx 1(0x2808FE11000)
    S64 unk_1D0;                // 0x1D0: BAR1 address: ctx 0(0x2808FE00000), ctx 1(0x2808FE10000)
    S64 unk_1D8;                // 0x1D8: BAR1 address: ctx 0(0x2808FE01400), ctx 1(0x2808FE11400)
    //--------------------------------------------------------------------
    int8_t  db_flag[8];             // 0x1E0: display buffer flag, 0 to 7
    rsx_ctx_dbuf_obj_t d_buf[8];    // 0x1E8: display buffer objects, 0 to 7
    //--------------------------------------------------------------------
    S64 driver_info_addr;       // 0x328: driver info address
    S64 driver_info_lpar;       // 0x330: driver info LPAR address
    S64 driver_info_addr_0;     // 0x338: driver info address too ?
} rsx_ctx_obj;


// LV1 RSX memory context object, size 0x50
struct rsx_mem_ctx_obj_t {
    S64 *core_mem_obj;          // 0x00: RSX device core memory object
    S32 id;                     // 0x08: RSX memory context ID, (index XOR 0x5A5A5A5A)
    S32 unk_0C;                 // 0x0C: 
    //--------------------------------------------------------------------
    S64 lm_start_lpar;          // 0x10: this memory context local memory(DDR) start LPAR address
    S64 lm_end_lpar;            // 0x18: this memory context local memory(DDR) end LPAR address
    S64 ddr_lpar;               // 0x20: base LPAR address of DDR memory
    //--------------------------------------------------------------------
    S32 unk_28;                 // 0x28: ?
    S32 unk_2C;                 // 0x2C: ?
    //--------------------------------------------------------------------
    S64 unk_30;                 // 0x30: ?
    S64 unk_38;                 // 0x38: ?
    //--------------------------------------------------------------------
    S64 unk_40;                 // 0x40: ?
    S64 unk_48;                 // 0x48: ?
} rsx_mem_ctx_obj;

// LV1 RSX object v framebuffer object, size 0x40
struct rsx_obj_vfb_obj_t {
    S64 unk_00;                 // 0x00: 
    S64 unk_08;                 // 0x08: 
    S64 unk_10;                 // 0x10: 
    S32 unk_18;                 // 0x18: 
    int8_t unk_1C;                  // 0x1C: 
    int8_t unk_1D[3];               // 0x1D: padding
    S64 unk_20;                 // 0x20: 
    S64 unk_28;                 // 0x28: 
    S64 unk_30;                 // 0x30: 
    S64 unk_38;                 // 0x38: 
} rsx_obj_vfb_obj;


// LV1 RSX device audio object, size 0x30
struct rsx_dev_audio_obj_t {
    S64 unk_00;                 // 0x00: ? eic_id_5
    S64 unk_08;                 // 0x08: ? eic_id_4
    S64 lpar_size;              // 0x10: RSX audio device LPAR size, 0x1000(4 KB), (memory region 9 size)
    S64 io_addr;                // 0x18: RSX audio device IO address, 0x2808FF20000, (memory region 9 start)
    S64 lpar_addr;              // 0x20: RSX audio device LPAR address, we use 0x30000001B000
    S64 io_addr_0;              // 0x28: copy of IO address, 0x2808FF20000
} rsx_dev_audio_obj;


// LV1 RSX object video RSX object, size 0xC0
struct rsx_obj_video_rsx_t {
    int8_t unk_00;                   // 0x00: RSX LV1 debug level, 0(off) to 3(max debug output)
    int8_t unk_01[3];                // 0x00: padding
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
    int8_t unk_50;                   // 0x50: ?
    int8_t unk_51;                   // 0x51: ?
    int8_t unk_52;                   // 0x52: ?
    int8_t unk_53;                   // 0x53: ?
    int8_t unk_54;                   // 0x54: ?
    int8_t unk_55;                   // 0x55: ?
    int8_t unk_56;                   // 0x56: ?
    int8_t unk_57;                   // 0x57: ?
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
    int8_t unk_78[2];                // 0x78: ? 
    int8_t unk_7A[2];                // 0x7A: padding
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
} rsx_graph_obj;



// RSX device FIFO object, size 0x80
struct rsx_fifo_obj_t {
    S64 fc1_addr;           // 0x00: 0x28002020000(memory region 4 address)
    S32 fc1_size;           // 0x08: 0x200(memory region 4 size)
    S32 unk_0C;             // 0x0C: ?
  //--------------------------------------------------------------------
    S64 fc2_addr;           // 0x10: 0x28002030000(memory region 5 address)
    S64 fc2_size;           // 0x18: 0x4000(memory region 5 size)
    //--------------------------------------------------------------------
    S64 run_out_addr;       // 0x20: 0x28002018000(memory region 3 address)
    S32 run_out_size;       // 0x28: 0x200(memory region 3 size)
    S32 unk_2C;             // 0x2C: ?
    //--------------------------------------------------------------------
    S64 dma_control_addr;   // 0x30: 0x28000C00000, dma_control_addr
    S64 dma_control_lpar;   // 0x38: 0x4400001C0000, dma_control_lpar address, DBG: fifo mmio region
    S32 dma_control_size;   // 0x40: 0x20000, dma_control_size
    //--------------------------------------------------------------------
    S32 pause;              // 0x44: pause, 1(on) or 2(off) ?
    S32 init;               // 0x48: init_flag
    S32 unk_4C;             // 0x4C: 
    //--------------------------------------------------------------------
    S64 unk_50[4];          // 0x50: ?
    //S64 unk_58;             // 0x58: 
    //S64 unk_60;             // 0x60: 
    //S64 unk_68;             // 0x68: 
    //--------------------------------------------------------------------
    S64 unk_70;             // 0x70: 
    S64 unk_78;             // 0x78: 
} rsx_fifo_obj;


// RSX hash table object, size 0x18
struct rsx_hash_tbl_obj_t {
    S32 unk_00;             // 0x00: 
    S32 size_0;             // 0x04: RSX hash table size, 0x8000(32 KB)
    S64 io_addr;            // 0x08: RSX hash table IO address, 0x28002010000
    S32 size_1;             // 0x10: RSX hash table size, 0x8000(32 KB)
    S32 bar0_offset;        // 0x14: offset into BAR0
} rsx_hash_tbl_obj;



// RSX framebuffer object, size 0x10
struct rsx_dev_fb_obj_t {
    S32 DDR_MB_CTags;       // 0x00: 0x80000
    S32 size;               // 0x04: BAR1(VRAM) size 0x10000000(256 MB), from register BAR0:0x2800010020C
    S64 io_addr;            // 0x08: BAR1(VRAM) start address, 0x28080000000
} rsx_dev_fb_obj;

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

// LV1 RSX device clock object, size 0x30
struct rsx_dev_clock_obj_t {
    S64 vtab;                 // 0x00: pointer -> virtual table
    //--------------------------------------------------------------------
    S32 id;                   // 0x08: clock ID
                                  //       0("Invalid")
                                  //       1("nvclk")
                                  //       2("geom clkshader clk")
                                  //       3("rop clk")
                                  //       4("memory clk")
                                  //       5("display clk")
                                  //       6("host clk")
    //--------------------------------------------------------------------
    S32 unk_0C;               // 0x0C: ? (init with 0)
    S32 unk_10;               // 0x10: ?
    S32 offset_0;             // 0x14: a IOIF offset, added to base address 
    S32 offset_1;             // 0x18: a IOIF offset, added to base address 
  S32 freq;                 // 0x1C: clock frequency in Hz
  S32 unk_20;               // 0x20: ?
  S32 unk_24;               // 0x24: ?
} rsx_dev_clock_obj;



// LV1 RSX IOIF 0 object, size 0x40
struct _rsx_bus_ioif0_obj_t {
    S64 vtab;                 // 0x00: pointer -> virtual table
    S32 chip_revision;        // 0x08: RSX chip revision
    int8_t init;                  // 0x0C: init flag
    int8_t pad1[3];               // 0x0D: padding 
    S64 base_addr;            // 0x10: interface base address, 0x28000000000
    S64 bar_0_addr;           // 0x18: BAR0 start address, 0x28000000000
    S64 bar_1_addr;           // 0x20: BAR1(VRAM) start address, 0x28080000000
    S64 bar_2_addr;           // 0x28: BAR2(PRAMIN) start address, 0x28002000000
    S32 bar_0_size;           // 0x30: BAR0 size, 0x2000000(32 MB)
    S32 bar_1_2_size;         // 0x34: BAR1/BAR2 size, 0x10000000(256 MB)
    S32 unk_38;               // 0x38: ?
    S32 unk_3C;               // 0x3C: ?
} rsx_bus_ioif0_obj_t;

// LV1 RSX utils bitmap object, size 0x38
struct rsx_utils_bm_obj_t {
    S64 value1;                 // 0x00: base, e.g. a memory start address
    S64 item_total;             // 0x08: count of items in bitmap
    S64 value2;                 // 0x10: size one item, e.g. a memory page size
    S32 seg_base;               // 0x18: 0x40, bits per segment(one u64)
    S32 unk_1C;                 // 0x1C: ?
    S64 unk_20;                 // 0x20: ?
    S32 seg_count;              // 0x28: segment count of bitmap
    S32 unk_2C;                 // 0x2C: ?
    S64 *bitmap;                // 0x30: address of bitmap
} rsx_utils_bm_obj;

// LV1 RSX debug object
struct rsx_dbg_obj_t {
    S64 unk_00;                  // 0x00: ? init 0
    S32 unk_08;                  // 0x08: ?
    S32 unk_0C;                  // 0x0C: ?
    S64 unk_10;                  // 0x10: init 0xF4240(1000000)
    S64 unk_18;                  // 0x18: init creation-time tb value + unk_10
    S64 unk_20;                  // 0x20: ?
} rsx_dbg_obj;

// LV1 RSX device core object, size 0x1C8
struct rsx_dev_core_obj_t {
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
} rsx_dev_core_obj;
