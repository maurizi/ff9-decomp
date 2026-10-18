#include "fxmod/fx_0d8.h"

INCLUDE_ASM("asm/us/fx_0d8/nonmatchings/fx_0d8", func_801E7704);

INCLUDE_ASM("asm/us/fx_0d8/nonmatchings/fx_0d8", func_801E796C);

INCLUDE_ASM("asm/us/fx_0d8/nonmatchings/fx_0d8", func_801E7BF8);

void func_801E7CEC(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 *arg5, s32 arg6,
                   s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8FA0);
    Unk80100C78Entry *entry;
    SVECTOR r;
    s32 s;
    s32 t;

    BTLFX_COPY_SVEC(&r, rot);
    entry = D_801E9940;
    s = ex->math_sin((frame << 11) / n);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A), pos, &r, *scale * 3 / 2, frame, 1, s, 0, 0);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x19), pos, &r, *scale * 3 / 2, frame, 1, s, 2, 2);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1D), pos, &r, *scale * 5 / 2, frame, 1, s, 0, 0);
    t = (frame << 12) / n + 0x1800;
    ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), pos, &r, t, frame, 1, *arg5);
    ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1C), pos, &r, t, frame, 1, *arg5);
}

INCLUDE_ASM("asm/us/fx_0d8/nonmatchings/fx_0d8", func_801E7F80);

INCLUDE_ASM("asm/us/fx_0d8/nonmatchings/fx_0d8", fx_0d8_op83);

s32 fx_0d8_op84(s32 mode, Fx0d8Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 a;
    s32 b;
    s32 c;
    s32 t;
    s32 u;
    s32 k;
    s32 v;
    u16 ry;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E8FA0 = env;
        work->unk0 = 0;
        work->unk4 = 0;
    } else {
        a = 0;
        b = 0;
        c = 0;
        frame = entry->unk0;
        ex->func_800E7D3C(0x10, 1, (s16 *)&work->unk8);
        ry = 0;
        switch (work->unk0) {
        case 0:
            ry = 0x200;
            a = (frame << 12) / 3;
            u = ex->math_sin(a >> 2);
            b = u >> 1;
            c = ((0x1000 - a) >> 1) + 0x800;
            if (frame >= 3) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            u = (frame << 12) / 9;
            k = (frame & 1) << 8;
            c = k + 0x1000;
            v = (u >> 1) + 0x800;
            a = k + v;
            ry = (u >> 3) + 0x200;
            b = c;
            if (frame >= 9) {
                work->unk0 = 2;
                entry->unk0 = -1;
            }
            break;
        case 2:
            u = (frame << 12) / 9;
            t = u >> 2;
            ry = 0x400 - t;
            a = ex->math_cos(t);
            b = ex->math_sin(t) + 0x1000;
            c = ex->math_sin(t) * 3 / 2 + 0x1000;
            if (frame >= 9) {
                return 1;
            }
            break;
        }
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = ry;
        rot.pad = 0;
        env->unk28 = 0x80;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), &work->unk8, NULL, b / 3, 0, 1, a / 2);
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), &work->unk8, &rot, c / 2, 0, 1, a * 2 / 3);
        env->unk28 = 0;
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0d8/nonmatchings/fx_0d8", fx_0d8_headerPtr);
