#ifndef __RSX_OBJECT_H__
#define __RSX_OBJECT_H__






#define MAX_RSX_CORE_ID     16         // max rsx core object count
#define MAX_DEV             16         // max RSX devices



////////////////////////////////////////////////////////////////////////
// LV1 RSX objects






// LV1 RSX object channel object, size 0x28
typedef struct _rsx_channel_obj_t
{
	int32_t id;                     // 0x00: RSX channel object ID, 0 to 3
	int32_t unk_04;                 // 0x04: ? ptr
	int64_t *dma_obj;               // 0x08: DMA object
	int64_t fc1_addr;               // 0x10: channel fc1 address, e.g. 0(0x28002020000), 1(0x28002020080), ... size 0x80
	int64_t fc2_addr;               // 0x18: channel fc2 address, e.g. 0(0x28002030000), 1(0x28002031000), ... size 0x1000
	int64_t graph_addr;             // 0x20: channel graph address, 0x28002060000
} __attribute__((packed)) rsx_channel_obj_t;

// LV1 RSX object sw object, size 0x18
typedef struct _rsx_sw_class_obj_t
{
	int32_t id;                     // 0x00: ID ?
	int32_t class;                  // 0x04: 0xCAFEBABE
	int64_t opd;                    // 0x08: OPD of rsx_object_sw_driver()
	int64_t bar2_addr;              // 0x10: BAR2(PRAMIN) address
} __attribute__((packed)) rsx_sw_class_obj_t;

// LV1 RSX object nv object, size 0x10
typedef struct _rsx_nv_class_obj_t
{
	int32_t unk_00;                 // 0x00: 
	int32_t class;                  // 0x04: 
	int64_t bar2_addr;              // 0x08: BAR2(PRAMIN) address
} __attribute__((packed)) rsx_nv_class_obj_t;

// LV1 RSX object context DMA object, size 0x18
typedef struct _rsx_ctx_dma_obj_t
{
	uint32_t class;                 // 0x00: 
	int32_t unk_04;                 // 0x04: 
	int64_t bar2_addr;              // 0x08: BAR2(PRAMIN) address
	int64_t bar1_offset;            // 0x10: BAR1(VRAM) offset
} __attribute__((packed)) rsx_ctx_dma_obj_t;

// LV1 RSX context display buffer object, size 0x28
typedef struct _rsx_ctx_dbuf_obj_t
{
	int8_t unk_00;                  // 0x00: flag
	int8_t pad0[3];                 // 0x01: 
	int32_t width;                  // 0x04: display buffer width in pixels
	int32_t height;                 // 0x08: display buffer height in pixels
	int32_t pitch;                  // 0x0C: display buffer pitch size in byte
	int64_t unk_10;                 // 0x10: 
  int64_t bar1_addr;              // 0x18: BAR1(VRAM) address of display buffer
  int64_t dma_obj;                // 0x20: DMA object, e.g. 0xDAC10001
} __attribute__((packed)) rsx_ctx_dbuf_obj_t;



