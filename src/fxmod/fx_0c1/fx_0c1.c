#include "fxmod/fx_0c1.h"

s32 func_801E7704(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    BTLFX_EXPORTS(D_801E7C18)->btlfx_lerpVec((s16 *)&D_801E7C20, (s16 *)&D_801E7C28, (frame << 12) / 9, (s16 *)pos);
    pt->unk0.vx = pos->vx << 12;
    pt->unk0.vy = pos->vy << 12;
    pt->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_0c1_op80(s32 mode, Fx0c1Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone bone; /* also holds angles at frame 0x24 */
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x50;
    } else if (mode == 1) {
        D_801E7C18 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk10);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk18);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk20);
        ex->func_800DC8AC(0x10, 0x17, &bone);
        ex->func_800CFE88((MATRIX *)&bone, 0x64, (s32)&work->unk28);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk28);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x20);
        }
    } else {
        frame = entry->unk0;
        if (frame == 0x24) {
            ex->func_800DC818(0x10, 0x17, (u16 *)&work->unk38);
            D_801E7C20.vx = work->unk38.vx;
            D_801E7C20.vy = work->unk38.vy;
            D_801E7C20.vz = work->unk38.vz;
            ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk18);
            ex->func_800CFD78((s16 *)&work->unk10, (s16 *)&work->unk18, 0x12C, (s16 *)&work->unk30);
            ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&work->unk20, (s16 *)&bone);
            ex->func_800CFD78((s16 *)&work->unk20, (s16 *)&bone, 0, (s16 *)&D_801E7C28);
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk30);
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&D_801E7C30);
            if (work->unk8 != NULL) {
                ((EffMotion *)work->unk8->unk0)->unk8 = func_801E7704;
            }
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->func_800E7CC4(0x10, 0x80);
            }
            ex->btlfx_aimAngles((s16 *)&work->unk38, (s16 *)&work->unk20, (s16 *)&work->unk48);
            work->unk40.vx = work->unk48.vx;
            work->unk40.vy = work->unk48.vy;
            work->unk40.vz = work->unk48.vz;
        }
        if (frame == 0x2D) {
            work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk20);
            if (work->unkC != NULL) {
                work->unkC->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        if (frame >= 0x24) {
            if (work->unk4 != NULL) {
                if (work->unk4->unk30 == -1) {
                    work->unk4 = NULL;
                }
                if (work->unk4 != NULL) {
                    work->unk4->unk5C.vx = work->unk40.vx;
                    work->unk4->unk5C.vy = work->unk40.vy;
                    work->unk4->unk5C.vz = work->unk40.vz;
                }
            }
            if (work->unk8 != NULL) {
                if (work->unk8->unk30 == -1) {
                    work->unk8 = NULL;
                }
                if (work->unk8 != NULL) {
                    work->unk8->unk5C.vx = work->unk48.vx;
                    work->unk8->unk5C.vy = work->unk48.vy;
                    work->unk8->unk5C.vz = work->unk48.vz;
                }
            }
        }
        return frame >= 0x46;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0c1/nonmatchings/fx_0c1", fx_0c1_headerPtr);
