/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>




/***********************************************************************
* 
***********************************************************************/
const char *rsx_bus_ioif0_get_class_name_string() {
    return "rsx ioif0 bus";
}

/***********************************************************************
* 
***********************************************************************/
S32 rsx_bus_ioif0_get_init_flag(rsx_bus_ioif0_obj_t* ioif0) {
    return ioif0->init;
}

/***********************************************************************
* 
***********************************************************************/
S32 rsx_bus_ioif0_get_chip_revision(rsx_bus_ioif0_obj_t* ioif0) {
    return ioif0->chip_revision;
}

/***********************************************************************
* 
***********************************************************************/
S64 rsx_bus_ioif0_get_BAR0_addr(rsx_bus_ioif0_obj_t* ioif0) {
    return ioif0->bar_0_addr;
}

/***********************************************************************
* 
***********************************************************************/
S32 rsx_bus_ioif0_get_BAR0_size(rsx_bus_ioif0_obj_t* ioif0) {
    return ioif0->bar_0_size;
}

/***********************************************************************
* 
***********************************************************************/
S64 rsx_bus_ioif0_get_BAR1_addr(rsx_bus_ioif0_obj_t* ioif0) {
    return ioif0->bar_1_addr;
}

/***********************************************************************
* 
***********************************************************************/
S32 rsx_bus_ioif0_get_BAR1_size(rsx_bus_ioif0_obj_t* ioif0) {
    return ioif0->bar_1_2_size;
}

/***********************************************************************
* 
***********************************************************************/
S64 rsx_bus_ioif0_get_BAR2_addr(rsx_bus_ioif0_obj_t* ioif0) {
    return ioif0->bar_2_addr;
}

/***********************************************************************
* 
***********************************************************************/
S32 rsx_bus_ioif0_get_unk_38(rsx_bus_ioif0_obj_t* ioif0) {
    return ioif0->unk_38;
}

/***********************************************************************
* 
***********************************************************************/
void rsx_bus_ioif0_write_BAR0_by_offset(rsx_bus_ioif0_obj_t* ioif0, S32 offset, S32 value) {
    write_BAR0(value, ioif0->bar_0_addr + offset);
    return;
}

/***********************************************************************
* 
***********************************************************************/
S32 rsx_bus_ioif0_read_BAR0_by_offset(rsx_bus_ioif0_obj_t* ioif0, S64 offset) {
    return read_BAR0(ioif0->bar_0_addr + offset);
}





/***********************************************************************
* 
***********************************************************************/
void rsx_bus_ioif0_217814(rsx_bus_ioif0_obj_t* ioif0) {
    
    U64 value1 = 0x94A1510, value2 = 0xEE6B28;
    S32  value;
    
    /* bulshit
    
    for(i = 0; i < 3; i++) {
        value1 >>= 1;
        value2 >>= 1;
        
        if ((value1 & 1) != 0)
          break;
    }
    printf("(0x%08X)\n", value1);
    
    
    
    
    U64 r6, r9, r11;
    U64 r0 = 0x66666667;
    U64 r10 = r0;
    
    
    
    r6 = value1 >>31;
    r0 = value1 * r0;
    r0 = r0 >>1;
    r0 = r0 - r6;
    r9 = r9 + r0;
    
    
    printf("(0x%016llX) (0x%016llX)\n", r9, value1);
    */
  
  
  
  
  
  
  
  
  
  
  
  
  // store the result of the shit above in BAR0
  write_BAR0(value1, 0x28000009200);
  write_BAR0(value2, 0x28000009210);
  
  
  value = read_BAR0(0x28000088140);
    value |= 0x80000000;                  // set value[00:00]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x40000000;                  // set value[01:01]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x20000000;                  // set value[02:02]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x10000000;                  // set value[03:03]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x8000000;                   // set value[04:04]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x4000000;                   // set value[05:05]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x2000000;                   // set value[06:06]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x1000000;                   // set value[07:07]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x800000;                    // set value[08:08]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x400000;                    // set value[09:09]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x200000;                    // set value[10:10]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x100000;                    // set value[11:11]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x80000;                     // set value[12:12]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x20000;                     // set value[14:14]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x10000;                     // set value[15:15]
  write_BAR0(value, 0x28000088140);
  
  value = read_BAR0(0x28000088140);
    value |= 0x8000;                      // set value[16:16]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x4000;                      // set value[17:17]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x2000;                      // set value[18:18]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x1000;                      // set value[19:19]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x800;                       // set value[20:20]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x400;                       // set value[21:21]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x200;                       // set value[22:22]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x100;                       // set value[23:23]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x80;                        // set value[24:24]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x40;                        // set value[25:25]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 0x20;                        // set value[26:26]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 8;                           // set value[28:28]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 4;                           // set value[29:29]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 2;                           // set value[30:30]
    write_BAR0(value, 0x28000088140);
    
    value = read_BAR0(0x28000088140);
    value |= 1;                           // set value[31:31]
  write_BAR0(value, 0x28000088140);
  
    return;
}