// LV1 RSX context object, size 0x340
typedef struct _rsx_ctx_obj_t
{
	int64_t core_id;                // 0x000: RSX core object ID
	uint32_t id;                    // 0x008: RSX context ID, (index XOR 0x55555555)
	int32_t *unk_00C;               // 0x00C: ? ptr
	int64_t *mem_ctx;               // 0x010: RSX memory context
	uint64_t sys_mode;              // 0x018: system mode flags, [27:27]("flag local pb"), [28:28]("flag system sema"), [29:29]("flag gsemu ctx"), [30:30]("small io page size")
	int64_t unk_020;                // 0x020: ? eic, ctx 0(0x38), ctx 1(0x39)
	int32_t idx;                    // 0x028: RSX context object index, 0, 1 or 2
	int32_t unk_02C;                // 0x02C: ?
	int64_t io_offset;              // 0x030: IO offset, ctx 0(0x80000000), ctx 1(0x90000000)
	int64_t unk_038;                // 0x038: ?
	int64_t io_size;                // 0x040: IO size, 0x10000000(256 MB)
	int64_t page_size;              // 0x048: page size, based on system mode[29:29]: 0(1 MB), 1(64 KB)
	//--------------------------------------------------------------------
	int64_t *nv_obj[8];             // 0x050: nv class objects
	//--------------------------------------------------------------------
	int64_t *sw_obj;                // 0x090: sw class object, 0xCAFEBABE
	//--------------------------------------------------------------------
	int64_t *dma_098;               // 0x098: dma class object, 0xFEED0000 or 0xFEED0001
	//--------------------------------------------------------------------
	int64_t *dma_0A0;               // 0x0A0: dma class object, 0xFEED0000
	int64_t *dma_0A8;               // 0x0A8: dma class object, 0xFEED0001
	//--------------------------------------------------------------------
	int64_t unk_0B0;                // 0x0B0: 
	//--- if "flag gsemu ctx" --------------------------------------------
	int64_t *dma_0B8;               // 0x0B8: dma class object, 0xFEED0003
	int64_t *dma_0C0;               // 0x0C0: dma class object, 0xFEED0004
	//--------------------------------------------------------------------
	int64_t reports_lpar_addr;      // 0x0C8: 
	int64_t reports_addr;           // 0x0D0: BAR1 reports address, ctx 0(0x2808FE00000), ctx 1(0x2808FE10000)
	int64_t reports_size;           // 0x0D8: size of reports, 0x10000(64 KB)
	int64_t unk_0E0;                // 0x0E0: BAR1 address: ctx 0(0x2808FE01000), ctx 1(0x2808FE11000)
	int64_t unk_0E8;                // 0x0E8: BAR1 address: ctx 0(0x2808FE00000), ctx 1(0x2808FE10000)
	int64_t unk_0F0;                // 0x0F0: BAR1 address: ctx 0(0x2808FF10000), ctx 1(0x2808FF10000)
	int64_t unk_0F8;                // 0x0F8: BAR1 address: ctx 0(0x2808FE01400), ctx 1(0x2808FE11400)
	//--------------------------------------------------------------------
	int64_t *dma_array_0[8];        // 0x100: dma class objects, 0x66604200 to 0x66604207
  //--------------------------------------------------------------------
	int64_t *dma_array_1[8];        // 0x140: dma class objects, 0x66604208 to 0x6660420F
	//--------------------------------------------------------------------
	int64_t *dma_180;               // 0x180: dma class object, 0x66606660
	int64_t *dma_188;               // 0x188: dma class object, 0x66616661
	//-------------------------------------------------------------------- 
	int64_t *dma_190;               // 0x190: dma class object, 0x66626660
	int64_t *dma_198;               // 0x198: dma class object, 0xBAD68000
	//--------------------------------------------------------------------
	int64_t *dma_1A0;               // 0x1A0: dma class object, 0x13378086
	int64_t *dma_1A8;               // 0x1A8: dma class object, 0x13378080
	//-------------------------------------------------------------------- 
	int64_t *dma_1B0;               // 0x1B0: dma class object, 0x56616660
	int64_t *dma_1B8;               // 0x1B8: dma class object, 0x56616661
	//-------------------------------------------------------------------- 
	int64_t *ch_obj;                // 0x1C0: RSX channel object
  int64_t unk_1C8;                // 0x1C8: BAR1 address: ctx 0(0x2808FE01000), ctx 1(0x2808FE11000)
  int64_t unk_1D0;                // 0x1D0: BAR1 address: ctx 0(0x2808FE00000), ctx 1(0x2808FE10000)
  int64_t unk_1D8;                // 0x1D8: BAR1 address: ctx 0(0x2808FE01400), ctx 1(0x2808FE11400)
	//--------------------------------------------------------------------
	int8_t  db_flag[8];             // 0x1E0: display buffer flag, 0 to 7
	rsx_ctx_dbuf_obj_t d_buf[8];    // 0x1E8: display buffer objects, 0 to 7
	//--------------------------------------------------------------------
	int64_t driver_info_addr;       // 0x328: driver info address
	int64_t driver_info_lpar;       // 0x330: driver info LPAR address
	int64_t driver_info_addr_0;     // 0x338: driver info address too ?
} __attribute__((packed)) rsx_ctx_obj_t;


