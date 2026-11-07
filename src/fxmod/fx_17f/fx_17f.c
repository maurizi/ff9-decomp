#include "fxmod/fx_17f.h"

void fx_17f_op80MeltVertex(SVECTOR *v, s32 i) {
    v->vy -= fx_17f_vertexDrops[i];
    if (v->vy < 0) {
        v->vy = 0;
    }
    if (v->vy < FX_MELT_SPREAD_HEIGHT) {
        v->vx = v->vx * 0x5C / 90;
        v->vz = (s16)(v->vz - 0xC8) * 0x5C / 90 + 0xC8;
    }
}

s32 fx_17f_op80(s32 mode, Fx17fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    SVECTOR ang;
    s32 frame;
    s32 i;
    s32 fade;

    if (mode == 0) {
        *args = sizeof(Fx17fWork);
    } else if (mode == 1) {
        fx_17f_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos20);
        if (ex->btlfx_isEnemyUnit(BTLFX_UNIT_CASTER) != 0) {
            ex->btlfx_getUnitRot(BTLFX_UNIT_CASTER, (s16 *)&work->rot14);
            ex->btlfx_getUnitScale(BTLFX_UNIT_CASTER, (VECTOR *)work->scale0);
            ex->btlfx_getUnitPos(BTLFX_UNIT_CASTER, (s16 *)&work->posC);
            ex->btlfx_setPartnerByArg(0);
            ex->btlfx_fadeUnitById(BTLFX_UNIT_CASTER, -1);
            work->model1C = (BtlModelHeader *)ex->btlfx_swapModelData(0xE, fx_17f_modelBackup);
            ex->btlfx_fadeUnitById(BTLFX_UNIT_PARTNER, 0x1000);
            ex->btlfx_fadeUnitById(BTLFX_UNIT_PARTNER, -2);
            ex->btlfx_setUnitRot(BTLFX_UNIT_PARTNER, (s16 *)&work->rot14);
            ex->btlfx_setUnitScale(BTLFX_UNIT_PARTNER, work->scale0);
            ex->btlfx_aimAngles((s16 *)&work->posC, (s16 *)&work->pos20, (s16 *)&ang);
            ex->btlfx_pointAhead((s16 *)&work->posC, (s16 *)&ang, -0x104, (s16 *)&work->posC);
            ex->btlfx_setUnitPos(BTLFX_UNIT_PARTNER, (s16 *)&work->posC);
        }
        work->pos20.vx = (work->pos20.vx + work->posC.vx) / 2;
        work->pos20.vy = (work->pos20.vy + work->posC.vy) / 2 - 0x11C;
        work->pos20.vz = (work->pos20.vz + work->posC.vz) / 2;
        for (i = 0; i < FX_MELT_VERTICES; i++) {
            fx_17f_vertexDrops[i] = (ex->btlfx_rand() & 7) + 4;
        }
    } else {
        frame = entry->frame;
        ex->btl_forEachModelVertex(work->model1C, fx_17f_op80MeltVertex);
        if (frame >= FX_MELT_FADE_FRAME) {
            fade = 0x80 - (frame - FX_MELT_FADE_FRAME) * 8;
            if (fade < 0x10) {
                fade = 0x10;
            }
            ex->btlfx_fadeUnitById(BTLFX_UNIT_PARTNER, fade);
        }
        if (frame == FX_MELT_SPLASH_FRAME) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MELT_GOO_SPLASH), (s32)&work->pos20);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
        }
        if (frame == FX_MELT_BLOB_FRAME) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MELT_GOO_BLOB), (s32)&work->pos20);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MELT_PINK_SMOKE), (s32)&work->pos20);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
        }
        if (frame == FX_MELT_RIPPLE_FRAME) {
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MELT_GOO_BLOB_2), (s32)&work->posC);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MELT_GOO_RIPPLE), (s32)&work->posC);
        }
        work->pos20.vy += 5;
        if (frame >= FX_MELT_FRAMES) {
            ex->btlfx_fadeUnitById(BTLFX_UNIT_PARTNER, -1);
            ex->btlfx_swapModelData(0xE, fx_17f_modelBackup);
            work->posC.vx = 0;
            work->posC.vy = 0;
            work->posC.vz = FX_MELT_HIDE_Z;
            ex->btlfx_setUnitPos(BTLFX_UNIT_PARTNER, (s16 *)&work->posC);
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17f/nonmatchings/fx_17f", fx_17f_headerPtr);
