#include "fxmod/fx_0a7.h"

s32 fx_0a7_op80(s32 mode, Fx0a7Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x58;
    } else if (mode == 1) {
        D_801E7C78 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk18);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk20);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk28);
        ex->func_800DC858(0x10, 0, (u16 *)&work->unk30);
        ex->btlfx_aimAngles((s16 *)&work->unk30, (s16 *)&work->unk20, (s16 *)&work->unk48);
        ex->btlfx_aimAngles((s16 *)&work->unk20, (s16 *)&work->unk30, (s16 *)&work->unk50);
        ex->func_800CFD78((s16 *)&work->unk30, (s16 *)&work->unk48, 0x12C, (s16 *)&work->unk38);
        ex->func_800CFD78((s16 *)&work->unk30, (s16 *)&work->unk48, 0xC8, (s16 *)&work->unk40);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk38);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk40);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7C88;
            work->unk0->unk12 = 0xB;
            work->unk0->unkC = 0x40;
        }
        if (work->unk4 != NULL) {
            work->unk4->unk14 = (s32)D_801E7F48;
            work->unk4->unk12 = 0x2D;
            work->unk4->unkC = 0x18;
        }
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x20);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0, 0x20);
        }
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 12:
            work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk20);
            work->unk10 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk20);
            if (work->unkC != NULL) {
                work->unkC->unk14 = (s32)D_801E8388;
                work->unkC->unk12 = 0x51;
                work->unkC->unkC = 0x40;
            }
            if (work->unkC != NULL) {
                work->unkC->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            if (work->unk10 != NULL) {
                work->unk10->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        case 30:
            work->unk14 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk28);
            break;
        }
        if (work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk50.vx;
                work->unk0->unk5C.vy = work->unk50.vy;
                work->unk0->unk5C.vz = work->unk50.vz;
            }
        }
        if (work->unk4 != NULL) {
            if (work->unk4->unk30 == -1) {
                work->unk4 = NULL;
            }
            if (work->unk4 != NULL) {
                work->unk4->unk5C.vx = work->unk48.vx;
                work->unk4->unk5C.vy = work->unk48.vy;
                work->unk4->unk5C.vz = work->unk48.vz;
            }
        }
        if (frame >= 0xC) {
            if (work->unkC != NULL) {
                if (work->unkC->unk30 == -1) {
                    work->unkC = NULL;
                }
                if (work->unkC != NULL) {
                    work->unkC->unk5C.vx = work->unk48.vx;
                    work->unkC->unk5C.vy = work->unk48.vy;
                    work->unkC->unk5C.vz = work->unk48.vz;
                }
            }
            if (work->unk10 != NULL) {
                if (work->unk10->unk30 == -1) {
                    work->unk10 = NULL;
                }
                if (work->unk10 != NULL) {
                    work->unk10->unk5C.vx = work->unk48.vx;
                    work->unk10->unk5C.vy = work->unk48.vy;
                    work->unk10->unk5C.vz = work->unk48.vz;
                }
            }
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a7/nonmatchings/fx_0a7", fx_0a7_headerPtr);
