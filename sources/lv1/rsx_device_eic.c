#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "inc/rsx_lv1.h"



/*
id:
------------------------------------------------------------------------
 0: 
 1: 
 2: 
 3: 
 4: 
 5: audio
 6: 
 7: rsx ctx 0
 8: rsx ctx 1
 9: rsx ctx 2
10: 
11: 
12: 
13: 
14: 
------------------------------------------------------------------------
*/


/***********************************************************************
* 
***********************************************************************/
int32_t rsx_device_eic_219C18(int32_t id, int64_t *out)
{
	
	
	
	// HSPRG0:
	// addis     r9, r13, 0
	// addi      r9, r9, -0x67E8
	// ld        r29, 0(r9)
	/*
	if(r29 == 0)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return 1;
	}
	*/
	
	// get global eic_obj
	
	
	// check id
	if(id > 14)
	  return LV1_NO_ENTRY;
	
	
	//TODO: external interrupt controller stuff...
	
	
	
	
	
	return 0;  // OK
}


/***********************************************************************
* 
***********************************************************************/
void rsx_device_eic_init_flags(void)
{
	int32_t value = 0;
	
	
	value = read_BAR0(0x28000088008);
	value |= 0x100000;                   // set flags[11:11]
	write_BAR0(value, 0x28000088008);
	
	value = read_BAR0(0x28000088008);
	value |= 1;                          // set flags[31:31]
	write_BAR0(value, 0x28000088008);
	
	value = read_BAR0(0x28000088008);
	value |= 0x10000;                    // set flags[15:15]
	write_BAR0(value, 0x28000088008);
	
	value = read_BAR0(0x28000088008);
	value |= 0x1000;                     // set flags[19:19]
	write_BAR0(value, 0x28000088008);
	
	value = read_BAR0(0x28000088008);
	value |= 0x100;                      // set flags[23:23]
	write_BAR0(value, 0x28000088008);
	
	value = read_BAR0(0x28000088008);
	value |= 0x10;                       // set flags[27:27]
	write_BAR0(value, 0x28000088008);
	
	value = read_BAR0(0x28000088004);
	value |= 1;                          // set flags[31:31]
	write_BAR0(value, 0x28000088004);
	
	value = 0x0000E000;
	write_BAR0(value, 0x2800008800C);    // set flags[16:18]
	
	return;
}



