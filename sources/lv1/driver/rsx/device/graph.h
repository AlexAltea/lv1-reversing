/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"


//--- init -------------------------------------------------------------
void rsx_device_graph_21E9CC(S64 *vtab);
void rsx_device_graph_21D2DC(rsx_graph_obj_t* graph);
void rsx_device_graph_21D350(rsx_graph_obj_t* graph);
S32 rsx_device_graph_21EF60(S64 *vtab);
void rsx_device_graph_21ED38(S64 *vtab, S32 offset, S32 arg1, S32 arg2);
void rsx_device_graph_21DA04(rsx_graph_obj_t* graph, S32 idx);
S32 rsx_device_graph_21E950(void);
S32 rsx_device_graph_21D6C0(void);
void rsx_device_graph_21E95C(S64 *vtab, S32 *arg1, S32 *arg2, S32 *arg3, S32 *arg4, S32 *arg5);
void rsx_device_graph_21DD9C(rsx_graph_obj_t* graph);
void rsx_device_graph_21E93C(S64 *vtab, S64 *arg1, S32 *arg2);
void rsx_device_graph_21D7A0(rsx_graph_obj_t* graph);
void rsx_device_graph_init(rsx_graph_obj_t* graph);
//----------------------------------------------------------------------



S64 rsx_device_graph_get_graph_channel_address_by_id(rsx_graph_obj_t* graph, S32 channel_id);
S32 rsx_device_graph_21D224(rsx_graph_obj_t* graph, S32 arg1);
void rsx_device_graph_21D054(rsx_graph_obj_t* graph);
void rsx_device_graph_21D038(rsx_graph_obj_t* graph);
void rsx_device_graph_21D01C(rsx_graph_obj_t* graph);


#endif // __RSX_DEVICE_GRAPH_H__
