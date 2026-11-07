#include "fxmod/fx_1e6.h"

void fx_1e6_buildTrail(Fx1e6Trail *trail, s32 size, s32 start) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1e6_env);
    MATRIX m;
    VECTOR world;
    SVECTOR pathPos;
    SVECTOR *pt;
    s32 count;
    s32 i;
    s32 j;

    count = (size - 4) / 8;
    pt = trail->pos4;
    trail->lastSlot0 = trail->count2 = count - 1;
    trail->head1 = 0;
    if (start < count) {
        for (i = 0; i < count - start; i++) {
        fxm_posPathGet(&fx_1e6_posPath, 0, &pathPos);
        pathPos.vx += fx_1e6_trailOrigin.vx;
        pathPos.vy += fx_1e6_trailOrigin.vy;
        pathPos.vz += fx_1e6_trailOrigin.vz;
        pt->vx = pathPos.vx;
        pt->vy = pathPos.vy;
        pt->vz = pathPos.vz;
        pt->pad = 0;
        pt++;
        }
    } else {
        start = count;
    }
    for (i = 0; i < start;) {
        j = i + 1;
        fxm_posPathGet(&fx_1e6_posPath, start - j, &pathPos);
        pathPos.vx = pathPos.vx * 6 / 10;
        pathPos.vy = pathPos.vy * 6 / 10;
        pathPos.vz = pathPos.vz * 6 / 10;
        ex->math_rotMatrixYXZ(&fx_1e6_trailRot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&pathPos);
        gte_rtv0tr();
        gte_stlvnl(&world);
        world.vx += fx_1e6_trailOrigin.vx;
        world.vy += fx_1e6_trailOrigin.vy;
        world.vz += fx_1e6_trailOrigin.vz;
        pt->vx = world.vx;
        pt->vy = world.vy;
        pt->vz = world.vz;
        pt->pad = 0x80;
        pt++;
        i = j;
    }
}

s32 fx_1e6_op80(s32 mode, Fx1e6Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    u32 frame;

    if (mode == 0) {
        *args = sizeof(Fx1e6Work80);
    } else if (mode == 1) {
        fx_1e6_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos4);
        work->pos4.vy -= 0x12C;
        fx_1e6_trailOrigin.vx = work->pos4.vx;
        fx_1e6_trailOrigin.vy = work->pos4.vy;
        fx_1e6_trailOrigin.vz = work->pos4.vz;
        fxm_posPathInit(&fx_1e6_posPath, (u8 *)ex->btlfx_getFileData(entry->file, FX_CORRECT_PATH_KEYS));
        work->alphaC = 0x1000;
    } else {
        frame = entry->frame;
        ex->btlfx_aimAngles((s16 *)&env->camera->eye, (s16 *)&work->pos4, (s16 *)&fx_1e6_trailRot);
        if (frame < FX_CORRECT_TRAIL_FRAMES) {
            if ((s32)frame >= FX_CORRECT_TRAIL_FADE_FRAME) {
                work->alphaC -= 0x400;
            }
            fx_1e6_buildTrail(&fx_1e6_op80Trail, 4 + FX_CORRECT_TRAIL_POINTS * 8, frame * 4);
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_CORRECT_RED_TRAIL), 0, NULL, 0, 0x1000, work->alphaC, BTLFX_ABR_ADD, (u8 *)&fx_1e6_op80Trail, 0);
        }
        return (s32)frame >= FX_CORRECT_OP80_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1e6/nonmatchings/fx_1e6", fx_1e6_headerPtr);

