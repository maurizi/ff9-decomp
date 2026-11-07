#include "fxmod/fx_1fa.h"

s32 fx_1fa_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1fa_emitPos0.vx = 0;
        fx_1fa_emitPos0.vy = 0;
        fx_1fa_emitPos0.vz = -0x618;
        fx_1fa_env = env;
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_ORBIT_GLOW_2), (s32)&fx_1fa_emitPos0);
    } else {
        return entry->frame >= FX_CARBUNCLE_GLOW_FRAMES;
    }
    return 0;
}

#define LOAD(r, a, b) \
    fx_1fa_spiralSprites[r][0].sprite8 = (u8 *)ex->btlfx_getFileData(entry->file, a); \
    fx_1fa_spiralSprites[r][1].sprite8 = (u8 *)ex->btlfx_getFileData(entry->file, b); \
    fx_1fa_spiralSprites[r][2].sprite8 = (u8 *)ex->btlfx_getFileData(entry->file, b); \
    fx_1fa_spiralSprites[r][3].sprite8 = (u8 *)ex->btlfx_getFileData(entry->file, b);

s32 fx_1fa_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx1faRing *sprite;
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;
    s32 j;
    s32 fade;
    s32 scale;
    s32 offX;

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1fa_timer[0] = 0;
        fx_1fa_env = env;
        LOAD(0, 3, 0x12)
        LOAD(1, 8, 0xF)
        LOAD(2, 0x15, 0x10)
        LOAD(3, 0x16, 0xD)
        LOAD(4, 0x18, 0xE)
        LOAD(5, 0x19, 0xB)
        LOAD(6, 0x1A, 0xC)
        LOAD(7, 0x1B, 9)
        LOAD(8, 0x1C, 0xA)
        LOAD(9, 1, 6)
        LOAD(10, 4, 7)
        LOAD(11, 0x11, 5)
        for (i = 0; i < FX_CARBUNCLE_SPARK_ROWS; i++) {
            for (j = 0; j < FX_CARBUNCLE_SPARK_COLS; j++) {
                fx_1fa_spiralSprites[i][j].angleC = i * -0x2A9 + j * 0xC0;
                fx_1fa_spiralSprites[i][j].radius10 = j * 16 + 16;
                fx_1fa_spiralSprites[i][j].delay0 = i + 4 - j;
                fx_1fa_spiralSprites[i][j].timer4 = FX_CARBUNCLE_SPARK_FRAMES;
            }
        }
        fx_1fa_emitPos1.vx = env->camera->points[3].vx;
        fx_1fa_emitPos1.vy = env->camera->points[3].vy;
        fx_1fa_emitPos1.vz = env->camera->points[3].vz;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            fx_1fa_timer[0] = FX_CARBUNCLE_SPARKS_FRAMES;
            break;
        case FX_CARBUNCLE_VIOLET_RINGS_FRAME:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_VIOLET_RINGS), (s32)&fx_1fa_emitPos1);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
            break;
        case FX_CARBUNCLE_TARGET_RINGS_FRAME:
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&fx_1fa_emitPos2[i]);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_GREEN_RING), (s32)&fx_1fa_emitPos2[i]);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(i, 0x80);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_YELLOW_RED_RINGS), (s32)&fx_1fa_emitPos2[i]);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(i, 0x80);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_CLOCK_FACE), (s32)&fx_1fa_emitPos2[i]);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(i, 0x80);
                }
            }
            break;
        }
        if (fx_1fa_timer[0] != 0) {
            fx_1fa_timer[0]--;
            env->otZ = 0x200;
            for (i = 0; i < FX_CARBUNCLE_SPARK_ROWS; i++) {
                for (j = 0; j < FX_CARBUNCLE_SPARK_COLS; j++) {
                    sprite = (Fx1faRing *)(i * 0x50 + j * 0x14 + (s32)fx_1fa_spiralSprites);
                    if (sprite->delay0 != 0) {
                        sprite->delay0--;
                    } else if (sprite->timer4 != 0) {
                        sprite->angleC -= 0x40;
                        sprite->radius10 -= 2;
                        sprite->timer4--;
                        scale = (sprite->radius10 << 8) + 0x4000;
                        fade = sprite->timer4 << 9;
                        offX = (ex->math_sin(sprite->angleC) * sprite->radius10) >> 12;
                        ex->btlfx_drawParticles(sprite->sprite8, &fx_1fa_emitPos1, NULL, scale, 0, BTLFX_ABR_ADD, fade, offX,
                                                (ex->math_cos(sprite->angleC) * sprite->radius10) >> 12);
                    }
                }
            }
            env->otZ = 0;
        }
        return frame >= FX_CARBUNCLE_OP81_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1fa/nonmatchings/fx_1fa", fx_1fa_headerPtr);
