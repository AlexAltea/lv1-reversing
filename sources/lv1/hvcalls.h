/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "common/types.h"

// HyperCalls

S32 lv1_allocate_memory(U64 size, U64 log_page_size, U64 zero, U64 flags, U64* base_addr, U64* muid);
S32 lv1_write_htab_entry(U64 vas_id, U64 slot, U64 pte_hi, U64 pte_lo);
S32 lv1_construct_virtual_address_space(U64 log_pteg_count, U64 n_sizes, U64 page_sizes, U64* vas_id, U64* hv_pteg_count);
S32 lv1_get_virtual_address_space_id_of_ppe(U64 ppe_id, U64* vas_id);
S32 lv1_query_logical_partition_address_region_info(U64 lpar_id, U64* base_addr, U64* size, U64* access_right, U64* max_page_size, U64* flags);
S32 lv1_select_virtual_address_space(U64 vas_id);
S32 lv1_pause(U64 mode);
S32 lv1_destruct_virtual_address_space(U64 vas_id);
S32 lv1_configure_irq_state_bitmap(U64 ppe_id, U64 cpu_id, U64 bitmap_addr);
S32 lv1_connect_irq_plug_ext(U64 ppe_id, U64 cpu_id, U64 virq, U64 outlet, U64 zero);
S32 lv1_release_memory(U64 base_addr);

/**
 * LV1 SysCall 15 (0xF): lv1_put_iopte
 * @param[in]   ioas_id       R3  IO address space ID
 * @param[in]   ioif_addr     R4  IO interface address
 * @param[in]   lpar_addr     R5  LPAR address
 * @param[in]   io_id         R6  IO ID
 * @param[in]   flags         R7  Flags
 * @return                    R3  Status code
 */
S32 lv1_put_iopte(U64 ioas_id, U64 ioif_addr, U64 lpar_addr, U64 io_id, U64 flags);

