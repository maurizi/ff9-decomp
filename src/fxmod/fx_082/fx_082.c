#include "fxmod/fx_082.h"

INCLUDE_ASM("asm/us/fx_082/nonmatchings/fx_082", fx_082_op80);

s32 fx_082_op81(s32 mode, Fx082Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 v[3];
    s32 frame;
    s32 a;
    s32 b;
    s32 k;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode != 1) {
        frame = entry->unk0;
        v[0] = work->unk0[0];
        v[1] = work->unk0[1];
        v[2] = work->unk0[2];
        a = (frame << 8) >> 2;
        k = (ex->math_cos(a) << 11) >> 12;
        b = frame << 9;
        v[1] -= (ex->math_sin(b) * k) >> 12;
        k = (ex->math_cos(a) << 11) >> 12;
        k = (ex->math_sin(b) * k) >> 12;
        v[0] += k;
        v[2] += k;
        ex->func_800E7ACC(0, v);
        if (frame >= 0x10) {
            ex->func_800E7ACC(0, work->unk0);
            return 1;
        }
    } else {
        D_801E7E64 = env;
        ex->btlfx_getUnitScale(0, (VECTOR *)work);
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_082/nonmatchings/fx_082", fx_082_headerPtr);
