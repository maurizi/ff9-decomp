#include "fxmod/fx_1ea.h"

void fx_1ea_buildTrail(Fx1eaPath *path, Fx1eaTrail *trail, s32 size, s32 base, s32 start, s32 pad) {
    SVECTOR v;
    SVECTOR *p;
    s32 n;
    s32 i;

    n = (size - 4) / 8;
    p = trail->pos4;
    trail->lastSlot0 = trail->count2 = n - 1;
    trail->head1 = 0;
    if (start < n) {
        for (i = 0; i < n - start; i++) {
            p->pad = 0;
            p++;
        }
    } else {
        start = n;
    }
    for (i = 0; i < start;) {
        i++;
        fxm_posPathGet(path, start - i + base, &v);
        v.vx += v.vx / 10;
        v.vy += v.vy / 10;
        v.vz += v.vz / 10;
        v.vx += fx_1ea_trailOrigin.vx;
        v.vy += fx_1ea_trailOrigin.vy;
        v.vz += fx_1ea_trailOrigin.vz;
        p->vx = v.vx;
        p->vy = v.vy;
        p->vz = v.vz;
        p->pad = pad;
        p++;
    }
}

void fx_1ea_buildTrailRotY(Fx1eaPath *path, Fx1eaTrail *trail, s32 size, s32 base, s32 start, s32 pad, s32 ang) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1ea_env);
    MATRIX m;
    VECTOR o;
    SVECTOR rot;
    SVECTOR v;
    SVECTOR *p;
    s32 n;
    s32 i;

    n = (size - 4) / 8;
    p = trail->pos4;
    trail->lastSlot0 = trail->count2 = n - 1;
    trail->head1 = 0;
    if (start < n) {
        for (i = 0; i < n - start; i++) {
            p->pad = 0;
            p++;
        }
    } else {
        start = n;
    }
    for (i = 0; i < start;) {
        i++;
        fxm_posPathGet(path, start - i + base, &v);
        rot.vx = 0;
        rot.vy = ang;
        rot.vz = 0;
        v.vx += v.vx / 10;
        v.vy += v.vy / 10;
        v.vz += v.vz / 10;
        ex->math_rotMatrixYXZ(&rot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&v);
        gte_rtv0tr();
        gte_stlvnl(&o);
        o.vx += fx_1ea_trailOrigin.vx;
        o.vy += fx_1ea_trailOrigin.vy;
        o.vz += fx_1ea_trailOrigin.vz;
        p->vx = o.vx;
        p->vy = o.vy;
        p->vz = o.vz;
        p->pad = pad;
        p++;
    }
}

s32 fx_1ea_op80(s32 mode, Fx1eaWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        fx_1ea_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&work->pos8);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&work->pos8);
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

INCLUDE_RODATA("asm/us/fx_1ea/nonmatchings/fx_1ea", fx_1ea_headerPtr);

