#include "fxmod/fx_008.h"

s32 fx_008_op80(s32 mode, Fx008Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;
    u16 n;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E78EC = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk4);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E78FC;
            work->unk0->unk12 = 0xB;
            work->unk0->unkC = 0x8C;
        }
        work->unk16 = 0x1000;
        work->unk14 = 0;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->unkC);
    } else {
        frame = entry->unk0;
        t = frame - 0x18;
        if (t >= 0 && t < 0x10) {
            ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unkC, work->unk14, work->unk16);
            if (t >= 4) {
                work->unk16 -= 0x155;
            }
            n = work->unk14 + 1;
            work->unk14 = n;
            if (n >= 6) {
                n = 5;
            }
            work->unk14 = n;
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_008/nonmatchings/fx_008", fx_008_headerPtr);
