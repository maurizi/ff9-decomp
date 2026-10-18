#include "fxmod/fx_0ad.h"

s32 fx_0ad_op80(s32 mode, Fx0adWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E792C = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        ex->func_800DC858(0x10, 0, (u16 *)&work->unk8);
        ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&work->unk10, (s16 *)&work->unk18);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk8);
        if (work->unk4 != NULL) {
            work->unk4->unk14 = (s32)D_801E793C;
            work->unk4->unk12 = 0x24;
            work->unk4->unkC = 0x5C;
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = 0x20;
        }
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            if (work->unk4 != NULL) {
                work->unk4->unk5C.vx = work->unk18.vx;
                work->unk4->unk5C.vy = work->unk18.vy;
                work->unk4->unk5C.vz = work->unk18.vz;
            }
            if (frame == 0x16) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x10);
                }
            }
            if (frame >= 0x28) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ad/nonmatchings/fx_0ad", fx_0ad_headerPtr);