// LV1 RSX memory context object, size 0x50
typedef struct _rsx_mem_ctx_obj_t
{
	int64_t *core_mem_obj;          // 0x00: RSX device core memory object
	int32_t id;                     // 0x08: RSX memory context ID, (index XOR 0x5A5A5A5A)
	int32_t unk_0C;                 // 0x0C: 
	//--------------------------------------------------------------------
	int64_t lm_start_lpar;          // 0x10: this memory context local memory(DDR) start LPAR address
	int64_t lm_end_lpar;            // 0x18: this memory context local memory(DDR) end LPAR address
	int64_t ddr_lpar;               // 0x20: base LPAR address of DDR memory
	//--------------------------------------------------------------------
	int32_t unk_28;                 // 0x28: ?
	int32_t unk_2C;                 // 0x2C: ?
	//--------------------------------------------------------------------
	int64_t unk_30;                 // 0x30: ?
	int64_t unk_38;                 // 0x38: ?
	//--------------------------------------------------------------------
	int64_t unk_40;                 // 0x40: ?
	int64_t unk_48;                 // 0x48: ?
} __attribute__((packed)) rsx_mem_ctx_obj_t;

// LV1 RSX object v framebuffer object, size 0x40
typedef struct _rsx_obj_vfb_obj_t
{
	int64_t unk_00;                 // 0x00: 
	int64_t unk_08;                 // 0x08: 
	int64_t unk_10;                 // 0x10: 
	int32_t unk_18;                 // 0x18: 
	int8_t unk_1C;                  // 0x1C: 
	int8_t unk_1D[3];               // 0x1D: padding
	int64_t unk_20;                 // 0x20: 
	int64_t unk_28;                 // 0x28: 
	int64_t unk_30;                 // 0x30: 
	int64_t unk_38;                 // 0x38: 
} __attribute__((packed)) rsx_obj_vfb_obj_t;


// LV1 RSX device audio object, size 0x30
typedef struct _rsx_dev_audio_obj_t
{
	int64_t unk_00;                 // 0x00: ? eic_id_5
	int64_t unk_08;                 // 0x08: ? eic_id_4
	int64_t lpar_size;              // 0x10: RSX audio device LPAR size, 0x1000(4 KB), (memory region 9 size)
	int64_t io_addr;                // 0x18: RSX audio device IO address, 0x2808FF20000, (memory region 9 start)
	int64_t lpar_addr;              // 0x20: RSX audio device LPAR address, we use 0x30000001B000
	int64_t io_addr_0;              // 0x28: copy of IO address, 0x2808FF20000
} __attribute__((packed)) rsx_dev_audio_obj_t;


