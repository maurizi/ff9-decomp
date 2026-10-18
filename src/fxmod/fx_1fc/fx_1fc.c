#include "fxmod/fx_1fc.h"

s32 fx_1fc_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801EC14C = env;
        ex->btlfx_initEffModels(D_801EC15C, 0x20);
        for (i = 0; i < env->unkC->unk24; i++) {
            ex->btlfx_getUnitPos(i, (s16 *)&D_801EDEDC[i]);
            ex->btlfx_getUnitScale(i, (s32 *)&D_801EDF1C[i]);
        }
        ex->func_800E6E1C(0x240, 0x100, 0x80, 0x80, D_801EDADC, 0, 0x200, 0x2BC);
        func_801E8AB8();
        func_801E8B04(0, 1, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0xE));
        func_801E8B04(0, 2, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0xC));
        func_801E8B04(0, 3, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0xA));
    } else {
        frame = entry->unk0;
        env->unk2C->unk30[1].vz = env->unk2C->unk0[2].vz;
        func_801E8B60(0);
        return frame >= 0x78;
    }
    return 0;
}

s32 fx_1fc_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801EC14C = env;
    } else {
        return entry->unk0 > 0;
    }
    return 0;
}

s32 fx_1fc_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801EC14C = env;
    } else {
        return entry->unk0 > 0;
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
        work->unk8.vx = 0;
        work->unk8.vy = 0x800;
        work->unk8.vz = 0;
        work->unk10.vx = 0;
        work->unk10.vy = 0;
        work->unk10.vz = 0;
        work->unk20.vx = 0x1000;
        work->unk20.vy = 0x1000;
        work->unk20.vz = 0x1000;
        D_801EC14C = env;
        func_801E87D0((Fx1fcPath *)&D_801EDA5C, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x13));
        work->unk136[0] = ex->func_800E8F50(&D_801E99A4, 1);
        work->unk8.vx = 0;
        work->unk8.vy = 0x800;
        work->unk8.vz = 0;
        work->unk10.vx = 0;
        work->unk10.vy = 0;
        work->unk10.vz = 0;
        work->unk20.vx = 0x1000;
        work->unk20.vy = 0x1000;
        work->unk20.vz = 0x1000;
        func_801E87D0((Fx1fcPath *)&D_801EDA64, (u16 *)ex->btlfx_getFileData(entry->unk8, 2));
        for (i = 0; i < 3; i++) {
            model = ex->Hi_RegisterTexPtrModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, D_801E8CC0[i]),
                                               D_801E8CC8, D_801E8CD8);
            work->unk136[i + 1] = model;
        }
        work->unk30.vx = 0;
        work->unk30.vy = 0;
        work->unk30.vz = 0;
        work->unk38.vx = 0;
        work->unk38.vy = 0;
        work->unk38.vz = 0;
        work->unk48.vx = 0x1000;
        work->unk48.vy = 0x1000;
        work->unk48.vz = 0x1000;
        for (i = 0; i < 5; i++) {
            work->unk136[i + 4] = ex->func_800E8F50((Unk800E8F50 *)func_801E86A0(D_801E8CE8, i), 1);
        }
        for (i = 0; i < 10; i++) {
            func_801E87D0((Fx1fcPath *)&D_801EDA7C[i],
                          (u16 *)func_801E86A0((u8 *)ex->btlfx_getFileData(entry->unk8, 6), i));
        }
        func_801E8754((Fx1fcPath *)&D_801EDACC, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x17));
        func_801E88D8((Fx1fcPath *)&D_801EDAD4, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x18));
        work->unk130 = -1;
        work->unk132 = -1;
        work->unk134 = 0;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 3:
            work->unk118.vx = -0x1C;
            work->unk118.vy = -0x267;
            work->unk118.vz = 0x342;
            work->unk128.vx = 0x1CF;
            work->unk128.vy = -0x7B7;
            work->unk128.vz = 0x27B;
            work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk118);
            if (work->unk0 != NULL) {
                work->unk0->unk22 = 0x118;
            }
            break;
        case 12:
            sum.vx = 0;
            sum.vy = 0;
            sum.vz = 0;
            for (q = 0; q < env->unkC->unk24; q++) {
                sum.vx += work->unk58[q].vx;
                sum.vy += (&work->unk58[q])->vy;
                sum.vz += (&work->unk58[q])->vz;
            }
            work->unk120.vx = sum.vx / env->unkC->unk24;
            work->unk120.vy = sum.vy / env->unkC->unk24;
            work->unk120.vz = sum.vz / env->unkC->unk24;
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk120);
            break;
        case 38:
            ex->func_800E3D54(work->unk0);
            break;
        case 22:
            work->unk130 = 0x10;
            break;
        case 42:
            work->unk132 = 0x10;
            work->unk134 = 0;
            for (q = 0; q < env->unkC->unk24; q++) {
                ex->func_800E7ACC(q, (s32 *)&D_801EDF1C[q]);
            }
            env->unk2C->unk0[0].vx = 0;
            env->unk2C->unk0[0].vy = 0;
            env->unk2C->unk0[0].vz = 0x4B0;
            break;
        }
        k = frame - 1;
        if ((u32)k < 0x18) {
            func_801E87E8((Fx1fcPath_0b4 *)&D_801EDA5C, k, &work->unk8, &a);
            work->unk10.vx = a.vx;
            work->unk10.vy = a.vy;
            work->unk10.vz = a.vz;
            work->unk8.vz = -work->unk8.vz;
            ex->Hi_SetEffModelSlice(work->unk136[0], work->unk10.vy);
            ex->Hi_DrawSliceEffModel(&work->unk8, (VECTOR *)&work->unk10, (s32)&work->unk20, work->unk136[0]);
        }
        if ((u32)k < 0x28) {
            switch (k) {
            case 0:
                func_801E887C((Fx1fcPath *)&D_801EDA6C, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x1A));
                break;
            case 4:
                func_801E887C((Fx1fcPath *)&D_801EDA6C, (u16 *)ex->btlfx_getFileData(entry->unk8, 0x1B));
                break;
            }
            if (k < 4) {
                m0 = 1;
                m1 = 2;
            } else {
                m0 = 2;
                m1 = 3;
            }
            func_801E87E8((Fx1fcPath_0b4 *)&D_801EDA64, k, &work->unk8, &b);
            work->unk10.vx = b.vx;
            work->unk10.vy = b.vy;
            work->unk10.vz = b.vz;
            work->unk8.vz = -work->unk8.vz;
            func_801E8894(&D_801EDA6C, k, &t);
            ex->Hi_DrawMorphEffModel(&work->unk8, (VECTOR *)&work->unk10, (s32)&work->unk20, work->unk136[m0],
                                     work->unk136[m1], t);
        }
        if ((u32)(frame - 1) < 0x18) {
            k2 = frame - 1;
            j = 4;
            for (i = 0; i < 10; i++) {
                func_801E87E8((Fx1fcPath_0b4 *)&D_801EDA7C[i], k2, &work->unk30, &c);
                work->unk38.vx = c.vx;
                work->unk38.vy = c.vy;
                work->unk38.vz = c.vz;
                ex->Hi_DrawEffModel(&work->unk30, (VECTOR *)&work->unk38, (s32)&work->unk48, work->unk136[j]);
                j++;
                if (j >= 9) {
                    j = 4;
                }
            }
        }
        if ((u32)frame < 0x1C) {
            func_801E876C((Fx1fcPath *)&D_801EDACC, frame, &d);
            func_801E88F0((Fx1fcTrack6 *)&D_801EDAD4, frame, &e);
            for (q = 0; q < env->unkC->unk24; q++) {
                work->unk58[q].vx = D_801EDEDC[q].vx + d.vx;
                work->unk58[q].vy = D_801EDEDC[q].vy + d.vy;
                work->unk58[q].vz = D_801EDEDC[q].vz + d.vz;
                ex->btlfx_setUnitPos(q, (s16 *)&work->unk58[q]);
                work->unk98[q].vx = (D_801EDF1C[q].vx * e.vx) / 4096;
                work->unk98[q].vy = (D_801EDF1C[q].vy * e.vy) / 4096;
                work->unk98[q].vz = (D_801EDF1C[q].vz * e.vz) / 4096;
                ex->func_800E7ACC(q, (s32 *)&work->unk98[q]);
            }
        }
        if ((u32)(frame - 3) < 0x23 && work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk128.vx;
                work->unk0->unk5C.vy = work->unk128.vy;
                work->unk0->unk5C.vz = work->unk128.vz;
            }
        }
        if (work->unk130 >= 0) {
            func_801E8954(0, 0xFF, 0x10 - work->unk130, 0x10, 1, 0);
            if (--work->unk130 == -1) {
                work->unk134 = 1;
            }
        }
        if (work->unk132 >= 0) {
            func_801E8954(0xFF, 0, 0x10 - work->unk132, 0x10, 1, 0);
            work->unk132--;
        }
        if (work->unk134 != 0) {
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 1, 0xFF, 0xFF, 0xFF);
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
        D_801EC14C = env;
        for (j = 0; j < env->unkC->unk24; j++) {
            work->unkA0[j] = 0;
            ex->btlfx_getUnitHomePos(j, (s16 *)&work->unk20[j]);
            work->unk20[j].vy = -0xBB8;
            ex->btlfx_setUnitPos(j, (s16 *)&work->unk20[j]);
        }
    } else {
        frame = entry->unk0;
        for (i = 0; i < env->unkC->unk24; i++) {
            if (work->unkA0[i] == 0 && func_801E89EC(i, frame, 4, &work->unk20[i]) != 0) {
                work->unkA0[i] = 1;
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk20[i]);
                ex->btlfx_setUnitPos(i, (s16 *)&work->unk20[i]);
                a = (SVECTOR *)((u8 *)work + i * 8 + 0x20);
                b = (SVECTOR *)((u8 *)work + i * 8 + 0x60);
                b->vx = a->vx;
                b->vy = a->vy;
                b->vz = a->vz;
                work->unk0[i] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk60[i]);
                if (work->unk0[i] != NULL) {
                    work->unk0[i]->unk22 = 0x80;
                }
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 func_801E86A0(u8 *base, u16 i) {
    return (s32)base + ((s32 *)base)[(u16)i + 1];
}

void func_801E86B8(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801EC14C)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E86FC(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E8754(Fx1fcPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1fcPoint *)data;
}

s32 func_801E876C(Fx1fcPath *path, s32 t, SVECTOR *out) {
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

void func_801E87D0(Fx1fcPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1fcPoint *)data;
}

s32 func_801E87E8(Fx1fcPath_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
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

void func_801E887C(Fx1fcPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1fcPoint *)data;
}

s32 func_801E8894(Fx1fcKeys *keys, s32 i, s32 *out) {
    s32 r = 1;

    if (i > keys->unk2 - 1) {
        i = keys->unk2 - 1;
    } else {
        r = 0;
    }
    *out = *(s16 *)((u8 *)keys->unk4 + (i << 1));
    return r;
}

void func_801E88D8(Fx1fcPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1fcPoint *)data;
}

