/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "memory.h"

////////////////////////////////////////////////////////////////////////
// BAR0, NV registers, start of address space 0x28000000000
// full address space as real memory for test
void *BAR0 = NULL;

////////////////////////////////////////////////////////////////////////
// DDR, 256 MB DDR memory
void *DDR = NULL;

////////////////////////////////////////////////////////////////////////
// driver info, 0xC000 byte, as extern buffer, need this area in BE
void *driver_info = NULL;



// init LV1 globals
S32 g_rsx_enabled           = 1;        // 
S32 g_rsx_unk_00            = 0;        // ? 
S32 g_rsx_unk_01            = 0;        // ? 
S32 g_rsx_unk_02            = 0;        // ? 
S64 g_rsx_bar2_addr         = 0;        // 0x28002000000
S64 g_rsx_bar0_addr         = 0;        // 0x28000000000
S64 *g_rsx_core_obj_tbl[16] = {};       // array for 16 posible device core objects

S32 g_rsx_ctx_count         = 0;        // count of currently allocated RSX contexts

S64 *g_rsx_dbg_obj          = NULL;     // RSX debugger object
S64 *g_rsx_ioif_obj         = NULL;     // RSX IO interface 0 object

S64 *g_rsx_ctx_tbl[16]      = {};       // ? RSX context related, 0 to 15

S32 g_rsx_open_status       = 0;        // 0(?), 1(?) or 2(?) 
S64 *g_core_mem_obj         = NULL;     // RSX device memory core object
S32 g_rsx_core_id           = 0xFF;     // RSX device core ID(0 to 15), init 0xFF(invalid)

S64 *g_rsx_eic_obj          = NULL;     // RSX external interrupt controller object

S08 g_rsx_clock_flag         = 1;       // ?



// LV1 return registers
U64 g_r3 = 0;
U64 g_r4 = 0;
U64 g_r5 = 0;
U64 g_r6 = 0;
U64 g_r7 = 0;
U64 g_r8 = 0;






/***********************************************************************
* rotate
***********************************************************************/
U32 ROTL32(U32 v, S32 n) {
  return (((n &= 32 - 1) == 0) ? v : (v << n) | (v >> (32 - n)));
}

U64 ROTL64(U64 v, S32 n) {
  return (((n &= 64 - 1) == 0) ? v : (v << n) | (v >> (64 - n)));
}

U32 ROTR32(U32 v, S32 n) {
  return (((n &= 32 - 1) == 0) ? v : (v >> n) | (v << (32 - n)));
}

U64 ROTR64(U64 v, S32 n) {
  return (((n &= 64 - 1) == 0) ? v : (v >> n) | (v << (64 - n)));
}


