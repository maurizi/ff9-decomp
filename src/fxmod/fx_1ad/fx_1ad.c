#include "fxmod/fx_1ad.h"

static s32 fx_1ad_op80ParticleMotion(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    s32 t = (frame << 12) / n;
    s32 row = (pt->slot - 1) % 6;
    s32 chainPos = t * fx_1ad_boneChains[row][0];
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1ad_env);
    s32 frac = chainPos & 0xFFF;
    s32 link = chainPos / 4096;
    SVECTOR from;
    SVECTOR to;
    SVECTOR point;

    ex->btlfx_getUnitBonePos(0x10, fx_1ad_boneChains[row][link + 1], (u16 *)&from);
    ex->btlfx_getUnitBonePos(0x10, fx_1ad_boneChains[row][link + 2], (u16 *)&to);
    ex->btlfx_lerpVec((s16 *)&from, (s16 *)&to, frac, (s16 *)&point);
    pos->vx = point.vx;
    pos->vy = point.vy;
    pos->vz = point.vz;
    pt->pos.vx = point.vx << 12;
    pt->pos.vy = point.vy << 12;
    pt->pos.vz = point.vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_1ad_op80(s32 mode, Fx1adWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    MATRIX boneMtx;
    Pool6CEntry *emitter;
    s32 frame;
    s32 pulse;
    s32 t;
    s32 otz;
    s32 tick;

    if (mode == 0) {
        *args = 0x28;
    } else if (mode == 1) {
        work->done0 = 0;
        work->frame4 = 0;
        fx_1ad_env = env;
        ex->btlfx_initEffModels(fx_1ad_effModels, 1);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        work->model24 = (s16)ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_6_LEGS_POD), 1);
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitBonePos(0x10, 0xF, (u16 *)&work->pos8);
        pos.vx = work->pos8.vx;
        pos.vy = work->pos8.vy;
        pos.vz = work->pos8.vz;
        if (work->done0 == 0) {
            otz = 0x40;
            tick = work->frame4;
            ex->btlfx_drawScrollMove(tick * 4, 0x200, 0x100, 0x20, otz, ((tick & 3) << 5) + 0x240, ((tick * 16) & 0xC0) + 0x100);
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_6_LEGS_BACKDROP), work->frame4);
            if (frame == 0) {
                work->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_6_LEGS_GREEN_TRAIL), (s32)&work->pos10);
                if (work->emit20 != NULL) {
                    work->emit20->trails = (s32)fx_1ad_op80Trails0;
                    work->emit20->trailSlots = 0x48;
                    work->emit20->trailStride = 0x3C;
                }
                if (work->emit20 != NULL) {
                    work->emit20->otZ = otz;
                }
                if (work->emit20 != NULL) {
                    work->emit20->file->m.module.update = fx_1ad_op80ParticleMotion;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_6_LEGS_BUBBLE_TRAIL), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_1ad_op80Trails1;
                    emitter->trailSlots = 0x10;
                    emitter->trailStride = 0x6C;
                    emitter->otZ = 0x80;
                }
            }
            if (frame >= 0xC) {
                t = ((frame - 0xC) << 12) / 57;
                pulse = ex->math_cos((frame * 0x52 + 0x200) & 0x3FE);
                scale.vz = 0x1800;
                scale.vx = scale.vy = pulse / 3 + 0x1800;
                ex->btlfx_getUnitBoneMatrix(0x10, 0x10, (BtlBone *)&boneMtx);
                ex->btlfx_matrixAngles(&boneMtx, (s16 *)&rot);
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model24);
                ex->Hi_ModifyEffModelRGB(work->model24, (ex->math_sin(t >> 1) >> 5) - 0x80,
                                         (ex->math_sin(t >> 1) >> 5) - 0x80, (ex->math_sin(t >> 1) >> 5) - 0x80);
            }
            if (frame >= 0x45) {
                work->done0 = 1;
                ex->btlfx_clearEffModels();
                return 1;
            }
        }
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1ad/nonmatchings/fx_1ad", fx_1ad_headerPtr);
