/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "inc/rsx_lv1.h"



/***********************************************************************
* 
***********************************************************************/
void rsx_object_vfb_init(rsx_obj_vfb_obj_t* vfb_obj) {
    vfb_obj->unk_00 = 0;
    vfb_obj->unk_08 = 0;
    vfb_obj->unk_10 = 0;
    vfb_obj->unk_18 = 0;
    vfb_obj->unk_1C = 0;
    vfb_obj->unk_20 = 0;
    vfb_obj->unk_28 = 0;
    vfb_obj->unk_30 = 0;
    vfb_obj->unk_38 = 0;
  
    return;
}