////////////////////////////////////////////////////////////////////////
// BAR0, NV registers, start 0x28000000000, size 32 MB
// (184 KB real memory)
/***********************************************************************
* write 32bit value to BAR0
***********************************************************************/
void write_BAR0(S32 value, S64 addr) {
	S32 offset;
	
	
	// invalid address
  if(!(addr & 0x28000000000))
	  return;
	
	offset = addr & 0xFFFFFFFF;
	
	// 0x28000000000 - 0x28000004FFF, (20 KB)
	if((offset >= 0) && (offset < 0x5000))
		*(S32*)(BAR0 + offset) = value;
	
	// 0x28000008000 - 0x2800000AFFF, (12 KB)
	else if((offset >= 0x8000) && (offset < 0xB000))
		*(S32*)(BAR0 + 0x5000 + offset - 0x8000) = value;
	
	// 0x2800000C000 - 0x2800000CFFF, (4 KB)
	else if((offset >= 0xC000) && (offset < 0xD000))
		*(S32*)(BAR0 + 0x8000 + offset - 0xC000) = value;
	
	// 0x28000080000 - 0x28000088FFF, (36 KB), not perfect
	else if((offset >= 0x80000) && (offset < 0x89000))
		*(S32*)(BAR0 + 0x9000 + offset - 0x80000) = value;
	
	// 0x2800008A000 - 0x2800008AFFF, (4 KB)
	else if((offset >= 0x8A000) && (offset < 0x8B000))
		*(S32*)(BAR0 + 0x12000 + offset - 0x8A000) = value;
	
	// 0x2800008C000 - 0x2800008CFFF, (4 KB)
	else if((offset >= 0x8C000) && (offset < 0x8D000))
		*(S32*)(BAR0 + 0x13000 + offset - 0x8C000) = value;
	
	// 0x28000090000 - 0x28000093FFF, (16 KB)
	else if((offset >= 0x90000) && (offset < 0x94000))
		*(S32*)(BAR0 + 0x14000 + offset - 0x90000) = value;
	
	// 0x28000100000 - 0x28000100FFF, (4 KB)
	else if((offset >= 0x100000) && (offset < 0x101000))
	  *(S32*)(BAR0 + 0x18000 + offset - 0x100000) = value;
	
	// 0x28000200000 - 0x28000200FFF, (4 KB)
	else if((offset >= 0x200000) && (offset < 0x201000))
	  *(S32*)(BAR0 + 0x19000 + offset - 0x200000) = value;
	
	// 0x28000400000 - 0x2800040FFFF, (64 KB)
	else if((offset >= 0x400000) && (offset < 0x410000))
	  *(S32*)(BAR0 + 0x1A000 + offset - 0x400000) = value;
	
	// 0x28000600000 - 0x28000600FFF, (4 KB)
	else if((offset >= 0x600000) && (offset < 0x601000))
	  *(S32*)(BAR0 + 0x2A000 + offset - 0x600000) = value;
	
	// 0x28000602000 - 0x28000602FFF, (4 KB)
	else if((offset >= 0x602000) && (offset < 0x603000))
	  *(S32*)(BAR0 + 0x2B000 + offset - 0x602000) = value;
	
	// 0x28000680000 - 0x28000680FFF, (4 KB)
	else if((offset >= 0x680000) && (offset < 0x681000))
	  *(S32*)(BAR0 + 0x2C000 + offset - 0x680000) = value;
	
	// 0x28000682000 - 0x28000682FFF, (4 KB)
	else if((offset >= 0x682000) && (offset < 0x683000))
	  *(S32*)(BAR0 + 0x2D000 + offset - 0x682000) = value;
	  
	else
	  return;
}


/***********************************************************************
* read 32bit value from BAR0
***********************************************************************/
S32 read_BAR0(S64 addr) {
	S32 offset;
	
	
	// invalid address
  if(!(addr & 0x28000000000))
	  return -1;
	
	offset = addr & 0xFFFFFFFF;
	
	// 0x28000000000 - 0x28000004FFF, (20 KB)
	if((offset >= 0) && (offset < 0x5000))
		return *(S32*)(BAR0 + offset);
	
	// 0x28000008000 - 0x2800000AFFF, (12 KB)
	else if((offset >= 0x8000) && (offset < 0xB000))
		return *(S32*)(BAR0 + 0x5000 + offset - 0x8000);
	
	// 0x2800000C000 - 0x2800000CFFF, (4 KB)
	else if((offset >= 0xC000) && (offset < 0xD000))
		return *(S32*)(BAR0 + 0x8000 + offset - 0xC000);
	
	// 0x28000080000 - 0x28000088FFF, (36 KB), not perfect
	else if((offset >= 0x80000) && (offset < 0x89000))
		return *(S32*)(BAR0 + 0x9000 + offset - 0x80000);
	
	// 0x2800008A000 - 0x2800008AFFF, (4 KB)
	else if((offset >= 0x8A000) && (offset < 0x8B000))
		return *(S32*)(BAR0 + 0x12000 + offset - 0x8A000);
	
	// 0x2800008C000 - 0x2800008CFFF, (4 KB)
	else if((offset >= 0x8C000) && (offset < 0x8D000))
		return *(S32*)(BAR0 + 0x13000 + offset - 0x8C000);
	
	// 0x28000090000 - 0x28000093FFF, (16 KB)
	else if((offset >= 0x90000) && (offset < 0x94000))
		return *(S32*)(BAR0 + 0x14000 + offset - 0x90000);
	
	// 0x28000100000 - 0x28000100FFF, (4 KB)
	else if((offset >= 0x100000) && (offset < 0x101000))
	  return *(S32*)(BAR0 + 0x18000 + offset - 0x100000);
	
	// 0x28000200000 - 0x28000200FFF, (4 KB)
	else if((offset >= 0x200000) && (offset < 0x201000))
	  return *(S32*)(BAR0 + 0x19000 + offset - 0x200000);
	
	// 0x28000400000 - 0x2800040FFFF, (64 KB)
	else if((offset >= 0x400000) && (offset < 0x410000))
	  return *(S32*)(BAR0 + 0x1A000 + offset - 0x400000);
	
	// 0x28000600000 - 0x28000600FFF, (4 KB)
	else if((offset >= 0x600000) && (offset < 0x601000))
	  return *(S32*)(BAR0 + 0x2A000 + offset - 0x600000);
	
	// 0x28000602000 - 0x28000602FFF, (4 KB)
	else if((offset >= 0x602000) && (offset < 0x603000))
	  return *(S32*)(BAR0 + 0x2B000 + offset - 0x602000);
	
	// 0x28000680000 - 0x28000680FFF, (4 KB)
	else if((offset >= 0x680000) && (offset < 0x681000))
	  return *(S32*)(BAR0 + 0x2C000 + offset - 0x680000);
	
	// 0x28000682000 - 0x28000682FFF, (4 KB)
	else if((offset >= 0x682000) && (offset < 0x683000))
	  return *(S32*)(BAR0 + 0x2D000 + offset - 0x682000);
	
	else
	  return -1;
}

