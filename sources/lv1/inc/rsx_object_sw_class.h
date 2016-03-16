#ifndef __RSX_OBJECT_SW_CLASS_H__
#define __RSX_OBJECT_SW_CLASS_H__



uint32_t rsx_object_sw_class_get_object_class(rsx_sw_class_obj_t *sw_obj);
int64_t rsx_object_sw_class_get_BAR2_address(rsx_sw_class_obj_t *sw_obj);
void rsx_object_sw_class_create_object(rsx_sw_class_obj_t *sw_obj, uint32_t type, int64_t *opd);


#endif // __RSX_OBJECT_SW_CLASS_H__
