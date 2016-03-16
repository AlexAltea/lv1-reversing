#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "inc/rsx_lv1.h"






/***********************************************************************
* 
***********************************************************************/
void rsx_device_fb_21ADAC(rsx_dev_fb_obj_t *fb)
{
	int32_t value = 0, size_MB;
	
	
	// ?
	value = read_BAR0(0x28000100084);
	value &= 0xFFFFFFDF;                     // unset value[26:26]
	write_BAR0(value, 0x28000100084);
	
	value = read_BAR0(0x28000100080);
	value |= 0x8000000;                      // set value[04:04]
	write_BAR0(value, 0x28000100080);
	
	write_BAR0(0xCCCC5044, 0x28000100330);
	write_BAR0(0xFFFF18FF, 0x2800010032C);
	write_BAR0(0x0000440A, 0x28000100350);
	
	value = read_BAR0(0x2800010033C);
	value &= 0x000080FF;                     // unset value[16:16], value[24:31]
	value |= 0x00041200;                     // set value[13:13], value[19:19], value[22.22]
	write_BAR0(value, 0x2800010033C);
	
	write_BAR0(0x000001FB, 0x28000100340);
	write_BAR0(0x00000000, 0x28000100344);
	write_BAR0(0x00000000, 0x28000100348);
	write_BAR0(0x0000000A, 0x28000100338);
	write_BAR0(0x33332022, 0x28000100334);
	write_BAR0(0x00000806, 0x28000100354);
	
	// get size in MB from frame buffer object
	size_MB = fb->size >>20;
	
	switch(size_MB)
	{
		case 128:
		  value = read_BAR0(0x28000088304);
		  value &= 0xFFFFFFFC;                 // unset value[30:30] and [31:31]
		  value |= 1;                          // set value[31:31]
		  write_BAR0(value, 0x28000088304);
		  break;
		case 256:
		  value = read_BAR0(0x28000088304);
		  value &= 0xFFFFFFFC;                 // unset value[30:30] and [31:31]
		  value |= 2;                          // set value[30:30]
		  write_BAR0(value, 0x28000088304);
		  break;
		case 64:
		  value = read_BAR0(0x28000088304);
		  value &= 0xFFFFFFFC;                 // unset value[30:30] and [31:31]
		  write_BAR0(value, 0x28000088304);
		default:
		  printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		  return;
	}
	
	return;
}


/***********************************************************************
* 
***********************************************************************/
void rsx_device_fb_init(rsx_dev_fb_obj_t *fb)
{
	int32_t value = 0, size, DDR_MB_CTags;
	rsx_dev_core_obj_t *core = NULL;
	
	
	// get and store DDR size value from BAR0:0x2800010020C
	write_BAR0(0x10000000, 0x2800010020C);  // Test
	value = read_BAR0(0x2800010020C);
	value &= 0xFFF00000;                // should be 0x10000000, (256 MB)
	fb->size = value;
	
	// get device core object
	core = rsx_core_device_get_core_object_by_id(g_rsx_core_id);
	if(core == NULL)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return;
	}
	
	// get and store BAR1 address(0x28080000000)
	fb->io_addr = rsx_bus_ioif0_get_BAR1_addr((void*)core->ioif0);
	
	// get BAR1/BAR2 size
	size = rsx_bus_ioif0_get_BAR1_size((void*)core->ioif0);
	
	// if BAR1/BAR2 size from IOIF0 not match with the size value from BAR0, error
	if(size != fb->size)
	{
		printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
		return;
	}
	
	// load DDR_MB_CTags from BAR0, increment by 1 and store back
	DDR_MB_CTags = read_BAR0(0x28000100320);
	DDR_MB_CTags++;
	write_BAR0(DDR_MB_CTags, 0x28000100320);
	
	rsx_device_fb_21ADAC(fb);
  
  return;
}
