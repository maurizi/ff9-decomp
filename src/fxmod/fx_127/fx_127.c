#include "fxmod/fx_127.h"

static s32 fx_127_op80ParticleMotion0(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_127_env);
    s32 ret = fxm_lerpParticle(out, pos, &D_801E82F8, &D_801E8300, 5, frame, n);

    D_801E8308.vx = pos->vx;
    D_801E8308.vy = 0;
    D_801E8308.vz = pos->vz;
    if (fx_127_emitter != NULL) {
        if (fx_127_emitter->state == BTLFX_EMIT_DONE) {
            fx_127_emitter = NULL;
        }
    }
    ex->btlfx_moveEmitterTo((s32)fx_127_emitter, (s32)&D_801E8308);
    return ret;
}

static s32 fx_127_op80ParticleMotion1(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_lerpParticle(out, pos, &D_801E82F8, &D_801E8300, 5, frame, n);
}

static s32 fxm_lerpParticle(VECTOR *out, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 len, s32 frame, s32 n) {
    BTLFX_EXPORTS(fx_127_env)->btlfx_lerpVec((s16 *)from, (s16 *)to, (frame << 12) / len, (s16 *)pos);
    out->vx = pos->vx << 12;
    out->vy = pos->vy << 12;
    out->vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_127_op80(s32 mode, Fx127Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m1;
    MATRIX m2;
    MATRIX m3;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x84;
    } else if (mode == 1) {
        fx_127_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos24);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot2C);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos34);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos3C);
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitBoneMatrix(0x10, 0x3C, (BtlBone *)&m1);
        ex->btlfx_matrixPointAhead(&m1, 0, (s32)&work->unk44);
        ex->btlfx_aimAngles((s16 *)&work->unk44, (s16 *)&work->pos34, (s16 *)&work->unk74);
        switch (frame) {
        case 0xA:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&fx_127_emitPos0);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&fx_127_emitPos0);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&fx_127_emitPos0);
            work->pos4C.vx = work->unk44.vx;
            work->pos4C.vy = work->unk44.vy;
            work->pos4C.vz = work->unk44.vz;
            work->pos4C.vy = 0;
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos4C);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 0x22:
            ex->btlfx_aimAngles((s16 *)&work->pos34, (s16 *)&work->unk44, (s16 *)&work->unk54);
            work->unk5C.vx = work->unk54.vx;
            work->unk5C.vy = work->unk54.vy;
            work->unk5C.vz = work->unk54.vz;
            work->unk5C.vy += 0x800;
            D_801E8310.vx = work->unk5C.vx;
            D_801E8310.vy = work->unk5C.vy;
            D_801E8310.vz = work->unk5C.vz;
            ex->btlfx_getUnitBoneMatrix(0x10, 0x3C, (BtlBone *)&m2);
            ex->btlfx_matrixPointAhead(&m2, 0, (s32)&work->unk64);
            ex->btlfx_getUnitBoneMatrix(0x10, 0x3C, (BtlBone *)&m3);
            ex->btlfx_matrixPointAhead(&m3, 0, (s32)&D_801E82F8);
            D_801E8300.vx = work->pos34.vx;
            D_801E8300.vy = work->pos34.vy;
            D_801E8300.vz = work->pos34.vz;
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&fx_127_emitPos1);
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&fx_127_emitPos2);
            fx_127_emitter = work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&fx_127_emitPos3);
            if (work->emit10 != NULL) {
                work->emit10->trails = (s32)fx_127_op80Trails0;
                work->emit10->trailSlots = 0x112;
                work->emit10->trailStride = 0x18;
            }
            if (work->emit14 != NULL) {
                work->emit14->trails = (s32)fx_127_op80Trails1;
                work->emit14->trailSlots = 0x28;
                work->emit14->trailStride = 0x40;
            }
            if (work->emit10 != NULL) {
                ((Fx127EffHdr *)work->emit10->file)->unk8 = fx_127_op80ParticleMotion0;
            }
            if (work->emit14 != NULL) {
                ((Fx127EffHdr *)work->emit14->file)->unk8 = fx_127_op80ParticleMotion1;
            }
            if (work->emit10 != NULL) {
                work->emit10->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            if (work->emit14 != NULL) {
                work->emit14->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            break;
        case 0x27:
            ex->btlfx_pointAhead((s16 *)&work->pos34, (s16 *)&work->unk5C, -0xC8, (s16 *)&work->unk6C);
            D_801E8318.vx = work->unk6C.vx;
            D_801E8318.vy = work->unk6C.vy;
            D_801E8318.vz = work->unk6C.vz;
            break;
        }
        if (frame >= 0xA && frame < 0xE) {
            if (work->emit0 != NULL && work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit0, (s32)&work->unk44);
            if (work->emit4 != NULL && work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit4, (s32)&work->unk44);
            if (work->emit8 != NULL && work->emit8->state == BTLFX_EMIT_DONE) {
                work->emit8 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit8, (s32)&work->unk44);
            ex->btlfx_aimAngles((s16 *)&work->unk44, (s16 *)&work->pos34, (s16 *)&work->unk74);
            work->unk7C.vx = work->unk74.vx;
            work->unk7C.vy = work->unk74.vy;
            work->unk7C.vz = work->unk74.vz;
            work->unk7C.vy += 0x800;
        }
        if (frame >= 0xA) {
            if (work->emit0 != NULL) {
                if (work->emit0->state == BTLFX_EMIT_DONE) {
                    work->emit0 = NULL;
                }
                if (work->emit0 != NULL) {
                    work->emit0->rot.vx = work->unk74.vx;
                    work->emit0->rot.vy = work->unk74.vy;
                    work->emit0->rot.vz = work->unk74.vz;
                }
            }
            if (work->emit4 != NULL) {
                if (work->emit4->state == BTLFX_EMIT_DONE) {
                    work->emit4 = NULL;
                }
                if (work->emit4 != NULL) {
                    work->emit4->rot.vx = work->unk74.vx;
                    work->emit4->rot.vy = work->unk74.vy;
                    work->emit4->rot.vz = work->unk74.vz;
                }
            }
            if (work->emit8 != NULL) {
                if (work->emit8->state == BTLFX_EMIT_DONE) {
                    work->emit8 = NULL;
                }
                if (work->emit8 != NULL) {
                    work->emit8->rot.vx = work->unk7C.vx;
                    work->emit8->rot.vy = work->unk7C.vy;
                    work->emit8->rot.vz = work->unk7C.vz;
                }
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

s32 fx_127_op81(s32 mode, Fx127Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x40;
    } else if (mode == 1) {
        fx_127_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        work->unk18.vx = D_801E8310.vx;
        work->unk18.vy = D_801E8310.vy;
        work->unk18.vz = D_801E8310.vz;
        work->unk20.vx = D_801E8310.vx;
        work->unk20.vy = D_801E8310.vy;
        work->unk20.vz = D_801E8310.vz;
        work->pos30.vx = work->pos10.vx;
        work->pos30.vy = work->pos10.vy;
        work->pos30.vz = work->pos10.vz;
        work->unk20.vy += 0x800;
        work->unk28.vx = D_801E8318.vx;
        work->unk28.vy = D_801E8318.vy;
        work->unk28.vz = D_801E8318.vz;
        work->pos38.vx = work->pos10.vx;
        work->pos38.vy = work->pos10.vy;
        work->pos38.vz = work->pos10.vz;
        work->pos38.vy = 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos30);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->pos38);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 4:
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos38);
            break;
        }
        if (frame >= 0) {
            if (work->emit0 != NULL) {
                if (work->emit0->state == BTLFX_EMIT_DONE) {
                    work->emit0 = NULL;
                }
                if (work->emit0 != NULL) {
                    work->emit0->rot.vx = work->unk18.vx;
                    work->emit0->rot.vy = work->unk18.vy;
                    work->emit0->rot.vz = work->unk18.vz;
                }
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_127/nonmatchings/fx_127", fx_127_headerPtr);
