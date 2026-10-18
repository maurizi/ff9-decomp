#include "fxmod/fx_19f.h"

INCLUDE_ASM("asm/us/fx_19f/nonmatchings/fx_19f", func_801E7704);

void func_801E7894(Unk80100C78Entry *entry, Fx19fTrail *trail, SVECTOR *from, SVECTOR *to, s32 amp, s32 arg5, s32 arg6,
                   s32 arg7, s32 arg8, s32 arg9) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EAA48);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 h;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    i = 0;
    p = trail->unk4;
    trail->unk0 = trail->unk2 = 0x18;
    trail->unk1 = 0;
    do {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x140);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = ex->btlfx_randCentered(0x80);
        a = t >> 1;
        h = (ex->math_sin(a) * h) >> 12;
        i++;
        h += 0x20 + ((ex->math_sin(a) * amp) >> 12);
        ex->func_800D044C(&pos, (s32)p, 0, (XY32 *)&rot, h, arg5);
        p->pad = arg9;
        arg5 += arg6 + ex->btlfx_randCentered(0x100);
        p++;
    } while (i < 25);
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), 0, NULL, arg7, 0x1000, arg8, 1, (u8 *)trail, 0);
}

INCLUDE_ASM("asm/us/fx_19f/nonmatchings/fx_19f", fx_19f_op81);

s32 fx_19f_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    Fx19fKeys *k;
    s32 frame;
    s32 f;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801EAA48 = env;
    } else {
        frame = entry->unk0;
        k = (Fx19fKeys *)ex->btlfx_getFileData(entry->unk8, 0x2B);
        f = frame;
        if (f >= k->unk2) {
            f = k->unk2 - 1;
        }
        k = (Fx19fKeys *)((u8 *)k + (f * 6 + 4));
        v.vx = k->unk0;
        v.vy = k->unk2 - 100;
        v.vz = ((s16 *)k)[2];
        env->unk2C->unk0[0].vx = v.vx;
        env->unk2C->unk0[0].vy = v.vy;
        env->unk2C->unk0[0].vz = v.vz;
        k = (Fx19fKeys *)ex->btlfx_getFileData(entry->unk8, 0x2C);
        f = frame;
        if (f >= k->unk2) {
            f = k->unk2 - 1;
        }
        k = (Fx19fKeys *)((u8 *)k + (f * 6 + 4));
        v.vx = k->unk0;
        v.vy = k->unk2 - 100;
        v.vz = ((s16 *)k)[2];
        env->unk2C->unk0[1].vx = v.vx;
        env->unk2C->unk0[1].vy = v.vy;
        env->unk2C->unk0[1].vz = v.vz;
        if (frame >= 0x5A) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_19f/nonmatchings/fx_19f", func_801E9968);

INCLUDE_ASM("asm/us/fx_19f/nonmatchings/fx_19f", func_801E9C08);

INCLUDE_ASM("asm/us/fx_19f/nonmatchings/fx_19f", func_801E9D20);

INCLUDE_ASM("asm/us/fx_19f/nonmatchings/fx_19f", func_801E9FC4);

INCLUDE_ASM("asm/us/fx_19f/nonmatchings/fx_19f", fx_19f_op83);

INCLUDE_RODATA("asm/us/fx_19f/nonmatchings/fx_19f", fx_19f_headerPtr);
