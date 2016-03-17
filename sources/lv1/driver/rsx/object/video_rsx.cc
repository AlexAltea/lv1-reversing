/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>





/***********************************************************************
* interrupt callback ? opd(stru_32C870)
***********************************************************************/
void rsx_object_video_rsx_2245F4(rsx_obj_video_rsx_t* video_rsx) {
    
    // TODO:
    
    return;
}


/***********************************************************************
* 
***********************************************************************/
S32 rsx_object_video_rsx_223230(rsx_obj_video_rsx_t* video_rsx, S32 id, S32 arg2) {
    S32 offset, value;
    
    
    if (id <= 1)  // valid id, 0 or 1 {
        // ?
        offset = (rsx_object_video_rsx_2231BC(video_rsx, id)) <<13;
        
        // ?
        video_rsx->unk_78[id] = 0;
        video_rsx->unk_7C[id] = 0;
        video_rsx->unk_84[id] = 1;
        
        // set BAR0 registers
        value = read_BAR0(offset + 0x28000600810);
        value |= 1;                                 // set value[31:31]
        write_BAR0(value, offset + 0x28000600810);
        
        value = read_BAR0(offset + 0x28000600810);
        value |= 2;                                 // set value[30:30]
        write_BAR0(value, offset + 0x28000600810);
        
        value = read_BAR0(offset + 0x28000600810);
        value |= 4;                                 // set value[29:29]
        write_BAR0(value, offset + 0x28000600810);
        
        value = read_BAR0(offset + 0x28000600810);
        value &= 0xFFFFFFEF;                        // unset value[27:27]
        write_BAR0(value, offset + 0x28000600810);
        
        value = read_BAR0(offset + 0x28000600810);
        value &= 0xFFFFDFFF;                        // unset value[18:18]
        write_BAR0(value, offset + 0x28000600810);
        
        value = read_BAR0(offset + 0x28000600810);
        value &= 0xFFFFBFFF;                        // unset value[17:17]
        write_BAR0(value, offset + 0x28000600810);
        
        value = read_BAR0(offset + 0x28000600810);
        value |= 0x10000;                           // set value[15:15]
        write_BAR0(value, offset + 0x28000600810);
        
        value = read_BAR0(offset + 0x28000600810);
        value &= 0xF00FFFFF;                        // unset value[04:11]
        value |= 0x4000000;                         // set value[05:05]
        write_BAR0(value, offset + 0x28000600810);
        
        value = read_BAR0(offset + 0x28000600810);
        value |= 0x10000000;                        // set value[03:03]
        write_BAR0(value, offset + 0x28000600810);
        
        write_BAR0(arg2, offset + 0x2800060080C);   
        
        value = video_rsx->unk_84[id];
        write_BAR0(value, offset + 0x28000680300);
        
        return LV1_SUCCESS;
    }
    
    return LV1_ILLEGAL_PARAMETER_VALUE;
}


