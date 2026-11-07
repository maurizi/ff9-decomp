#include "fxmod/fx_0f7.h"

s32 fx_0f7_op80(s32 mode, Fx0f7Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0f7Spark *spark;
    s32 frame;
    s32 i;
    s32 alpha;
    s32 unused;

    if (mode == 0) {
        *args = sizeof(Fx0f7Work);
    } else if (mode == 1) {
        work->emitC = NULL;
        work->emit10 = NULL;
        work->emit14 = NULL;
        work->timer0 = 0;
        fx_0f7_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SOLUTION_9_RUNE_RINGS), (s32)&work->pos4);
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            work->timer0 = FX_SOLUTION_9_RUNES_FRAMES;
            break;
        case FX_SOLUTION_9_HALO_RINGS_FRAME:
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SOLUTION_9_HALO_RINGS), (s32)&work->pos4);
            if (work->emit10 != NULL) {
                work->emit10->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case FX_SOLUTION_9_BLUE_RINGS_FRAME:
            ex->btlfx_stopEmitter(work->emitC);
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SOLUTION_9_BLUE_RINGS), (s32)&work->pos4);
            if (work->emit14 != NULL) {
                work->emit14->otZ = ex->btlfx_addRadiusSixth(0, 0xC0);
            }
            break;
        }
        if (work->emitC != NULL && work->emitC->state == BTLFX_EMIT_DONE) {
            work->emitC = NULL;
        }
        if (work->emit14 != NULL && work->emit14->state == BTLFX_EMIT_DONE) {
            work->emit14 = NULL;
        }
        if (work->emitC != NULL) {
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_SOLUTION_9_BACKDROP), frame);
        }
        if (work->emit14 != NULL) {
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_SOLUTION_9_BACKDROP_2), frame);
        }
        if (work->timer0 != 0) {
            work->timer0--;
            for (i = 0; i < FX_SOLUTION_9_RUNE_COUNT; i++) {
                spark = &fx_0f7_sparks[i];
                if (spark->delay0 == 0) {
                    alpha = 0x1000;
                    spark->x10 += spark->speedX18;
                    spark->age1C++;
                    if (spark->age1C < FX_SOLUTION_9_RUNE_FADE_FRAMES) {
                        alpha = (spark->alphaC * spark->age1C) >> 3;
                    }
                    if (work->timer0 < FX_SOLUTION_9_RUNE_FADE_FRAMES) {
                        alpha = (spark->alphaC * work->timer0) >> 3;
                    }
                    if (spark->kind4 == 0) {
                        ex->btlfx_drawSpriteAtOt(spark->ot8, (u8 *)ex->btlfx_getFileData(entry->file, FX_SOLUTION_9_RUNES), spark->x10, spark->y14, 0,
                                          BTLFX_ABR_ADD, alpha);
                    } else {
                        ex->btlfx_drawSpriteAtOt(spark->ot8, (u8 *)ex->btlfx_getFileData(entry->file, FX_SOLUTION_9_RUNES_2), spark->x10, spark->y14,
                                          0, BTLFX_ABR_ADD, alpha);
                    }
                } else {
                    spark->delay0--;
                }
            }
        }
        return frame >= FX_SOLUTION_9_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0f7/nonmatchings/fx_0f7", fx_0f7_headerPtr);