s32 fx_1e6_op81(s32 mode, Fx1e6Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    s32 frame;
    s32 morph;
    s32 shakeFrame;
    s32 shakeStep;

    if (mode == 0) {
        *args = sizeof(Fx1e6Work81);
    } else if (mode == 1) {
        fx_1e6_env = env;
        ex->btlfx_initEffModels(fx_1e6_effModels, FX_CORRECT_MODEL_SLOTS);
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->pos48);
        work->scale34.vx = 0x1000;
        work->scale34.vy = 0x1000;
        work->scale34.vz = 0x1000;
        work->frame58 = 0;
        work->rot1C.vx = 0;
        work->rot1C.vy = 0;
        work->rot1C.vz = 0;
        work->pos24.vx = work->pos48.vx;
        work->pos24.vy = work->pos48.vy;
        work->pos24.vz = work->pos48.vz;
        work->pos24.vy = -0x3CA;
        work->model64 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_CORRECT_GOLD_BALL), 1);
        work->model66 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_CORRECT_GOLD_BALL_OPEN), 1);
        work->timer5C = -1;
        work->timer60 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_CORRECT_STREAMERS_FRAME:
            work->pos50.vx = work->pos48.vx;
            work->pos50.vy = work->pos48.vy;
            work->pos50.vz = work->pos48.vz;
            work->pos50.vy -= 0xFA;
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CORRECT_STREAMER), (s32)&work->pos50);
            work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CORRECT_STREAMER_2), (s32)&work->pos50);
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CORRECT_STREAMER_3), (s32)&work->pos50);
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CORRECT_SPIRAL_STREAMER), (s32)&work->pos50);
            work->emit0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CORRECT_SPIRAL_STREAMER_2), (s32)&work->pos50);
            work->emit0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CORRECT_SPIRAL_STREAMER_3), (s32)&work->pos50);
            work->emit0[6] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CORRECT_YELLOW_STARS), (s32)&work->pos50);
            if (work->emit0[0] != NULL) {
                work->emit0[0]->trails = (s32)fx_1e6_op81Trails0;
                work->emit0[0]->trailSlots = FX_CORRECT_STREAMER_TRAIL_SLOTS;
                work->emit0[0]->trailStride = FX_CORRECT_STREAMER_TRAIL_STRIDE;
            }
            if (work->emit0[1] != NULL) {
                work->emit0[1]->trails = (s32)fx_1e6_op81Trails1;
                work->emit0[1]->trailSlots = FX_CORRECT_STREAMER_TRAIL_SLOTS;
                work->emit0[1]->trailStride = FX_CORRECT_STREAMER_TRAIL_STRIDE;
            }
            if (work->emit0[2] != NULL) {
                work->emit0[2]->trails = (s32)fx_1e6_op81Trails2;
                work->emit0[2]->trailSlots = FX_CORRECT_STREAMER_TRAIL_SLOTS;
                work->emit0[2]->trailStride = FX_CORRECT_STREAMER_TRAIL_STRIDE;
            }
            if (work->emit0[3] != NULL) {
                work->emit0[3]->trails = (s32)fx_1e6_op81Trails3;
                work->emit0[3]->trailSlots = FX_CORRECT_SPIRAL_TRAIL_SLOTS;
                work->emit0[3]->trailStride = FX_CORRECT_SPIRAL_TRAIL_STRIDE;
            }
            if (work->emit0[4] != NULL) {
                work->emit0[4]->trails = (s32)fx_1e6_op81Trails4;
                work->emit0[4]->trailSlots = FX_CORRECT_SPIRAL_TRAIL_SLOTS;
                work->emit0[4]->trailStride = FX_CORRECT_SPIRAL_TRAIL_STRIDE;
            }
            if (work->emit0[5] != NULL) {
                work->emit0[5]->trails = (s32)fx_1e6_op81Trails5;
                work->emit0[5]->trailSlots = FX_CORRECT_SPIRAL_TRAIL_SLOTS;
                work->emit0[5]->trailStride = FX_CORRECT_SPIRAL_TRAIL_STRIDE;
            }
            break;
        case 0:
            work->timer5C = FX_CORRECT_FADE_IN_FRAMES;
            break;
        case FX_CORRECT_FADE_OUT_FRAME:
            work->timer60 = FX_CORRECT_FADE_OUT_FRAMES;
            break;
        }
        if ((u32)frame < FX_CORRECT_BALL_FRAMES) {
            shakeFrame = frame;
            if (frame >= FX_CORRECT_OPEN_FRAME) {
                if (work->frame58 >= FX_CORRECT_OPEN_FRAMES + 1) {
                    morph = 0;
                } else {
                    morph = 0x1000 - (work->frame58 << 12) / FX_CORRECT_OPEN_FRAMES;
                }
                work->frame58++;
            } else {
                morph = 0x1000;
            }
            rot.vx = work->rot1C.vx;
            rot.vy = work->rot1C.vy;
            rot.vz = work->rot1C.vz;
            pos.vx = work->pos24.vx;
            pos.vy = work->pos24.vy;
            pos.vz = work->pos24.vz;
            if ((u32)shakeFrame < FX_CORRECT_SHAKE_FRAMES) {
                pos.vy += frame * 4;
                work->pos24.vy = pos.vy;
                shakeStep = shakeFrame % FX_CORRECT_SHAKE_FRAMES; switch (shakeStep) {
                case 0:
                case 4:
                    break;
                case 1:
                case 3:
                    rot.vz = 0x155;
                    pos.vx -= 0x14;
                    break;
                case 2:
                    rot.vz = 0x200;
                    pos.vx -= 0x28;
                    break;
                case 5:
                case 7:
                    rot.vz = -0x155;
                    pos.vx += 0x14;
                    break;
                case 6:
                    rot.vz = -0x200;
                    pos.vx += 0x28;
                    break;
                }
            }
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&work->scale34, work->model64, work->model66, morph);
            if (work->timer5C >= 0 || work->timer60 >= 0) {
                if (work->timer5C >= 0) {
                    work->rgb44 = fxm_lerpRatio(-0x80, 0, FX_CORRECT_FADE_IN_FRAMES - work->timer5C, FX_CORRECT_FADE_IN_FRAMES);
                    work->timer5C--;
                }
                if (work->timer60 >= 0) {
                    work->rgb44 = fxm_lerpRatio(0, -0x80, FX_CORRECT_FADE_OUT_FRAMES - work->timer60, FX_CORRECT_FADE_OUT_FRAMES);
                    work->timer60--;
                }
                ex->Hi_ModifyEffModelAbr(work->model64, BTLFX_ABR_ADD);
                ex->Hi_ModifyEffModelRGB(work->model64, work->rgb44, work->rgb44, work->rgb44);
            }
        }
        if (frame >= FX_CORRECT_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_1e6_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_posPathInit(Fx1e6Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1e6Point *)data;
}

static s32 fxm_posPathGet(Fx1e6Path *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx1e6Point *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &path->keys4[t];
    out->vx = key->x;
    out->vy = key->y;
    out->vz = key->z;
    return done;
}
