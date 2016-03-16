/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#ifndef __RSX_DEVICE_FIFO_H__
#define __RSX_DEVICE_FIFO_H__


//--- init ---------------------------------------------------------------------------
void rsx_device_fifo_21B5FC(rsx_fifo_obj_t* fifo, S32 arg1, S32 arg2);
void rsx_device_fifo_21BA08(rsx_fifo_obj_t* fifo);
void rsx_device_fifo_init(rsx_fifo_obj_t* fifo);
//------------------------------------------------------------------------------------




S64 rsx_device_fifo_get_channel_dma_control_lpar_address(rsx_fifo_obj_t* fifo, S32 channel_id);
void rsx_device_fifo_21B668(rsx_fifo_obj_t* fifo, S32 channel_id, S32 arg1);
void rsx_device_fifo_21C234(rsx_fifo_obj_t* fifo, S32 channel_id);
void rsx_device_fifo_21CCC0(rsx_fifo_obj_t* fifo, S32 channel_id, S32 offset);
S64 rsx_device_fifo_21B7DC(rsx_fifo_obj_t* fifo, S32 idx);
S64 rsx_device_fifo_get_fc2_channel_address_by_id(rsx_fifo_obj_t* fifo, S32 channel_id);
S64 rsx_device_fifo_get_fc1_channel_address_by_id(rsx_fifo_obj_t* fifo, S32 channel_id);
S32 rsx_device_fifo_pause(rsx_fifo_obj_t* fifo);
void rsx_device_fifo_continue(rsx_fifo_obj_t* fifo);


#endif // __RSX_DEVICE_FIFO_H__
