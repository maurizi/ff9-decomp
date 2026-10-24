#include "fxmod/fx_0dc.h"

void func_801E7718(s16 *a, s16 *b, s32 i, s16 *out) {
    BTLFX_EXPORTS(D_801E9354)->func_800CFD78(a, b, -((D_801E92F0[i] * 0x455) >> 9), out);
}

s32 func_801E7788(s32 mode, Fx0dcItem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx0dcWork *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR *p;
    Pool6CEntry *e;
    Fx0dcItem *q;
    s32 t;
    s32 c;
    s32 k;
    s32 n;
    s32 d;

    if (mode == 1) {
        it->unk0 = 0;
        it->unk8 = 0;
    } else if (it->unk2 == 0) {
        SVECTOR rot;
        VECTOR scale;
        SVECTOR v38;
        SVECTOR v40;
        SVECTOR v48;
        SVECTOR v50;
        SVECTOR v58;

        p = &D_801E9288[it->unk4];
        switch (it->unk0) {
        case 0:
            if (it->unk8 == 0) {
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x25), (s32)&it->unk18);
                it->unk34 = e;
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
            }
            scale.vx = 0;
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->unk34 != NULL) {
                it->unk34->unk5C.vx = rot.vx;
                it->unk34->unk5C.vy = rot.vy;
                it->unk34->unk5C.vz = rot.vz;
            }
            if (++it->unk8 > 0) {
                it->unk0 = 1;
                it->unk8 = 0;
            }
            break;
        case 1:
            {
                s32 a = it->unk8 << 10;

                scale.vx = scale.vy = (a * 0x8AA) >> 12;
                scale.vz = (ex->math_sin(a >> 2) * 0x8AA) >> 12;
            }
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->unk34 != NULL) {
                it->unk34->unk5C.vx = rot.vx;
                it->unk34->unk5C.vy = rot.vy;
                it->unk34->unk5C.vz = rot.vz;
            }
            if (++it->unk8 >= 4) {
                it->unk0 = 2;
                it->unk8 = 0;
            }
            break;
        case 2:
            scale.vx = scale.vy = scale.vz = 0x8AA;
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->unk34 != NULL) {
                it->unk34->unk5C.vx = rot.vx;
                it->unk34->unk5C.vy = rot.vy;
                it->unk34->unk5C.vz = rot.vz;
            }
            if (frame < 12) {
                rot.pad = 1;
                v38.vx = it->unkC.vx;
                v38.vy = it->unkC.vy;
                v38.vz = it->unkC.vz;
                {
                    s32 a = (frame << 10) / 12;
                    s32 b;

                    ex->func_800CFD78((s16 *)&v38, (s16 *)&rot, 0x2F6, (s16 *)&v40);
                    d = ex->btlfx_getFileData(entry->unk8, 0x24);
                    b = (ex->math_sin(a) * 0x1154) >> 12;
                    ex->func_800D23D8((u8 *)d, &v40, &rot, b, 0, 1, ex->math_cos(a) * 2 / 3);
                }
            }
            n = 6;
            if (work->unk78 != 0) {
                it->unk8 = 0;
                if (it->unk4 < 3) {
                    n = 10;
                }
                BTLFX_COPY_SVEC(&v50, &it->unk18);
                for (k = 0; k < 4; k++, n++) {
                    q = it;
                    if (k != 0) {
                        q = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    }
                    if (q != NULL) {
                        q->unk2 = 1;
                        q->unk0 = n;
                        q->unk4 = k;
                        q->unk8 = 0;
                        q->unk2C = work->unk20[n];
                        func_801E7718((s16 *)&v50, (s16 *)p, n, (s16 *)&v48);
                        BTLFX_COPY_SVEC(&q->unk20, p);
                        q->unkC.vx = v48.vx << 4;
                        q->unkC.vy = v48.vy << 4;
                        q->unkC.vz = v48.vz << 4;
                        q->unk28 = ex->btlfx_randCentered(0x84);
                        q->unk2A = ex->btlfx_randCentered(0x84);
                        d = 0x80; /* through a variable, so the 0x80 isn't folded into k * 0x29 */
                        t = -(((ex->btlfx_rand() & 0xF) + d - k * 0x29) * 16);
                        BTLFX_COPY_SVEC(&v58, p);
                        v58.vx += 0x80;
                        ex->func_800CFCA8(&v58, t, (s32)&q->unk18);
                        q->unk18.vx += ex->btlfx_randCentered(0x200);
                        q->unk18.vy += ex->btlfx_randCentered(0x200);
                        q->unk18.vz += ex->btlfx_randCentered(0x200);
                    }
                }
            }
            break;
        }
        if (scale.vx != 0) {
            scale.vx *= 2;
            scale.vy *= 2;
            scale.vz = scale.vz * 5 / 2;
            ex->Hi_SetEffModelOffset(it->unk2C, 2);
            ex->Hi_DrawEffModel(p, (VECTOR *)&it->unkC, (s32)&scale, it->unk2C);
            ex->Hi_DrawEffModel(p, (VECTOR *)&it->unkC, (s32)&scale, it->unk30);
            return 0;
        }
    } else {
        VECTOR pos;
        VECTOR scale;

        t = ((it->unk8 << 12) / 22) >> 2;
        c = ex->math_cos(t);
        it->unkC.vx += it->unk18.vx;
        it->unkC.vy += it->unk18.vy;
        it->unkC.vz += it->unk18.vz;
        it->unk18.vy += 0x40;
        it->unk20.vx += it->unk28;
        it->unk20.vy += it->unk2A;
        c = (c * 3) >> 1;
        if (it->unk0 < 6) {
            scale.vx = scale.vy = (ex->math_cos(t) * 0x19FE) >> 12;
            scale.vz = (ex->math_cos(t) * 0x1154) >> 12;
        } else {
            scale.vx = scale.vy = (ex->math_cos(t) * 0x1154) >> 12;
            scale.vz = (ex->math_cos(t) * 0x15A9) >> 12;
        }
        pos.vx = it->unkC.vx >> 4;
        pos.vy = it->unkC.vy >> 4;
        pos.vz = it->unkC.vz >> 4;
        ex->Hi_DrawEffModel(&it->unk20, &pos, (s32)&scale, it->unk2C);
        ex->Hi_ModifyEffModelAbr(it->unk2C, 1);
        ex->Hi_ModifyEffModelRGB(it->unk2C, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
        if (++it->unk8 >= 22) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0dc/nonmatchings/fx_0dc", fx_0dc_headerPtr);

s32 fx_0dc_op80(s32 mode, Fx0dcWork80 *w, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR rot2;
    Fx0dcItem *it;
    Fx0dcItem *q; /* separate from it: one shard pointer per state (register allocation) */
    Pool6CEntry *e;
    SVECTOR *p;
    s32 frame;
    s32 k;
    s32 c;
    s32 g;
    s32 t;
    s32 i;
    s32 j;
    s32 tp; /* the 0x9A tpage, held in a register across the calls */
    s32 r; /* spawn radius */

    if (mode == 0) {
        init->unk0 = 0x88;
        init->unk8 = 0x38;
        init->unk14 = (s32)func_801E7788;
        init->unk4 = 0x20;
    } else if (mode == 1) {
        D_801E9354 = env;
        ex->btlfx_initEffModels(D_801E9364, 0x19);
        tp = 0x9A;
        w->unk8 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x14), tp, 0x3DC0,
                                                  0x80, 0);
        w->unkC = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x12), tp, 0x3DC0,
                                                  0x80, 0);
        w->unk10 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x13), tp, 0x3DC0,
                                                   0x80, 0);
        w->unk14 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x11), 0xBA, 0x3D40,
                                                   0, 0x80);
        w->unk18 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xF), 0xBA, 0x3D40,
                                                   0, 0x80);
        w->unk1C = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x10), 0xBA, 0x3D40,
                                                   0, 0x80);
        for (i = 0; i < 14; i++) {
            w->unk20[i] = (s16)ex->Hi_RegisterTexEffModel(
                (BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, D_801E92B8[i]), tp, 0x3DC0, 0x80, 0);
        }
        w->unk0 = 2;
        w->unk4 = 0;
        w->unk7A = 0;
        w->unk78 = 0;
        w->unk7C = 0;
        w->unk80 = 0;
        ex->btlfx_getUnitPos(0, (s16 *)&w->unk58);
        BTLFX_COPY_SVEC(&w->unk60, &w->unk58);
        w->unk60.vy -= 0x22A;
        BTLFX_COPY_SVEC(&w->unk70, &w->unk58);
        w->unk70.vy -= 0x80;
        BTLFX_COPY_SVEC(&w->unk68, &w->unk58);
        w->unk68.vy -= 0x5C6;
        e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x26), (s32)&w->unk58);
        if (e != NULL) {
            e->unk22 = 0x80;
        }
        e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x27), (s32)&w->unk68);
        if (e != NULL) {
            e->unk22 = 0x80;
        }
        e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x2A), (s32)&w->unk58);
        if (e != NULL) {
            e->unk22 = 0x80;
        }
        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0, 0x80);
    } else {
        frame = entry->unk0;
        pos.vx = w->unk58.vx;
        pos.vy = w->unk58.vy;
        pos.vz = w->unk58.vz;
        scale.vx = scale.vy = 0x1DFE;
        scale.vz = 0x1354;
        if (w->unk4 < 3) {
            t = w->unk4 << 8;
            scale.vx = scale.vy = (t * scale.vx) >> 10;
            scale.vz = (scale.vz * ex->math_sin(t)) >> 12;
        }
        k = 0;
        switch (w->unk0) {
        case 2:
            k = 0x1000;
            c = 0x2000 - (frame << 10);
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(w->unk8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk8);
            ex->Hi_ModifyEffModelRGB(w->unk8, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk14);
            if (frame < 6) {
                c = 0xFF - frame * 0xFF / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c >> 2, c * 2 / 3, c);
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0xFF, 0xFF, 0xFF);
                }
            }
            if (frame >= 4) {
                w->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            k = 0x1000;
            if (frame & 1) {
                it = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it != NULL) {
                    it->unk2 = 0;
                    it->unk4 = w->unk7A;
                    p = &D_801E9288[w->unk7A];
                    if (w->unk7A >= 3) {
                        it->unk2C = w->unkC;
                        it->unk30 = w->unk18;
                    } else {
                        it->unk2C = w->unk10;
                        it->unk30 = w->unk1C;
                    }
                    it->unkC.vx = w->unk58.vx;
                    it->unkC.vy = w->unk58.vy;
                    it->unkC.vz = w->unk58.vz;
                    r = 200;
                    it->unkC.vx += (ex->math_sin(p->vy + 0x800) * r) >> 12;
                    it->unkC.vz += (ex->math_cos(p->vy + 0x800) * r) >> 12;
                    it->unk18.vx = it->unkC.vx;
                    it->unk18.vy = it->unkC.vy;
                    it->unk18.vz = it->unkC.vz;
                    w->unk7A++;
                }
            }
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(w->unk8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk14);
            if (w->unk7A >= 6) {
                w->unk0 = 4;
                entry->unk0 = -1;
                w->unk84 = e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1C), (s32)&w->unk58);
                if (e != NULL) {
                    e->unk22 = 0x5C;
                }
            }
            break;
        case 4:
            k = 0x1000;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(w->unk8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk14);
            if (frame >= 8) {
                w->unk0 = 5;
                entry->unk0 = -1;
            }
            break;
        case 5:
            k = 0x1000;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(w->unk8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, w->unk14);
            rot.pad = 1;
            if (frame >= 10) {
                w->unk0 = 6;
                w->unk78 = 1;
                entry->unk0 = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
                ex->func_800E3D54(w->unk84);
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x20), (s32)&w->unk58);
                if (e != NULL) {
                    e->unk22 = 0x60;
                }
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1F), (s32)&w->unk58);
                if (e != NULL) {
                    e->unk22 = 0x60;
                }
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1D), (s32)&w->unk60);
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x21), (s32)&w->unk60);
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x22), (s32)&w->unk60);
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x2B), (s32)&w->unk60);
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
                rot.vx = -0x400;
                rot.vy = 0;
                rot.vz = -0x400;
                for (j = 0; j < 6; j++) {
                    q = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (q != NULL) {
                        q->unk2 = 1;
                        q->unk0 = j;
                        q->unk4 = j;
                        q->unk8 = 0;
                        q->unk2C = w->unk20[j];
                        func_801E7718((s16 *)&w->unk58, (s16 *)&rot, j, (s16 *)&rot2);
                        BTLFX_COPY_SVEC(&q->unk20, &rot);
                        q->unkC.vx = rot2.vx * 16;
                        q->unkC.vy = rot2.vy * 16;
                        q->unkC.vz = rot2.vz * 16;
                        q->unk28 = ex->btlfx_randCentered(0x5C);
                        q->unk2A = ex->btlfx_randCentered(0x80);
                        ex->func_800CFCA8(&rot, -(((ex->btlfx_rand() & 0xF) + (0x5C - j * 8)) * 16), (s32)&q->unk18);
                        q->unk18.vx += ex->btlfx_randCentered(0x3E0);
                        q->unk18.vz += ex->btlfx_randCentered(0x3E0);
                    }
                }
            }
            break;
        case 6:
            rot.vx = -0x400;
            rot.vz = -0x400;
            rot.vy = 0;
            if (frame < 18) {
                k = ex->math_cos((frame << 10) / 18);
            }
            if (frame == 8) {
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1E), (s32)&w->unk60);
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
            }
            if (frame < 8) {
                g = 0xFF - ((frame * 0xFF) >> 3);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, g >> 2, g * 2 / 3, g);
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0xFF, 0xFF, 0xFF);
                }
            }
            if (frame >= 24) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (w->unk0 >= 2 && w->unk0 < 6) {
            w->unk7C++;
            if (w->unk7C < 11) {
                w->unk80 += 11 - (w->unk7C * 10) / 11;
            } else {
                w->unk80++;
            }
            ex->func_800E6624(w->unk80, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
        }
        if (k != 0) {
            rot2.vx = 0x400;
            rot2.vy = 0;
            rot2.vz = w->unk4 * 41;
            rot2.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23), &w->unk58, &rot2, (w->unk4 << 6) + 0x15A9,
                              0, 1, k);
            rot2.vz = w->unk4 * 24 + 0x400;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23), &w->unk58, &rot2, (w->unk4 << 6) + 0x15A9,
                              0, 1, k);
            env->unk28 = 0x127;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), &w->unk58, NULL, (w->unk4 << 5) + 0x15A9,
                              0, 1, k);
            env->unk28 = 0;
        }
        w->unk4++;
    }
    return 0;
}

