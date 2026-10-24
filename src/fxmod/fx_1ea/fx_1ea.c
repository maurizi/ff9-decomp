#include "fxmod/fx_1ea.h"

void func_801E7804(Fx1eaPath *path, Fx1eaTrail *trail, s32 size, s32 base, s32 start, s32 pad) {
    SVECTOR v;
    SVECTOR *p;
    s32 n;
    s32 i;

    n = (size - 4) / 8;
    p = trail->unk4;
    trail->unk0 = trail->unk2 = n - 1;
    trail->unk1 = 0;
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
        func_801E8C4C(path, start - i + base, &v);
        v.vx += v.vx / 10;
        v.vy += v.vy / 10;
        v.vz += v.vz / 10;
        v.vx += D_801E9B58.vx;
        v.vy += D_801E9B58.vy;
        v.vz += D_801E9B58.vz;
        p->vx = v.vx;
        p->vy = v.vy;
        p->vz = v.vz;
        p->pad = pad;
        p++;
    }
}

void func_801E79D0(Fx1eaPath *path, Fx1eaTrail *trail, s32 size, s32 base, s32 start, s32 pad, s32 ang) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8DD8);
    MATRIX m;
    VECTOR o;
    SVECTOR rot;
    SVECTOR v;
    SVECTOR *p;
    s32 n;
    s32 i;

    n = (size - 4) / 8;
    p = trail->unk4;
    trail->unk0 = trail->unk2 = n - 1;
    trail->unk1 = 0;
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
        func_801E8C4C(path, start - i + base, &v);
        rot.vx = 0;
        rot.vy = ang;
        rot.vz = 0;
        v.vx += v.vx / 10;
        v.vy += v.vy / 10;
        v.vz += v.vz / 10;
        ex->func_800316D8(&rot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&v);
        gte_rtv0tr();
        gte_stlvnl(&o);
        o.vx += D_801E9B58.vx;
        o.vy += D_801E9B58.vy;
        o.vz += D_801E9B58.vz;
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
        D_801E8DD8 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk10);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&work->unk8);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
    } else {
        s32 frame = entry->unk0;

        if (work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk10.vx;
                work->unk0->unk5C.vy = work->unk10.vy;
                work->unk0->unk5C.vz = work->unk10.vz;
            }
        }
        if (work->unk4 != NULL) {
            if (work->unk4->unk30 == -1) {
                work->unk4 = NULL;
            }
            if (work->unk4 != NULL) {
                work->unk4->unk5C.vx = work->unk10.vx;
                work->unk4->unk5C.vy = work->unk10.vy;
                work->unk4->unk5C.vz = work->unk10.vz;
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
        D_801E8DD8 = env;
        ex->btlfx_initEffModels(D_801E8DE8, 0xA);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk84);
        D_801E9B58.vx = work->unk84.vx;
        D_801E9B58.vy = work->unk84.vy;
        D_801E9B58.vz = work->unk84.vz;
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk8C);
        work->unk2C.vx = 0;
        work->unk2C.vy = 0;
        work->unk2C.vz = 0;
        work->unk34.vx = 0;
        work->unk34.vy = 0;
        work->unk34.vz = 0;
        work->unk44.vx = 0x1800;
        work->unk44.vy = 0x1800;
        work->unk44.vz = 0x1800;
        func_801E8CB0((Fx1eaPath *)&D_801E95F0, (u16 *)ex->btlfx_getFileData(entry->unk8, 0xB));
        for (i = 0; i < 5; i++) {
            work->unkA4[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801E8D5C[i]), 1);
        }
        for (i = 0; i < 5; i++) {
            func_801E8CB0((Fx1eaPath *)&D_801E95F8[i], (u16 *)ex->btlfx_getFileData(entry->unk8, D_801E8D74[i]));
            work->unkA4[i + 5] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801E8D68[i]), 1);
        }
        D_801E95B8.unk8 = 0;
        D_801E95B8.unkA = 1;
        D_801E95B8.unkC = 0x1000;
        D_801E95B8.unkE = 0x1000;
        D_801E95B8.unk10 = 0x40;
        D_801E95D8.unk8 = 0;
        D_801E95D8.unkA = 1;
        D_801E95D8.unkC = 0x1000;
        D_801E95D8.unkE = 0x1000;
        D_801E95D8.unk10 = 0x40;
        work->unk98 = -1;
        work->unk9C = -1;
        work->unkA0 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0xF:
            work->unk0[0] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk84);
            work->unk0[1] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk84);
            break;
        case 0x1E:
            work->unk0[2] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk84);
            break;
        case 0x37:
            work->unk0[3] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk84);
            work->unk0[4] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk84);
            work->unk0[5] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1D), (s32)&work->unk84);
            if (work->unk0[3] != NULL) {
                work->unk0[3]->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        case 0x3C:
            work->unk0[6] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x22), (s32)&work->unk84);
            break;
        case 0x46:
            work->unk9C = 8;
            work->unk0[7] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1C), (s32)&work->unk84);
            work->unk0[8] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk84);
            work->unk0[10] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x23), (s32)&work->unk84);
            ex->func_800E3D54(work->unk0[0]);
            ex->func_800E3D54(work->unk0[1]);
            ex->func_800E3D54(work->unk0[2]);
            break;
        case 0x4B:
            work->unk0[9] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk84);
            break;
        case 0x3D:
            work->unk98 = 8;
            break;
        case 0x4E:
            work->unkA0 = 0xF;
            break;
        }
        if (frame >= 0x1E && frame < 0x45) {
            k = frame - 0x1E;
            if (work->unk98 >= 0) {
                work->unk54 = func_801E8B44(0, -0x80, 8 - work->unk98, 8);
                work->unk98--;
            }
            switch (k) {
            case 0:
            case 9:
            case 0x18:
            case 0x1B:
                work->unk94 = 0;
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
                if (work->unk94 <= n) {
                    t = 0x1000 - (work->unk94 << 12) / n;
                }
            }
            func_801E8CC8(&D_801E95F0, k, &work->unk2C, &pos);
            work->unk34.vx = pos.vx;
            work->unk34.vy = pos.vy;
            work->unk34.vz = pos.vz;
            work->unk34.vx += work->unk84.vx;
            work->unk2C.vy = 0x800;
            work->unk34.vy += work->unk84.vy;
            work->unk34.vz += work->unk84.vz;
            ex->Hi_DrawMorphEffModel(&work->unk2C, (VECTOR *)&work->unk34, (s32)&work->unk44, work->unkA4[a],
                                     work->unkA4[b], t);
            if (work->unk98 >= 0) {
                ex->Hi_ModifyEffModelRGB(work->unkA4[a], work->unk54, work->unk54, work->unk54);
            }
            for (i2 = 0; i2 < 5; i2++) {
                func_801E8CC8(&D_801E95F8[i2], k, &work->unk2C, &pos2);
                work->unk34.vx = pos2.vx;
                work->unk34.vy = pos2.vy;
                work->unk34.vz = pos2.vz;
                work->unk34.vx += work->unk84.vx;
                work->unk2C.vy = 0x800;
                work->unk34.vy += work->unk84.vy;
                work->unk34.vz += work->unk84.vz;
                ex->Hi_SetEffModelSlice(work->unkA4[i2 + 5], work->unk34.vy);
                ex->Hi_DrawSliceEffModel(&work->unk2C, (VECTOR *)&work->unk34, (s32)&work->unk44, work->unkA4[i2 + 5]);
                if (work->unk98 >= 0) {
                    ex->Hi_ModifyEffModelRGB(work->unkA4[i2 + 5], work->unk54, work->unk54, work->unk54);
                }
            }
            work->unk94++;
        }
        if (frame >= 0 && frame < 0x1E) {
            D_801E95B8.unkA = frame;
            D_801E95B8.unkA = D_801E95B8.unkA > 0x1E ? 0x1E : D_801E95B8.unkA;
            if (frame >= 0xF) {
                D_801E95B8.unkC = 0x1000 - (frame - 0xF) * 0x111;
                if (work->unk0[1] != NULL) {
                    work->unk0[1]->unk20 = 0x1000 - D_801E95B8.unkC;
                }
            }
            for (i3 = 0; i3 < 2; i3++) {
                func_801E8C34(&D_801E95B8.unk0, (u16 *)ex->btlfx_getFileData(entry->unk8, D_801E8D80[i3]));
                func_801E7804(&D_801E95B8.unk0, (Fx1eaTrail *)D_801E9628[i3], 0x100, D_801E95B8.unk8, D_801E95B8.unkA,
                              D_801E95B8.unk10);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), 0, NULL, 0, D_801E95B8.unkE,
                                    D_801E95B8.unkC, 1, D_801E9628[i3], 0);
            }
        }
        if (frame >= 0 && frame < 0x1E) {
            D_801E95D8.unkA = frame;
            D_801E95D8.unkA = D_801E95D8.unkA > 0xF ? 0xF : D_801E95D8.unkA;
            if (frame >= 0xF) {
                D_801E95D8.unkC = 0x1000 - (frame - 0xF) * 0x111;
            }
            ang = 0;
            for (i4 = 0; i4 < 6; i4++) {
                func_801E8C34(&D_801E95D8.unk0, (u16 *)ex->btlfx_getFileData(entry->unk8, D_801E8D84[i4]));
                func_801E79D0(&D_801E95D8.unk0, (Fx1eaTrail *)D_801E9828[i4], 0x88, D_801E95D8.unk8, D_801E95D8.unkA,
                              D_801E95D8.unk10, ang);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x21), 0, NULL, 0, D_801E95D8.unkE,
                                    D_801E95D8.unkC, 1, D_801E9828[i4], 0);
                if (i4 & 1) {
                    ang += 0x555;
                }
            }
        }
        if (work->unk9C >= 0) {
            s32 c = func_801E8B44(0, 0x1000, 8 - work->unk9C, 8);

            work->unk9C--;
            if (work->unk0[8] != NULL) {
                work->unk0[8]->unk20 = c;
            }
        }
        if (work->unkA0 >= 0) {
            s32 c = func_801E8B44(0x1000, 0, 0xF - work->unkA0, 0xF);

            work->unkA0--;
            if (work->unk0[8] != NULL) {
                work->unk0[8]->unk20 = c;
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
        D_801E8DD8 = env;
        work->unk2 = -1;
        work->unk0 = -1;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0x0:
            work->unk0 = 0x10;
            break;
        case 0x33:
            work->unk2 = 0x10;
            break;
        }
        if (work->unk0 >= 0) {
            func_801E8B9C(0, 0xFF, 0x10 - work->unk0, 0x10, 1, 0);
            work->unk0--;
        } else if (work->unk2 >= 0) {
            func_801E8B9C(0xFF, 0, 0x10 - work->unk2, 0x10, 1, 0);
            work->unk2--;
        } else {
            ex->btlfx_drawScreenTint((u32 *)env->ot + 0xFFF, 1, 0xFF, 0xFF, 0xFF);
        }
        return frame >= 0x43;
    }
    return 0;
}

void func_801E8B00(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E8DD8)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8B44(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E8B9C(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801E8DD8)->btlfx_drawScreenTint((u32 *)(D_801E8DD8->ot + ot), arg4, c, c, c);
}

void func_801E8C34(Fx1eaPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1eaPoint *)data;
}

s32 func_801E8C4C(Fx1eaPath *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx1eaPoint *p;

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

void func_801E8CB0(Fx1eaPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1eaPoint *)data;
}

s32 func_801E8CC8(Fx1eaPath_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx1eaKey *k;
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
