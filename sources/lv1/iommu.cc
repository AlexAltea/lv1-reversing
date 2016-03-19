/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "io.h"

#include "lv1/lv1.h"

#define MAGIC4    0x4156968E5E0D1602ULL
#define MAGIC9_1  0x997997D2B58E3C01ULL
#define MAGIC9_2  0x997997D2B58E3C02ULL
#define MAGIC9_3  0x997997D2B58E3C03ULL

// --- begin: types -----------------------------------------------------
// Static
const U64 count_ioas = 0x1;
ioas_t ioas_arr[count_ioas] = {
    { 0x20000510000ULL, 0ULL, 0x23, 0, {0, 0, 8}}
};

struct unk0_t {
    void*  unk_00;
    U32    unk_08;
    U32    unk_0C;
    void*  unk_10;
    void*  unk_18;
    U64    unk_20;
    void*  unk_28;
    U32    unk_30;
    U64    unk_34;
    U32    unk_3C;
    void*  unk_40;
    void*  unk_48;
};

struct unk1_t {
    U64      unk_00;
    U32      unk_08;
    U32      unk_0C;
    U64      unk_10;             // 0x10: ptr
    unk0_t** array_unk_18;       // 0x18: ptr
    U32      unk_20;             // 0x20: 6
    U32      unk_28;             // 0x24: 0x25
};

struct ioas_t {
    U64    unk_00;
    U64    unk_08;
    U32    unk_10;    // 0x10
    U32    unk_14;
    unk1_t unk_18;
    unk1_t unk_40;
};

// --- end: types -----------------------------------------------------

S32 is_id_valid(unk1_t* arg1, S64 io_id) {
    U64 r8 = 0;
    U64 index = 0;
    if ((arg1->unk_20 << 1) <= io_id) {
        U64 cntlz = 0;
        //while (!(io_id & 1))
        r8 = arg1->unk_20 - (cntlz - 0x40);

    }

    void* ptr = (void*)arg1->array_unk_18[index];
    if (!ptr) {
        return 1;
    }
}

// --- begin: iopte -----------------------------------------------------
// arg1 not sure if arg1 is of type ioas_t
S32 sub_2D5DC0(ioas_t* ioas, S64 ioif_addr, S64 lpar_addr, S64 io_id, S64 flags) {

}

S32 put_iopte(S64 ioas_id, S64 ioif_addr, S64 lpar_addr, S64 io_id, S64 flags, S64 unk_08) {
    if (ioas_id >= count_ioas) {
        return LV1_NO_ENTRY;
    }
    ioas_t* ioas = ioas_arr[ioas_id];
    U64 ioif_addr_max = 1 << ioas->unk_10; // 0x800000000 ?
    if (ioif_addr >= ioif_addr_max) {
        return LV1_ILLEGAL_PARAMETER_VALUE;
    }
    if (io_id > 0x7FF) {
        return LV1_ILLEGAL_PARAMETER_VALUE;
    }
    if (flags & 0/*TODO*/) {
        return LV1_ILLEGAL_PARAMETER_VALUE;
    }

    // r27 = *(U64*)(-0x67E8(HSPRG0)) // Active LPAR?
    if (unk_08 != 0) {
        U08 ret = is_id_valid(&ioas->unk_18, io_id);  // clrlwi r3,r3,24 => cast to U08? (I forgot again)
        if (ret == 0) {
            U64 r9;
            if (ioas->unk_18.unk_0C - 1 == 0) {
                r9 = 0;1 << ioas->unk_18.unk_08;
            } else {

            }
        }
    }

    /////

    U32 context_id = ioif_addr >> 28;
    auto& r31 = ioas->unk_40;
    U08 ret = is_id_valid(&r31, context_id);
    if (ret != 0) {
        return LV1_NO_ENTRY;
    }

    // Maybe the if/else below can be merged
    U64 r9 = r31.unk_40 - 1;
    if (r9 == 0) {
        r9 = (1 << r31.unk_18) - 1;
    } else {
        r9 = 2 * (1 << (r31.unk_18 + r9 - 1)) - 1;
    }

    if (r9 < context_id) {
        *(U64*)(nullptr); // wat?
    }
    // else
    (1 << r31.unk_18);
    
    // ...
    unk0_t* r3 = r31.array_unk1[0/*todo*/];
    if (r3 == nullptr) {
        *(U64*)(nullptr); // k. if you insist
    }
    //(r3->unk_30 * 0/*todo*/) + r3->unk_48;


    /////

    U64 ret = sub_2D5DC0(nullptr/*todo*/, ioif_addr, lpar_addr, io_id, flags);
    switch (ret) {
    case MAGIC4:
        return LV1_ILLEGAL_PARAMETER_VALUE;
    case MAGIC9_1:
        return LV1_NO_ENTRY;
    case MAGIC9_2:
        return LV1_ALIGNMENT_ERROR;
    case MAGIC9_3:
        return LV1_ACCESS_VIOLATION;
    }
    if (ret != 0) {
        // ...
    }
    return LV1_SUCCESS;
}
// --- end: iopte -------------------------------------------------------


// --- begin: allocate_io_segment ---------------------------------------
S64 sub_2DABBC() {
    return 0; // Value at HSPRG0(-0x6FF0)
}
S64 sub_2DBCF8(S64 arg1, S64 arg2) {
    return 0;
}
S64 sub_2DBDBC() {
    S64 unk_08 = sub_2DABBC();
    S64 unk_10 = 0; // Pointer to HSPRG0(-0x6FF0)
    sub_2DBCF8(unk_08, unk_10);
}
void sub2DBE04(S64 ioas_id, S64 segment_size, S64 io_page_size) {
    //sub_27A0(1); // HSPRG0(-0x6F80) // Generic
    sub_2DBDBC();
}
// --- end: allocate_io_segment -----------------------------------------