/***********************************************************************
* 
***********************************************************************/
void rsx_object_video_rsx_222F2C(rsx_obj_video_rsx_t* video_rsx, S32 arg1) {
    S32 value, offset = 0;
    
    
    if (arg1 == 0) {
        value = read_BAR0(0x2800008C004);
        value &= 0xFFFFFFE0;                        // unset value[27:31]
        value |= 0x9;                               // set value[28:28] and value[31:31]
        write_BAR0(value, 0x2800008C004);
        
        value = read_BAR0(0x2800008C004);
        value &= 0xFFF0FFFF;                        // unset value[12:15]
        value |= 0x50000;                           // set value[13:13] and value[15:15]
        write_BAR0(value, 0x2800008C004);
    }
    
    if (arg1 == 1) {
        value = read_BAR0(0x2800008C014);
        value &= 0xFFFFFFE0;                        // unset value[27:31]
        value |= 0xD;                               // set value[28:29] and value[31:31]
        write_BAR0(value, 0x2800008C014);
        
        value = read_BAR0(0x2800008C014);
        value &= 0xFFF0FFFF;                        // unset value[12:15]
        value |= 0x70000;                           // set value[13:15]
        write_BAR0(value, 0x2800008C014);
    }
    
    if (arg1 == 2) {
        value = read_BAR0(0x2800008C024);
        value &= 0xFFFFFFE0;                        // unset value[27:31]
        value |= 9;                                 // set value[28:28] and value[31:31]
        write_BAR0(value, 0x2800008C024);
        
        value = read_BAR0(0x2800008C024);
        value &= 0xFFF0FFFF;                        // unset value[12:15]
        value |= 0x50000;                           // set value[13:13] and value[15:15]
        write_BAR0(value, 0x2800008C024);
    }
    
    if (arg1 == 3) {
        value = read_BAR0(0x2800008C024);
        value &= 0xFFFFFFE0;                        // unset value[27:31]
        value |= 0xD;                               // set value[28:28] and value[31:31]
        write_BAR0(value, 0x2800008C024);
        
        value = read_BAR0(0x2800008C024);
        value &= 0xFFF0FFFF;                        // unset value[12:15]
        value |= 0x50000;                           // set value[28:29] and value[31:31]
        write_BAR0(value, 0x2800008C024);
    }
    
    offset = arg1 * 0x10;
    
    value = read_BAR0(offset + 0x2800008C004);
  value &= 0x7FFFFFFF;                          // unset value[00:00]
  write_BAR0(value, offset + 0x2800008C004);
    
    value = read_BAR0(offset + 0x2800008C004);
  value &= 0xCFFFFFFF;                          // unset value[02:02] and value[03:03]
  value |= 0x10000000;                          // set value[03:03]
  write_BAR0(value, offset + 0x2800008C004);
    
    value = read_BAR0(offset + 0x2800008C004);
  value &= 0xFEFFFFFF;                          // unset value[07:07]
  write_BAR0(value, offset + 0x2800008C004);
    
    value = read_BAR0(offset + 0x2800008C004);
  value |= 0x800000;                            // set value[08:08]
  write_BAR0(value, offset + 0x2800008C004);
  
  value = read_BAR0(offset + 0x2800008C004);
  value &= 0xFFFFF8FF;                          // unset value[21:23]
  write_BAR0(value, offset + 0x2800008C004);
  
  value = read_BAR0(offset + 0x2800008C008);
    value = 2;                                    // set value[30:30], old value lost
  write_BAR0(value, offset + 0x2800008C008);
  
  value = read_BAR0(offset + 0x2800008C00C);
  value = 0xA;                                  // set value[28:28] and value[31:31], old value lost
  write_BAR0(value, offset + 0x2800008C00C);
  
    return;
}


/***********************************************************************
* 
***********************************************************************/
S32 rsx_object_video_rsx_2231BC(rsx_obj_video_rsx_t* video_rsx, S32 idx) {
    if (idx > 3) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return 0;
    }
    
    return ((idx & 0xFFFFFFFE) | video_rsx->unk_2C[idx]);
}


/***********************************************************************
* 
***********************************************************************/
S32 rsx_object_video_rsx_223510(rsx_obj_video_rsx_t* video_rsx, S32 id, S32 arg2) {
    S32 i, value1, value2, offset;
    
    
    if ((id > 1) || ((arg2 - 1) > 3))
      return LV1_ILLEGAL_PARAMETER_VALUE;
    
    // ?
    video_rsx->unk_2C[id] = arg2;
  
  
  // ?
    offset = (rsx_object_video_rsx_2231BC(video_rsx, id)) <<4;
    value1 = read_BAR0(offset + 0x2800008C000);
    value1 |= 1;                                      // set value[31:31]
    write_BAR0(value1, offset + 0x2800008C000);
    
    
    // ?
    offset = (rsx_object_video_rsx_2231BC(video_rsx, id)) <<13;
    value1 = read_BAR0(offset + 0x28000680804);
    value1 &= 0xFFFF;                                 // unset value[00:15]
    
    offset = (rsx_object_video_rsx_2231BC(video_rsx, id)) <<13;
    value2 = read_BAR0(offset + 0x2800068080C);
    value2 &= 0xFFFF;                                 // unset value[00:15]
    
    // TODO:
    // complicated clock frequency calculations in conjunction with 2
    // unknown BAR0 register values(value1 and value2), impossible!
    // I set the right result values immediate.
    
    video_rsx->unk_A0[0] = 0;
    video_rsx->unk_B0[0] = 0xE343;
    
    // 
    for(i = 0; i < 2; i++) {
      if ((video_rsx->unk_52 == 0) && (i != id))
        continue;
      
      if (arg2 == 2) {
            offset = (rsx_object_video_rsx_2231BC(video_rsx, i)) <<13;
            value1 = read_BAR0(offset + 0x28000600804);
            value1 &= 0xFFFFFFFC;          // unset value1[30:30] and value[31:31]
            value1 |= 1;                   // set value1[31:31]
            write_BAR0(value1, offset + 0x28000600804);
        }
      else {
            offset = (rsx_object_video_rsx_2231BC(video_rsx, i)) <<13;
            value1 = read_BAR0(offset + 0x28000600804);
            value1 &= 0xFFFFFFFC;          // unset value1[30:30] and value[31:31]
            value1 |= 1;                   // set value1[30:30]
            write_BAR0(value1, offset + 0x28000600804);
        }
    }
    
    return LV1_SUCCESS;
}


