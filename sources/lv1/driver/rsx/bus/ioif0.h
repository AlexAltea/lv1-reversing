/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#ifndef __RSX_BUS_IOIF0_H__
#define __RSX_BUS_IOIF0_H__


const char *rsx_bus_ioif0_get_class_name_string(void);
S32 rsx_bus_ioif0_get_init_flag(rsx_bus_ioif0_obj_t* ioif0);
S32 rsx_bus_ioif0_get_chip_revision(rsx_bus_ioif0_obj_t* ioif0);
S64 rsx_bus_ioif0_get_BAR0_addr(rsx_bus_ioif0_obj_t* ioif0);
S32 rsx_bus_ioif0_get_BAR0_size(rsx_bus_ioif0_obj_t* ioif0);
S64 rsx_bus_ioif0_get_BAR1_addr(rsx_bus_ioif0_obj_t* ioif0);
S32 rsx_bus_ioif0_get_BAR1_size(rsx_bus_ioif0_obj_t* ioif0);
S64 rsx_bus_ioif0_get_BAR2_addr(rsx_bus_ioif0_obj_t* ioif0);
S32 rsx_bus_ioif0_get_unk_38(rsx_bus_ioif0_obj_t* ioif0);
void rsx_bus_ioif0_write_BAR0_by_offset(rsx_bus_ioif0_obj_t* ioif0, S32 offset, S32 value);
S32 rsx_bus_ioif0_read_BAR0_by_offset(rsx_bus_ioif0_obj_t* ioif0, S64 offset);


rsx_bus_ioif0_obj_t* rsx_bus_ioif0_init(void);
void rsx_bus_ioif0_set_chip_revision(rsx_bus_ioif0_obj_t* ioif0);
void rsx_bus_ioif0_219018(rsx_bus_ioif0_obj_t* ioif0);
void rsx_bus_ioif0_217814(rsx_bus_ioif0_obj_t* ioif0);



#endif // __RSX_BUS_IOIF0_H__
