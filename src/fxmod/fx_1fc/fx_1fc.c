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
        func_801E8B04(0, 1, 0, ex->btlfx_getFileData(entry->unk8, 0xE));
        func_801E8B04(0, 2, 0, ex->btlfx_getFileData(entry->unk8, 0xC));
        func_801E8B04(0, 3, 0, ex->btlfx_getFileData(entry->unk8, 0xA));
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

INCLUDE_ASM("asm/us/fx_1fc/nonmatchings/fx_1fc", fx_1fc_op83);

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
