#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "inc/lv1_misc.h"




// misc LV1 fake functions, no time or interest to RE this shit too
// and e.g. fixed LPAR addresses are ok for this RSX RE work...






/***********************************************************************
* setup a unk LV1 struct with given values, maybe interrupt related?
* 
* lv1_unk_obj_00_t* obj = (IN) struct to set
* S64 arg1          = (IN) ?, e.g. 0
* S64 opd_64        = (IN) a pointer to a OPD, for test here no pointer, interrupt thread/callback?
* void *obj_1           = (IN) a pointer to a object, e.g. the RSX object-video-rsx object
***********************************************************************/
void lv1_sub_2CA414(lv1_unk_obj_00_t* obj_0, S64 arg1, S64 opd_64, void *obj_1) {
	obj_0->unk_00 = arg1;
	obj_0->unk_08 = opd_64;
	obj_0->unk_10 = obj_1;
	obj_0->unk_18 = 0;
	obj_0->unk_20 = 0;
	return;
}

/***********************************************************************
* in real, a checksum from the name is builded, thats the base value for
* compute the offset into param list.
***********************************************************************/
void get_param_value(const char *name, uS64 *value) {
	if ((strcmp(name, "sys.boot.gos")) == 0)
	{
	  *value = SYS_BOOT_GOS;
	  return;
	}
	
	if ((strcmp(name, "sys.lv1.rsxenable")) == 0)
	{
	  *value = SYS_LV1_RSXENABLE;
	  return;
	}
	
	if ((strcmp(name, "sys.lv1.rsxdebug")) == 0)
	{
	  *value = SYS_LV1_RSXDEBUG;
	  return;
	}
	
	if ((strcmp(name, "sys.ac.misc2")) == 0)
	{
	  *value = SYS_AC_MISC2;
	  return;
	}
	
	if ((strcmp(name, "sys.rdcy.2")) == 0)
	{
	  *value = SYS_RDCY_2;
	  return;
	}
	
	if ((strcmp(name, "be.0.tb_clk")) == 0)
	{
	  *value = BE_0_TB_CLOCK;
	  return;
	}
	
	return;
}

/***********************************************************************
* 
***********************************************************************/
void *lv1_kmalloc(S32 size) {
	return (void*)(malloc(size));
}

/***********************************************************************
* 
***********************************************************************/
void lv1_kfree(void *addr) {
	free(addr);
}

/***********************************************************************
* map mmio, fake function, return fixed LPAR address values
* 
* S32 core_id    = (IN) 
* S64 io_addr    = (IN) 
* S32 page_sh    = (IN) page size shift bit count, 1 <<12 = 0x1000(4 KB), 1 <<20 = 0x100000(1 MB)
* S32 io_size    = (IN) size to map
* S64 *lpar_addr = (OUT) mapped logical partition address
***********************************************************************/
S32 rsx_map_io_to_lpar(S32 core_id, S64 io_addr, S32 page_sh, S32 io_size, S64 *lpar_addr) {
	switch(io_addr)
	{
		case 0x28000000000:             // rsx device  0, BAR0
		  *lpar_addr = 0;
		  break;
		case 0x2808FF20000:             // rsx device  1, audio
		  *lpar_addr = 0x30000001B000;
		  break;
		case 0x28000080100:             // rsx device  2, unk(DEX)?
		  *lpar_addr = -1;
		  break;
		case 0x28001000000:             // rsx device  3, unk(DEX)?
		  *lpar_addr = -1;
		  break;
		case 0x28001800000:             // rsx device  4, unk(DEX)?
		  *lpar_addr = -1;
		  break;
		case 0x2800008A000:             // rsx device  5, unk(DEX)?
		  *lpar_addr = -1;
		  break;
		case 0x28000200000:             // rsx device  6, unk(DEX)?
		  *lpar_addr = -1;
		  break;
		case 0x28000600000:             // rsx device  7, unk(DEX)?
		  *lpar_addr = -1;
		  break;
		case 0x2808FF10000:             // rsx device  8, GPU, global semaphore
		  *lpar_addr = 0x300000020000;
		  break;
		case 0x28000400000:             // rsx device  9, unk(DEX)?
		  *lpar_addr = -1;
		  break;
		case 0x28000100000:             // rsx device 10, unk(DEX)?
		  *lpar_addr = -1;
		  break;
		case 0x2800000A000:             // rsx device 11, unk(DEX)?
		  *lpar_addr = -1;
		  break;
		case 0x28000680000:             // rsx device 12, unk(DEX)?
		  *lpar_addr = -1;
		  break;
		case 0x28000090000:             // rsx device 13, unk(DEX)?
		  *lpar_addr = -1;
		  break;
		case 0x28000002000:             // rsx device 14, unk(DEX)?
		  *lpar_addr = -1;
		  break;
		case 0x28000088000:             // rsx device 15, unk(DEX)?
		  *lpar_addr = -1;
		  break;
		case 0x28080000000:             // map local memory
		  *lpar_addr = 0x7000B0000000;
		  break;
		case 0x2808FE00000:             // map reports
		  *lpar_addr = 0x480000340000;
		  break;
		case 0x28000C00000:             // map dma control
		  *lpar_addr = 0x4400001C0000;
		  break;
	}
	
	if ((io_addr < 0x28000000000) && (io_size == 0xC000))  // map driver info
	  *lpar_addr = 0x4000000C0000;
	
	return 0;  // OK
}


// rsx_map_io_to_lpar():                                                                       LPAR
// (device_id   , 0x28000000000, 12, 0x2000000, &lpar);  // rsx device  0, BAR0                0x0, seams to be 0x0
// (core_obj_id , 0x2808FF20000, 12,    0x1000, &lpar);  // rsx device  1, audio               0x30000001B000
// (            , 0x28000080100, 12,    0x8000, &lpar);  // rsx device  2                      
// (            , 0x28001000000, 12,    0x1000, &lpar);  // rsx device  3                      
// (            , 0x28001800000, 12,  0x200000, &lpar);  // rsx device  4                      
// (            , 0x2800008A000, 12,    0x1000, &lpar);  // rsx device  5                      
// (            , 0x28000200000, 12,    0x1000, &lpar);  // rsx device  6                      
// (device_id   , 0x28000600000, 12,   0x10000, &lpar);  // rsx device  7                      
// (device_id(0), 0x2808FF10000, 12,    0x1000, &lpar);  // rsx device  8, global semaphore    0x300000020000
// (            , 0x28000400000, 12,    0x8000, &lpar);  // rsx device  9                      
// (            , 0x28000100000, 12,    0x1000, &lpar);  // rsx device 10                      
// (            , 0x2800000A000, 12,    0x1000, &lpar);  // rsx device 11                      
// (            , 0x28000680000, 12,    0x3000, &lpar);  // rsx device 12                      
// (            , 0x28000090000, 12,    0x1000, &lpar);  // rsx device 13                      
// (            , 0x28000002000, 12,    0x2000, &lpar);  // rsx device 14                      
// (            , 0x28000088000, 12,    0x1000, &lpar);  // rsx device 15                      

// (2           , 0x28080000000, 20, 0xFE00000, &lpar);  // map local memory                   0x7000B0000000
// (2           ,      0x590000, 12,    0xC000, &lpar);  // map driver info area               0x4000000C0000
// (2           , 0x2808FE00000, 12,   0x40000, &lpar);  // map reports area                   0x480000340000
// (2           , 0x28000C00000, 12,   0x20000, &lpar);  // map dma control area               0x4400001C0000

