#include "fxmod/fx_106.h"

s32 func_801E7704(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7C10);
    s32 t = (frame - 4) << 8;

    ex->btlfx_lerpVec((s16 *)&D_801E9400, (s16 *)&D_801E9408, t, (s16 *)pos);
    pos->vy -= (ex->math_sin(t >> 1) * 200) >> 12;
    out->vx = pos->vx << 12;
    out->vy = pos->vy << 12;
    out->vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_106_op80(s32 mode, Fx106Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        D_801E7C10 = env;
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        D_801E9408.vx = work->unk10.vx;
        D_801E9408.vy = work->unk10.vy;
        D_801E9408.vz = work->unk10.vz;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk18);
        ex->btlfx_pointAhead((s16 *)&work->unk18, (s16 *)&work->unk8, 0, (s16 *)&work->unk20);
        D_801E9400.vx = work->unk20.vx;
        D_801E9400.vy = work->unk20.vy;
        D_801E9400.vz = work->unk20.vz;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&D_801E9410);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk20);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7C20;
            work->unk0->unk12 = 0x23;
            work->unk0->unkC = 0x20;
            if (work->unk0 != NULL) {
                ((Fx106EffHdr *)work->unk0->file)->unk8 = func_801E7704;
                if (work->unk0 != NULL) {
                    work->unk0->unk22 = ex->btlfx_addRadiusSixth(0, 0x80);
                }
            }
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

s32 fx_106_op81(s32 mode, Fx106Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        D_801E7C10 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unkC);
        work->unk14.vx = work->unkC.vx;
        work->unk14.vy = work->unkC.vy;
        work->unk14.vz = work->unkC.vz;
        work->unk14.vy -= 0x190;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk14);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unkC);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E8080;
            work->unk0->unk12 = 0x33;
            work->unk0->unkC = 0x20;
        }
    } else {
        frame = entry->frame;
        if (frame == 4) {
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unkC);
            if (work->unk8 != NULL) {
                work->unk8->unk14 = (s32)D_801E86E0;
                work->unk8->unk12 = 0x1E;
                work->unk8->unkC = 0x70;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_106/nonmatchings/fx_106", fx_106_headerPtr);
