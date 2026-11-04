#include "fxmod/fx_183.h"

s32 fx_183_op80(s32 mode, Fx183Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        fx_183_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CHERRY_BLOSSOM_LIGHT_STREAK), (s32)&work->pos8);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CHERRY_BLOSSOM_SPARKLE_CIRCLE), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        s32 frame = entry->frame;

        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->rot10.vx;
                work->emit0->rot.vy = work->rot10.vy;
                work->emit0->rot.vz = work->rot10.vz;
            }
        }
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->rot10.vx;
                work->emit4->rot.vy = work->rot10.vy;
                work->emit4->rot.vz = work->rot10.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_183_op81(s32 mode, Fx183Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR dropPos;
    s32 frame;

    if (mode == 0) {
        *args = 0x4C;
    } else if (mode == 1) {
        fx_183_env = env;
        ex->btlfx_initEffModels(fx_183_effModels, 7);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos8);
        work->pos18.vx = work->pos8.vx;
        work->pos18.vy = work->pos8.vy;
        work->pos18.vz = work->pos8.vz;
        work->pos18.vy = -0x7D0;
        work->pos10.vx = env->targets->centerX;
        work->pos10.vy = env->targets->centerY;
        work->pos10.vz = env->targets->centerZ;
        ex->btlfx_aimAngles((s16 *)&work->pos18, (s16 *)&work->pos10, (s16 *)&work->rot20);
        work->rot20.vx = ex->ratan2(work->pos10.vy - work->pos18.vy, work->pos18.vz - work->pos10.vz);
        work->scale38.vx = 0x1000;
        work->scale38.vy = 0x1000;
        work->scale38.vz = 0x1000;
        work->model48 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_CHERRY_BLOSSOM_SPEAR), 1);
    } else {
        frame = entry->frame;
        if (frame == 4) {
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CHERRY_BLOSSOM_PINK_RING), (s32)&work->pos10);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CHERRY_BLOSSOM_PETAL), (s32)&work->pos10);
            if (work->emit4 != NULL) {
                work->emit4->otZ = 0x80;
            }
        }
        if (frame < 5) {
            ex->btlfx_lerpVec((s16 *)&work->pos18, (s16 *)&work->pos10, frame << 10, (s16 *)&dropPos);
            work->pos28.vx = dropPos.vx;
            work->pos28.vy = dropPos.vy;
            work->pos28.vz = dropPos.vz;
            ex->Hi_SetEffModelSlice(work->model48, work->pos28.vy);
            ex->Hi_DrawSliceEffModel(&work->rot20, &work->pos28, (s32)&work->scale38, work->model48);
        }
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_183_op82(s32 mode, Fx183Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sc;
    SVECTOR pathPos;
    s32 frame;
    s32 i;
    s32 j;
    s32 pathFrame;

    if (mode == 0) {
        *args = 0x5C;
    } else if (mode == 1) {
        work->pos40.vx = env->targets->centerX;
        work->pos40.vy = env->targets->centerY;
        fx_183_env = env;
        work->pos40.vz = env->targets->centerZ;
        work->rot4.vx = 0;
        work->rot4.vy = 0;
        work->rot4.vz = 0;
        work->scale1C.vx = 0x1000;
        work->scale1C.vy = 0x1000;
        work->scale1C.vz = 0x1000;
        work->posC.vx = 0;
        work->posC.vy = 0;
        work->posC.vz = 0;
        work->scale2C.vx = 0x1800;
        work->scale2C.vy = 0x1800;
        work->scale2C.vz = 0x1800;
        for (j = 0; j < 6; j++) {
            fxm_rotPosPathInit(&fx_183_rotPosPath[j], (u16 *)ex->btlfx_getFileData(entry->file, fx_183_pathFileIds[j]));
            work->model50[j] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_183_modelFileIds[j]), 1);
        }
        work->timer48 = -1;
        work->timer4C = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0xF:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CHERRY_BLOSSOM_PETAL_STORM), (s32)&work->pos40);
            if (work->emit0 != NULL) {
                work->emit0->otZ = 0x80;
            }
            break;
        case 0:
            work->timer48 = 4;
            break;
        case 0x12:
            work->timer4C = 8;
            break;
        }
        if (frame >= 0 && frame < 0x1A) {
            pathFrame = frame;
            if (work->timer48 >= 0 || work->timer4C >= 0) {
                if (work->timer48 >= 0) {
                    work->fade3C = fxm_lerpRatio(-0x80, 0, 4 - work->timer48, 4);
                    work->timer48--;
                }
                if (work->timer4C >= 0) {
                    work->fade3C = fxm_lerpRatio(0, -0x80, 8 - work->timer4C, 8);
                    work->timer4C--;
                }
            }
            for (i = 0; i < 6; i++) {
                fxm_rotPosPathGet(&fx_183_rotPosPath[i], pathFrame, &work->rot4, &pathPos);
                work->posC.vx = pathPos.vx;
                work->posC.vy = pathPos.vy;
                work->posC.vz = pathPos.vz;
                work->posC.vx += work->pos40.vx;
                work->posC.vy += work->pos40.vy;
                work->posC.vz += work->pos40.vz;
                if (i == 0) {
                    sc.vx = work->scale1C.vx;
                    sc.vy = work->scale1C.vy;
                    sc.vz = work->scale1C.vz;
                } else {
                    sc.vx = work->scale2C.vx;
                    sc.vy = work->scale2C.vy;
                    sc.vz = work->scale2C.vz;
                }
                ex->Hi_SetEffModelSlice(work->model50[i], work->posC.vy);
                ex->Hi_DrawSliceEffModel(&work->rot4, &work->posC, (s32)&sc, work->model50[i]);
                if (work->timer48 >= 0 || work->timer4C >= 0) {
                    ex->Hi_ModifyEffModelRGB(work->model50[i], work->fade3C, work->fade3C, work->fade3C);
                }
            }
        }
        if (frame >= 0x50) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_183_op83(s32 mode, Fx183Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        s32 unit = *args;
        work->unit14 = unit;
        fx_183_env = env;
        if (unit >= env->targets->targetCount) {
            return 1;
        }
        ex->btlfx_getUnitBodyPos(unit, 0, (s16 *)&work->posC);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CHERRY_BLOSSOM_PETAL_TRAIL), (s32)&work->posC);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CHERRY_BLOSSOM_WHITE_RING), (s32)&work->posC);
        work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CHERRY_BLOSSOM_PETAL_VORTEX), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_183_op83Trails[work->unit14];
            work->emit0->trailSlots = 0xC;
            work->emit0->trailStride = 0x50;
        }
        if (work->emit0 != NULL) {
            work->emit0->otZ = 0x80;
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = 0x80;
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_183_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_rotPosPathInit(Fx183Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx183Key *)data;
}

static s32 fxm_rotPosPathGet(Fx183Path *path, s32 t, SVECTOR *rot, SVECTOR *pos) {
    Fx183Key *key;
    s32 done = 1;

    if (t > path->count2 - 1) {
        t = path->count2 - 1;
    } else {
        done = 0;
    }
    key = &path->keys4[t];
    if (rot != NULL) {
        rot->vx = key->rot6[0];
        rot->vy = key->rot6[1];
        rot->vz = key->rot6[2];
    }
    if (pos != NULL) {
        pos->vx = key->pos0[0];
        pos->vy = key->pos0[1];
        pos->vz = key->pos0[2];
    }
    return done;
}

INCLUDE_RODATA("asm/us/fx_183/nonmatchings/fx_183", fx_183_headerPtr);