// LV1 RSX object video RSX object, size 0xC0
typedef struct _rsx_obj_video_rsx_t
{
	int8_t unk_00;                   // 0x00: RSX LV1 debug level, 0(off) to 3(max debug output)
	int8_t unk_01[3];                // 0x00: padding
	//--------------------------------------------------------------------
	int32_t unk_04;                  // 0x04: ?
  int32_t unk_08;                  // 0x08: ?
  int32_t unk_0C;                  // 0x0C: ?
  int32_t unk_10;                  // 0x10: ?
  int32_t unk_14;                  // 0x14: ?
  int32_t unk_18;                  // 0x18: ?
  int32_t unk_1C;                  // 0x1C: ?
  int32_t unk_20;                  // 0x20: ?
  //--------------------------------------------------------------------
  int32_t unk_24[2];               // 0x24: ?
  //--------------------------------------------------------------------
  int32_t unk_2C[3];               // 0x2C: [0], 
  // int32_t unk_30;               // 0x30: [1], 
  // int32_t unk_34;               // 0x34: [2], 
  //--------------------------------------------------------------------
  int64_t unk_38;                  // 0x38: 
  int64_t unk_40;                  // 0x40: 
  //--------------------------------------------------------------------
  int32_t unk_48;                  // 0x48: ? init with 0x20200
  int32_t unk_4C;                  // 0x4C: ? init with 0x20200
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
  int32_t unk_58;                  // 0x58: ?
  int32_t unk_5C;                  // 0x5C: ?
  int32_t unk_60;                  // 0x60: ?
  int32_t unk_64;                  // 0x64: ?
  int32_t unk_68;                  // 0x68: ?
  int32_t unk_6C;                  // 0x6C: ?
  int32_t unk_70;                  // 0x70: ?
  int32_t unk_74;                  // 0x74: ?
  //--------------------------------------------------------------------
  int8_t unk_78[2];                // 0x78: ? 
  int8_t unk_7A[2];                // 0x7A: padding
  int32_t unk_7C[2];               // 0x7C: ?
  int32_t unk_84[2];               // 0x84: ?
  //--------------------------------------------------------------------
  int32_t unk_8C;                  // 0x8C: ?
  //--------------------------------------------------------------------
  int64_t *unk_90;                 // 0x90: prt, unk lv1 obj
  int64_t *unk_98;                 // 0x98: prt, unk lv1 obj
  //--------------------------------------------------------------------
  int64_t unk_A0[2];               // 0xA0: ?
  //--------------------------------------------------------------------
  int64_t unk_B0[2];               // 0xB0: ? clock
} __attribute__((packed)) rsx_obj_video_rsx_t;


// RSX device graph object, size 0x198
typedef struct _rsx_graph_obj_t
{
  int64_t graph_addr;            // 0x000: RSX graph address 0x28002060000 (memory region 6 start address)
  int32_t graph_offset;          // 0x008: RSX graph offset (0x60000), from 0x28002000000
  int32_t graph_size;            // 0x00C: RSX graph size 0x80000(512 KB) (memory region 6 size)
  //---------------------------------------------------------------------------------------
  int64_t channel_addr[16];      // 0x010: RSX channel address into GRAPH, 0x28002060000
  //---------------------------------------------------------------------------------------
  int32_t max_channels;          // 0x090: 4, the max RSX channels
  int32_t unk_094;               // 0x094: 
  int32_t unk_098;               // 0x098: ? offset into graph memory
  int32_t unk_09C;               // 0x09C: 
  int32_t unk_0A0;               // 0x0A0: ? offset into graph memory
  int32_t unk_0A4;               // 0x0A4: 
  int32_t unk_0A8;               // 0x0A8: ? offset into graph memory
  int32_t unk_0AC;               // 0x0AC: 
  //---------------------------------------------------------------------------------------
  int64_t *vtab;                 // 0x0B0: vtab
  //---------------------------------------------------------------------------------------
  int64_t unk_0B8;               // 0x0B8: 
  int64_t unk_0C0;               // 0x0C0: 
  int64_t unk_0C8;               // 0x0C8: 
  int64_t unk_0D0;               // 0x0D0: 
  int64_t unk_0D8;               // 0x0D8: 
  int64_t unk_0E0;               // 0x0E0: 
  int64_t unk_0E8;               // 0x0E8: 
  int64_t unk_0F0;               // 0x0F0: 
  int64_t unk_0F8;               // 0x0F8: 
  int64_t unk_100;               // 0x100: 
  int64_t unk_108;               // 0x108: 
  int64_t unk_110;               // 0x110: 
  int64_t unk_118;               // 0x118: 
  int64_t unk_120;               // 0x120: 
  int64_t unk_128;               // 0x128: 
  int64_t unk_130;               // 0x130: 
  int64_t unk_138;               // 0x138: 
  int64_t unk_140;               // 0x140: 
  int64_t unk_148;               // 0x148: 
  int64_t unk_150;               // 0x150: 
  int64_t unk_158;               // 0x158: 
  int64_t unk_160;               // 0x160: 
  int64_t unk_168;               // 0x168: 
  int64_t unk_170;               // 0x170: 
  int64_t unk_178;               // 0x178: 
  int64_t unk_180;               // 0x180: 
  
  int32_t unk_188;               // 0x188: 
  int32_t unk_18C;               // 0x18C: 
  int32_t unk_190;               // 0x190: 
  int32_t unk_194;               // 0x194: 
} __attribute__((packed)) rsx_graph_obj_t;



