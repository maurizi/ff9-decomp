#include "fxmod/fx_083.h"

INCLUDE_RODATA("asm/us/fx_083/nonmatchings/fx_083", fx_083_headerPtr);

s32 fx_083_op80(s32 mode, Fx083Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s16 tex[8];
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;
    s32 tint;
    s32 pillar;
    s32 ang;

    if (mode == 0) {
        *args = sizeof(Fx083Work);
    } else if (mode == 1) {
        fx_083_env = env;
        ex->btlfx_initEffModels(fx_083_effModels, FX_SCAN_MODEL_COUNT);
        for (i = FX_SCAN_TIMERS - 1; i >= 0; i--) {
            work->timers0[i] = 0;
        }
        tex[0] = tex[2] = 0xF8;
        tex[1] = 0x3DC0;
        tex[3] = 0x3D80;
        tex[4] = 0;
        tex[6] = 0;
        tex[5] = 0x80;
        tex[7] = 0;
        for (i = 0; i < FX_SCAN_RINGS; i++) {
            work->model28[i] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SCAN_METAL_SPHERE), FX_SCAN_SPHERE_TPAGE, FX_SCAN_SPHERE_CLUT,
                                                        FX_SCAN_SPHERE_U, FX_SCAN_SPHERE_V);
            work->ringRot38[i].vy = i << 10;
            work->ringRot38[i].vx = 0x100;
            work->ringRot38[i].vz = 0;
            ex->btlfx_getUnitPos(0, (s16 *)&work->emitStartPos180[i]);
        }
        work->ringScale98.vx = FX_SCAN_RING_SCALE;
        work->ringScale98.vy = FX_SCAN_RING_SCALE;
        work->ringScale98.vz = FX_SCAN_RING_SCALE;
        for (i = 0; i < FX_SCAN_PILLARS; i++) {
            work->modelB0[i] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SCAN_RED_BAND), FX_SCAN_BAND_TPAGE, FX_SCAN_BAND_CLUT,
                                                        FX_SCAN_BAND_U, 0);
            work->pillarOnC4[i] = 0;
            work->pillarFadeD8[i] = -1;
        }
        work->radius168 = ex->btlfx_getUnitRadius(0);
        work->height16C = ex->btlfx_getUnitHeight(0);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos160);
        work->pos160.vy = -0x80;
        work->pillarOnC4[0] = 1;
        work->rot140.vx = 0;
        work->rot140.vy = 0;
        work->rot140.vz = 0;
        work->fade13C = 0;
        work->orbit1C0 = 0;
        work->center158.vx = work->pos160.vx;
        work->center158.vy = work->pos160.vy - work->height16C;
        work->center158.vz = work->pos160.vz;
        work->pillarScale148.vx = work->radius168;
        work->pillarScale148.vz = work->radius168;
        work->radius168 += 0x100 + (work->radius168 >> 2);
        /* Store order AC, vy, vx, vz: sets the local-alloc lifetimes so vx takes a3 (original register choice). */
        work->ringHeightAC = work->center158.vy;
        work->pillarPosEC[0].vy = work->center158.vy;
        work->pillarPosEC[0].vx = work->center158.vx;
        work->pillarPosEC[0].vz = work->center158.vz;
        work->pillarScale148.vy = work->height16C / 3;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0x0:
            work->timers0[FX_SCAN_T_RINGS] = FX_SCAN_RINGS_FRAMES;
            work->timers0[FX_SCAN_T_RINGS_OPEN] = FX_SCAN_RINGS_OPEN_FRAMES;
            break;
        case FX_SCAN_PILLARS_FRAME:
            work->timers0[FX_SCAN_T_PILLARS] = FX_SCAN_PILLARS_FRAMES;
            work->fade13C = FX_SCAN_PILLAR_FADE_IN_FRAMES;
            break;
        case FX_SCAN_GLYPH_ANIM_FRAME:
            work->timers0[FX_SCAN_T_GLYPH_ANIM] = FX_SCAN_GLYPH_ANIM_FRAMES;
            break;
        case FX_SCAN_DESCEND_FRAME:
            work->timers0[FX_SCAN_T_DESCEND] = FX_SCAN_DESCEND_FRAMES;
            work->timers0[FX_SCAN_T_PILLAR_SPAWN] = FX_SCAN_DESCEND_FRAMES;
            work->emit170[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SCAN_GLYPH_TEXT), (s32)&work->emitStartPos180[0]);
            work->emit170[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SCAN_GLYPH_TEXT), (s32)&work->emitStartPos180[1]);
            work->emit170[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SCAN_GLYPH_TEXT), (s32)&work->emitStartPos180[2]);
            work->emit170[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SCAN_GLYPH_TEXT), (s32)&work->emitStartPos180[3]);
            work->timers0[FX_SCAN_T_GLYPHS] = FX_SCAN_GLYPHS_FRAMES;
            break;
        case FX_SCAN_ORBS_FRAME:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SCAN_ORB_TILES), (s32)&work->pos160);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x100);
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SCAN_YELLOW_RING), (s32)&work->pos160);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x100);
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SCAN_GLYPH_TEXT_2), (s32)&work->pos160);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x100);
            }
            break;
        case FX_SCAN_CLOSE_FRAME:
            work->timers0[FX_SCAN_T_RINGS_CLOSE] = FX_SCAN_RINGS_OPEN_FRAMES;
            break;
        case FX_SCAN_PILLAR_FADE_FRAME:
            work->pillarFadeD8[4] = FX_SCAN_PILLAR_FADE_FRAMES;
            break;
        case FX_SCAN_PILLAR_FADE_FRAME + 1:
            work->pillarFadeD8[3] = FX_SCAN_PILLAR_FADE_FRAMES;
            break;
        case FX_SCAN_PILLAR_FADE_FRAME + 2:
            work->pillarFadeD8[2] = FX_SCAN_PILLAR_FADE_FRAMES;
            break;
        case FX_SCAN_PILLAR_FADE_FRAME + 3:
            work->pillarFadeD8[1] = FX_SCAN_PILLAR_FADE_FRAMES;
            break;
        case FX_SCAN_PILLAR_FADE_FRAME + 4:
            work->pillarFadeD8[0] = FX_SCAN_PILLAR_FADE_FRAMES;
            break;
        }
        for (i = 0; i < FX_SCAN_RINGS; i++) {
            work->ringRot38[i].vy += 0x40;
        }
        work->orbit1C0 += 0x10;
        if (work->timers0[FX_SCAN_T_DESCEND] != 0) {
            work->timers0[FX_SCAN_T_DESCEND]--;
            work->ringHeightAC = ex->btlfx_lerp(work->pos160.vy, work->center158.vy, work->timers0[FX_SCAN_T_DESCEND] << 7);
        }
        if (work->timers0[FX_SCAN_T_RINGS_OPEN] != 0) {
            work->timers0[FX_SCAN_T_RINGS_OPEN]--;
            work->ringOffsetA8 = 0x1000 - ((ex->math_sin((FX_SCAN_RINGS_OPEN_FRAMES - work->timers0[FX_SCAN_T_RINGS_OPEN]) << 6) << 12) >> 12);
        }
        if (work->timers0[FX_SCAN_T_RINGS_CLOSE] != 0) {
            work->timers0[FX_SCAN_T_RINGS_CLOSE]--;
            work->ringOffsetA8 = 0x1000 - ((ex->math_sin(work->timers0[FX_SCAN_T_RINGS_CLOSE] << 6) << 12) >> 12);
        }
        if (work->timers0[FX_SCAN_T_RINGS] != 0) {
            work->timers0[FX_SCAN_T_RINGS]--;
            for (i = 0; i < FX_SCAN_RINGS; i++) {
                work->ringPos58[i].vx =
                    work->center158.vx + ((ex->math_sin(work->ringRot38[i].vy) * (work->radius168 + work->ringOffsetA8)) >> 12);
                work->ringPos58[i].vz =
                    work->center158.vz + ((ex->math_cos(work->ringRot38[i].vy) * (work->radius168 + work->ringOffsetA8)) >> 12);
                work->ringPos58[i].vy = work->ringHeightAC - ((ex->math_sin(0x200) * work->ringOffsetA8) >> 12);
                ex->Hi_DrawEffModel(&work->ringRot38[i], &work->ringPos58[i], (s32)&work->ringScale98, work->model28[i]);
            }
            env->camera->points[0].vx = work->ringPos58[0].vx;
            env->camera->points[0].vy = work->ringPos58[0].vy;
            env->camera->points[0].vz = work->ringPos58[0].vz;
            env->camera->pointRots[0].vx = 0x100;
            env->camera->pointRots[0].vy = work->ringRot38[0].vy;
            env->camera->pointRots[0].vz = 0;
        }
        if (work->timers0[FX_SCAN_T_GLYPHS] != 0) {
            work->timers0[FX_SCAN_T_GLYPHS]--;
            for (i = 0; i < FX_SCAN_RINGS; i++) {
                ang = i << 10;
                work->emitPos1A0[i].vx = work->center158.vx + ((ex->math_sin(ang + work->orbit1C0) *
                                                         (work->radius168 + work->ringOffsetA8 + 0x200)) >> 12);
                work->emitPos1A0[i].vz = work->center158.vz + ((ex->math_cos(ang + work->orbit1C0) *
                                                         (work->radius168 + work->ringOffsetA8 + 0x200)) >> 12);
                work->emitPos1A0[i].vy = work->ringPos58[0].vy;
                if (work->emit170[i] != NULL && work->emit170[i]->state == BTLFX_EMIT_DONE) {
                    work->emit170[i] = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->emit170[i], (s32)&work->emitPos1A0[i]);
            }
        }
        if (work->timers0[FX_SCAN_T_GLYPH_ANIM] != 0) {
            work->timers0[FX_SCAN_T_GLYPH_ANIM]--;
            ex->btlfx_drawScrollMove(0, FX_SCAN_GLYPH_VRAM_X, FX_SCAN_GLYPH_VRAM_Y, FX_SCAN_GLYPH_VRAM_W, FX_SCAN_GLYPH_VRAM_H, fx_083_texAnimSrc1[work->timers0[FX_SCAN_T_GLYPH_ANIM] >> 1].x0,
                              fx_083_texAnimSrc1[work->timers0[FX_SCAN_T_GLYPH_ANIM] >> 1].y4);
        }
        if (work->timers0[FX_SCAN_T_GLYPH_FLICKER] != 0) {
            work->timers0[FX_SCAN_T_GLYPH_FLICKER]--;
            ex->btlfx_drawScrollMove(0, FX_SCAN_GLYPH_VRAM_X, FX_SCAN_GLYPH_VRAM_Y, FX_SCAN_GLYPH_VRAM_W, FX_SCAN_GLYPH_VRAM_H, fx_083_texAnimSrc2[work->timers0[FX_SCAN_T_GLYPH_FLICKER]].x0, fx_083_texAnimSrc2[work->timers0[FX_SCAN_T_GLYPH_FLICKER]].y4);
        } else if (frame >= 0x15 && !(ex->btlfx_rand() & 7)) {
            work->timers0[FX_SCAN_T_GLYPH_FLICKER] = FX_SCAN_FLICKER_FRAMES;
        }
        if (work->timers0[FX_SCAN_T_PILLAR_SPAWN] != 0) {
            work->pillarPosEC[0].vy = work->ringHeightAC;
            if (!(work->timers0[FX_SCAN_T_PILLAR_SPAWN] & 7) && (pillar = work->timers0[FX_SCAN_T_PILLAR_SPAWN] >> 3) > 0) {
                work->pillarOnC4[pillar] = 1;
                work->pillarPosEC[pillar].vx = work->pillarPosEC[0].vx;
                work->pillarPosEC[pillar].vy = work->pillarPosEC[0].vy;
                work->pillarPosEC[pillar].vz = work->pillarPosEC[0].vz;
            }
            work->timers0[FX_SCAN_T_PILLAR_SPAWN]--;
        }
        if (work->timers0[FX_SCAN_T_PILLARS] != 0) {
            work->timers0[FX_SCAN_T_PILLARS]--;
            ex->btlfx_aimAngles((s16 *)&env->camera->eye, (s16 *)&work->center158, (s16 *)&work->rot140);
            work->rot140.vx = 0;
            work->rot140.vz = 0;
            for (i = 0; i < FX_SCAN_PILLARS; i++) {
                if (work->pillarOnC4[i] != 0) {
                    ex->Hi_DrawEffModel(&work->rot140, &work->pillarPosEC[i], (s32)&work->pillarScale148, work->modelB0[i]);
                    if (work->pillarFadeD8[i] > 0) {
                        work->pillarFadeD8[i]--;
                        tint = (work->pillarFadeD8[i] << 5) - 0x100;
                        ex->Hi_ModifyEffModelRGB(work->modelB0[i], tint, tint, tint);
                        if (work->pillarFadeD8[i] == 0) {
                            work->pillarOnC4[i] = 0;
                        }
                    }
                    if (i == 0 && work->fade13C != 0) {
                        work->fade13C--;
                        tint = -(work->fade13C * 0x10);
                        ex->Hi_ModifyEffModelRGB(work->modelB0[0], tint, tint, tint);
                    }
                }
            }
        }
        if (frame >= FX_SCAN_FRAMES) {
            return 1;
        }
        ex->btlfx_drawScrollMove(0x2710 - frame * 4, FX_SCAN_BAND_VRAM_X, FX_SCAN_BAND_VRAM_Y, FX_SCAN_BAND_VRAM_W, FX_SCAN_BAND_VRAM_H, FX_SCAN_BAND_SRC_X, FX_SCAN_BAND_SRC_Y);
    }
    return 0;
}
