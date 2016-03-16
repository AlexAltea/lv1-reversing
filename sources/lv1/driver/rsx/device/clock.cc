/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "inc/rsx_lv1.h"



// RSX device clock settings
static rsx_dev_clock_setting_t set[3] = {
    {1, 2, 0x4028, 0x402C, 0},
    {6, 3, 0x4010, 0x4014, 0},
    {9, 3,      0,      0, 0}
};



/***********************************************************************
* 
***********************************************************************/
static void rsx_device_clock_rsx_21F0C0(rsx_dev_clock_obj_t* clock_obj) {
    S32 value_1, value_2;
    S64 t1, t2, t3;
    
    
    // is clock ID valid?
    if ((clock_obj->id == 1) || (clock_obj->id == 5)) {
        // read BAR0 registers
        value_1 = read_BAR0(g_rsx_bar0_addr + clock_obj->offset_0);
        value_2 = read_BAR0(g_rsx_bar0_addr + clock_obj->offset_1);
        
        // if value_1[22:22] is set
        if ((value_1 & 0x400) >>10) {
            t1 = (value_2 & 0xFF00) >>8;     // value_2[16:23], clock 1(2),   clock 5(4)
            t2 = value_2 & 0xFF;             // value_2[24:31], clock 1(0xA), clock 5(0x1A)
            t3 = (value_1 & 0x70000) >>16;   // value_1[13:15], 0
            
            clock_obj->unk_20 = t2;          // 
            clock_obj->unk_24 = t1;          // 
            
            clock_obj->freq = (S32)(((t1 * 0x5F5E100) / t2) / (1 <<t3));
            return;
        }
        else // not clock 1 or 5, set default frequency: 100 MHz
          clock_obj->freq = 0x5F5E100;
    }
  
  printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
  return;
}

/***********************************************************************
* 
***********************************************************************/
S32 rsx_device_clock_rsx_get_frequency(rsx_dev_clock_obj_t* clock_obj) {
    rsx_device_clock_rsx_21F0C0((void*)clock_obj);
    return clock_obj->freq;
}
 
/***********************************************************************
* 
***********************************************************************/
void rsx_device_clock_rsx_create(rsx_dev_clock_obj_t* clock_obj, S32 id, S32 arg1) {
    S32 i;
    
    
    if (g_rsx_clock_flag != 0) {
        clock_obj->id = id;            // store clock ID
        //clock_obj->vtab =;           // useless in C
        clock_obj->unk_10 = arg1;      // 
        clock_obj->unk_0C = 0;         // 
        
        for(i = 0; i < 3; i++)
            if (set[i].unk_00 == arg1) {
                clock_obj->unk_0C   = set[i].unk_04;
                clock_obj->offset_0 = set[i].offset_0;
                clock_obj->offset_1 = set[i].offset_1;
                
                return;
            }
    }
    
  printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
  return;
}
