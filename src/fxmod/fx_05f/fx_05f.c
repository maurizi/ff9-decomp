#include "fxmod/fx_05f.h"

INCLUDE_ASM("asm/us/fx_05f/nonmatchings/fx_05f", fx_05f_op80);

s32 fx_05f_op81(s32 mode, Fx05fWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 t;
    SVECTOR v;

    if (mode == 0) {
        *arg4 = 0x68;
    } else if (mode == 1) {
        D_801E8320 = env;
        work->unk40 = env->unkC->unk24;
        for (i = 0; i < work->unk40; i++) {
            ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk0[i]);
            work->unk44 = -0x190 - (ex->btlfx_rand() & 0x7F);
            ex->func_800DC818(i, 0, (u16 *)&v);
            work->unk48[i] = -v.vy;
        }
    } else {
        frame = entry->unk0;
        if (frame < 0x20) {
            t = frame << 7;
            for (i = 0; i < work->unk40; i++) {
                BTLFX_COPY_SVEC(&v, &work->unk0[i]);
                v.vy += (work->unk44 * ex->math_sin(t >> 2)) >> 12;
                ex->btlfx_setUnitPos(i, (s16 *)&v);
            }
        } else {
            t = (frame - 0x20) << 10;
            for (i = 0; i < work->unk40; i++) {
                BTLFX_COPY_SVEC(&v, &work->unk0[i]);
                v.vy += (work->unk48[i] * t) >> 12;
                v.vy += (work->unk44 * ex->math_cos(t >> 2)) >> 12;
                ex->btlfx_setUnitPos(i, (s16 *)&v);
            }
        }
        if (frame >= 0x24) {
            for (i = 0; i < work->unk40; i++) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&v);
                ex->btlfx_setUnitPos(i, (s16 *)&v);
            }
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_05f/nonmatchings/fx_05f", fx_05f_headerPtr);