s32 func_801E88F0(Fx1fcTrack6 *track, s32 t, VECTOR *out) {
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

void func_801E8954(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801EC14C)->btlfx_drawScreenTint((u32 *)(D_801EC14C->unk20 + ot), arg4, c, c, c);
}

s32 func_801E89EC(s32 a0, s32 t, s32 n, SVECTOR *b) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EC14C);
    SVECTOR a;
    SVECTOR out;

    ex->btlfx_getUnitHomePos(a0, (s16 *)&a);
    func_801E86B8(b, &a, (t << 12) / n, &out);
    ex->btlfx_setUnitPos(a0, (s16 *)&out);
    return t >= n;
}

void func_801E8AB8(void) {
    s32 i;
    s32 m = -1;

    for (i = 63; i >= 0; i--) {
        D_801EDF9C[i].unk6 = m;
    }
    for (i = 3; i >= 0; i--) {
        D_801EE39C[i] = 0;
    }
}

void func_801E8B04(s16 start, u16 dst, s16 ch, s16 *data) {
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

void func_801E8B60(s32 ch) {
    Unk800F78A4 *env = D_801EC14C;
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
                        ((SVECTOR *)env->unk2C)[d].vx = D_801EDF9C[i].unk0[0];
                        ((SVECTOR *)env->unk2C)[d].vy = D_801EDF9C[i].unk0[1];
                        ((SVECTOR *)env->unk2C)[d].vz = D_801EDF9C[i].unk0[2];
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
