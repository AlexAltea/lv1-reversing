/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

// Constants
#define _512_KB  (512*1024)
#define _256_MB  (256*1024*1024)

#define ES16(v) ((U16)( \
    (((v) & 0xFF00) >> 8 ) | \
    (((v) & 0x00FF) << 8 )))

#define ES32(v) ((U32)( \
    (((v) & 0xFF000000) >> 24) | \
    (((v) & 0x00FF0000) >> 8 ) | \
	(((v) & 0x0000FF00) << 8 ) | \
	(((v) & 0x000000FF) << 24)))
							          
#define ES64(v) ((U64)(\
    ((((U64)(v)) & 0xFF00000000000000ULL) >> 56) | \
	((((U64)(v)) & 0x00FF000000000000ULL) >> 40) | \
	((((U64)(v)) & 0x0000FF0000000000ULL) >> 24) | \
	((((U64)(v)) & 0x000000FF00000000ULL) >> 8 ) | \
    ((((U64)(v)) & 0x00000000FF000000ULL) << 8 ) | \
	((((U64)(v)) & 0x0000000000FF0000ULL) << 24) | \
	((((U64)(v)) & 0x000000000000FF00ULL) << 40) | \
	((((U64)(v)) & 0x00000000000000FFULL) << 56)))


U32 ROTL32(U32 v, S32 n);
U64 ROTL64(U64 v, S32 n);
U32 ROTR32(U32 v, S32 n);
U64 ROTR64(U64 v, S32 n);


////////////////////////////////////////////////////////////////////////
// LV1 segment 008 globals
extern S32 g_rsx_enabled;              // 
extern S32 g_rsx_unk_00;               // ? 
extern S32 g_rsx_unk_01;               // ? 
extern S32 g_rsx_unk_02;               // ?
extern S64 g_rsx_bar2_addr;            // 0x28002000000
extern S64 g_rsx_bar0_addr;            // 0x28000000000
extern S64 *g_rsx_core_obj_tbl[16];    // RSX device core objects, 0 to 15

extern S32 g_rsx_ctx_count;            // count of allocated RSX contexts

extern S64 *g_rsx_dbg_obj;
extern S64 *g_rsx_ioif_obj;

extern S64 *g_rsx_ctx_tbl[16];         // ? RSX context related, 0 to 15

extern S32 g_rsx_open_status;          // 
extern S64 *g_core_mem_obj;            // RSX device memory core object
extern S32 g_rsx_core_id;              // 

extern S64 *g_rsx_eic_obj;             // RSX external interrupt object

extern S08 g_rsx_clock_flag;           // 

////////////////////////////////////////////////////////////////////////
// LV1 return register
extern U64 g_r3;
extern U64 g_r4;
extern U64 g_r5;
extern U64 g_r6;
extern U64 g_r7;
extern U64 g_r8;




////////////////////////////////////////////////////////////////////////
// BAR0, TEST: 32 MB, start 0x28000000000
extern void *BAR0;
#define BAR0_SIZE    184 KB
#define BAR0_START   0x28000000000

void write_BAR0(S32 value, S64 addr);
S32 read_BAR0(S64 addr);
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
// DDR, 256 MB RSX DDR memory
extern void *DDR;
#define DDR_SIZE   256 MB

void DDR_write08(S08 value, S64 addr);
void DDR_write16(S16 value, S64 addr);
void DDR_write32(S32 value, S64 addr);
void DDR_write64(S64 value, S64 addr);
S08 DDR_read08(S64 addr);
S16 DDR_read16(S64 addr);
S32 DDR_read32(S64 addr);
S64 DDR_read64(S64 addr);
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
// driver info, 0xC000 byte
extern void *driver_info;
#define DRVI_SIZE   48 KB

void DRVI_write08(S32 idx, S08 value, S32 offset);
void DRVI_write16(S32 idx, S16 value, S32 offset);
void DRVI_write32(S32 idx, S32 value, S32 offset);
void DRVI_write64(S32 idx, S64 value, S32 offset);
S08 DRVI_read08(S32 idx, S32 offset);
S16 DRVI_read16(S32 idx, S32 offset);
S32 DRVI_read32(S32 idx, S32 offset);
S64 DRVI_read64(S32 idx, S32 offset);
////////////////////////////////////////////////////////////////////////


// init
void alloc_lv1_memory(void);
void free_lv1_memory(void);
