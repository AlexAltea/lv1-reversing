#ifndef __MEMORY_H__
#define __MEMORY_H__


#define GB  *1024*1024*1024
#define MB       *1024*1024
#define KB            *1024

#define ES16(v)((uint16_t)(((v & 0xFF00) >> 8 ) | \
                           ((v & 0x00FF) << 8 )))

#define ES32(v)((uint32_t)(((v & 0xFF000000) >> 24) | \
                           ((v & 0x00FF0000) >> 8 ) | \
							             ((v & 0x0000FF00) << 8 ) | \
							             ((v & 0x000000FF) << 24)))
							          
#define ES64(v) ((uint64_t)(((((uint64_t)v) & 0xFF00000000000000ULL) >> 56) | \
							              ((((uint64_t)v) & 0x00FF000000000000ULL) >> 40) | \
							              ((((uint64_t)v) & 0x0000FF0000000000ULL) >> 24) | \
							              ((((uint64_t)v) & 0x000000FF00000000ULL) >> 8 ) | \
							              ((((uint64_t)v) & 0x00000000FF000000ULL) << 8 ) | \
							              ((((uint64_t)v) & 0x0000000000FF0000ULL) << 24) | \
							              ((((uint64_t)v) & 0x000000000000FF00ULL) << 40) | \
							              ((((uint64_t)v) & 0x00000000000000FFULL) << 56)))


uint32_t ROTL32(uint32_t v, int32_t n);
uint64_t ROTL64(uint64_t v, int32_t n);
uint32_t ROTR32(uint32_t v, int32_t n);
uint64_t ROTR64(uint64_t v, int32_t n);


////////////////////////////////////////////////////////////////////////
// LV1 segment 008 globals
extern int32_t g_rsx_enabled;              // 
extern int32_t g_rsx_unk_00;               // ? 
extern int32_t g_rsx_unk_01;               // ? 
extern int32_t g_rsx_unk_02;               // ?
extern int64_t g_rsx_bar2_addr;            // 0x28002000000
extern int64_t g_rsx_bar0_addr;            // 0x28000000000
extern int64_t *g_rsx_core_obj_tbl[16];    // RSX device core objects, 0 to 15

extern int32_t g_rsx_ctx_count;            // count of allocated RSX contexts

extern int64_t *g_rsx_dbg_obj;
extern int64_t *g_rsx_ioif_obj;

extern int64_t *g_rsx_ctx_tbl[16];         // ? RSX context related, 0 to 15

extern int32_t g_rsx_open_status;          // 
extern int64_t *g_core_mem_obj;            // RSX device memory core object
extern int32_t g_rsx_core_id;              // 

extern int64_t *g_rsx_eic_obj;             // RSX external interrupt object

extern int8_t g_rsx_clock_flag;            // 

////////////////////////////////////////////////////////////////////////
// LV1 return register
extern uint64_t g_r3;
extern uint64_t g_r4;
extern uint64_t g_r5;
extern uint64_t g_r6;
extern uint64_t g_r7;
extern uint64_t g_r8;




////////////////////////////////////////////////////////////////////////
// BAR0, TEST: 32 MB, start 0x28000000000
extern void *BAR0;
#define BAR0_SIZE    184 KB
#define BAR0_START   0x28000000000

void write_BAR0(int32_t value, int64_t addr);
int32_t read_BAR0(int64_t addr);
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
// DDR, 256 MB RSX DDR memory
extern void *DDR;
#define DDR_SIZE   256 MB

void DDR_write08(int8_t value, int64_t addr);
void DDR_write16(int16_t value, int64_t addr);
void DDR_write32(int32_t value, int64_t addr);
void DDR_write64(int64_t value, int64_t addr);
int8_t DDR_read08(int64_t addr);
int16_t DDR_read16(int64_t addr);
int32_t DDR_read32(int64_t addr);
int64_t DDR_read64(int64_t addr);
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
// driver info, 0xC000 byte
extern void *driver_info;
#define DRVI_SIZE   48 KB

void DRVI_write08(int32_t idx, int8_t value, int32_t offset);
void DRVI_write16(int32_t idx, int16_t value, int32_t offset);
void DRVI_write32(int32_t idx, int32_t value, int32_t offset);
void DRVI_write64(int32_t idx, int64_t value, int32_t offset);
int8_t DRVI_read08(int32_t idx, int32_t offset);
int16_t DRVI_read16(int32_t idx, int32_t offset);
int32_t DRVI_read32(int32_t idx, int32_t offset);
int64_t DRVI_read64(int32_t idx, int32_t offset);
////////////////////////////////////////////////////////////////////////


// init
void alloc_lv1_memory(void);
void free_lv1_memory(void);



#endif // __MAIN_H__