/***********************************************************************
* 
***********************************************************************/
void rsx_bus_ioif0_219018(rsx_bus_ioif0_obj_t* ioif0) {
    S32 i, value;
    S64 addr = 0;
    
    
    // set some flags in BAR0
    value = read_BAR0(0x28000088208);
    value |= 2;                            // set value[30:30]
    write_BAR0(value, 0x28000088208);
    
    value = read_BAR0(0x28000088208);
    value |= 1;                            // set value[31:31]
    write_BAR0(value, 0x28000088208);
    
    write_BAR0(0, 0x2800008820C);          // set value to 0
    
    addr = 0x28000088280;
    write_BAR0(1, addr);                   // set value[31:31]
    
    for(i = 1; i < 32; i++) {
        if (i < 23) {
            if (((1 << i) & 0x80) != 0) {
        write_BAR0(0x301F001, addr + (i * 4));
            continue;
          }
      if (((1 << i) & 0xF64A00) != 0) {
              write_BAR0(0x300F001, addr + (i * 4));
            continue;
          }
        }
        write_BAR0(1, addr + (i * 4));
    }
    
    rsx_bus_ioif0_217814(ioif0);
    
    value = read_BAR0(0x280000884C0);
    value |= 0x10;                         // set value[27:27]
    write_BAR0(value, 0x280000884C0);
    
    value = read_BAR0(0x280000884C0);
    value &= 0xFFFFFF1F;                   // unset value[24:26]
    write_BAR0(value, 0x280000884C0);
    
    value = read_BAR0(0x28000088208);
    value &= 0xFFFFF8FF;                   // unset value[21:23]
    value |= 0x100;                        // set value[23:23]
    write_BAR0(value, 0x28000088208);
  
  value = read_BAR0(0x28000088208);
    value &= 0xFFFF8FFF;                   // unset value[17:19]
    value |= 0x1000;                       // set value[19:19]
    write_BAR0(value, 0x28000088208);
  
  value = read_BAR0(0x28000088304);
    value |= 0x20000000;                   // set value[02:02]
    write_BAR0(value, 0x28000088304);
  
  value = read_BAR0(0x28000088304);
    value &= 1;                            // unset value[01:01]
    write_BAR0(value, 0x28000088304);
  
    return;
}












/***********************************************************************
* 
***********************************************************************/
void rsx_bus_ioif0_set_chip_revision(rsx_bus_ioif0_obj_t* ioif0) {
    //S32 value1, value2;
    
    /*

    // read a value from 0x28000000004
    value1 = rsx_bus_ioif0_read_BAR0_by_offset(ioif0, 4);
    if (value1 != 0) {
        printf("rsx already booted\n");
        //...
        
    }
    else {
        value2 = rsx_bus_ioif0_read_BAR0_by_offset(ioif0, 0x88400);
        
        rsx_bus_ioif0_write_BAR0_by_offset(ioif0, 4, 0x1000001);
        
        value1 = rsx_bus_ioif0_read_BAR0_by_offset(ioif0, 4);
        if ((value1 ^ 0x1000000) != 1) {
          printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
          return;
      }
        
        
    }
    
    */
  
  ioif0->chip_revision = 0x5C;
  
  return;    
}




/***********************************************************************
* 
***********************************************************************/
rsx_bus_ioif0_obj_t* rsx_bus_ioif0_init(void) {
    rsx_bus_ioif0_obj_t* ioif0 = NULL;
    
    
    // check whether global RSX IOIF0 object already exists
    if (g_rsx_ioif_obj != NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return NULL;
    }
    
    // allocate RSX IO interface 0 object
    ioif0 = lv1_kmalloc(sizeof(rsx_bus_ioif0_obj_t));
    if (ioif0 == NULL) {
        printf("rsx driver assert failed. [%s : %04d : %s()]\n", __FILE__, __LINE__, __func__);
        return NULL;
    }
    
    // set members
    //ioif0->vtab = ;                      // useless in C
    ioif0->bar_1_addr   = 0x28080000000;   // BAR1(VRAM) start address
    ioif0->bar_2_addr   = 0x28002000000;   // BAR2(RAMIN) start address
    ioif0->bar_0_size   = 0x2000000;       // BAR0 size, (32 MB)
    ioif0->bar_1_2_size = 0x10000000;      // BAR1 and BAR2 size, (256 MB)
    ioif0->init         = 0;               // init flag unset
    ioif0->base_addr    = 0x28000000000;   
    ioif0->bar_0_addr   = 0x28000000000;   // BAR0 start address
    
    // store global RSX IOIF0 object
    g_rsx_ioif_obj = (void*)ioif0;
    
    // invalidate a value in BAR0, init/reset ?
    write_BAR0(0xFFFFFFFF, 0x28000088100);
    
  // get RSX chip revision
    rsx_bus_ioif0_set_chip_revision(ioif0);
    
    return ioif0;
}



