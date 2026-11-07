#include "fxmod/fx_04f.h"

static s32 fx_04f_op80PoolTask(s32 mode, s32 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx04fWork *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ahead;
    SVECTOR rot;
    VECTOR scale;
    VECTOR pos;
    s32 ang;
    s32 bright;
    s32 size;
    s32 *baseSizePtr;
    s32 baseSize;

    if (mode == 1) {
        *it = 0;
    } else {
        ang = ((frame << 12) / FX_REFLECT_BAND_FRAMES) >> 1;
        bright = ex->math_sin(ang);
        if (frame < 5) {
            baseSize = ang + 0x200;
            size = ((ex->math_sin((frame << 10) >> 2) * (work->scaleX4C >> 1)) >> 12) + baseSize;
        } else {
            baseSize = ang + 0x200;
            size = (work->scaleX4C >> 1) + *(baseSizePtr = &baseSize); /* the address taken changes baseSize's register (permuter) */
        }
        scale.vy = size;
        scale.vx = size;
        scale.vz = (work->scaleZ50 * 7) >> 3;
        BTLFX_COPY_SVEC(&rot, &work->rot38);
        rot.vy += 0x800;
        ex->btlfx_pointAhead((s16 *)&work->pos14, (s16 *)&rot, *it, (s16 *)&ahead);
        pos.vx = ahead.vx;
        pos.vy = ahead.vy;
        pos.vz = ahead.vz;
        *it -= 0x18;
        ex->Hi_DrawEffModel(&work->rot38, &pos, (s32)&scale, work->model4);
        ex->Hi_ModifyEffModelRGB(work->model4, (bright >> 5) - 0x80, (bright >> 5) - 0x80, (bright >> 5) - 0x80);
        if (frame >= FX_REFLECT_BAND_FRAMES) {
            return 1;
        }
    }
    return 0;
}

s32 fx_04f_op80(s32 mode, Fx04fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 r;
    s32 g;
    s32 b;
    s32 glow;
    s32 growth;
    VECTOR scale;
    SVECTOR emitPos;
    s32 ratio;

    if (mode == 0) {
        init->workSize = sizeof(Fx04fWork);
        init->elemSize = sizeof(s32);
        init->elemFn = (s32)fx_04f_op80PoolTask;
        init->poolCount = FX_REFLECT_BAND_COUNT;
    } else {
        if (mode == 1) {
        fx_04f_env = env;
        ex->btlfx_initEffModels(fx_04f_effModels, FX_REFLECT_MODEL_COUNT);
        work->model4 = (s16)ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_REFLECT_GLOW_BAND));
        work->modelC = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_REFLECT_STAR), FX_REFLECT_TPAGE, FX_REFLECT_CLUT, 0, FX_REFLECT_STAR_V);
        work->model10 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_REFLECT_PANELS), FX_REFLECT_TPAGE, FX_REFLECT_CLUT, 0, 0);
        work->state0 = FX_REFLECT_STATE_OPEN;
        work->frame48 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
        ex->btlfx_getUnitRot(0, (s16 *)&work->rot38);
        work->radius40 = ex->btlfx_getUnitRadius(0);
        ex->btlfx_pointAhead((s16 *)&work->pos14, (s16 *)&work->rot38, work->radius40, (s16 *)&work->pos14);
        ex->btlfx_pointAhead((s16 *)&work->pos14, (s16 *)&work->rot38, 0x180, (s16 *)&work->pos24);
        work->rot38.vy += 0x800;
        work->pos2C[0] = work->pos14.vx;
        work->pos2C[1] = work->pos14.vy;
        work->pos2C[2] = work->pos14.vz;
        BTLFX_COPY_SVEC(&work->pos1C, &work->pos14);
    } else {
        r = 0;
        g = 0;
        b = 0;
        glow = 0;
        frame = entry->frame;
        switch (work->state0) {
        case FX_REFLECT_STATE_OPEN:
            r = frame << 9;
            b = 0;
            g = 0;
            scale.vx = scale.vy = ((0x2000 - ex->math_sin(r >> 2)) * 0xC00) >> 12;
            scale.vz = 0;
            if (frame >= FX_REFLECT_OPEN_FRAMES) {
                work->state0 = FX_REFLECT_STATE_GLOW;
                entry->frame = -1;
                work->emit44 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REFLECT_HALO_RINGS), (s32)&work->pos1C);
                if (work->emit44 != NULL) {
                    work->emit44->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            break;
        case FX_REFLECT_STATE_GLOW:
            ratio = (frame << 12) / FX_REFLECT_GLOW_FRAMES;
            if (frame < FX_REFLECT_WHITEN_FRAMES) {
                r = 0x1000;
                b = (frame << 12) / FX_REFLECT_WHITEN_FRAMES;
                g = b;
            } else {
                b = 0x1000;
                g = 0x1000;
                r = 0x1000;
            }
            glow = ratio;
            growth = glow * 0xC00;
            scale.vy = (growth >> 12) + 0xC00;
            scale.vx = (growth >> 12) + 0xC00;
            scale.vz = growth >> 11;
            if (frame % FX_REFLECT_BAND_INTERVAL == 0) {
                ex->btlfx_poolSpawn((s16 *)entry->pool);
            }
            if (work->emit44 != NULL) {
                if (work->emit44->state == BTLFX_EMIT_DONE) {
                    work->emit44 = NULL;
                } else {
                    work->rot38.vy += 0x800;
                    ex->btlfx_pointAhead((s16 *)&work->pos14, (s16 *)&work->rot38, frame * 0x12, (s16 *)&emitPos);
                    work->rot38.vy += 0x800;
                    ex->btlfx_moveEmitterTo((s32)work->emit44, (s32)&emitPos);
                }
            }
            if (frame >= FX_REFLECT_GLOW_FRAMES) {
                work->state0 = FX_REFLECT_STATE_FADE;
                entry->frame = -1;
            }
            break;
        case FX_REFLECT_STATE_FADE:
            scale.vy = 0x1800;
            scale.vx = 0x1800;
            scale.vz = 0x1800;
            ratio = (frame << 12) / FX_REFLECT_FADE_FRAMES;
            b = 0x1000 - ratio;
            g = b;
            r = b;
            glow = b;
            if (frame >= FX_REFLECT_FADE_FRAMES + 1) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->scaleX4C = scale.vx;
        work->scaleZ50 = scale.vz;
        work->rot38.vz = 0;
        ex->Hi_DrawEffModel(&work->rot38, (VECTOR *)work->pos2C, (s32)&scale, work->modelC);
        ex->Hi_ModifyEffModelAbr(work->modelC, BTLFX_ABR_ADD);
        ex->Hi_ModifyEffModelRGB(work->modelC, (r >> 5) - 0x80, (g >> 5) - 0x80, (b >> 5) - 0x80);
        work->rot38.vz = -work->frame48 * 0x18;
        ex->Hi_DrawEffModel(&work->rot38, (VECTOR *)work->pos2C, (s32)&scale, work->model10);
        ex->Hi_ModifyEffModelAbr(work->model10, BTLFX_ABR_ADD);
        ex->Hi_ModifyEffModelRGB(work->model10, (glow * 2 / 3 >> 5) - 0x80, (glow * 2 / 3 >> 5) - 0x80, (glow * 2 / 3 >> 5) - 0x80);
        work->rot38.vz = 0;
        ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_REFLECT_SPARKLE_TEXTURE), work->frame48);
        ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_REFLECT_STREAK_TEXTURE), work->frame48);
        work->frame48++;
    }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_04f/nonmatchings/fx_04f", fx_04f_headerPtr);