// RSX device FIFO object, size 0x80
typedef struct _rsx_fifo_obj_t
{
	int64_t fc1_addr;           // 0x00: 0x28002020000(memory region 4 address)
	int32_t fc1_size;           // 0x08: 0x200(memory region 4 size)
	int32_t unk_0C;             // 0x0C: ?
  //--------------------------------------------------------------------
	int64_t fc2_addr;           // 0x10: 0x28002030000(memory region 5 address)
	int64_t fc2_size;           // 0x18: 0x4000(memory region 5 size)
	//--------------------------------------------------------------------
	int64_t run_out_addr;       // 0x20: 0x28002018000(memory region 3 address)
	int32_t run_out_size;       // 0x28: 0x200(memory region 3 size)
	int32_t unk_2C;             // 0x2C: ?
	//--------------------------------------------------------------------
	int64_t dma_control_addr;   // 0x30: 0x28000C00000, dma_control_addr
	int64_t dma_control_lpar;   // 0x38: 0x4400001C0000, dma_control_lpar address, DBG: fifo mmio region
	int32_t dma_control_size;   // 0x40: 0x20000, dma_control_size
	//--------------------------------------------------------------------
	int32_t pause;              // 0x44: pause, 1(on) or 2(off) ?
	int32_t init;               // 0x48: init_flag
	int32_t unk_4C;             // 0x4C: 
	//--------------------------------------------------------------------
	int64_t unk_50[4];          // 0x50: ?
	//int64_t unk_58;             // 0x58: 
	//int64_t unk_60;             // 0x60: 
	//int64_t unk_68;             // 0x68: 
	//--------------------------------------------------------------------
	int64_t unk_70;             // 0x70: 
	int64_t unk_78;             // 0x78: 
} __attribute__((packed)) rsx_fifo_obj_t;


// RSX hash table object, size 0x18
typedef struct _rsx_hash_tbl_obj_t
{
	int32_t unk_00;             // 0x00: 
	int32_t size_0;             // 0x04: RSX hash table size, 0x8000(32 KB)
	int64_t io_addr;            // 0x08: RSX hash table IO address, 0x28002010000
	int32_t size_1;             // 0x10: RSX hash table size, 0x8000(32 KB)
	int32_t bar0_offset;        // 0x14: offset into BAR0
} __attribute__((packed)) rsx_hash_tbl_obj_t;



// RSX framebuffer object, size 0x10
typedef struct _rsx_dev_fb_obj_t
{
	int32_t DDR_MB_CTags;       // 0x00: 0x80000
	int32_t size;               // 0x04: BAR1(VRAM) size 0x10000000(256 MB), from register BAR0:0x2800010020C
	int64_t io_addr;            // 0x08: BAR1(VRAM) start address, 0x28080000000
} __attribute__((packed)) rsx_dev_fb_obj_t;

// RSX external interrupt controller object, DUMMY, TODO
typedef struct _rsx_eic_obj_t
{
	int32_t unk_000;              // 0x00: 
	// ...
} __attribute__((packed)) rsx_eic_obj_t;

// RSX memory regions settings
typedef struct _rsx_mem_reg_setting_t
{
	int32_t id;                   // 0x00: region ID
	int32_t unk_04;               // 0x04: ?
	int64_t type;                 // 0x08: type of segment
	int64_t offset;               // 0x10: offset, added to base address
	int64_t size;                 // 0x18: size of region
} __attribute__((packed)) rsx_mem_reg_setting_t;

