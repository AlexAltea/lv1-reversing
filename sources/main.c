#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "lv1/inc/rsx_lv1.h"












/***********************************************************************
* dump BAR0
***********************************************************************/
void dump_BAR0(void)
{
	int32_t i, value;
	FILE *fd = fopen("./BAR0.bin", "wb");
	
	for(i = 0; i < 0x1C0000; i++)
	{
		value = ES32(read_BAR0(0x28000000000 + (i * 4)));
		fwrite(&value, 1, sizeof(int32_t), fd);
	}
	
	fclose(fd);
}

/***********************************************************************
* dump DDR memory from offset
***********************************************************************/
void dump_DDR(int32_t offset)
{
	FILE *fd = fopen("./DDR.bin", "wb");
	fwrite(DDR + offset, 1, DDR_SIZE - offset, fd);
	fclose(fd);
}

/***********************************************************************
* dump driver info
***********************************************************************/
void dump_driver_info(void)
{
	FILE *fd = fopen("./driver_info.bin", "wb");
	fwrite(driver_info, 1, DRVI_SIZE, fd);
	fclose(fd);
}






/***********************************************************************
* main
***********************************************************************/
int32_t main(int32_t argc, char* argv[])
{
	printf("programstart:\n\n");
	alloc_lv1_memory();
	int32_t ret = -1;
	
	
	
	//////////////////////////////////////////////////////////////////////
	// RSX hypercall test
	
	//////////////////////////////////////////////////////////////////////
	// TEST: hypercall 210 lv1_gpu_open()
	// if lv2 kernel start up and starts SYSTEM_IDLE_1 then rsx_init(void)
	// exec lv1_gpu_open(0). Thats the init of RSX.
	lv1_gpu_open(0);
	ret = g_r3;
	printf("lv1_gpu_open(0) ret(%d)\n\n", ret);
	
	
	
	//////////////////////////////////////////////////////////////////////
	// TEST: hypercall 212 lv1_gpu_device_map()
	int64_t dev_lpar_addr, dev_lpar_size;
	
	lv1_gpu_device_map(8);
	ret           = g_r3;
	dev_lpar_addr = g_r4;
	dev_lpar_size = g_r5;
	printf("lv1_gpu_device_map(8) ret(%d)\n", ret);
	printf("dev_lpar_addr: 0x%016llX\ndev_lpar_size: 0x%08X\n\n", dev_lpar_addr, (uint32_t)dev_lpar_size);
	
	
	
	//////////////////////////////////////////////////////////////////////
	// TEST: hypercall 214 lv1_gpu_memory_allocate()
	uint32_t mem_ctx_id = 0;
	int64_t ddr_lpar = 0;
	int32_t local_size = 0xFE00000;  // 0x500000
	int64_t arg1 = 8, arg2 = 0x30000, arg3 = 15, arg4 = 8;
	
	lv1_gpu_memory_allocate(local_size, arg1, arg2, arg3, arg4);
	ret        = g_r3;
	mem_ctx_id = g_r4;
	ddr_lpar   = g_r5;
	printf("lv1_gpu_memory_allocate() ret(%d)\n", ret);
	printf("mem_ctx_id: 0x%08X\nddr_lpar:   0x%016llX\n\n", mem_ctx_id, ddr_lpar);
	
	/*// TEST: second memory context, 249 MB like a game(than vsh, the first one, get 5 MB)
	mem_ctx_id = 0;
	ddr_lpar = 0;
	local_size = 0xF900000;  // 249 MB
	arg1 = 8, arg2 = 0x30000, arg3 = 15, arg4 = 8;
	
	lv1_gpu_memory_allocate(local_size, arg1, arg2, arg3, arg4);
	ret        = g_r3;
	mem_ctx_id = g_r4;
	ddr_lpar   = g_r5;
	printf("lv1_gpu_memory_allocate() ret(%d)\n", ret);
	printf("mem_ctx_id: 0x%08X\nddr_lpar:   0x%016llX\n\n", mem_ctx_id, ddr_lpar);
	*/
	
	
	
	//////////////////////////////////////////////////////////////////////
	// TEST: hypercall 217 lv1_gpu_context_allocate()
	uint64_t system_mode = 0x820;
	uint32_t rsx_ctx_id = 0;
	int64_t dma_control_lpar = 0;
	int64_t driver_info_lpar = 0;
	int64_t reports_lpar = 0;
	int32_t reports_lpar_size = 0;
	
	lv1_gpu_context_allocate(mem_ctx_id, system_mode);
	ret               = g_r3;
	rsx_ctx_id        = g_r4;
	dma_control_lpar  = g_r5;
	driver_info_lpar  = g_r6;
	reports_lpar      = g_r7;
	reports_lpar_size = g_r8;
	printf("lv1_gpu_context_allocate() ret(%d)\n", ret);
	printf("rsx_ctx_id:        0x%08X\n"
	       "dma_control_lpar:  0x%016llX\n"
	       "driver_info_lpar:  0x%016llX\n"
	       "reports_lpar:      0x%016llX\n"
	       "reports_lpar_size: 0x%08X\n\n", rsx_ctx_id, dma_control_lpar,
	       driver_info_lpar, reports_lpar, reports_lpar_size);
	
	
	
	
	
	//////////////////////////////////////////////////////////////////////
	// TEST: hypercall 221 lv1_gpu_context_iomap()
	uint64_t unk_1 = 0;
	int64_t lpar_addr = 0;
	int32_t size = 0;
	uint64_t flags = 0;
	
	lv1_gpu_context_iomap(rsx_ctx_id, unk_1, lpar_addr, size, flags);
	ret = g_r3;
	printf("lv1_gpu_context_iomap() ret(%d)\n", ret);
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//////////////////////////////////////////////////////////////////////
	// TEST
	//lv1_test();
  
  //dump_DDR(0);   //0xFE00000
  dump_driver_info();
	dump_BAR0();
	
	free_lv1_memory();
	printf("\nprogramend:\n");
	return 0;
}
