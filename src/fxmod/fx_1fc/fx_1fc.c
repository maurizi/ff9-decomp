#include "fxmod/fx_1fc.h"

s32 fx_1fc_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        fx_1fc_env = env;
        ex->btlfx_initEffModels(fx_1fc_effModels, 0x20);
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitPos(i, (s16 *)&D_801EDEDC[i]);
            ex->btlfx_getUnitScale(i, (s32 *)&D_801EDF1C[i]);
        }
        ex->btlfx_drawBgOffscreen(0x240, 0x100, 0x80, 0x80, D_801EDADC, 0, 0x200, 0x2BC);
        fxm_camChanReset();
        fxm_camChanAdd(0, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0xE));
        fxm_camChanAdd(0, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0xC));
        fxm_camChanAdd(0, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0xA));
    } else {
        frame = entry->frame;
        env->camera->pointRots[1].vz = env->camera->points[2].vz;
        fxm_camChanUpdate(0);
        return frame >= 0x78;
    }
    return 0;
}

s32 fx_1fc_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        fx_1fc_env = env;
    } else {
        return entry->frame > 0;
    }
    return 0;
}

s32 fx_1fc_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        fx_1fc_env = env;
    } else {
        return entry->frame > 0;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1fc/nonmatchings/fx_1fc", fx_1fc_headerPtr);

s32 fx_1fc_op83(s32 mode, Fx1fcWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sum;
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    SVECTOR d;
    VECTOR e;
    s32 t;
    s32 frame;
    s32 i;
    s32 j;
    s32 k;
    s32 q;
    s32 k2;
    s32 f;
    s32 m0;
    s32 m1;
    u16 model;

    if (mode == 0) {
        *arg4 = sizeof(Fx1fcWork83);
    } else if (mode == 1) {
        work->rot8.vx = 0;
        work->rot8.vy = 0x800;
        work->rot8.vz = 0;
        work->unk10.vx = 0;
        work->unk10.vy = 0;
        work->unk10.vz = 0;
        work->unk20.vx = 0x1000;
        work->unk20.vy = 0x1000;
        work->unk20.vz = 0x1000;
        fx_1fc_env = env;
        fxm_rotPosPathInit((Fx1fcPath *)&fx_1fc_rotPosPath0, (u16 *)ex->btlfx_getFileData(entry->file, 0x13));
        work->model136[0] = ex->btlfx_registerEffectModel(&D_801E99A4, 1);
        work->rot8.vx = 0;
        work->rot8.vy = 0x800;
        work->rot8.vz = 0;
        work->unk10.vx = 0;
        work->unk10.vy = 0;
        work->unk10.vz = 0;
        work->unk20.vx = 0x1000;
        work->unk20.vy = 0x1000;
        work->unk20.vz = 0x1000;
        fxm_rotPosPathInit((Fx1fcPath *)&fx_1fc_rotPosPath1, (u16 *)ex->btlfx_getFileData(entry->file, 2));
        for (i = 0; i < 3; i++) {
            model = ex->Hi_RegisterTexPtrModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, D_801E8CC0[i]),
                                               D_801E8CC8, D_801E8CD8);
            work->model136[i + 1] = model;
        }
        work->rot30.vx = 0;
        work->rot30.vy = 0;
        work->rot30.vz = 0;
        work->unk38.vx = 0;
        work->unk38.vy = 0;
        work->unk38.vz = 0;
        work->unk48.vx = 0x1000;
        work->unk48.vy = 0x1000;
        work->unk48.vz = 0x1000;
        for (i = 0; i < 5; i++) {
            work->model136[i + 4] = ex->btlfx_registerEffectModel((Unk800E8F50 *)fxm_offsetTableEntry(D_801E8CE8, i), 1);
        }
        for (i = 0; i < 10; i++) {
            fxm_rotPosPathInit((Fx1fcPath *)&fx_1fc_rotPosPath2[i],
                          (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 6), i));
        }
        fxm_posPathInit((Fx1fcPath *)&fx_1fc_posPath, (u16 *)ex->btlfx_getFileData(entry->file, 0x17));
        fxm_vecPathInit((Fx1fcPath *)&fx_1fc_vecPath, (u16 *)ex->btlfx_getFileData(entry->file, 0x18));
        work->unk130 = -1;
        work->unk132 = -1;
        work->unk134 = 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 3:
            work->pos118.vx = -0x1C;
            work->pos118.vy = -0x267;
            work->pos118.vz = 0x342;
            work->unk128.vx = 0x1CF;
            work->unk128.vy = -0x7B7;
            work->unk128.vz = 0x27B;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos118);
            if (work->emit0 != NULL) {
                work->emit0->otZ = 0x118;
            }
            break;
        case 12:
            sum.vx = 0;
            sum.vy = 0;
            sum.vz = 0;
            for (q = 0; q < env->targets->targetCount; q++) {
                sum.vx += work->unk58[q].vx;
                sum.vy += (&work->unk58[q])->vy;
                sum.vz += (&work->unk58[q])->vz;
            }
            work->pos120.vx = sum.vx / env->targets->targetCount;
            work->pos120.vy = sum.vy / env->targets->targetCount;
            work->pos120.vz = sum.vz / env->targets->targetCount;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos120);
            break;
        case 38:
            ex->btlfx_stopEmitter(work->emit0);
            break;
        case 22:
            work->unk130 = 0x10;
            break;
        case 42:
            work->unk132 = 0x10;
            work->unk134 = 0;
            for (q = 0; q < env->targets->targetCount; q++) {
                ex->btlfx_setUnitScale(q, (s32 *)&D_801EDF1C[q]);
            }
            env->camera->points[0].vx = 0;
            env->camera->points[0].vy = 0;
            env->camera->points[0].vz = 0x4B0;
            break;
        }
        k = frame - 1;
        if ((u32)k < 0x18) {
            fxm_rotPosPathGet((Fx1fcPath_0b4 *)&fx_1fc_rotPosPath0, k, &work->rot8, &a);
            work->unk10.vx = a.vx;
            work->unk10.vy = a.vy;
            work->unk10.vz = a.vz;
            work->rot8.vz = -work->rot8.vz;
            ex->Hi_SetEffModelSlice(work->model136[0], work->unk10.vy);
            ex->Hi_DrawSliceEffModel(&work->rot8, (VECTOR *)&work->unk10, (s32)&work->unk20, work->model136[0]);
        }
        if ((u32)k < 0x28) {
            switch (k) {
            case 0:
                fxm_valuePathInit((Fx1fcPath *)&fx_1fc_valuePath, (u16 *)ex->btlfx_getFileData(entry->file, 0x1A));
                break;
            case 4:
                fxm_valuePathInit((Fx1fcPath *)&fx_1fc_valuePath, (u16 *)ex->btlfx_getFileData(entry->file, 0x1B));
                break;
            }
            if (k < 4) {
                m0 = 1;
                m1 = 2;
            } else {
                m0 = 2;
                m1 = 3;
            }
            fxm_rotPosPathGet((Fx1fcPath_0b4 *)&fx_1fc_rotPosPath1, k, &work->rot8, &b);
            work->unk10.vx = b.vx;
            work->unk10.vy = b.vy;
            work->unk10.vz = b.vz;
            work->rot8.vz = -work->rot8.vz;
            fxm_valuePathGet(&fx_1fc_valuePath, k, &t);
            ex->Hi_DrawMorphEffModel(&work->rot8, (VECTOR *)&work->unk10, (s32)&work->unk20, work->model136[m0],
                                     work->model136[m1], t);
        }
        if ((u32)(frame - 1) < 0x18) {
            k2 = frame - 1;
            j = 4;
            for (i = 0; i < 10; i++) {
                fxm_rotPosPathGet((Fx1fcPath_0b4 *)&fx_1fc_rotPosPath2[i], k2, &work->rot30, &c);
                work->unk38.vx = c.vx;
                work->unk38.vy = c.vy;
                work->unk38.vz = c.vz;
                ex->Hi_DrawEffModel(&work->rot30, (VECTOR *)&work->unk38, (s32)&work->unk48, work->model136[j]);
                j++;
                if (j >= 9) {
                    j = 4;
                }
            }
        }
        if ((u32)frame < 0x1C) {
            fxm_posPathGet((Fx1fcPath *)&fx_1fc_posPath, frame, &d);
            fxm_vecPathGet((Fx1fcTrack6 *)&fx_1fc_vecPath, frame, &e);
            for (q = 0; q < env->targets->targetCount; q++) {
                work->unk58[q].vx = D_801EDEDC[q].vx + d.vx;
                work->unk58[q].vy = D_801EDEDC[q].vy + d.vy;
                work->unk58[q].vz = D_801EDEDC[q].vz + d.vz;
                ex->btlfx_setUnitPos(q, (s16 *)&work->unk58[q]);
                work->unk98[q].vx = (D_801EDF1C[q].vx * e.vx) / 4096;
                work->unk98[q].vy = (D_801EDF1C[q].vy * e.vy) / 4096;
                work->unk98[q].vz = (D_801EDF1C[q].vz * e.vz) / 4096;
                ex->btlfx_setUnitScale(q, (s32 *)&work->unk98[q]);
            }
        }
        if ((u32)(frame - 3) < 0x23 && work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->unk128.vx;
                work->emit0->rot.vy = work->unk128.vy;
                work->emit0->rot.vz = work->unk128.vz;
            }
        }
        if (work->unk130 >= 0) {
            fxm_tintScreenLerp(0, 0xFF, 0x10 - work->unk130, 0x10, 1, 0);
            if (--work->unk130 == -1) {
                work->unk134 = 1;
            }
        }
        if (work->unk132 >= 0) {
            fxm_tintScreenLerp(0xFF, 0, 0x10 - work->unk132, 0x10, 1, 0);
            work->unk132--;
        }
        if (work->unk134 != 0) {
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
        }
        return frame >= 0x3D;
    }
    return 0;
}