s32 fx_0dc_op84(s32 mode, Fx0dcWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 a;
    s32 b;
    s32 c;
    s32 k;
    s32 t;
    s32 d;
    u16 r;

    if (mode == 0) {
        *arg4 = 0x10;
    } else {
        if (mode == 1) {
            D_801E9354 = env;
            work->unk0 = 0;
            work->unk4 = 0;
        } else {
            r = c = b = a = 0;
            frame = entry->unk0;
            ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->unk8);
            switch (work->unk0) {
            case 0:
                r = 0x200;
                a = (frame << 12) / 3;
                b = ex->math_sin(a >> 2) >> 1;
                c = ((0x1000 - a) >> 1) + 0x800;
                if (frame >= 3) {
                    work->unk0 = 1;
                    entry->unk0 = -1;
                }
                break;
            case 1:
                k = (frame & 1) << 8;
                c = k + 0x1000;
                d = (frame << 12) / 9;
                a = (k + 0x800) + (d >> 1);
                r = (d >> 3) + 0x200;
                b = c;
                if (frame >= 9) {
                    work->unk0 = 2;
                    entry->unk0 = -1;
                }
                break;
            case 2:
                d = (frame << 12) / 9;
                t = d >> 2;
                r = 0x400 - t;
                a = ex->math_cos(t);
                b = ex->math_sin(t) + 0x1000;
                c = ex->math_sin(t) * 2 + 0x1000;
                if (frame >= 9) {
                    return 1;
                }
                break;
            }
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = r;
            rot.pad = 0;
            env->unk28 = 0x80;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x29), &work->unk8, NULL, b / 3, 0, 1, a / 2);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x28), &work->unk8, &rot, c, 0, 1, a * 2 / 3);
            env->unk28 = 0;
            work->unk4++;
        }
    }
    return 0;
}
