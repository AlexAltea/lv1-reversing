/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "clock.h"

#include "lv1/driver/rsx/assert.h"
#include "lv1/driver/rsx/mmio.h"

// RSX device clock settings
static rsx_dev_clock_setting_t set[3] = {
    {1, 2, 0x4028, 0x402C, 0},
    {6, 3, 0x4010, 0x4014, 0},
    {9, 3,      0,      0, 0}
};



/***********************************************************************
* 
***********************************************************************/
void rsx_device_clock_t::sub21F0C0() {
    RSX_ASSERT(id == 1 || id == 5);
    S32 value1 = rsx_rd32(offset_0);
    S32 value2 = rsx_rd32(offset_1);
        
    // if value_1[22:22] is set
    if ((value1 & 0x400) >>10) {
        S32 t1 = (value2 & 0xFF00) >> 8;     // value_2[16:23], clock 1(2),   clock 5(4)
        S32 t2 = (value2 & 0xFF);            // value_2[24:31], clock 1(0xA), clock 5(0x1A)
        S32 t3 = (value1 & 0x70000) >> 16;   // value_1[13:15], 0
            
        unk_20 = t2;          // 
        unk_24 = t1;          //     
        freq = (S32)(((t1 * 0x5F5E100) / t2) / (1 << t3));
    }
    else {
        // Set default frequency: 100 MHz
        freq = 0x5F5E100;
    }
}

/***********************************************************************
* 
***********************************************************************/
S32 rsx_device_clock_t::get_frequency() {
    sub21F0C0();
    return freq;
}
 
/***********************************************************************
* 
***********************************************************************/
void rsx_device_clock_rsx_create(rsx_device_clock_t* clock_obj, S32 id, S32 arg1) {
    S32 i;
    
    
    if (g_rsx_clock_flag != 0) {
        clock_obj->id = id;            // store clock ID
        //clock_obj->vtab =;           // useless in C
        clock_obj->unk_10 = arg1;      // 
        clock_obj->unk_0C = 0;         // 
        
        for (i = 0; i < 3; i++)
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