s32 fx_1fc_op84(s32 mode, Fx1fcWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 j;
    SVECTOR *a;
    SVECTOR *b;

    if (mode == 0) {
        *arg4 = 0xB0;
    } else if (mode == 1) {
        fx_1fc_env = env;
        for (j = 0; j < env->targets->targetCount; j++) {
            work->unkA0[j] = 0;
            ex->btlfx_getUnitHomePos(j, (s16 *)&work->unk20[j]);
            work->unk20[j].vy = -0xBB8;
            ex->btlfx_setUnitPos(j, (s16 *)&work->unk20[j]);
        }
    } else {
        frame = entry->frame;
        for (i = 0; i < env->targets->targetCount; i++) {
            if (work->unkA0[i] == 0 && fxm_moveUnitHome(i, frame, 4, &work->unk20[i]) != 0) {
                work->unkA0[i] = 1;
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk20[i]);
                ex->btlfx_setUnitPos(i, (s16 *)&work->unk20[i]);
                a = (SVECTOR *)((u8 *)work + i * 8 + 0x20);
                b = (SVECTOR *)((u8 *)work + i * 8 + 0x60);
                b->vx = a->vx;
                b->vy = a->vy;
                b->vz = a->vz;
                work->emit0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->unk60[i]);
                if (work->emit0[i] != NULL) {
                    work->emit0[i]->otZ = 0x80;
                }
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

static s32 fxm_offsetTableEntry(u8 *base, u16 i) {
    return (s32)base + ((s32 *)base)[(u16)i + 1];
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_1fc_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_posPathInit(Fx1fcPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1fcPoint *)data;
}

static s32 fxm_posPathGet(Fx1fcPath *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx1fcPoint *p;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    p = &path->unk4[t];
    out->vx = p->x;
    out->vy = p->y;
    out->vz = p->z;
    return done;
}

static void fxm_rotPosPathInit(Fx1fcPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1fcPoint *)data;
}

static s32 fxm_rotPosPathGet(Fx1fcPath_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx1fcKey *k;
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

static void fxm_valuePathInit(Fx1fcPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1fcPoint *)data;
}

static s32 fxm_valuePathGet(Fx1fcKeys *keys, s32 i, s32 *out) {
    s32 r = 1;

    if (i > keys->unk2 - 1) {
        i = keys->unk2 - 1;
    } else {
        r = 0;
    }
    *out = *(s16 *)((u8 *)keys->unk4 + (i << 1));
    return r;
}

static void fxm_vecPathInit(Fx1fcPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1fcPoint *)data;
}