/***********************************************************************
* 
***********************************************************************/
static void init_BAR0() {
	S64 bar0_addr = 0x28000000000;
	
	
	// 
	write_BAR0(0xE4D001A1, bar0_addr);
	//write_BAR0(0x01000001, bar0_addr + 4);
	
	
	
	// clock 5
	write_BAR0(0x400 /*[22:22]*/ | 0   /*[13:15]*/, bar0_addr + 0x4010);
	write_BAR0(0x1A00/*[16:23]*/ | 0x04/*[24:31]*/, bar0_addr + 0x4014);
	
	// clock 1
	write_BAR0(0x400 /*[22:22]*/ | 0   /*[13:15]*/, bar0_addr + 0x4028);
	write_BAR0(0x0A00/*[16:23]*/ | 0x02/*[24:31]*/, bar0_addr + 0x402C);
	
	
	
	
	
	return;
}



////////////////////////////////////////////////////////////////////////
// DDR, 256 MB RSX DDR memory

static U32 addr_pramin_to_vram(U32 offset) {
  U32 vram_size = 0x10000000; // 256 MB
  U32 rev_size = 0x80000; // 512 KB
  return vram_size - (offset - (offset % rev_size)) - rev_size + (offset % rev_size);
}

/*
static U32 addr_vram_to_pramin(U32 offset) {
  U32 vram_size = 256 MB;
  U32 rev_size = 512 KB;
  return (offset - vram_size) ^ -rev_size;
}
*/

/***********************************************************************
* write 32bit value to DDR memory
***********************************************************************/
static void _write_DDR(S32 size, S64 value, S64 addr) {
	// invalid IO Interface 0 address
  if(!(addr & 0x28000000000))
	  return;
	
	// BAR1: (VRAM) (LV1 process space address), BE
	if(addr & 0x80000000) {
		switch(size) {
			case  8:
			  *(S08*)(DDR + (addr - 0x28080000000)) = value;
			  break;
			case 16:
			  *(S16*)(DDR + (addr - 0x28080000000)) = ES16(value);
			  break;
			case 32:
			  *(S32*)(DDR + (addr - 0x28080000000)) = ES32(value);
			  break;
			case 64:
			  *(S64*)(DDR + (addr - 0x28080000000)) = ES64(value);
			  break;
		}
		return;
	}
	
	// BAR2: (PRAMIN) system reserved, LE
	if(addr & 0x2000000) {
		switch(size) {
			case  8:
			  *(S08*)(DDR + addr_pramin_to_vram(addr & 0xFFFFF)) = value;
			  break;
			case 16:
			  *(S16*)(DDR + addr_pramin_to_vram(addr & 0xFFFFF)) = value;
			  break;
			case 32:
			  *(S32*)(DDR + addr_pramin_to_vram(addr & 0xFFFFF)) = value;
			  break;
			case 64:
			  *(S64*)(DDR + addr_pramin_to_vram(addr & 0xFFFFF)) = value;
			  break;
		}
		return;
	}
	
	return;
}

