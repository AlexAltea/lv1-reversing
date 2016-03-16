/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

S32 rsx_object_nv_class_get_object_size(void);
S64 rsx_object_nv_class_get_BAR2_address(rsx_nv_class_obj_t* nv_obj);
U32 rsx_object_nv_class_get_object_class(rsx_nv_class_obj_t* nv_obj);
void rsx_object_nv_class_create_object(rsx_nv_class_obj_t* nv_obj, U32 arg1, U32 arg2);



#endif // __RSX_OBJECT_NV_CLASS_H__