static s32 fxm_vecPathGet(Fx1fcTrack6 *track, s32 t, VECTOR *out) {
    s32 last = track->unk2 - 1;
    s32 done = 1;
    Fx1fcKey6 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->unk4[t];
    out->vx = k->x;
    out->vy = k->y;
    out->vz = k->z;
    return done;
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_1fc_env)->btlfx_drawScreenTint((u32 *)(fx_1fc_env->ot + ot), arg4, c, c, c);
}

static s32 fxm_moveUnitHome(s32 a0, s32 t, s32 n, SVECTOR *b) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1fc_env);
    SVECTOR a;
    SVECTOR out;

    ex->btlfx_getUnitHomePos(a0, (s16 *)&a);
    fxm_lerpSvec(b, &a, (t << 12) / n, &out);
    ex->btlfx_setUnitPos(a0, (s16 *)&out);
    return t >= n;
}

static void fxm_camChanReset(void) {
    s32 i;
    s32 m = -1;

    for (i = 63; i >= 0; i--) {
        D_801EDF9C[i].unk6 = m;
    }
    for (i = 3; i >= 0; i--) {
        D_801EE39C[i] = 0;
    }
}

static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 c;

    data++;
    c = *data;
    for (i = 0; i < 64; i++) {
        if (D_801EDF9C[i].unk6 == -1) {
            D_801EDF9C[i].unk6 = 0;
            D_801EDF9C[i].unk0 = data;
            D_801EDF9C[i].unk4 = start;
            D_801EDF9C[i].unkA = dst;
            D_801EDF9C[i].unkC = 0;
            D_801EDF9C[i].unkE = c;
            D_801EDF9C[i].unk8 = ch;
            return;
        }
    }
}