S32 lv1_disconnect_irq_plug_ext(U64 ppe_id, U64 cpu_id, U64 virq);
S32 lv1_construct_event_receive_port(U64* outlet);
S32 lv1_destruct_event_receive_port(U64 outlet);
S32 lv1_send_event_locally(U64 outlet);
S32 lv1_end_of_interrupt(U64 irq);
S32 lv1_connect_irq_plug(U64 virq, U64 irq);
S32 lv1_disconnect_irq_plus(U64 virq);
S32 lv1_end_of_interrupt_ext(U64 ppe_id, U64 cpu_id, U64 virq);
S32 lv1_did_update_interrupt_mask(U64 ppe_id, U64 cpu_id);
S32 lv1_shutdown_logical_partition(U64 cmd);
S32 lv1_destruct_logical_spe(U64 spe_id);
S32 lv1_construct_logical_spe(U64 pshift1, U64 pshift2, U64 pshift3, U64 pshift4, U64 pshift5, U64 vas_id, U64 spe_type, U64* priv2_addr, U64* problem_phys, U64* local_store_phys, U64* unused, U64* shadow_addr, U64* spe_id);
S32 lv1_set_spe_interrupt_mask(U64 spe_id, U64 class_, U64 mask);
S32 lv1_disable_logical_spe(U64 spe_id, U64 zero);
S32 lv1_clear_spe_interrupt_status(U64 spe_id, U64 class_, U64 stat, U64 zero);
S32 lv1_get_spe_interrupt_status(U64 spe_id, U64 class_, U64* stat);
S32 lv1_get_logical_ppe_id(U64* ppe_id);
S32 lv1_get_logical_partition_id(U64* lpar_id);
S32 lv1_get_spe_irq_outlet(U64 spe_id, U64 class_, U64* outlet);
S32 lv1_set_spe_privilege_state_area_1_register(U64 spe_id, U64 offset, U64 value);
S32 lv1_get_repository_node_value(U64 lpar_id, U64 n1, U64 n2, U64 n3, U64 n4, U64* v1, U64* v2);
S32 lv1_read_htab_entries(U64 vas_id, U64 slot, U64* hi1, U64* hi2, U64* hi3, U64* hi4, U64* rcbits);
S32 lv1_set_dabr(U64 dabr, U64 flags);
S32 lv1_allocate_io_segment(U64 ioas_id, U64 seg_size, U64 io_pagesize, U64* ioif_addr);
S32 lv1_release_io_segment(U64 ioas_id, U64 ioif_addr);
S32 lv1_construct_io_irq_outlet(U64 interrupt_id, U64* outlet);
S32 lv1_destruct_io_irq_outlet(U64 outlet);
S32 lv1_map_htab(U64 lpar_id, U64* htab_addr);
S32 lv1_unmap_htab(U64 htab_addr);
S32 lv1_get_version_info(U64* firm_vers);
S32 lv1_insert_htab_entry(U64 vas_id, U64 pteg, U64 pte_hi, U64 pte_lo, U64 lockflags, U64 flags, U64* index, U64* evicted_hi, U64* evicted_lo);
S32 lv1_read_virtual_uart(U64 port, U64 buffer, U64 bytes, U64* bytes_read);
S32 lv1_write_virtual_uart(U64 port, U64 buffer, U64 bytes, U64* bytes_written);
S32 lv1_set_virtual_uart_param(U64 port, U64 param, U64 value);
S32 lv1_get_virtual_uart_param(U64 port, U64 param, U64* value);
S32 lv1_configure_virtual_uart(U64 lpar_addr, U64* outlet);
S32 lv1_open_device(U64 bus, U64 dev, U64 zero);
S32 lv1_close_device(U64 bus, U64 dev);
S32 lv1_map_device_mmio_region(U64 bus, U64 dev, U64 bus_addr, U64 size, U64 page_size, U64* lpar_addr);
S32 lv1_unmap_device_mmio_region(U64 bus, U64 dev, U64 lpar_addr);
S32 lv1_allocate_device_dma_region(U64 bus, U64 dev, U64 io_size, U64 io_pagesize, U64 flag, U64* dma_region);
S32 lv1_free_device_dma_region(U64 bus, U64 dev, U64 dma_region);
S32 lv1_map_device_dma_region(U64 bus, U64 dev, U64 lpar_addr, U64 dma_region, U64 size, U64 flags);
S32 lv1_unmap_device_dma_region(U64 bus, U64 dev, U64 dma_region, U64 size);
S32 lv1_read_pci_config(U64 ps3bus, U64 bus, U64 dev, U64 func, U64 offset, U64 size, U64* result);
S32 lv1_write_pci_config(U64 ps3bus, U64 bus, U64 dev, U64 func, U64 offset, U64 size, U64 data);
S32 lv1_net_add_multicast_address(U64 bus, U64 dev, U64 addr, U64 flags);
S32 lv1_net_remove_multicast_address(U64 bus, U64 dev, U64 zero, U64 one);
S32 lv1_net_start_tx_dma(U64 bus, U64 dev, U64 bus_addr, U64 zero);
S32 lv1_net_stop_tx_dma(U64 bus, U64 dev, U64 zero);
S32 lv1_net_start_rx_dma(U64 bus, U64 dev, U64 bus_addr, U64 zero);
S32 lv1_net_stop_rx_dma(U64 bus, U64 dev, U64 zero);
S32 lv1_net_set_interrupt_status_indicator(U64 bus, U64 dev, U64 irq_status_addr, U64 zero);
S32 lv1_net_set_interrupt_mask(U64 bus, U64 dev, U64 mask, U64 zero);
S32 lv1_net_control(U64 bus, U64 dev, U64 p1, U64 p2, U64 p3, U64 p4, U64* v1, U64* v2);
S32 lv1_connect_interrupt_event_receive_port(U64 bus, U64 dev, U64 outlet, U64 irq);
S32 lv1_disconnect_interrupt_event_receive_port(U64 bus, U64 dev, U64 outlet, U64 irq);
S32 lv1_deconfigure_virtual_uart_irq(void);
S32 lv1_enable_logical_spe(U64 spe_id, U64 resource_id);

/**
 * LV1 SysCall 116 (0x74): lv1_allocate_io_segment
 * @param[in]   ioas_id       R3  IO address space ID
 * @param[in]   segment_size  R4  IO segment size
 * @param[in]   io_page_size  R5  IO page size
 * @param[out]  ioas_id       R4  IO interface address
 * @return                    R3  Status code
 */
S32 lv1_allocate_io_segment(S64 ioas_id, S64 segment_size, S64 io_page_size, S64& ioif_addr);

/**
 * LV1 SysCall 210 (0xD2): lv1_gpu_open
 */
void lv1_gpu_open(S32 arg1);

/**
 * LV1 SysCall 211 (0xD3): lv1_gpu_close
 */
void lv1_gpu_close(void);

/**
 * LV1 SysCall 212 (0xD4): lv1_gpu_device_map
 */
void lv1_gpu_device_map(S32 device_id);

/**
 * LV1 SysCall 214 (0x6): lv1_gpu_memory_allocate
 */
void lv1_gpu_memory_allocate(S32 local_size, S64 arg4, S64 arg5, S64 arg6, S64 arg7);

/**
 * LV1 SysCall 217 (0x9): lv1_gpu_context_allocate
 */
void lv1_gpu_context_allocate(U32 mem_ctx_id, U64 system_mode);

/**
 * LV1 SysCall 221 (0xD): lv1_gpu_context_iomap
 */
void lv1_gpu_context_iomap(U32 rsx_ctx_id, U64 arg1, S64 lpar_addr, S32 size, U64 flags);
