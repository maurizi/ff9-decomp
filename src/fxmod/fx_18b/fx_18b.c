#include "fxmod/fx_18b.h"

INCLUDE_ASM("asm/us/fx_18b/nonmatchings/fx_18b", func_801E7704);

void func_801E7EB8(s32 r, s32 i, s32 ang, s32 arg3, s16 *out) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E827C);
    s32 s = (ex->math_sin((i << 11) >> 4) * D_801E8284) >> 12;

    r += (s * ex->math_sin(D_801E8280 + (i << 10))) >> 12;
    out[0] = (ex->math_cos(ang) * r) >> 12;
    out[1] = (ex->math_sin(ang) * r) >> 12;
}

s32 fx_18b_op80(s32 mode, Fx18bWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    RECT rect;
    Pool6CEntry *p;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        D_801E827C = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk14);
        work->unk8 = ex->func_800E7CC4(0, 0x20);
        D_801E8280 = 0;
        work->unk0 = 0;
        D_801E8284 = 0;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            t = (frame << 12) / 40;
            D_801E8284 = ex->math_sin(t >> 2) / 320;
            D_801E8280 -= 0x200;
            if (frame >= 0x28) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 40;
            D_801E8284 = ex->math_cos(t >> 2) / 320;
            D_801E8280 += 0x200;
            if (frame == 1) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unkC);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
            }
            if (frame >= 0x28) {
                return 1;
            }
            break;
        }
        rect.x = 0x1C0;
        rect.y = 0x100;
        rect.w = 0x100;
        rect.h = 0x100;
        func_801E7704(1, 0xA0, 0x68, 0x10, 0x10, 8, 0x88, 0x88, &rect, func_801E7EB8);
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_18b/nonmatchings/fx_18b", fx_18b_headerPtr);