// RSX core memory object, size 0x190
typedef struct _rsx_core_mem_obj_t
{
	int64_t core_id;                 // 0x000: RSX device core object ID
	int64_t bar_1_addr;              // 0x008: BAR1(VRAM) start address, 0x28080000000
	int64_t ddr_lpar_addr;           // 0x010: LPAR start address of mapped DDR memory, 0x7000B0000000
	int64_t bar_1_size;              // 0x018: BAR1/BAR2 size, 0x10000000(256 MB)
	int64_t ddr_page_size;           // 0x020: DDR memory page size, 0x100000(1 MB)
	int64_t ddr_sys_reserv;          // 0x028: size of system reserved DDR memory, 0x200000(2 MB)
	//-------------------------------------------------------------------------------------------------------------------------------
	int32_t unk_030;                 // 0x030: ? 
	int32_t unk_034;                 // 0x034: ? 
  //-------------------------------------------------------------------------------------------------------------------------------
	int32_t unk_038;                 // 0x038: ? 
	int32_t unk_03C;                 // 0x03C: ?
	//-------------------------------------------------------------------------------------------------------------------------------
	int32_t unk_040;                 // 0x040: ?
	int32_t DDR_MB_CTags;            // 0x044: DDR_MB_CTags(BAR0: 0x28000100320), 
	int32_t unk_048;                 // 0x048: ?
	//-------------------------------------------------------------------------------------------------------------------------------
	int32_t unk_04C;                 // 0x04C: ? 
  int64_t unk_050;                 // 0x050: ?
  int64_t unk_058;                 // 0x058: ?
  //-------------------------------------------------------------------------------------------------------------------------------
	int64_t driver_info_addr;        // 0x060: LV1 driver info base LV1 process EA
	int64_t driver_info_lpar;        // 0x068: mapped driver info LPAR address
	int64_t driver_info_total_size;  // 0x070: driver info total size, 0xC000(48 KB), (memory region 11 size)
	int64_t driver_info_size;        // 0x078: driver info size, 0x4000(16 KB), for one of the 3 RSX contexts
  //-------------------------------------------------------------------------------------------------------------------------------
	int64_t unk_080;                 // 0x080: ? IO address 0x28002000000
	int32_t reports_size_total;      // 0x088: reports size total, 0x40000(256 KB), (memory region 10 size)
	int32_t unk_08C;                 // 0x08C: ? 
	int64_t reports_lpar_addr;       // 0x090: mapped reports LPAR address
	int64_t reports_addr;            // 0x098: reports base IO address, 0x2808FE00000, (memory region 10 start address)
	int64_t reports_size;            // 0x0A0: reports size, 0x10000(64 KB), 4 posible areas(remains from older versions with max 4 contexts?)
	//-------------------------------------------------------------------------------------------------------------------------------
	int64_t unk_0A8;                 // 0x0A8: ? 0
	int64_t unk_0B0;                 // 0x0B0: ? 0x300000
	int64_t unk_0B8;                 // 0x0B8: ? 0x1000
	//--- RSX bitmap objects --------------------------------------------------------------------------------------------------------
	//                                          items  value_1          value_2    description
	int64_t *bm_reports;              // 0x0C0: 4      0x480000340000   0x10000    4 report areas of 0x10000(64 KB) size
	int64_t *bm_local_mem;            // 0x0C8: 254    0x7000B0000000   0x100000   254 local memory pages of 1MB size
	int64_t *bm_driver_info;          // 0x0D0: 3      0x590000         0x4000     3 driver info areas of 0x4000(16 KB) size
	int64_t *bm_class_nv_sw;          // 0x0D8: 128    0x28002050000    0x20       128 class(nv, sw) objects of 0x20 byte size
	int64_t *bm_unk_00;               // 0x0E0: 512    0                1          ? DDR_MB_CTags related
	int64_t *bm_unk_01;               // 0x0E8: 768    0                1          ?
	int64_t *bm_unk_02;               // 0x0F0: 15     0                1          ?
	int64_t *bm_unk_03;               // 0x0F8: 8      0                1          ?
	int64_t *bm_ctx_dma;              // 0x100: 256    0x28002040000    0x10       256 context DMA objects of 0x10 byte size
	//-------------------------------------------------------------------------------------------------------------------------------
	int64_t *rsx_mem_ctx[16];         // 0x108: RSX memory context object array, 0 to 15
	int64_t *unk_188;                 // 0x188: ?
} __attribute__((packed)) rsx_core_mem_obj_t;

