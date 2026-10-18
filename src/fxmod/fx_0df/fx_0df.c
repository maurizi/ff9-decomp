#include "fxmod/fx_0df.h"

INCLUDE_ASM("asm/us/fx_0df/nonmatchings/fx_0df", func_801E7704);

void func_801E7EB8(s32 arg0, s32 arg1, SVECTOR *pos, SVECTOR *arg3, s32 *arg4, s32 *arg5, s32 arg6, s32 arg7) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9E90);

    ex->func_800D1680((u8 *)ex->btlfx_getFileData(D_801EA414->unk8, 0x10), pos, arg3, *arg4 / 2, arg7, 1, *arg5, 0, 0);
    BTLFX_COPY_SVEC(&D_801EA350[D_801EA410], pos);
    D_801EA410 = (D_801EA410 + 1) % 24;
}

INCLUDE_ASM("asm/us/fx_0df/nonmatchings/fx_0df", func_801E7FE0);

INCLUDE_ASM("asm/us/fx_0df/nonmatchings/fx_0df", func_801E80B0);

INCLUDE_ASM("asm/us/fx_0df/nonmatchings/fx_0df", func_801E80F4);

INCLUDE_ASM("asm/us/fx_0df/nonmatchings/fx_0df", fx_0df_op80);

void func_801E93F4(Unk80100C78Entry *entry, Fx0dfBolt *bolt, s32 *jitter, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 arg9, s32 arg10, s32 pad) {
    Unk800F78A4 *env = D_801E9E90;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR p;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->unk4;
    bolt->unk2 = 0xE;
    bolt->unk0 = 0xE;
    bolt->unk1 = 0;
    for (i = 0; i < 15; i++) {
        t = 0x1000 - (i << 12) / 14;
        t += *jitter++;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = *jitter++;
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += ex->btlfx_lerp(amp0, amp1, t);
        ex->func_800D044C(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = pad;
        ang += dang + *jitter++;
        pt++;
    }
    env->unk28 = 0x200;
    ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), 0, 0, arg9, 0x1000, arg10 * 2 / 3, 2, (u8 *)bolt, 0);
    env->unk28 = 0;
}

INCLUDE_ASM("asm/us/fx_0df/nonmatchings/fx_0df", func_801E9600);

INCLUDE_ASM("asm/us/fx_0df/nonmatchings/fx_0df", fx_0df_op83);

s32 fx_0df_op84(s32 mode, Fx0dfWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 a;
    s32 b;
    s32 c;
    s32 k;
    s32 t;
    s32 d;
    u16 r;

    if (mode == 0) {
        *arg4 = 0x10;
    } else {
        if (mode == 1) {
            D_801E9E90 = env;
            work->unk0 = 0;
            work->unk4 = 0;
        } else {
            r = c = b = a = 0;
            frame = entry->unk0;
            ex->func_800E7D3C(0x10, 1, (s16 *)&work->unk8);
            switch (work->unk0) {
            case 0:
                r = 0x200;
                a = (frame << 12) / 3;
                b = ex->math_sin(a >> 2) >> 1;
                c = ((0x1000 - a) >> 1) + 0x800;
                if (frame >= 3) {
                    work->unk0 = 1;
                    entry->unk0 = -1;
                }
                break;
            case 1:
                k = (frame & 1) << 8;
                c = k + 0x1000;
                d = (frame << 12) / 9;
                a = (k + 0x800) + (d >> 1);
                r = (d >> 3) + 0x200;
                b = c;
                if (frame >= 9) {
                    work->unk0 = 2;
                    entry->unk0 = -1;
                }
                break;
            case 2:
                d = (frame << 12) / 9;
                t = d >> 2;
                r = 0x400 - t;
                a = ex->math_cos(t);
                b = ex->math_sin(t) + 0x1000;
                c = ex->math_sin(t) * 2 + 0x1000;
                if (frame >= 9) {
                    return 1;
                }
                break;
            }
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = r;
            rot.pad = 0;
            env->unk28 = 0x80;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), &work->unk8, NULL, b, 0, 1, a / 2);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), &work->unk8, &rot, c * 3 / 2, 0, 1, a * 2 / 3);
            env->unk28 = 0;
            work->unk4++;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0df/nonmatchings/fx_0df", fx_0df_headerPtr);