static void fxm_camChanUpdate(s32 ch) {
    Unk800F78A4 *env = fx_1fc_env;
    s32 i;
    s32 k;
    s32 d;

    for (i = 0; i < 64; i++) {
        if (D_801EDF9C[i].unk8 == ch) {
            if (D_801EDF9C[i].unk6 == 0 && D_801EDF9C[i].unk4 == D_801EE39C[ch]) {
                D_801EDF9C[i].unk6 = *D_801EDF9C[i].unk0;
                D_801EDF9C[i].unk0++;
            }
            if (D_801EDF9C[i].unk6 > 0) {
                D_801EDF9C[i].unk6--;
                for (k = 0; k < 3; k++) {
                    d = ((D_801EDF9C[i].unkA >> (k * 4)) & 0xF) - 1;
                    if (d >= 0) {
                        ((SVECTOR *)env->camera)[d].vx = D_801EDF9C[i].unk0[0];
                        ((SVECTOR *)env->camera)[d].vy = D_801EDF9C[i].unk0[1];
                        ((SVECTOR *)env->camera)[d].vz = D_801EDF9C[i].unk0[2];
                        if (D_801EDF9C[i].unkC < D_801EDF9C[i].unkE) {
                            D_801EDF9C[i].unk0 += 3;
                            D_801EDF9C[i].unkC++;
                        }
                    }
                }
            }
        }
    }
    D_801EE39C[ch]++;
}
