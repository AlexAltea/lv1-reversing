#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "inc/rsx_lv1.h"




/***********************************************************************
* 
***********************************************************************/
uint32_t rsx_object_sw_class_get_object_class(rsx_sw_class_obj_t *sw_obj)
{
	return sw_obj->class;
}

/***********************************************************************
* 
***********************************************************************/
int64_t rsx_object_sw_class_get_BAR2_address(rsx_sw_class_obj_t *sw_obj)
{
	return sw_obj->bar2_addr;
}


/***********************************************************************
* 
***********************************************************************/
void rsx_object_sw_class_create_object(rsx_sw_class_obj_t *sw_obj, uint32_t class, int64_t *opd)
{
	
	
	
	
	sw_obj->class = class;              // 0xCAFEBABE
	//sw_obj->opd = type;                 // OPD of rsx_object_sw_driver()
	
	// check OPD
	/*if(opd == NULL)
  {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }*/
	
	// check class
	if(class == 0)
	{
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
	
	sw_obj->id = -1; 
	
	
	// hier weiter....
	
	
	
	
	
	
	
	
	
	
	
	
	
	return;
}



