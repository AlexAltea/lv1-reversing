/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#pragma once

#include "common/types.h"

enum {
    IOSTE_VALID_MASK    = 0x8000000000000000ULL,
    IOSTE_TAG_MASK      = 0x3000000000000000ULL,
    IOSTE_PT_BASE_MASK  = 0x000003FFFFFFF000ULL, 
    IOSTE_NNPT_MASK     = 0x0000000000000FE0ULL,  // Number of pages per IO page table
    IOSTE_PS_MASK       = 0x000000000000000FULL,  // Page size

    IOSTE_PS_4K         = 0x1,  // Page size:  4 KB
    IOSTE_PS_64K        = 0x3,  // Page size: 64 KB
    IOSTE_PS_1M         = 0x5,  // Page size:  1 MB
    IOSTE_PS_16M        = 0x7,  // Page size: 16 MB

	IOPTE_PROT_NONE	    = 0x0000000000000000ULL,
    IOPTE_PROT_MASK	    = 0xC000000000000000ULL,
	IOPTE_PROT_R	    = 0x4000000000000000ULL,  // Read
	IOPTE_PROT_W        = 0x8000000000000000ULL,  // Write
	IOPTE_PROT_RW	    = 0xC000000000000000ULL,  // Read-Write
	IOPTE_COHERENT	    = 0x2000000000000000ULL,
};

S32 put_iopte(S64 ioas_id, S64 ioif_addr, S64 lpar_addr, S64 io_id, S64 flags, S64 unk_08);

void sub2DBE04(S64 ioas_id, S64 segment_size, S64 io_page_size);
