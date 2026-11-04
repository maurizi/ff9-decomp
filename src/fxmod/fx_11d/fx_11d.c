#include "fxmod/fx_11d.h"

static s32 fx_11d_op80ParticleMotion0(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    if (frame >= 4) {
        frame = 3;
    }
    return fxm_boneLerpParticle(out, pos, &fx_11d_boneKeys0[frame], frame, n);
}

static s32 fx_11d_op80ParticleMotion1(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    if (frame >= 4) {
        frame = 3;
    }
    return fxm_boneLerpParticle(out, pos, &fx_11d_boneKeys1[frame], frame, n);
}

static s32 fxm_boneLerpParticle(VECTOR *out, SVECTOR *pos, Fx11dPathKey *key, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_11d_env);
    SVECTOR boneA;
    SVECTOR boneB;
    SVECTOR mid;

    ex->btlfx_getUnitBonePos(0x10, key->bone0, (u16 *)&boneA);
    ex->btlfx_getUnitBonePos(0x10, key->bone2, (u16 *)&boneB);
    ex->btlfx_lerpVec((s16 *)&boneA, (s16 *)&boneB, key->blend4, (s16 *)&mid);
    mid.vz -= 0x32;
    pos->vx = mid.vx;
    pos->vy = mid.vy;
    pos->vz = mid.vz;
    out->vx = pos->vx << 12;
    out->vy = pos->vy << 12;
    out->vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_11d_op80(s32 mode, Fx11dWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x34;
    } else if (mode == 1) {
        fx_11d_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos1C);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ABSORB_TEAL_RING), (s32)&work->pos1C);
        if (work->emit0 != NULL) {
            work->emit0->otZ = -0x80;
        }
    } else {
        frame = entry->frame;
        if (frame >= 0 && frame < 0x18 && frame % 12 == 0) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ABSORB_RED_GLOW), (s32)&fx_11d_emitPos0);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ABSORB_RED_GLOW_2), (s32)&fx_11d_emitPos1);
            if (work->emit4 != NULL) {
                ((Fx11dEffHdr *)work->emit4->file)->motion8 = fx_11d_op80ParticleMotion0;
            }
            if (work->emit8 != NULL) {
                ((Fx11dEffHdr *)work->emit8->file)->motion8 = fx_11d_op80ParticleMotion1;
            }
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x100);
            }
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0x10, 0x100);
            }
        }
        if (frame >= 0 && frame < 0x18 && frame % 12 == 0) {
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ABSORB_TRAIL), (s32)&fx_11d_emitPos0);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ABSORB_TRAIL_2), (s32)&fx_11d_emitPos1);
            if (work->emitC != NULL) {
                work->emitC->trails = (s32)fx_11d_op80Trails0;
                work->emitC->trailSlots = 4;
                work->emitC->trailStride = 0x70;
            }
            if (work->emit10 != NULL) {
                work->emit10->trails = (s32)fx_11d_op80Trails1;
                work->emit10->trailSlots = 4;
                work->emit10->trailStride = 0x70;
            }
            if (work->emitC != NULL) {
                ((Fx11dEffHdr *)work->emitC->file)->motion8 = fx_11d_op80ParticleMotion0;
            }
            if (work->emit10 != NULL) {
                ((Fx11dEffHdr *)work->emit10->file)->motion8 = fx_11d_op80ParticleMotion1;
            }
            /* sets unkC twice; unk10 keeps its default */
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0x10, 0x100);
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0x10, 0x100);
            }
        }
        switch (frame) {
        case 0:
            ex->btlfx_getUnitBonePos(0x10, 0x37, (u16 *)&work->pos24);
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ABSORB_VIOLET_BLOBS), (s32)&work->pos24);
            break;
        case 0x1E:
            ex->btlfx_getUnitBonePos(0x10, 0, (u16 *)&work->pos2C);
            work->pos2C.vy -= 0xC8;
            work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ABSORB_LENS_FLARE), (s32)&work->pos2C);
            if (work->emit18 != NULL) {
                work->emit18->trails = (s32)fx_11d_op80Trails2;
                work->emit18->trailSlots = 8;
                work->emit18->trailStride = 0x204;
            }
            break;
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_11d/nonmatchings/fx_11d", fx_11d_headerPtr);
