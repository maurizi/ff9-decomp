#include "fxmod/fx_0dd.h"

INCLUDE_RODATA("asm/us/fx_0dd/nonmatchings/fx_0dd", fx_0dd_headerPtr);

INCLUDE_ASM("asm/us/fx_0dd/nonmatchings/fx_0dd", fx_0dd_op80);

s32 fx_0dd_op84(s32 mode, Fx0ddWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
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
            D_801E83B0 = env;
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
                c = ex->math_sin(t) * 3 / 2 + 0x1000;
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
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x15), &work->unk8, NULL, b / 3, 0, 1, a / 2);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x16), &work->unk8, &rot, c / 2, 0, 1, a * 2 / 3);
            env->unk28 = 0;
            work->unk4++;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_0dd/nonmatchings/fx_0dd", func_801E8238);

INCLUDE_ASM("asm/us/fx_0dd/nonmatchings/fx_0dd", func_801E824C);

INCLUDE_ASM("asm/us/fx_0dd/nonmatchings/fx_0dd", func_801E82E8);

INCLUDE_ASM("asm/us/fx_0dd/nonmatchings/fx_0dd", func_801E82FC);
