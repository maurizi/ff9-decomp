#include "fxmod/fx_0aa.h"

s32 func_801E7704(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7784(pt, pos, &D_801E9088, &D_801E9098, frame, n);
}

s32 func_801E7744(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7784(pt, pos, &D_801E9090, &D_801E90A0, frame, n);
}

s32 func_801E7784(Pool30Entry *pt, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 frame, s32 n) {
    BTLFX_EXPORTS(D_801E7D78)->btlfx_lerpVec((s16 *)from, (s16 *)to, frame << 9, (s16 *)pos);
    pt->unk0.vx = pos->vx << 12;
    pt->unk0.vy = pos->vy << 12;
    pt->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_0aa_op80(s32 mode, Fx0aaWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        D_801E7D78 = env;
    } else {
        frame = entry->unk0;
        if (frame == 4) {
            ex->func_800DC818(0x10, 0x12, (u16 *)&work->unk10);
            ex->func_800DC818(0x10, 0x2B, (u16 *)&work->unk18);
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk10);
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk18);
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&D_801E90A8);
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&D_801E90B0);
            if (work->unk8 != NULL) {
                work->unk8->unk14 = (s32)D_801E7D88;
                work->unk8->unk12 = 0xC;
                work->unk8->unkC = 0x70;
            }
            if (work->unkC != NULL) {
                work->unkC->unk14 = (s32)D_801E82C8;
                work->unkC->unk12 = 0xC;
                work->unkC->unkC = 0x70;
            }
            if (work->unk8 != NULL) {
                ((EffMotion *)work->unk8->unk0)->unk8 = func_801E7704;
            }
            if (work->unkC != NULL) {
                ((EffMotion *)work->unkC->unk0)->unk8 = func_801E7744;
            }
            D_801E9088.vx = work->unk10.vx;
            D_801E9088.vy = work->unk10.vy;
            D_801E9088.vz = work->unk10.vz;
            D_801E9090.vx = work->unk18.vx;
            D_801E9090.vy = work->unk18.vy;
            D_801E9090.vz = work->unk18.vz;
            D_801E9098.vx = env->targets->centerX;
            D_801E9098.vy = env->targets->centerY;
            D_801E9098.vz = env->targets->centerZ;
            D_801E90A0.vx = env->targets->centerX;
            D_801E90A0.vy = env->targets->centerY;
            D_801E90A0.vz = env->targets->centerZ;
            D_801E9098.vx = D_801E9088.vx;
            D_801E90A0.vx = D_801E9090.vx;
        }
        return frame >= 0x1E;
    }
    return 0;
}

s32 fx_0aa_op81(s32 mode, Fx0aaWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        work->unk14.vx = env->targets->centerX;
        work->unk14.vy = env->targets->centerY;
        D_801E7D78 = env;
        work->unk14.vz = env->targets->centerZ;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk14);
            break;
        case 4:
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk14);
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk14);
            if (work->unk0 != NULL) {
                work->unk0->unk14 = (s32)D_801E8808;
                work->unk0->unk12 = 8;
                work->unk0->unkC = 0x110;
            }
            break;
        case 6:
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk14);
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk14);
            break;
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0aa/nonmatchings/fx_0aa", fx_0aa_headerPtr);
