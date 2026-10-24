#include "fxmod/fx_123.h"

s32 func_801E77B4(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7A24(pt, pos, &D_801E968C[0], frame, n);
}

s32 func_801E77E8(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7A24(pt, pos, &D_801E968C[1], frame, n);
}

s32 func_801E781C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7A24(pt, pos, &D_801E968C[2], frame, n);
}

s32 func_801E7850(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7A24(pt, pos, &D_801E968C[3], frame, n);
}

s32 func_801E7884(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7A24(pt, pos, &D_801E968C[4], frame, n);
}

s32 func_801E78B8(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7A24(pt, pos, &D_801E96B4[0], frame, n);
}

s32 func_801E78EC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7A24(pt, pos, &D_801E968C[0], frame, n);
}

s32 func_801E7920(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7A24(pt, pos, &D_801E968C[1], frame, n);
}

s32 func_801E7954(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7A24(pt, pos, &D_801E968C[2], frame, n);
}

s32 func_801E7988(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7A24(pt, pos, &D_801E968C[3], frame, n);
}

s32 func_801E79BC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7A24(pt, pos, &D_801E968C[4], frame, n);
}

s32 func_801E79F0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E7A24(pt, pos, &D_801E96B4[0], frame, n);
}

s32 func_801E7A24(Pool30Entry *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n) {
    pos->vx = src->vx;
    pos->vy = src->vy;
    pos->vz = src->vz;
    pt->unk0.vx = pos->vx << 12;
    pt->unk0.vy = pos->vy << 12;
    pt->unk0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_123/nonmatchings/fx_123", fx_123_headerPtr);

s32 fx_123_op80(s32 mode, Fx123Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone m;
    Fx123Bone *b;
    Pool6CEntry *p;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x8C;
    } else if (mode == 1) {
        D_801E90CC = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk4C);
        work->unk54.vx = work->unk4C.vx;
        work->unk54.vy = work->unk4C.vy;
        work->unk54.vz = work->unk4C.vz;
        work->unk54.vy -= 0xC8;
        work->unk54.vz -= 0x190;
    } else {
    frame = entry->unk0;
    switch (frame) {
    case 0:
        for (i = 0; i < 6; i++) {
            work->unk0[i] = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 9), (s32)&D_801E9BFC[i]);
            if (p != NULL) {
                p->unk22 = 0x20;
            }
        }
        break;
    case 20:
        for (i = 0; i < 6; i++) {
            work->unk18[i] = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 9), (s32)&D_801E9BFC[i]);
            if (p != NULL) {
                p->unk22 = 0x20;
            }
        }
        break;
    case 30:
        for (i = 0; i < 6; i++) {
            work->unk30[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&D_801E9C2C[i]);
        }
        break;
    case 43:
        work->unk48 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk54);
        break;
    case 38:
        for (i = 0; i < 6; i++) {
            ex->btlfx_stopEmitter(work->unk18[i]);
        }
        break;
    }
    if (frame >= 0 && frame < 0x12) {
        b = D_801E8F64;
        for (i = 0; i < 6; i++, b++) {
            ex->btlfx_getUnitBoneMatrix(0x10, b->unk0, &m);
            ex->btlfx_matrixPointAhead((MATRIX *)&m, b->unk2, (s32)&work->unk5C[i]);
            if (work->unk0[i] != NULL && work->unk0[i]->unk30 == -1) {
                work->unk0[i] = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->unk0[i], (s32)&work->unk5C[i]);
        }
    }
    if (frame >= 0x14 && frame < 0x26) {
        b = D_801E8F64;
        for (i = 0; i < 6; i++, b++) {
            ex->btlfx_getUnitBoneMatrix(0x10, b->unk0, &m);
            ex->btlfx_matrixPointAhead((MATRIX *)&m, b->unk2, (s32)&work->unk5C[i]);
            if (work->unk18[i] != NULL && work->unk18[i]->unk30 == -1) {
                work->unk18[i] = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->unk18[i], (s32)&work->unk5C[i]);
        }
    }
    if (frame >= 0x1E && frame < 0x37) {
        b = D_801E8F64;
        for (i = 0; i < 6; i++, b++) {
            ex->btlfx_getUnitBoneMatrix(0x10, b->unk0, &m);
            ex->btlfx_matrixPointAhead((MATRIX *)&m, b->unk2, (s32)&work->unk5C[i]);
            if (work->unk30[i] != NULL && work->unk30[i]->unk30 == -1) {
                work->unk30[i] = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->unk30[i], (s32)&work->unk5C[i]);
        }
    }
    return frame >= 0x50;
    }
    return 0;
}