// RSX device clock setting
typedef struct _rsx_dev_clock_setting_t
{
	int32_t unk_00;               // 0x00: 
	int32_t unk_04;               // 0x04: 
	int32_t offset_0;             // 0x08: 
	int32_t offset_1;             // 0x0C: 
	int32_t unk_10;               // 0x10: 
} __attribute__((packed)) rsx_dev_clock_setting_t;

// LV1 RSX device clock object, size 0x30
typedef struct _rsx_dev_clock_obj_t
{
	int64_t vtab;                 // 0x00: pointer -> virtual table
	//--------------------------------------------------------------------
	int32_t id;                   // 0x08: clock ID
	                              //       0("Invalid")
	                              //       1("nvclk")
	                              //       2("geom clkshader clk")
	                              //       3("rop clk")
	                              //       4("memory clk")
	                              //       5("display clk")
	                              //       6("host clk")
	//--------------------------------------------------------------------
	int32_t unk_0C;               // 0x0C: ? (init with 0)
	int32_t unk_10;               // 0x10: ?
	int32_t offset_0;             // 0x14: a IOIF offset, added to base address 
	int32_t offset_1;             // 0x18: a IOIF offset, added to base address 
  int32_t freq;                 // 0x1C: clock frequency in Hz
  int32_t unk_20;               // 0x20: ?
  int32_t unk_24;               // 0x24: ?
} __attribute__((packed)) rsx_dev_clock_obj_t;



// LV1 RSX IOIF 0 object, size 0x40
typedef struct _rsx_bus_ioif0_obj_t
{
	int64_t vtab;                 // 0x00: pointer -> virtual table
	int32_t chip_revision;        // 0x08: RSX chip revision
	int8_t init;                  // 0x0C: init flag
	int8_t pad1[3];               // 0x0D: padding 
	int64_t base_addr;            // 0x10: interface base address, 0x28000000000
	int64_t bar_0_addr;           // 0x18: BAR0 start address, 0x28000000000
	int64_t bar_1_addr;           // 0x20: BAR1(VRAM) start address, 0x28080000000
	int64_t bar_2_addr;           // 0x28: BAR2(PRAMIN) start address, 0x28002000000
	int32_t bar_0_size;           // 0x30: BAR0 size, 0x2000000(32 MB)
	int32_t bar_1_2_size;         // 0x34: BAR1/BAR2 size, 0x10000000(256 MB)
	int32_t unk_38;               // 0x38: ?
	int32_t unk_3C;               // 0x3C: ?
} __attribute__((packed)) rsx_bus_ioif0_obj_t;

// LV1 RSX utils bitmap object, size 0x38
typedef struct _rsx_utils_bm_obj_t
{
	int64_t value1;                 // 0x00: base, e.g. a memory start address
	int64_t item_total;             // 0x08: count of items in bitmap
	int64_t value2;                 // 0x10: size one item, e.g. a memory page size
	int32_t seg_base;               // 0x18: 0x40, bits per segment(one u64)
	int32_t unk_1C;                 // 0x1C: ?
	int64_t unk_20;                 // 0x20: ?
	int32_t seg_count;              // 0x28: segment count of bitmap
	int32_t unk_2C;                 // 0x2C: ?
	int64_t *bitmap;                // 0x30: address of bitmap
} __attribute__((packed)) rsx_utils_bm_obj_t;

// LV1 RSX debug object
typedef struct _rsx_dbg_obj_t
{
	int64_t unk_00;                  // 0x00: ? init 0
	int32_t unk_08;                  // 0x08: ?
	int32_t unk_0C;                  // 0x0C: ?
	int64_t unk_10;                  // 0x10: init 0xF4240(1000000)
	int64_t unk_18;                  // 0x18: init creation-time tb value + unk_10
	int64_t unk_20;                  // 0x20: ?
} __attribute__((packed)) rsx_dbg_obj_t;

