#include "fxmod/fx_0c9.h"

s32 fx_0c9_op80(s32 mode, Fx0c9Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = sizeof(Fx0c9Work80);
    } else if (mode == 1) {
        fx_0c9_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_JEWEL_GEM_SPARKLES), (s32)&work->pos8);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_JEWEL_COMET_STREAKS), (s32)&work->pos8);
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= FX_JEWEL_FRAMES;
    }
    return 0;
}

static s32 fx_0c9_op81ParticleMotion(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    pos->vx = fx_0c9_particlePos.vx;
    pos->vy = fx_0c9_particlePos.vy;
    pos->vz = fx_0c9_particlePos.vz;
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_0c9_op81(s32 mode, Fx0c9Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 elapsed;
    s32 t;

    if (mode == 0) {
        *args = sizeof(Fx0c9Work81);
    } else if (mode == 1) {
        fx_0c9_env = env;
        ex->btlfx_initEffModels(fx_0c9_effModels, 1);
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 1, (s16 *)&work->posC);
        work->posC.vy -= 0x50;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos14);
        work->pos1C.vx = work->pos14.vx;
        work->pos1C.vy = work->pos14.vy;
        work->pos1C.vz = work->pos14.vz;
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_JEWEL_LIGHT_BURST), (s32)&work->pos14);
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        work->rot24.vx = 0;
        work->rot24.vy = 0;
        work->rot24.vz = 0;
        work->pos2C.vx = 0;
        work->pos2C.vy = 0;
        work->pos2C.vz = 0;
        work->scale3C.vx = FX_JEWEL_GEM_SCALE;
        work->scale3C.vy = FX_JEWEL_GEM_SCALE;
        work->scale3C.vz = FX_JEWEL_GEM_SCALE;
        work->model54 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_JEWEL_GEM), FX_JEWEL_GEM_TPAGE, FX_JEWEL_GEM_CLUT, FX_JEWEL_GEM_U, FX_JEWEL_GEM_V);
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_JEWEL_GEM_FRAME:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_JEWEL_ORE_GLINT), (s32)&fx_0c9_emitPos);
            if (work->emit0 != NULL) {
                work->emit0->trails = (s32)fx_0c9_trails;
                work->emit0->trailSlots = FX_JEWEL_ORE_GLINT_TRAIL_SLOTS;
                work->emit0->trailStride = FX_JEWEL_ORE_GLINT_TRAIL_STRIDE;
            }
            if (work->emit0 != NULL) {
                work->emit0->file->m.module.update = fx_0c9_op81ParticleMotion;
            }
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x10);
            }
            break;
        case FX_JEWEL_GLOW_RING_FRAME:
            work->pos4C.vx = work->pos1C.vx;
            work->pos4C.vy = work->pos1C.vy;
            work->pos4C.vz = work->pos1C.vz;
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_JEWEL_GLOW_RING), (s32)&work->pos4C);
            break;
        }
        if (frame >= FX_JEWEL_GEM_FRAME) {
            elapsed = frame - FX_JEWEL_GEM_FRAME;
            if (elapsed < FX_JEWEL_FLIGHT_FRAMES) {
                work->pos2C.vx = work->pos1C.vx;
                work->pos2C.vy = work->pos1C.vy;
                work->pos2C.vz = work->pos1C.vz;
            }
            ex->Hi_DrawEffModel(&work->rot24, &work->pos2C, (s32)&work->scale3C, work->model54);
            ex->Hi_ModifyEffModelAbr(work->model54, BTLFX_ABR_ADD);
            if (elapsed < FX_JEWEL_GROW_FRAMES) {
                work->scale3C.vx++;
                work->scale3C.vy++;
                work->scale3C.vz++;
            } else if (elapsed >= FX_JEWEL_SHRINK_FRAME) {
                work->scale3C.vx -= 8;
                work->scale3C.vy -= 8;
                work->scale3C.vz -= 8;
                if (work->scale3C.vx < 0) {
                    work->scale3C.vx = 0;
                    work->scale3C.vy = 0;
                    work->scale3C.vz = 0;
                }
            }
            if (elapsed < FX_JEWEL_FLIGHT_FRAMES) {
                fx_0c9_particlePos.vx = work->pos1C.vx;
                fx_0c9_particlePos.vy = work->pos1C.vy;
                fx_0c9_particlePos.vz = work->pos1C.vz;
                t = (elapsed << 12) / 15;
                ex->btlfx_lerpVec((s16 *)&work->pos14, (s16 *)&work->posC, t, (s16 *)&work->pos1C);
                work->pos1C.vy -= (ex->math_sin(t >> 1) * 0x258) >> 12;
            }
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_JEWEL_BACKDROP), elapsed);
        }
        if (frame >= FX_JEWEL_GEM_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0c9/nonmatchings/fx_0c9", fx_0c9_headerPtr);
