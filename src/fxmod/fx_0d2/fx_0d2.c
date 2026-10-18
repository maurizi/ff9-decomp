#include "fxmod/fx_0d2.h"

void func_801E77A4(Fx0d2Keys *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip) {
    SVECTOR v;
    SVECTOR *p;
    s32 cnt;
    s32 i;
    s32 j;
    s32 k;

    cnt = (size - 4) / 8;
    p = (SVECTOR *)(buf + 4);
    buf[0] = buf[2] = cnt - 1;
    buf[1] = 0;
    if (n < cnt) {
        for (i = 0; i < cnt - n; i++) {
            p->pad = 0;
            p++;
        }
    } else {
        n = cnt;
    }
    for (i = 0; i < n; i++) {
        k = i + 1;
        j = n - k + start;
        if (clip != 0 && j >= keys->unk2) {
            break;
        }
        func_801E9E88(keys, j, &v);
        p->vx = v.vx;
        p->vy = v.vy;
        p->vz = v.vz;
        p->pad = from + (to - from) / keys->unk2 * (n - i);
        p++;
    }
}

s32 fx_0d2_op80(s32 mode, Fx0d2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR w;
    s32 frame;
    s32 k;

    if (mode == 0) {
        *arg4 = sizeof(Fx0d2Work80);
    } else if (mode == 1) {
        D_801ED8E0 = env;
        ex->btlfx_initEffModels(D_801ED8F0, 0x20);
        work->unk14.vx = 0;
        work->unk14.vy = 0x800;
        work->unk14.vz = 0;
        work->unk2C.vx = 0x266;
        work->unk2C.vy = 0x266;
        work->unk2C.vz = 0x266;
        work->unk1C.vx = 0;
        work->unk1C.vy = 0;
        work->unk1C.vz = 0;
        work->unk3C.vx = 0;
        work->unk3C.vy = 0x800;
        work->unk3C.vz = 0;
        work->unk44.vx = 0;
        work->unk44.vy = 0;
        work->unk44.vz = 0;
        work->unk54.vx = 0x1000;
        work->unk54.vy = 0x1000;
        work->unk54.vz = 0x1000;
        work->unkA4 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x85), 1);
        func_801EA1D4();
        func_801EA220(0, 1, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x9A));
        func_801EA220(0, 2, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x96));
        func_801EA220(0, 3, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x97));
        func_801EA220(0, 4, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x94));
        func_801EA220(0, 9, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x9D));
        func_801EA220(0, 0xA, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x93));
        func_801EA220(0x68, 1, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x99));
        func_801EA220(0x68, 2, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x95));
        func_801EA220(0x67, 3, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x9C));
        func_801EA220(0x67, 4, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x9B));
        func_801EA220(0x68, 0xA, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x98));
        func_801EA220(0xB6, 1, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0xA0));
        func_801EA220(0xB6, 2, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0xA1));
        func_801EA220(0xB5, 3, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0xA2));
        func_801EA220(0xB5, 4, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0xA3));
        func_801EA220(0xDE, 1, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x1F));
        func_801EA220(0xDE, 2, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x1D));
        func_801EA220(0xDD, 3, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x1E));
        func_801EA220(0xDD, 4, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x20));
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 1:
            work->unk94 = 0;
            work->unk98 = frame;
            ex->Hi_SetSummonMotion(((Fx0d2Summon *)env->unk4)->unk54->unk180, ((Fx0d2Summon *)env->unk4)->unk60);
            work->unk9C = frame;
            func_801E9E70((Fx0d2Path *)&D_801EF228, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x92));
            break;
        case 0x29:
            work->unk94 = 1;
            work->unk98 = 0;
            ex->Hi_SetSummonMotion(((Fx0d2Summon *)env->unk4)->unk54->unk184, ((Fx0d2Summon *)env->unk4)->unk60);
            break;
        case 0x51:
            work->unk94 = 2;
            work->unk98 = 1;
            ex->Hi_SetSummonMotion(((Fx0d2Summon *)env->unk4)->unk54->unk188, ((Fx0d2Summon *)env->unk4)->unk60);
            break;
        case 0x69:
            work->unk2C.vx = 0x433;
            work->unk2C.vy = 0x433;
            work->unk2C.vz = 0x433;
            work->unk54.vx = 0x1C00;
            work->unk54.vy = 0x1C00;
            work->unk54.vz = 0x1C00;
            work->unk9C = 0;
            func_801E9E70((Fx0d2Path *)&D_801EF228, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x9F));
            break;
        case 0xB6:
            func_801E9EEC((Fx0d2Path *)&D_801EF230, (u16 *)ex->btlfx_getFileData(entry->unk8, 0xA7));
            break;
        }
        switch (frame) {
        case 1:
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA4), (s32)&D_801F0040);
            break;
        case 0x52:
            work->unk74.vx = 0x88;
            work->unk74.vy = -0x366;
            work->unk74.vz = -0x41F;
            work->unk7C.vx = -0x88;
            work->unk7C.vy = -0x366;
            work->unk7C.vz = -0x41F;
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA5), (s32)&work->unk74);
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA5), (s32)&work->unk7C);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = 0x80;
            }
            if (work->unk8 != NULL) {
                work->unk8->unk22 = 0x80;
            }
            break;
        case 0x69:
            ex->func_800E3D54(work->unk0);
            ex->func_800E3D54(work->unk4);
            ex->func_800E3D54(work->unk8);
            func_801E9E70((Fx0d2Path *)&D_801EF250, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x82));
            work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x83), (s32)&D_801F0050[0]);
            work->unk10 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x83), (s32)&D_801F0050[1]);
            if (work->unkC != NULL) {
                work->unkC->unk22 = 0x100;
            }
            if (work->unk10 != NULL) {
                work->unk10->unk22 = 0x100;
            }
            break;
        }
        k = frame - 1;
        if ((u32)k < 0x83) {
            func_801E9E88((Fx0d2Path *)&D_801EF228, work->unk9C, &v);
            work->unk1C.vx = v.vx;
            work->unk1C.vy = v.vy;
            work->unk1C.vz = v.vz;
            ex->Hi_DrawSummonModel(&work->unk14, (VECTOR *)&work->unk1C, (s32)&work->unk2C,
                                   ((Fx0d2Summon *)env->unk4)->unk60, work->unk98);
            work->unk9C++;
        }
        if ((u32)k < 0xDE) {
            if (frame < 0xB6) {
                work->unk44.vx = env->unk2C->unk30[3].vx;
                work->unk44.vy = env->unk2C->unk30[3].vy;
                work->unk44.vz = env->unk2C->unk30[3].vz;
            } else {
                func_801E9F04((Fx0d2KeyPath *)&D_801EF230, frame - 0xB6, &work->unk3C, &w);
                work->unk44.vx = w.vx;
                work->unk44.vy = w.vy;
                work->unk44.vz = w.vz;
            }
            ex->Hi_SetEffModelSlice(work->unkA4, work->unk44.vy);
            ex->Hi_DrawSliceEffModel(&work->unk3C, (VECTOR *)&work->unk44, (s32)&work->unk54, work->unkA4);
            if (k < 0x69) {
                ex->Hi_ModifyEffModelRGB(work->unkA4, -0x30, -0x30, -0x30);
            }
        }
        if (frame > 0) {
            work->unk6C.vx = env->unk2C->unk30[2].vx;
            work->unk6C.vy = env->unk2C->unk30[2].vy;
            work->unk6C.vz = env->unk2C->unk30[2].vz;
            if (work->unk0 != NULL && work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            ex->func_800E3D20((s32)work->unk0, (s32)&work->unk6C);
        }
        if (frame >= 0x69) {
            func_801E9E88((Fx0d2Path *)&D_801EF250, frame - 0x69, &work->unk8C);
            work->unk84.vx = work->unk8C.vx;
            work->unk84.vy = work->unk8C.vy;
            work->unk84.vz = work->unk8C.vz;
            work->unk84.vx = -work->unk84.vx;
            if (work->unkC != NULL && work->unkC->unk30 == -1) {
                work->unkC = NULL;
            }
            ex->func_800E3D20((s32)work->unkC, (s32)&work->unk84);
            if (work->unk10 != NULL && work->unk10->unk30 == -1) {
                work->unk10 = NULL;
            }
            ex->func_800E3D20((s32)work->unk10, (s32)&work->unk8C);
        }
        env->unk2C->unk30[1].vz = env->unk2C->unk0[2].vz;
        env->unk2C->unk5E = env->unk2C->unk0[3].vx;
        func_801EA27C(0);
        if (frame >= 0x12C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", fx_0d2_op81);

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", fx_0d2_op82);

INCLUDE_RODATA("asm/us/fx_0d2/nonmatchings/fx_0d2", fx_0d2_headerPtr);

INCLUDE_ASM("asm/us/fx_0d2/nonmatchings/fx_0d2", fx_0d2_op83);

s32 fx_0d2_op84(s32 mode, Fx0d2Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 j;
    Pool6CEntry *e;

    if (mode == 0) {
        *arg4 = sizeof(Fx0d2Work84);
    } else if (mode == 1) {
        D_801ED8E0 = env;
        for (j = 0; j < env->unkC->unk24; j++) {
            work->unkA0[j] = 0;
            ex->btlfx_getUnitHomePos(j, (s16 *)&work->unk20[j]);
            work->unk20[j].vy = -3000;
            ex->btlfx_setUnitPos(j, (s16 *)&work->unk20[j]);
        }
    } else {
        frame = entry->unk0;
        for (i = 0; i < env->unkC->unk24; i++) {
            if (work->unkA0[i] == 0 && func_801EA108(i, frame, 4, &work->unk20[i]) != 0) {
                work->unkA0[i] = 1;
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk20[i]);
                ex->btlfx_setUnitPos(i, (s16 *)&work->unk20[i]);
                /* PsyQ copyVector() */
                (&work->unk60[i])->vx = (&work->unk20[i])->vx, (&work->unk60[i])->vy = (&work->unk20[i])->vy,
                (&work->unk60[i])->vz = (&work->unk20[i])->vz;
                e = ex->func_800E3CEC((s32)ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk60[i]);
                work->unk0[i] = e;
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_0d2_op85(s32 mode, s16 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    s32 frame;

    if (mode == 0) {
        *arg4 = 2;
    } else if (mode == 1) {
        D_801ED8E0 = env;
        *work = -1;
    } else {
        frame = entry->unk0;
        if (frame == 0) {
            *work = 0x10;
        }
        if (*work >= 0) {
            func_801EA070(0xFF, 0, 0x10 - *work, 0x10, 2, 0);
            (*work)--;
        }
        return frame >= 0x10;
    }
    return 0;
}

s32 func_801E9DBC(u8 *base, u16 i) {
    return (s32)base + ((s32 *)base)[(u16)i + 1];
}

void func_801E9DD4(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801ED8E0)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E9E18(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E9E70(Fx0d2Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx0d2Point *)data;
}

s32 func_801E9E88(Fx0d2Path *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx0d2Point *p;

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

void func_801E9EEC(Fx0d2Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx0d2Point *)data;
}

s32 func_801E9F04(Fx0d2KeyPath *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx0d2Key *k;
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

void func_801E9F98(Fx0d2Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx0d2Point *)data;
}

s32 func_801E9FB0(Fx0d2Keys *keys, s32 i, s32 *out) {
    s32 r = 1;

    if (i > keys->unk2 - 1) {
        i = keys->unk2 - 1;
    } else {
        r = 0;
    }
    *out = *(s16 *)((u8 *)keys->unk4 + (i << 1));
    return r;
}

void func_801E9FF4(Fx0d2Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx0d2Point *)data;
}

s32 func_801EA00C(Fx0d2Path *track, s32 t, VECTOR *out) {
    s32 last = track->unk2 - 1;
    s32 done = 1;
    Fx0d2Point *k;

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

void func_801EA070(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801ED8E0)->btlfx_drawScreenTint((u32 *)(D_801ED8E0->unk20 + ot), arg4, c, c, c);
}

s32 func_801EA108(s32 a0, s32 t, s32 n, SVECTOR *b) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801ED8E0);
    SVECTOR a;
    SVECTOR out;

    ex->btlfx_getUnitHomePos(a0, (s16 *)&a);
    func_801E9DD4(b, &a, (t << 12) / n, &out);
    ex->btlfx_setUnitPos(a0, (s16 *)&out);
    return t >= n;
}

void func_801EA1D4(void) {
    s32 i;
    s32 m = -1;

    for (i = 63; i >= 0; i--) {
        D_801F0060[i].unk6 = m;
    }
    for (i = 3; i >= 0; i--) {
        D_801F0460[i] = 0;
    }
}

void func_801EA220(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 c;

    data++;
    c = *data;
    for (i = 0; i < 64; i++) {
        if (D_801F0060[i].unk6 == -1) {
            D_801F0060[i].unk6 = 0;
            D_801F0060[i].unk0 = data;
            D_801F0060[i].unk4 = start;
            D_801F0060[i].unkA = dst;
            D_801F0060[i].unkC = 0;
            D_801F0060[i].unkE = c;
            D_801F0060[i].unk8 = ch;
            return;
        }
    }
}

void func_801EA27C(s32 ch) {
    Unk800F78A4 *env = D_801ED8E0;
    s32 i;
    s32 k;
    s32 d;

    for (i = 0; i < 64; i++) {
        if (D_801F0060[i].unk8 == ch) {
            if (D_801F0060[i].unk6 == 0 && D_801F0060[i].unk4 == D_801F0460[ch]) {
                D_801F0060[i].unk6 = *D_801F0060[i].unk0;
                D_801F0060[i].unk0++;
            }
            if (D_801F0060[i].unk6 > 0) {
                D_801F0060[i].unk6--;
                for (k = 0; k < 3; k++) {
                    d = ((D_801F0060[i].unkA >> (k * 4)) & 0xF) - 1;
                    if (d >= 0) {
                        ((SVECTOR *)env->unk2C)[d].vx = D_801F0060[i].unk0[0];
                        ((SVECTOR *)env->unk2C)[d].vy = D_801F0060[i].unk0[1];
                        ((SVECTOR *)env->unk2C)[d].vz = D_801F0060[i].unk0[2];
                        if (D_801F0060[i].unkC < D_801F0060[i].unkE) {
                            D_801F0060[i].unk0 += 3;
                            D_801F0060[i].unkC++;
                        }
                    }
                }
            }
        }
    }
    D_801F0460[ch]++;
}
