/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"





S32 rsx_device_clock_rsx_get_frequency(rsx_dev_clock_obj_t* clock_obj);
void rsx_device_clock_rsx_create(rsx_dev_clock_obj_t* clock_obj, S32 id, S32 arg1);



#endif // __RSX_DEVICE_CLOCK_H__
