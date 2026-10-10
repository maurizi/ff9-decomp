/* STATUS: near miss, 3 aligned-diff rows. ovl_64 func_8019A5B0 (0xD8, toggle 480/320 wide FMV mode). Only the then-branch of the on != 0 case differs: the original loads the constant 1 (v1) before lui %hi(D_80071E31), so reorg fills the beqz delay slot with 'li v1,1' instead of the lui. Store order D_80071E31 then D_8019DC94.unk1 is right (the other orders are worse). */
#include "ovl_64.h"

s32 func_8019A5B0(s32 on) {
    if (D_80071E31 >= 0) {
        if (on != 0) {
            if (D_80071E31 != 0) {
                return 0;
            }
        } else if (D_80071E31 != 1) {
            return 0;
        }
    }
    if (func_80012AD4() & 2) {
        D_8019DC94.unk3 = 1;
    } else if (on != 0) {
        D_80071E31 = 1;
        D_8019DC94.unk1 = 1;
        D_8019F4D8.unk18 = 0x1E0;
        D_8019F4D8.unk2C = 0x18;
        D_8019DC94.unk3 = 2;
    } else {
        D_80071E31 = 0;
        D_8019DC94.unk1 = 0;
        D_8019F4D8.unk18 = 0x140;
        D_8019F4D8.unk2C = 0x10;
    }
    return 1;
}