s32 fx_1ea_op81(s32 mode, Fx1eaWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR pos2;
    s32 frame;
    s32 i;
    s32 i2;
    s32 i3;
    s32 i4;
    s32 k;
    s32 a;
    s32 b;
    s32 n;
    s32 t;
    s32 ang;

    if (mode == 0) {
        *arg4 = 0xB8;
    } else if (mode == 1) {
        fx_1ea_env = env;
        ex->btlfx_initEffModels(fx_1ea_effModels, 0xA);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos84);
        fx_1ea_trailOrigin.vx = work->pos84.vx;
        fx_1ea_trailOrigin.vy = work->pos84.vy;
        fx_1ea_trailOrigin.vz = work->pos84.vz;
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot8C);
        work->rot2C.vx = 0;
        work->rot2C.vy = 0;
        work->rot2C.vz = 0;
        work->pos34.vx = 0;
        work->pos34.vy = 0;
        work->pos34.vz = 0;
        work->scale44.vx = 0x1800;
        work->scale44.vy = 0x1800;
        work->scale44.vz = 0x1800;
        fxm_rotPosPathInit((Fx1eaPath *)&fx_1ea_rotPosPath0, (u16 *)ex->btlfx_getFileData(entry->file, 0xB));
        for (i = 0; i < 5; i++) {
            work->modelA4[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_1ea_morphModelFileIds[i]), 1);
        }
        for (i = 0; i < 5; i++) {
            fxm_rotPosPathInit((Fx1eaPath *)&fx_1ea_rotPosPath1[i], (u16 *)ex->btlfx_getFileData(entry->file, fx_1ea_slicePathFileIds[i]));
            work->modelA4[i + 5] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_1ea_sliceModelFileIds[i]), 1);
        }
        fx_1ea_ribbon0.index8 = 0;
        fx_1ea_ribbon0.countA = 1;
        fx_1ea_ribbon0.alphaC = 0x1000;
        fx_1ea_ribbon0.scaleE = 0x1000;
        fx_1ea_ribbon0.width10 = 0x40;
        fx_1ea_ribbon1.index8 = 0;
        fx_1ea_ribbon1.countA = 1;
        fx_1ea_ribbon1.alphaC = 0x1000;
        fx_1ea_ribbon1.scaleE = 0x1000;
        fx_1ea_ribbon1.width10 = 0x40;
        work->timer98 = -1;
        work->timer9C = -1;
        work->timerA0 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0xF:
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos84);
            work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos84);
            break;
        case 0x1E:
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos84);
            break;
        case 0x37:
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos84);
            work->emit0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos84);
            work->emit0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1D), (s32)&work->pos84);
            if (work->emit0[3] != NULL) {
                work->emit0[3]->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 0x3C:
            work->emit0[6] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x22), (s32)&work->pos84);
            break;
        case 0x46:
            work->timer9C = 8;
            work->emit0[7] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1C), (s32)&work->pos84);
            work->emit0[8] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos84);
            work->emit0[10] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x23), (s32)&work->pos84);
            ex->btlfx_stopEmitter(work->emit0[0]);
            ex->btlfx_stopEmitter(work->emit0[1]);
            ex->btlfx_stopEmitter(work->emit0[2]);
            break;
        case 0x4B:
            work->emit0[9] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos84);
            break;
        case 0x3D:
            work->timer98 = 8;
            break;
        case 0x4E:
            work->timerA0 = 0xF;
            break;
        }
        if (frame >= 0x1E && frame < 0x45) {
            k = frame - 0x1E;
            if (work->timer98 >= 0) {
                work->rgb54 = fxm_lerpRatio(0, -0x80, 8 - work->timer98, 8);
                work->timer98--;
            }
            switch (k) {
            case 0:
            case 9:
            case 0x18:
            case 0x1B:
                work->frame94 = 0;
                break;
            }
            if (k < 9) {
                a = 0;
                b = 1;
                n = 9;
            } else if (k < 0x18) {
                a = 1;
                b = 2;
                n = 0xF;
            } else if (k < 0x1B) {
                a = 2;
                b = 3;
                n = 3;
            } else if (k < 0x27) {
                a = 3;
                b = 4;
                n = 0xC;
            } else {
                a = 4;
                b = 4;
                n = 0;
            }
            t = 0x1000;
            if (a != b) {
                t = 0;
                if (work->frame94 <= n) {
                    t = 0x1000 - (work->frame94 << 12) / n;
                }
            }
            fxm_rotPosPathGet(&fx_1ea_rotPosPath0, k, &work->rot2C, &pos);
            work->pos34.vx = pos.vx;
            work->pos34.vy = pos.vy;
            work->pos34.vz = pos.vz;
            work->pos34.vx += work->pos84.vx;
            work->rot2C.vy = 0x800;
            work->pos34.vy += work->pos84.vy;
            work->pos34.vz += work->pos84.vz;
            ex->Hi_DrawMorphEffModel(&work->rot2C, (VECTOR *)&work->pos34, (s32)&work->scale44, work->modelA4[a],
                                     work->modelA4[b], t);
            if (work->timer98 >= 0) {
                ex->Hi_ModifyEffModelRGB(work->modelA4[a], work->rgb54, work->rgb54, work->rgb54);
            }
            for (i2 = 0; i2 < 5; i2++) {
                fxm_rotPosPathGet(&fx_1ea_rotPosPath1[i2], k, &work->rot2C, &pos2);
                work->pos34.vx = pos2.vx;
                work->pos34.vy = pos2.vy;
                work->pos34.vz = pos2.vz;
                work->pos34.vx += work->pos84.vx;
                work->rot2C.vy = 0x800;
                work->pos34.vy += work->pos84.vy;
                work->pos34.vz += work->pos84.vz;
                ex->Hi_SetEffModelSlice(work->modelA4[i2 + 5], work->pos34.vy);
                ex->Hi_DrawSliceEffModel(&work->rot2C, (VECTOR *)&work->pos34, (s32)&work->scale44, work->modelA4[i2 + 5]);
                if (work->timer98 >= 0) {
                    ex->Hi_ModifyEffModelRGB(work->modelA4[i2 + 5], work->rgb54, work->rgb54, work->rgb54);
                }
            }
            work->frame94++;
        }
        if (frame >= 0 && frame < 0x1E) {
            fx_1ea_ribbon0.countA = frame;
            fx_1ea_ribbon0.countA = fx_1ea_ribbon0.countA > 0x1E ? 0x1E : fx_1ea_ribbon0.countA;
            if (frame >= 0xF) {
                fx_1ea_ribbon0.alphaC = 0x1000 - (frame - 0xF) * 0x111;
                if (work->emit0[1] != NULL) {
                    work->emit0[1]->alpha = 0x1000 - fx_1ea_ribbon0.alphaC;
                }
            }
            for (i3 = 0; i3 < 2; i3++) {
                fxm_posPathInit(&fx_1ea_ribbon0.path0, (u16 *)ex->btlfx_getFileData(entry->file, fx_1ea_trailPathFileIds0[i3]));
                fx_1ea_buildTrail(&fx_1ea_ribbon0.path0, (Fx1eaTrail *)fx_1ea_ribbonTrails0[i3], 0x100, fx_1ea_ribbon0.index8, fx_1ea_ribbon0.countA,
                              fx_1ea_ribbon0.width10);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x20), 0, NULL, 0, fx_1ea_ribbon0.scaleE,
                                    fx_1ea_ribbon0.alphaC, 1, fx_1ea_ribbonTrails0[i3], 0);
            }
        }
        if (frame >= 0 && frame < 0x1E) {
            fx_1ea_ribbon1.countA = frame;
            fx_1ea_ribbon1.countA = fx_1ea_ribbon1.countA > 0xF ? 0xF : fx_1ea_ribbon1.countA;
            if (frame >= 0xF) {
                fx_1ea_ribbon1.alphaC = 0x1000 - (frame - 0xF) * 0x111;
            }
            ang = 0;
            for (i4 = 0; i4 < 6; i4++) {
                fxm_posPathInit(&fx_1ea_ribbon1.path0, (u16 *)ex->btlfx_getFileData(entry->file, fx_1ea_trailPathFileIds1[i4]));
                fx_1ea_buildTrailRotY(&fx_1ea_ribbon1.path0, (Fx1eaTrail *)fx_1ea_ribbonTrails1[i4], 0x88, fx_1ea_ribbon1.index8, fx_1ea_ribbon1.countA,
                              fx_1ea_ribbon1.width10, ang);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x21), 0, NULL, 0, fx_1ea_ribbon1.scaleE,
                                    fx_1ea_ribbon1.alphaC, 1, fx_1ea_ribbonTrails1[i4], 0);
                if (i4 & 1) {
                    ang += 0x555;
                }
            }
        }
        if (work->timer9C >= 0) {
            s32 c = fxm_lerpRatio(0, 0x1000, 8 - work->timer9C, 8);

            work->timer9C--;
            if (work->emit0[8] != NULL) {
                work->emit0[8]->alpha = c;
            }
        }
        if (work->timerA0 >= 0) {
            s32 c = fxm_lerpRatio(0x1000, 0, 0xF - work->timerA0, 0xF);

            work->timerA0--;
            if (work->emit0[8] != NULL) {
                work->emit0[8]->alpha = c;
            }
        }
        if (frame >= 0x64) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_1ea_op82(s32 mode, Fx1eaWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        fx_1ea_env = env;
        work->timer2 = -1;
        work->timer0 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0x0:
            work->timer0 = 0x10;
            break;
        case 0x33:
            work->timer2 = 0x10;
            break;
        }
        if (work->timer0 >= 0) {
            fxm_tintScreenLerp(0, 0xFF, 0x10 - work->timer0, 0x10, 1, 0);
            work->timer0--;
        } else if (work->timer2 >= 0) {
            fxm_tintScreenLerp(0xFF, 0, 0x10 - work->timer2, 0x10, 1, 0);
            work->timer2--;
        } else {
            ex->btlfx_drawScreenTint((u32 *)env->ot + OT_LAST, 1, 0xFF, 0xFF, 0xFF);
        }
        return frame >= 0x43;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_1ea_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_1ea_env)->btlfx_drawScreenTint((u32 *)(fx_1ea_env->ot + ot), arg4, c, c, c);
}

static void fxm_posPathInit(Fx1eaPath *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1eaPoint *)data;
}

static s32 fxm_posPathGet(Fx1eaPath *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx1eaPoint *p;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    p = &path->keys4[t];
    out->vx = p->x;
    out->vy = p->y;
    out->vz = p->z;
    return done;
}

static void fxm_rotPosPathInit(Fx1eaPath *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1eaPoint *)data;
}

static s32 fxm_rotPosPathGet(Fx1eaPath_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx1eaKey *k;
    s32 r = 1;

    if (i > path->count2 - 1) {
        i = path->count2 - 1;
    } else {
        r = 0;
    }
    k = &path->keys4[i];
    if (rot != NULL) {
        rot->vx = k->rot6[0];
        rot->vy = k->rot6[1];
        rot->vz = k->rot6[2];
    }
    if (pos != NULL) {
        pos->vx = k->pos0[0];
        pos->vy = k->pos0[1];
        pos->vz = k->pos0[2];
    }
    return r;
}