s32 fx_123_op81(s32 mode, Fx123Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    Pool6CEntry *p;
    s32 frame;
    Fx123Shot *s;
    s32 j;
    s32 i;
    s32 k;
    s32 t;
    s32 f;
    s32 n;

    if (mode == 0) {
        *arg4 = 0x188;
    } else if (mode == 1) {
        D_801E90CC = env;
        ex->btlfx_initEffModels(D_801E90DC, 7);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk64);
        for (n = 0; n < 6; n++) {
            func_801E8E20((Fx123Path *)&D_801E965C[n], (u16 *)ex->btlfx_getFileData(entry->unk8, D_801E9054[n]));
            setVector(&work->unk6C[n], 0, 0, 0);
            setVector(&work->unk9C[n], 0, 0, 0);
            work->unk17A[n] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 7), 0x97, 0x3D40, 0x80, 0);
        }
        setVector(&work->unkFC, 0x1000, 0x1000, 0x1000);
        work->unk186 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 5), 0x97, 0x3D40, 0x80, 0);
        work->unk178 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk140.vx = work->unk64.vx;
            work->unk140.vy = work->unk64.vy - 0x1013;
            work->unk140.vz = work->unk64.vz - 0x5F5;
            work->unk60 = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk140);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
            break;
        case 11:
            for (j = 0; j < 6; j++) {
                work->unk0[j] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 8), (s32)&D_801E9C5C[j]);
            }
            break;
        case 19:
            for (s = D_801E8F7C, j = 0; j < 6; j++, s++) {
                s->unk0.vx = env->targets->centerX;
                s->unk0.vy = work->unk9C[j].vy;
                s->unk0.vz = env->targets->centerZ;
                s->unk0.vx += ex->math_sin(s->unk20) * 350 / 4096;
                s->unk0.vz += ex->math_cos(s->unk20) * 350 / 4096;
                s->unk8.vx = work->unk6C[j].vx;
                s->unk8.vy = work->unk6C[j].vy;
                s->unk8.vz = work->unk6C[j].vz;
                s->unk10.vx = ex->math_sin(s->unk20) * 1400 / 4096;
                s->unk10.vy = 500;
                s->unk10.vz = ex->math_cos(s->unk20) * 1400 / 4096;
                s->unk10.vx += env->targets->centerX;
                s->unk10.vy += env->targets->centerY;
                s->unk10.vz += env->targets->centerZ;
            }
            break;
        case 17:
            work->unk178 = 4;
            break;
        }
        env->camera->points[0].vx = work->unk64.vx;
        env->camera->points[0].vy = work->unk64.vy;
        env->camera->points[0].vz = work->unk64.vz - 0x5F5;
        if (frame >= 0 && frame < 0x32) {
            k = frame;
            if (frame >= 3 && frame < 11) {
                pos.vx = work->unk64.vx;
                pos.vy = work->unk64.vy - 0x1013;
                pos.vz = work->unk64.vz - 0x5F5;
                setVector(&rot, 0, 0, 0);
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&work->unkFC, work->unk186);
                for (i = 0; i < 6; i++) {
                    func_801E8E38(&D_801E965C[i], 3, &work->unk6C[i], &work->unk148[i]);
                    addVector(&work->unk148[i], &work->unk64);
                }
            } else {
                if (frame >= 11) {
                    k = frame - 8;
                }
                for (s = D_801E8F7C, i = 0; i < 6; i++, s++) {
                    if (s->unk18 == 0) {
                        continue;
                    }
                    if (k >= 0 && k < 11) {
                        f = frame >= 11 ? frame - 8 : frame;
                        func_801E8E38(&D_801E965C[i], f, &work->unk6C[i], &work->unk148[i]);
                        copyVector(&work->unk9C[i], &work->unk148[i]);
                        addVector(&work->unk9C[i], &work->unk64);
                        addVector(&work->unk148[i], &work->unk64);
                    } else if (k >= s->unk1A + 13 && k < s->unk1A + 17) {
                        t = k - 13;
                        if (t < s->unk1A) {
                            continue;
                        }
                        if (s->unk1A == t) {
                            work->unk30[i] = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, D_801E9060[i]), (s32)&D_801E9C8C[i]);
                            if (p != NULL) {
                                p->unk22 = 0x80;
                                if (work->unk30[i] != NULL) {
                                    work->unk30[i]->unk14 = (s32)D_801E96BC[i];
                                    work->unk30[i]->unk12 = 2;
                                    work->unk30[i]->unkC = 0x70;
                                    if (work->unk30[i] != NULL) {
                                        *(s32 *)&work->unk30[i]->unk0->unk8 = D_801E906C[i];
                                    }
                                }
                            }
                        }
                        t -= s->unk1A;
                        copyVector(&work->unk6C[i], &s->unk8);
                        ex->btlfx_lerpVec((s16 *)&s->unk0, (s16 *)&s->unk10, t << 10, (s16 *)&work->unk148[i]);
                        copyVector(&work->unk9C[i], &work->unk148[i]);
                    }
                    if (k == s->unk1A + 17) {
                        copyVector(&work->unk110[i], &work->unk148[i]);
                        work->unk110[i].vy += 0x154;
                        work->unk48[i] = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk110[i]);
                        if (p != NULL) {
                            p->unk22 = 0x80;
                        }
                    }
                    copyVector(&D_801E968C[i], &work->unk9C[i]);
                    if (k == s->unk1A + 21) {
                        s->unk1E = 4;
                    }
                    if (k >= s->unk1A + 16) {
                        ex->Hi_SetEffModelSlice(work->unk17A[i], work->unk9C[i].vy - 100);
                        ex->Hi_DrawSliceEffModel(&work->unk6C[i], &work->unk9C[i], (s32)&work->unkFC, work->unk17A[i]);
                    } else {
                        ex->Hi_DrawEffModel(&work->unk6C[i], &work->unk9C[i], (s32)&work->unkFC, work->unk17A[i]);
                    }
                    if (s->unk1E >= 0) {
                        work->unk10C = func_801E8DC8(0, -0x80, 4 - s->unk1E, 4);
                        if (s->unk1E == 0) {
                            s->unk18 = 0;
                        } else {
                            s->unk1E--;
                        }
                        ex->Hi_ModifyEffModelAbr(work->unk17A[i], 1);
                        ex->Hi_ModifyEffModelRGB(work->unk17A[i], work->unk10C, work->unk10C, work->unk10C);
                    }
                }
            }
        }
        if (frame >= 11 && frame < 29) {
            for (j = 0; j < 6; j++) {
                if (work->unk0[j] != NULL && work->unk0[j]->unk30 == -1) {
                    work->unk0[j] = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->unk0[j], (s32)&work->unk148[j]);
            }
        }
        if (frame >= 0x50) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_123_op82(s32 mode, Fx123Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        work->unk14.vx = env->targets->centerX;
        work->unk14.vy = env->targets->centerY;
        D_801E90CC = env;
        work->unk14.vz = env->targets->centerZ;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk14);
            work->unk0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk14);
            work->unk0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x20), (s32)&work->unk14);
            break;
        case 0xC:
            work->unk0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk14);
            work->unk0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk14);
            break;
        }
        return frame >= 0x1E;
    }
    return 0;
}

s32 fx_123_op83(s32 mode, Fx123Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801E90CC = env;
        work->unk2 = -1;
        work->unk0 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk0 = 0x10;
            break;
        case 20:
            work->unk2 = 0x10;
            break;
        }
        if (work->unk0 >= 0) {
            func_801E8ECC(0, 0xFF, 0x10 - work->unk0, 0x10, 1, 0);
            work->unk0--;
        } else if (work->unk2 >= 0) {
            func_801E8ECC(0xFF, 0, 0x10 - work->unk2, 0x10, 1, 0);
            work->unk2--;
        } else {
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
        }
        return frame >= 0x20;
    }
    return 0;
}

void func_801E8D84(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E90CC)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8DC8(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E8E20(Fx123Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx123Point *)data;
}

s32 func_801E8E38(Fx123Path_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx123Key *k;
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

void func_801E8ECC(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801E90CC)->btlfx_drawScreenTint((u32 *)(D_801E90CC->ot + ot), arg4, c, c, c);
}
