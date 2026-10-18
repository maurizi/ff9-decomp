#include "fxmod/fx_057.h"

void func_801E7704(Pool30Entry *pt, Pool6CEntry *e, SVECTOR *pos, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 t,
                   s32 n) {
    Unk800F78A4 *env = D_801E8EA4;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Unk80100C78Entry *entry = D_801EA97C;
    u8 *buf = (u8 *)ex->func_800E0134(pt, e);
    s32 s;
    s32 d;
    SVECTOR v;

    if (t == 0) {
        ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 6), (s32)pos, NULL, -1, 0, 0, 0, buf, 0);
    }
    if (t < n - 4) {
        s = 0x1000;
    } else {
        s = 0x1000 - (((t - (n - 4)) << 12) >> 2);
    }
    ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 6), (s32)pos, (s32 *)e->unk28, t,
                      0x1C00 - (t << 12) / n, s, 1, buf, 1);
    if (pt->unk1F == 1) {
        if (D_801EA704 != NULL) {
            ex->func_800E3D20((s32)D_801EA704, (s32)pos);
        }
        BTLFX_COPY_SVEC(&D_801EA6FC, pos);
        if (t < 0x1D) {
            env->unk2C->unk0[3].vx = pos->vx;
            env->unk2C->unk0[3].vy = pos->vy;
            env->unk2C->unk0[3].vz = pos->vz;
        } else {
            d = t - 0x1D;
            if (d < 0x20) {
                ex->btlfx_lerpVec((s16 *)pos, (s16 *)&D_801EA6F4, 0x1000 - ex->math_cos(d << 5), (s16 *)&v);
            } else {
                BTLFX_COPY_SVEC(&v, &D_801EA6F4);
            }
            env->unk2C->unk0[3].vx = v.vx;
            env->unk2C->unk0[3].vy = v.vy;
            env->unk2C->unk0[3].vz = v.vz;
        }
    }
}

INCLUDE_ASM("asm/us/fx_057/nonmatchings/fx_057", fx_057_op80);

INCLUDE_ASM("asm/us/fx_057/nonmatchings/fx_057", fx_057_op81);

INCLUDE_RODATA("asm/us/fx_057/nonmatchings/fx_057", fx_057_headerPtr);
