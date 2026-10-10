/* STATUS: logic right, codegen far (128 rows). ovl_64 func_801999B4 (0x174, MBG_init: buffer setup). The original computes every buffer address as 'lui/ori a3, CONST; addu rX, t0, a3' with a3 reused right before each use and stores them all afterwards; gcc 2.8.1 -O2 hoists the constants into separate registers. Tried accumulating offsets (b += 0x10000, ...), -O1, -fno-schedule-insns, other compilers: none reproduce it. Maybe a different source shape (struct of pointers filled by a loop/macro?). */
#include "ovl_64.h"

void func_801999B4(s32 mode, u8 **buf) {
    u8 *b;

    if (D_80071E32 != 0) {
        func_80015C38("Warning! MBG_init() called when already initialized\n");
        D_80071E32 = 0;
    }
    switch (mode) {
    case 1:
        b = buf[0];
        D_8019DC94.unk48 = b;
        D_8019DC94.unk4C = b + 0x10000;
        D_8019DC94.unk58 = b + 0x20000;
        D_8019DC94.unk5C = b + 0x43E20;
        D_8019DC94.unk50 = b + 0x49E20;
        D_8019DC94.unk54 = b + 0x4CB20;
        break;
    case 2:
        b = buf[0];
        D_8019DC94.unk48 = b;
        D_8019DC94.unk4C = b + 0x10000;
        D_8019DC94.unk50 = b + 0x20000;
        D_8019DC94.unk54 = b + 0x22D00;
        b = buf[1];
        D_8019DC94.unk58 = b;
        D_8019DC94.unk5C = b + 0x23E20;
        break;
    }
    SetGraphDebug(0);
    D_80071E31 = -1;
    D_80071E32 = 1;
    D_80071E34 = 0;
    D_80071E30 = 0;
    D_8019DC94.unk38 = 3;
    D_8019DC94.unk3C = 4;
    D_8019DC94.unk40 = 0x7F;
    D_8019DC94.unk3 = 0;
    D_8019DC94.unk44 = D_8019F288;

}
