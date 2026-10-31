#include "fxmod/fx_0aa.h"

static s32 fx_0aa_op80ParticleMotion0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7784(pt, pos, &D_801E9088, &D_801E9098, frame, n);
}

static s32 fx_0aa_op80ParticleMotion1(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7784(pt, pos, &D_801E9090, &D_801E90A0, frame, n);
}

s32 func_801E7784(Pool30Entry *pt, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 frame, s32 n) {
    BTLFX_EXPORTS(fx_0aa_env)->btlfx_lerpVec((s16 *)from, (s16 *)to, frame << 9, (s16 *)pos);
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
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
        fx_0aa_env = env;
    } else {
        frame = entry->frame;
        if (frame == 4) {
            ex->btlfx_getUnitBonePos(0x10, 0x12, (u16 *)&work->pos10);
            ex->btlfx_getUnitBonePos(0x10, 0x2B, (u16 *)&work->pos18);
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos10);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos18);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&D_801E90A8);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&D_801E90B0);
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)D_801E7D88;
                work->emit8->trailSlots = 0xC;
                work->emit8->trailStride = 0x70;
            }
            if (work->emitC != NULL) {
                work->emitC->trails = (s32)D_801E82C8;
                work->emitC->trailSlots = 0xC;
                work->emitC->trailStride = 0x70;
            }
            if (work->emit8 != NULL) {
                ((EffMotion *)work->emit8->file)->update = fx_0aa_op80ParticleMotion0;
            }
            if (work->emitC != NULL) {
                ((EffMotion *)work->emitC->file)->update = fx_0aa_op80ParticleMotion1;
            }
            D_801E9088.vx = work->pos10.vx;
            D_801E9088.vy = work->pos10.vy;
            D_801E9088.vz = work->pos10.vz;
            D_801E9090.vx = work->pos18.vx;
            D_801E9090.vy = work->pos18.vy;
            D_801E9090.vz = work->pos18.vz;
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
        work->pos14.vx = env->targets->centerX;
        work->pos14.vy = env->targets->centerY;
        fx_0aa_env = env;
        work->pos14.vz = env->targets->centerZ;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos14);
            break;
        case 4:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos14);
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos14);
            if (work->emit0 != NULL) {
                work->emit0->trails = (s32)D_801E8808;
                work->emit0->trailSlots = 8;
                work->emit0->trailStride = 0x110;
            }
            break;
        case 6:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos14);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos14);
            break;
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0aa/nonmatchings/fx_0aa", fx_0aa_headerPtr);
