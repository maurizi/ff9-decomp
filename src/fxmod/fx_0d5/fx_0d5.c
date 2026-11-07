#include "fxmod/fx_0d5.h"

s32 fx_0d5_op80(s32 mode, Fx0d5Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    Pool6CEntry *emitter;
    Pool6CEntry *trailEmitter;
    s32 frame;
    s32 ringScale;
    s32 t;

    if (mode == 0) {
        *args = sizeof(Fx0d5Work80);
    } else if (mode == 1) {
        work->state0 = 0;
        fx_0d5_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRA_FLAME_BURST), (s32)&work->posC);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            if (frame < FX_SWDMAG_FIRA_GROUND_RING_FRAMES) {
                t = (frame << 12) / FX_SWDMAG_FIRA_GROUND_RING_FRAMES;
                if (frame < FX_SWDMAG_FIRA_RING_PULSE_FRAME) {
                    ringScale = ex->math_sin(frame << 6) / 4 + 0x800;
                } else {
                    ringScale = ex->math_sin(((frame - FX_SWDMAG_FIRA_RING_PULSE_FRAME) << 10) / (FX_SWDMAG_FIRA_GROUND_RING_FRAMES - FX_SWDMAG_FIRA_RING_PULSE_FRAME)) * 2 / 3 + 0xC00;
                }
                rot.vx = 0x400;
                rot.vz = 0;
                rot.vy = 0;
                rot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRA_GROUND_RING), &work->pos4, &rot, ringScale * 2 / 3, 0xC, BTLFX_ABR_ADD,
                                  ex->math_sin(t >> 1));
            }
            if (frame == FX_SWDMAG_FIRA_SMOKE_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRA_SMOKE), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame == FX_SWDMAG_FIRA_FLAME_SWIRL_FRAME) {
                trailEmitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRA_FLAME_SWIRL), (s32)&work->pos4);
                if (trailEmitter != NULL) {
                    trailEmitter->otZ = 0x80;
                    trailEmitter->trails = (s32)fx_0d5_trails;
                    trailEmitter->trailSlots = FX_SWDMAG_FIRA_FLAME_SWIRL_TRAIL_SLOTS;
                    trailEmitter->trailStride = FX_SWDMAG_FIRA_FLAME_SWIRL_TRAIL_STRIDE;
                }
                trailEmitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRA_FLAME_BURST_2), (s32)&work->posC);
                if (trailEmitter != NULL) {
                    trailEmitter->otZ = 0x80;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRA_HEAT_RING), (s32)&work->pos4);
            }
            if (frame == FX_SWDMAG_FIRA_SMOKE_2_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRA_SMOKE_2), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= FX_SWDMAG_FIRA_FRAMES) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 fx_0d5_op84(s32 mode, Fx0d5Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 alpha;
    s32 scale0;
    s32 scale1;
    s32 ang;
    s32 t;
    s32 flicker;
    s32 grow;
    u16 ry;

    if (mode == 0) {
        *args = sizeof(Fx0d5Work84);
    } else if (mode == 1) {
        fx_0d5_env = env;
        work->state0 = FX_SWDMAG_FIRA_FLASH_STATE_IN;
        work->frame4 = 0;
    } else {
        alpha = 0;
        scale0 = 0;
        scale1 = 0;
        frame = entry->frame;
        ex->btlfx_getWeaponPos(BTLFX_UNIT_CASTER, 1, (s16 *)&work->pos8);
        ry = 0;
        switch (work->state0) {
        case FX_SWDMAG_FIRA_FLASH_STATE_IN:
            ry = 0x200;
            alpha = (frame << 12) / FX_SWDMAG_FIRA_FLASH_IN_FRAMES;
            t = ex->math_sin(alpha >> 2);
            scale0 = t >> 1;
            scale1 = ((0x1000 - alpha) >> 1) + 0x800;
            if (frame >= FX_SWDMAG_FIRA_FLASH_IN_FRAMES) {
                work->state0 = FX_SWDMAG_FIRA_FLASH_STATE_FLICKER;
                entry->frame = -1;
            }
            break;
        case FX_SWDMAG_FIRA_FLASH_STATE_FLICKER:
            t = (frame << 12) / FX_SWDMAG_FIRA_FLASH_FLICKER_FRAMES;
            flicker = (frame & 1) << 8;
            scale1 = flicker + 0x1000;
            grow = (t >> 1) + 0x800;
            alpha = flicker + grow;
            ry = (t >> 3) + 0x200;
            scale0 = scale1;
            if (frame >= FX_SWDMAG_FIRA_FLASH_FLICKER_FRAMES) {
                work->state0 = FX_SWDMAG_FIRA_FLASH_STATE_FADE;
                entry->frame = -1;
            }
            break;
        case FX_SWDMAG_FIRA_FLASH_STATE_FADE:
            t = (frame << 12) / FX_SWDMAG_FIRA_FLASH_FADE_FRAMES;
            ang = t >> 2;
            ry = 0x400 - ang;
            alpha = ex->math_cos(ang);
            scale0 = ex->math_sin(ang) + 0x1000;
            scale1 = ex->math_sin(ang) * 3 / 2 + 0x1000;
            if (frame >= FX_SWDMAG_FIRA_FLASH_FADE_FRAMES) {
                return 1;
            }
            break;
        }
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = ry;
        rot.pad = 0;
        env->otZ = 0x80;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRA_SWORD_GLINT), &work->pos8, NULL, scale0 / 3, 0, BTLFX_ABR_ADD, alpha / 2);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_FIRA_SWORD_GLINT_2), &work->pos8, &rot, scale1 / 2, 0, BTLFX_ABR_ADD, alpha * 2 / 3);
        env->otZ = 0;
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0d5/nonmatchings/fx_0d5", fx_0d5_headerPtr);
