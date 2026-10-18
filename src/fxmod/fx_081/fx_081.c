#include "fxmod/fx_081.h"

void func_801E7704(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 arg3, s32 *arg4, s32 arg5, s32 arg6, s32 frame,
                   s32 n) {
    Unk800F78A4 *env = D_801E8ED8;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Unk80100C78Entry *entry = D_801EA9B0;
    SVECTOR v;
    s32 r;
    s32 t;
    s32 k;

    r = ex->func_800E0134(pt, p);
    if (frame == 0) {
        ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), (s32)pos, NULL, -1, 0, 0, 0, (u8 *)r, 0);
    }
    if (frame < n - 4) {
        t = 0x1000;
    } else {
        t = 0x1000 - (((frame - (n - 4)) << 12) >> 2);
    }
    ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), (s32)pos, (s32 *)p->unk28, frame,
                      0x1C00 - (frame << 12) / n, t, 1, (u8 *)r, 1);
    if (pt->unk1F == 1) {
        if (D_801EA738 != 0) {
            ex->func_800E3D20(D_801EA738, (s32)pos);
        }
        BTLFX_COPY_SVEC(&D_801EA730, pos);
        if (frame < 0x1D) {
            env->unk2C->unk0[3].vx = pos->vx;
            env->unk2C->unk0[3].vy = pos->vy;
            env->unk2C->unk0[3].vz = pos->vz;
            return;
        }
        k = frame - 0x1D;
        if (k < 0x20) {
            ex->btlfx_lerpVec((s16 *)pos, (s16 *)&D_801EA728, 0x1000 - ex->math_cos(k << 5), (s16 *)&v);
        } else {
            BTLFX_COPY_SVEC(&v, &D_801EA728);
        }
        env->unk2C->unk0[3].vx = v.vx;
        env->unk2C->unk0[3].vy = v.vy;
        env->unk2C->unk0[3].vz = v.vz;
    }
}

INCLUDE_ASM("asm/us/fx_081/nonmatchings/fx_081", fx_081_op80);

INCLUDE_ASM("asm/us/fx_081/nonmatchings/fx_081", fx_081_op81);

INCLUDE_RODATA("asm/us/fx_081/nonmatchings/fx_081", fx_081_headerPtr);
