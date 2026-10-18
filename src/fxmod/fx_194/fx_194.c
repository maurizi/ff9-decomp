#include "fxmod/fx_194.h"

void func_801E7704(SVECTOR *v) {
    s32 y = v->vy;
    s32 x = v->vx;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7DFC);
    s32 nx = x + ((ex->math_cos(D_801E7E04 + y * 0x18) * D_801E7E00) >> 13);
    s32 ny = y + ((ex->math_sin(D_801E7E04 + x * 0x18) * D_801E7E00) >> 12);

    v->vx = nx;
    v->vy = ny;
}

void func_801E77DC(s32 frame, s32 amp, SVECTOR *v) {
    s32 d;

    if (frame == 2) {
        d = (amp * 16) >> 7;
        v->vx -= d;
        v->vz -= d;
        v->vy -= d;
    }
    if (frame >= 0x13 && frame < 0x19) {
        SVECTOR *t = &D_801E7D78[frame - 0x13];
        v->vx += (t->vx * amp) >> 7;
        v->vy += (t->vy * amp) >> 7;
        v->vz += (t->vz * amp) >> 7;
    }
}

INCLUDE_ASM("asm/us/fx_194/nonmatchings/fx_194", fx_194_op80);

s32 fx_194_op81(s32 mode, Fx194Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E7DFC = env;
        work->unk0 = 0;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk4);
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = 0x200;
        }
        env->unk2C->unk0[3].vx = work->unk4.vx;
        env->unk2C->unk0[3].vy = work->unk4.vy;
        env->unk2C->unk0[3].vz = work->unk4.vz;
    } else {
        s32 frame = entry->unk0;
        if (work->unk0 == 0 && frame >= 0x20) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_194/nonmatchings/fx_194", fx_194_headerPtr);