/***********************************************************************
* read 32bit value from DDR memory
***********************************************************************/
static S32 _read_DDR(S32 size, S64 addr) {
	// invalid IO Interface 0 address
	if(!(addr & 0x28000000000))
	  return 0;
	
	// BAR1: (VRAM)(LV1 process space address), BE
	if(addr & 0x80000000) {
		switch(size) {
			case  8:
			  return *(S8*)(DDR + (addr - 0x28080000000));
			case 16:
			  return ES16(*(S16*)(DDR + (addr - 0x28080000000)));
			case 32:
			  return ES32(*(S32*)(DDR + (addr - 0x28080000000)));
			case 64:
			  return ES64(*(S64*)(DDR + (addr - 0x28080000000)));
		}	
	}
		
	// BAR2: (PRAMIN)system reserved, LE
	if(addr & 0x2000000) {
		switch(size) {
			case  8:
			  return *(S8*)(DDR + addr_pramin_to_vram(addr & 0xFFFFF));
			case 16:
			  return *(S16*)(DDR + addr_pramin_to_vram(addr & 0xFFFFF));
			case 32:
			  return *(S32*)(DDR + addr_pramin_to_vram(addr & 0xFFFFF));
			case 64:
			  return *(S64*)(DDR + addr_pramin_to_vram(addr & 0xFFFFF));
		}
	}
	
	return 0;
}

void DDR_write08(S08 value,  S64 addr){_write_DDR( 8, value, addr); return;}
void DDR_write16(S16 value, S64 addr){_write_DDR(16, value, addr); return;}
void DDR_write32(S32 value, S64 addr){_write_DDR(32, value, addr); return;}
void DDR_write64(S64 value, S64 addr){_write_DDR(64, value, addr); return;}
S08 DDR_read08(S64 addr) {return _read_DDR( 8, addr);}
S16 DDR_read16(S64 addr){return _read_DDR(16, addr);}
S32 DDR_read32(S64 addr){return _read_DDR(32, addr);}
S64 DDR_read64(S64 addr){return _read_DDR(64, addr);}


////////////////////////////////////////////////////////////////////////
// driver info

void DRVI_write08(S32 idx, S8 value, S32 offset) {
	*(S8*)((driver_info + idx * 0x4000) + offset) = value;
	return;
}
void DRVI_write16(S32 idx, S16 value, S32 offset) {
	*(S16*)((driver_info + idx * 0x4000) + offset) = ES16(value);
	return;
}
void DRVI_write32(S32 idx, S32 value, S32 offset) {
	*(S32*)((driver_info + idx * 0x4000) + offset) = ES32(value);
	return;
}
void DRVI_write64(S32 idx, S64 value, S32 offset) {
	*(S64*)((driver_info + idx * 0x4000) + offset) = ES64(value);
	return;
}

S8 DRVI_read08(S32 idx, S32 offset) {
	return *(S8*)((driver_info + idx * 0x4000) + offset);
}
S16 DRVI_read16(S32 idx, S32 offset) {
	return ES16(*(S16*)((driver_info + idx * 0x4000) + offset));
}
S32 DRVI_read32(S32 idx, S32 offset) {
	return ES32(*(S32*)((driver_info + idx * 0x4000) + offset));
}
S64 DRVI_read64(S32 idx, S32 offset) {
	return ES64(*(S64*)((driver_info + idx * 0x4000) + offset));
}



////////////////////////////////////////////////////////////////////////
// init
/***********************************************************************
* 
***********************************************************************/
void alloc_lv1_memory() {
	// BAR0
	BAR0 = malloc(BAR0_SIZE);
	memset(BAR0, 0, BAR0_SIZE);
	init_BAR0();
	
	// DDR memory
	DDR = malloc(DDR_SIZE);
	memset(DDR, 0, DDR_SIZE);
	
	// driver info
	driver_info = malloc(DRVI_SIZE);
	memset(driver_info, 0, DRVI_SIZE);
}

/***********************************************************************
* 
***********************************************************************/
void free_lv1_memory() {
	free(BAR0);
	free(DDR);
	free(driver_info);
}