/***********************************************************************
* 
***********************************************************************/
void rsx_object_video_rsx_init(rsx_obj_video_rsx_t* video_rsx) {
    S32 i, value, offset = 0;
    U64 rsx_debug = 0;
    lv1_unk_obj_00_t* video_rsx_unk_0 = NULL;
    lv1_unk_obj_00_t* video_rsx_unk_1 = NULL;
    
    
    video_rsx->unk_04 = -1;       // ?
    video_rsx->unk_08 = -1;       // ?
    video_rsx->unk_0C = 0;        // ?
    video_rsx->unk_10 = 0;        // ?
    video_rsx->unk_14 = -1;       // ?
    video_rsx->unk_18 = -1;       // ?
    video_rsx->unk_24[0] = 2;     // ?
  video_rsx->unk_24[1] = 2;     // ?
  video_rsx->unk_2C[0] = 0;     // ?
    video_rsx->unk_2C[1] = 1;     // ?
    video_rsx->unk_50 = 0;        // ?
  video_rsx->unk_51 = 0;        // ?
  video_rsx->unk_52 = 0;        // ?
  video_rsx->unk_58 = 0;        // ?
    video_rsx->unk_5C = 2;        // ?
    
    
    // 2 things of someone(0 and 1), channels?  TODO:
    // ?
    rsx_object_video_rsx_223510(video_rsx, 0, 2);
    
    // ?
    rsx_object_video_rsx_223510(video_rsx, 1, 2);
    
    
    
    // set BAR0 registers
    for(i = 0; i < 6; i++) {
        value = read_BAR0(offset + 0x2800008C004);
        value |= 0x80000000;                        // set value[00:00]
        write_BAR0(value, offset + 0x2800008C004);
        
        value = read_BAR0(offset + 0x2800008C004);
        value &= 0xCFFFFFFF;                        // unset value[02:02] and value[03:03]
        write_BAR0(value, offset + 0x2800008C004);
        
        value = read_BAR0(offset + 0x2800008C004);
        value |= 0x1000000;                         // set value[07:07]
        write_BAR0(value, offset + 0x2800008C004);
        
        value = read_BAR0(offset + 0x2800008C004);
        value &= 0xFFF0FFFF;                        // unset value[12:15]
        write_BAR0(value, offset + 0x2800008C004);
        
        value = read_BAR0(offset + 0x2800008C004);
        value &= 0x1FFFFFFF;                        // unset value[21:23]
        write_BAR0(value, offset + 0x2800008C004);
        
        value = read_BAR0(offset + 0x2800008C008);
        value = 1;                                  // set value[31:31], old value lost
        write_BAR0(value, offset + 0x2800008C008);
        
        offset+=0x10;
    }
    
    
    
    // 2 things of someone(0 and 1), channels? displaybuffers?: setup BAR0 registers
    //
    rsx_object_video_rsx_222F2C(video_rsx, 0);
    
    //
    rsx_object_video_rsx_222F2C(video_rsx, 1);
  
  
  
  // 2 times, for the 2 things: setup BAR0 registers, init video rsx object unk_38 and unk_40 with 0
  offset = 0x40;
  
  for(i = 0; i < 2; i++) {
        if (i == 0) {
            value = read_BAR0(0x2800008C044);
        value &= 0xFFFFFFC0;                    // unset value[26:31]
        value |= 0xA;                           // set value[28:28] and value[30:30]
        write_BAR0(value, 0x2800008C044);
        }
        
        if (i == 1) {
            value = read_BAR0(0x2800008C044);
        value &= 0xFFFFFFC0;                    // unset value[26:31]
        value |= 0xE;                           // set value[28:30]
        write_BAR0(value, 0x2800008C044);
        }
        
        value = read_BAR0(offset + 0x2800008C008);
        value = 0xFFFFFFFF;                       // set value[00:31]
        write_BAR0(value, offset + 0x2800008C008);
        
        value = read_BAR0(offset + 0x2800008C00C);
        value = 0;                                // unset value[00:31]
        write_BAR0(value, offset + 0x2800008C00C);
        
        value = read_BAR0(offset + 0x2800008C000);
        value |= 1;                               // set value[31:31]
        write_BAR0(value, offset + 0x2800008C000);
        
        offset+=0x10;
    }
  
  // set some rsx object values
  video_rsx->unk_48 = 0x20200;
  video_rsx->unk_4C = 0x20200;
  
  // set some BAR0 register flags
  value = read_BAR0(0x2800008C140);
  value |= 1;                          // set value[31:31]
  write_BAR0(value, 0x2800008C140);
  
  value = read_BAR0(0x2800008C140);
  value |= 0x10;                       // set value[27:27]
  write_BAR0(value, 0x2800008C140);
  
  value = read_BAR0(0x2800008C140);
  value |= 0x100;                      // set value[23:23]
  write_BAR0(value, 0x2800008C140);
  
  value = read_BAR0(0x2800008C140);
  value |= 0x1000;                     // set value[19:19]
  write_BAR0(value, 0x2800008C140);
  
  value = read_BAR0(0x2800008C140);
  value &= 0xFFFEFFFF;                 // unset value[15:15]
  write_BAR0(value, 0x2800008C140);
  
  value = read_BAR0(0x2800008C140);
  value &= 0xFFF7FFFF;                 // unset value[12:12]
  write_BAR0(value, 0x2800008C140);
  
  value = read_BAR0(0x280004000A4);
  value &= 0x7FFFFFFF;                 // unset value[00:00]
  write_BAR0(value, 0x280004000A4);
  
  // set some rsx object values
  video_rsx->unk_53 = 0;
  video_rsx->unk_54 = 0;
  
  // get RSX LV1 debug level
    get_param_value("sys.lv1.rsxdebug", &rsx_debug);
  video_rsx->unk_00 = (S8)(rsx_debug & 1);
  
  
  // allocate, init and store unk LV1 object 0 and 1 ? 
  video_rsx_unk_0 = lv1_kmalloc(sizeof(lv1_unk_obj_00_t));
  if (video_rsx_unk_0 != NULL) {
        lv1_sub_2CA414(video_rsx_unk_0,    // (IN) unk obj
                       0,                  // (IN) 0 ?
                       0/*stru_32C870*/,   // (IN) opd -> rsx_object_video_rsx_2245F4()
                       video_rsx);         // (IN) RSX object video rsx
    }
    
    video_rsx->unk_90 = (void*)video_rsx_unk_0;
    
  video_rsx_unk_1 = lv1_kmalloc(sizeof(lv1_unk_obj_00_t));
  if (video_rsx_unk_1 != NULL) {
        lv1_sub_2CA414(video_rsx_unk_1,    // (IN) unk obj
                       0,                  // (IN) 0 ?
                       0/*stru_32C870*/,   // (IN) opd -> rsx_object_video_rsx_2245F4()
                       video_rsx);         // (IN) RSX object video rsx
    }
    
  video_rsx->unk_98 = (void*)video_rsx_unk_0;
  
  // check unk LV1 object 0
  if (video_rsx->unk_90 == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return;
    }
  
  // mtocrf  cr0, r31(video_rsx_unk_0)
  
  // ? again, 2 things...
  for(i = 0; i < 2; i++)
        rsx_object_video_rsx_223230(video_rsx, i, 0);
    
    // ? again, 2 things...
    for(i = 0; i < 2; i++) {
      offset = (rsx_object_video_rsx_2231BC(video_rsx, i)) <<13;
      
      value = read_BAR0(offset + 0x28000680404);
      value &= 0x7FFFFFFF;                        // unset value[00:00]
      write_BAR0(value, offset + 0x28000680404);
    }
    
    return;
}
