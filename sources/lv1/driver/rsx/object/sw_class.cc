/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "sw_class.h"

/***********************************************************************
* 
***********************************************************************/
void rsx_object_sw_class_create_object(rsx_object_sw_class_t* sw_obj, U32 handle, S64 *opd) {
    
    
    
    
    sw_obj->handle = handle;              // 0xCAFEBABE
    //sw_obj->opd = type;                 // OPD of rsx_object_sw_driver()
    
    // check OPD
    /*if (opd == NULL) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return 0;
  }*/
    
    // check class
    if (class == 0) {
    printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
    return;
  }
    
    sw_obj->id = -1; 
    
    
    // hier weiter....
    
    
    
    
    
    
    
    
    
    
    
    
    
    return;
}



