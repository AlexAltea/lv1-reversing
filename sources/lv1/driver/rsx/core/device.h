/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#ifndef __RSX_CORE_DEVICE_H__
#define __RSX_CORE_DEVICE_H__


S32 rsx_core_device_map_device(rsx_dev_core_obj_t* core, S32 device_id, S64 *dev_lpar_addr, S64 *dev_lpar_size);
void rsx_core_device_finalize(rsx_dev_core_obj_t* core);
S32 rsx_core_device_close_device(S32 dev_core_id);

rsx_dev_core_obj_t* rsx_core_device_get_core_object(void);
S32 rsx_core_device_get_rsx_enable_flag(void);
rsx_dev_core_obj_t* rsx_core_device_get_core_object_by_id(S32 dev_core_id);

S32 rsx_core_device_open(S32 arg1, S32 dev_core_id);




#endif // __RSX_CORE_DEVICE_H__