// LV1 RSX device core object, size 0x1C8
typedef struct _rsx_dev_core_obj_t
{
	int32_t unk_000;                 // 0x000: ?
	int32_t *unk_004;                // 0x004: ? address
	int64_t device_id;               // 0x008: RSX device ID
	int64_t *ioif0;                  // 0x010: RSX IO interface 0 object
	int64_t core_id;                 // 0x018: RSX device core object ID  
	//int32_t unk_01C;               // 0x01C: sometime RSX device core object ID as 32bit
	int64_t *core_mem_obj;           // 0x020: RSX core memory object
	//--------------------------------------------------------------------
	int64_t *rsx_ctx[3];             // 0x028: RSX context 0(ID: 0x55555555), 1(ID: 0x55555554) and 3(ID: 0x55555553)
	//--------------------------------------------------------------------
	int64_t *unk_040;                // 0x040: ? removed object
  //--------------------------------------------------------------------
	int64_t dev_lpar_addr[MAX_DEV];  // 0x048: logical partition addresses of asserted devices, 0 to 15.
	                                 //        E.g. idx 1 = RSX audio device,
	                                 //        E.g. idx 8 = RSX device 8, our GPU.
  //--- objects --------------------------------------------------------
  int64_t *dev_clock_1_obj;        // 0x0C8: RSX device clock object 1, "nvclk"
  int64_t *dev_clock_5_obj;        // 0x0D0: RSX device clock object 5, "display clk"
  int64_t *eic_obj;                // 0x0D8: RSX external interrupt controller object
  int64_t *dev_fb_obj;             // 0x0E0: RSX device framebuffer object
  int64_t *dev_graph_obj;          // 0x0E8: RSX device graph object
  int64_t *unk_0F0;                // 0x0F0: ?
  int64_t *dev_fifo_obj;           // 0x0F8: RSX device fifo object
  int64_t *dev_audio_obj;          // 0x100: RSX device audio object
  int64_t *obj_hash_tbl_obj;       // 0x108: RSX object hash table object
  int64_t *obj_video_rsx_obj;      // 0x110: RSX object video_rsx object
  int64_t *obj_vfb_obj;            // 0x118: RSX object video framebuffer, vfb object ever zero, seams to be the 2D stuff for otherOS(no 3D)
  int64_t *bm_obj_channels;        // 0x120: a RSX utils bitmap object, 4 items, the max(4) RSX channels
  //--------------------------------------------------------------------
  // ??? tb values, for mesure things like total time used for interrupts, and other things
  int64_t unk_128;                 // 0x128: ?
  int64_t unk_130;                 // 0x130: ?
  int64_t unk_138;                 // 0x138: ?
  int64_t unk_140;                 // 0x140: ?
  int64_t unk_148;                 // 0x148: ?
  //------------------------------------------
  int64_t unk_150;                 // 0x150: ?
  int64_t unk_158;                 // 0x158: ?
  int64_t unk_160;                 // 0x160: ?
  //------------------------------------------
  int64_t unk_168;                 // 0x168: ?
  int64_t unk_170;                 // 0x170: ?
  int64_t unk_178;                 // 0x178: ?
  int64_t unk_180;                 // 0x180: ?
  int64_t unk_188;                 // 0x188: ?
  //------------------------------------------
  int64_t unk_190;                 // 0x190: ?
  int64_t unk_198;                 // 0x198: ?
  int64_t unk_1A0;                 // 0x1A0: ?
  int64_t unk_1A8;                 // 0x1A8: ?
  int32_t unk_1B0;                 // 0x1B0: ?
  int32_t unk_1B4;                 // 0x1B4: ?
  int32_t unk_1B8;                 // 0x1B8: ?
  int32_t unk_1BC;                 // 0x1BC: ?
  int64_t unk_1C0;                 // 0x1C0: ?
} __attribute__((packed)) rsx_dev_core_obj_t;





#endif // __RSX_OBJECT_H__
