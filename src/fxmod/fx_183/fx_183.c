#include "fxmod/fx_183.h"

s32 fx_183_op80(s32 mode, Fx183Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        fx_183_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x17), (s32)&work->pos8);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x18), (s32)&work->pos8);
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

s32 fx_183_op81(s32 mode, Fx183Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x4C;
    } else if (mode == 1) {
        fx_183_env = env;
        ex->btlfx_initEffModels(fx_183_effModels, 7);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos8);
        work->unk18.vx = work->pos8.vx;
        work->unk18.vy = work->pos8.vy;
        work->unk18.vz = work->pos8.vz;
        work->unk18.vy = -0x7D0;
        work->pos10.vx = env->targets->centerX;
        work->pos10.vy = env->targets->centerY;
        work->pos10.vz = env->targets->centerZ;
        ex->btlfx_aimAngles((s16 *)&work->unk18, (s16 *)&work->pos10, (s16 *)&work->rot20);
        work->rot20.vx = ex->ratan2(work->pos10.vy - work->unk18.vy, work->unk18.vz - work->pos10.vz);
        work->scale38.vx = 0x1000;
        work->scale38.vy = 0x1000;
        work->scale38.vz = 0x1000;
        work->model48 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x1D), 1);
    } else {
        frame = entry->frame;
        if (frame == 4) {
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->pos10);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos10);
            if (work->emit4 != NULL) {
                work->emit4->otZ = 0x80;
            }
        }
        if (frame < 5) {
            ex->btlfx_lerpVec((s16 *)&work->unk18, (s16 *)&work->pos10, frame << 10, (s16 *)&v);
            work->pos28.vx = v.vx;
            work->pos28.vy = v.vy;
            work->pos28.vz = v.vz;
            ex->Hi_SetEffModelSlice(work->model48, work->pos28.vy);
            ex->Hi_DrawSliceEffModel(&work->rot20, &work->pos28, (s32)&work->scale38, work->model48);
        }
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_183_op82(s32 mode, Fx183Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sc;
    SVECTOR v;
    s32 frame;
    s32 i;
    s32 j;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x5C;
    } else if (mode == 1) {
        work->pos40.vx = env->targets->centerX;
        work->pos40.vy = env->targets->centerY;
        fx_183_env = env;
        work->pos40.vz = env->targets->centerZ;
        work->rot4.vx = 0;
        work->rot4.vy = 0;
        work->rot4.vz = 0;
        work->unk1C.vx = 0x1000;
        work->unk1C.vy = 0x1000;
        work->unk1C.vz = 0x1000;
        work->posC.vx = 0;
        work->posC.vy = 0;
        work->posC.vz = 0;
        work->unk2C.vx = 0x1800;
        work->unk2C.vy = 0x1800;
        work->unk2C.vz = 0x1800;
        for (j = 0; j < 6; j++) {
            fxm_rotPosPathInit(&fx_183_rotPosPath[j], (u16 *)ex->btlfx_getFileData(entry->file, D_801E8294[j]));
            work->model50[j] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, D_801E8288[j]), 1);
        }
        work->unk48 = -1;
        work->unk4C = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0xF:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos40);
            if (work->emit0 != NULL) {
                work->emit0->otZ = 0x80;
            }
            break;
        case 0:
            work->unk48 = 4;
            break;
        case 0x12:
            work->unk4C = 8;
            break;
        }
        if (frame >= 0 && frame < 0x1A) {
            t = frame;
            if (work->unk48 >= 0 || work->unk4C >= 0) {
                if (work->unk48 >= 0) {
                    work->unk3C = fxm_lerpRatio(-0x80, 0, 4 - work->unk48, 4);
                    work->unk48--;
                }
                if (work->unk4C >= 0) {
                    work->unk3C = fxm_lerpRatio(0, -0x80, 8 - work->unk4C, 8);
                    work->unk4C--;
                }
            }
            for (i = 0; i < 6; i++) {
                fxm_rotPosPathGet(&fx_183_rotPosPath[i], t, &work->rot4, &v);
                work->posC.vx = v.vx;
                work->posC.vy = v.vy;
                work->posC.vz = v.vz;
                work->posC.vx += work->pos40.vx;
                work->posC.vy += work->pos40.vy;
                work->posC.vz += work->pos40.vz;
                if (i == 0) {
                    sc.vx = work->unk1C.vx;
                    sc.vy = work->unk1C.vy;
                    sc.vz = work->unk1C.vz;
                } else {
                    sc.vx = work->unk2C.vx;
                    sc.vy = work->unk2C.vy;
                    sc.vz = work->unk2C.vz;
                }
                ex->Hi_SetEffModelSlice(work->model50[i], work->posC.vy);
                ex->Hi_DrawSliceEffModel(&work->rot4, &work->posC, (s32)&sc, work->model50[i]);
                if (work->unk48 >= 0 || work->unk4C >= 0) {
                    ex->Hi_ModifyEffModelRGB(work->model50[i], work->unk3C, work->unk3C, work->unk3C);
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

s32 fx_183_op83(s32 mode, Fx183Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        s32 unit = *arg4;
        work->unk14 = unit;
        fx_183_env = env;
        if (unit >= env->targets->targetCount) {
            return 1;
        }
        ex->btlfx_getUnitBodyPos(unit, 0, (s16 *)&work->posC);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->posC);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->posC);
        work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1E), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_183_op83Trails[work->unk14];
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

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_183_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_rotPosPathInit(Fx183Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx183Key *)data;
}

static s32 fxm_rotPosPathGet(Fx183Path *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx183Key *k;
    s32 r = 1;

    if (i > path->unk2 - 1) {
        i = path->unk2 - 1;
    } else {
        r = 0;
    }
    k = &path->unk4[i];
    if (rot != NULL) {
        rot->vx = k->unk6[0];
        rot->vy = k->unk6[1];
        rot->vz = k->unk6[2];
    }
    if (pos != NULL) {
        pos->vx = k->unk0[0];
        pos->vy = k->unk0[1];
        pos->vz = k->unk0[2];
    }
    return r;
}

INCLUDE_RODATA("asm/us/fx_183/nonmatchings/fx_183", fx_183_headerPtr);
