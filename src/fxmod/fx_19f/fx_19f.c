#include "fxmod/fx_19f.h"

void func_801E7704(u32 *ot, s32 tpage, s32 w, s32 r, s32 g, s32 b) {
    Unk800F78A4 *env = D_801EAA48;
    POLY_G4 *prim;
    BtlfxExports *ex;
    s32 i;

    w >>= 1;
    prim = (POLY_G4 *)env->unk24;
    ex = BTLFX_EXPORTS(env);
    env->unk24 = (u8 *)(prim + 4);
    for (i = 0; i < 4; i++, prim++) {
        setlen(prim, 8);
        prim->code = 0x3A;
        if (!(i & 1)) {
            prim->r0 = prim->r2 = 0;
            prim->g0 = prim->g2 = 0;
            prim->b0 = prim->b2 = b;
            prim->r1 = prim->r3 = r;
            prim->g1 = prim->g3 = g;
            prim->b1 = prim->b3 = b;
            prim->x0 = prim->x2 = 0xA0 - w;
            prim->x1 = prim->x3 = 0xA0;
        } else {
            prim->r1 = prim->r3 = 0;
            prim->g1 = prim->g3 = 0;
            prim->b1 = prim->b3 = b;
            prim->r0 = prim->r2 = r;
            prim->g0 = prim->g2 = g;
            prim->b0 = prim->b2 = b;
            prim->x0 = prim->x2 = 0xA0;
            prim->x1 = prim->x3 = w + 0xA0;
        }
        prim->y0 = prim->y1 = (i >> 1) * 0x78;
        prim->y2 = prim->y3 = prim->y0 + 0x78;
        ex->btlfx_linkPrimAbr(0x08000000, ot, prim, tpage);
    }
}

void func_801E7894(Unk80100C78Entry *entry, Fx19fTrail *trail, SVECTOR *from, SVECTOR *to, s32 amp, s32 arg5, s32 arg6,
                   s32 arg7, s32 arg8, s32 arg9) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EAA48);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 h;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    i = 0;
    p = trail->unk4;
    trail->unk0 = trail->unk2 = 0x18;
    trail->unk1 = 0;
    do {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x140);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = ex->btlfx_randCentered(0x80);
        a = t >> 1;
        h = (ex->math_sin(a) * h) >> 12;
        i++;
        h += 0x20 + ((ex->math_sin(a) * amp) >> 12);
        ex->func_800D044C(&pos, (s32)p, 0, (XY32 *)&rot, h, arg5);
        p->pad = arg9;
        arg5 += arg6 + ex->btlfx_randCentered(0x100);
        p++;
    } while (i < 25);
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), 0, NULL, arg7, 0x1000, arg8, 1, (u8 *)trail, 0);
}

s32 fx_19f_op81(s32 mode, Fx19fWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 alpha;
    s32 base;
    s32 lt;
    s32 type;
    s32 e;
    s32 f;
    s32 g;
    Fx19fTrail17 *ta;
    Fx19fTrail17 *tb;
    SVECTOR *pb;
    s16 *s1p;
    s16 *s2p;
    s32 pad;
    s16 *q1;
    s32 k2;
    VECTOR pos2;
    VECTOR scale2;
    SVECTOR rot2;
    s32 arr60[3];
    s32 arr70[3];
    s32 arr80[3];
    s32 arr90[3];
    Fx19fKey12 *key;
    Fx19fTrail13 *tc;
    Fx19fKeys *keys;
    Pool6CEntry *p;
    SVECTOR *pt;
    s32 frame;
    s32 k;
    s32 cw;
    s32 f5;
    s32 q9;
    s32 q10;
    s32 ch;
    s32 n2;
    s32 v2;
    s32 n3;
    s32 j2;
    s32 i2;
    SVECTOR *pt2;
    s16 *q2;
    s32 c1;
    s32 c2;
    s32 c3;
    s32 c4;
    s32 c5;
    s32 c5b;
    s32 c6k;
    s32 c7;
    s32 c8;
    s32 kk;
    s32 c9;
    s32 c10;
    s32 c11;
    s32 c12;
    s32 c13;
    s32 j;
    s32 n;
    s32 i;
    s32 t;
    s32 v;
    s32 h;
    s32 f2;
    s32 m;
    s32 c6;
    s32 d7;
    s32 spE8;

    if (mode == 0) {
        *arg4 = 0x84;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EAA48 = env;
        ex->btlfx_initEffModels(D_801EAB28, 5);
        work->unk70 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x21), 1);
        work->unk74 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x27), 0);
        work->unk78 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1A), 0);
        work->unk80 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 5), 0);
        work->unk7C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x22), 1);
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
        FX19F_W(work->unk18, 0) = FX19F_W(work->unk8, 0);
        FX19F_W(work->unk18, 1) = FX19F_W(work->unk8, 1);
        work->unk18.vy -= 0x1EA;
        env->unk2C->unk0[3].vx = work->unk18.vx;
        env->unk2C->unk0[3].vy = work->unk18.vy;
        env->unk2C->unk0[3].vz = work->unk18.vz;
    } else {
        alpha = 0;
        base = 0;
        lt = 0;
        type = 0;
        e = 0;
        frame = entry->unk0;
        d7 = c6 = m = 0;
        f = 0;
        arr60[0] = arr60[1] = arr60[2] = 0;
        g = 0;
        switch (work->unk0) {
        case 0: {
            key = (Fx19fKey12 *)((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1C) + (frame * 12 + 4));
            work->unk48[0] = key->x + work->unk8.vx;
            work->unk48[1] = key->y + work->unk8.vy;
            work->unk48[2] = key->z + work->unk8.vz;
            work->unk60.vx = key->rx;
            work->unk60.vy = key->ry;
            work->unk60.vz = key->rz;
            work->unk54[0] = 0x2000;
            work->unk54[1] = 0x2000;
            work->unk54[2] = 0x2000;
            if (frame >= 7) {
                SVECTOR rot;

                ex->func_800CFC2C(frame, 7, 8, 0, 0x1000, &spE8);
                env->unk28 = -0x80;
                rot.vx = 0x400;
                rot.vy = 0;
                rot.vz = frame * 0x18;
                rot.pad = 1;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23), &work->unk8, &rot,
                                  ex->math_sin(spE8 >> 2), 0, 1, ex->math_cos(spE8 >> 2));
                env->unk28 = 0;
                c6 = ex->math_cos(spE8 >> 2) >> 1;
                d7 = ex->math_sin(spE8 >> 2);
                e = ex->math_sin(spE8 >> 2) >> 2;
            } else {
                c6 = (ex->math_cos((frame << 10) / 7) >> 1) + 0x800;
            }
            if (frame >= 15) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
        } break;
        case 1:
            alpha = 0x1000;
            t = (frame << 12) / 9;
            base = ((0x1000 - ex->math_cos(t >> 2)) << 5) >> 12;
            type = (t >> 3) + 0xE00;
            if (frame < 4) {
                c1 = (ex->math_cos(frame << 8) * 0x46) >> 12;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 2, c1, c1, c1);
            }
            if (frame >= 5) {
                c2 = ex->math_sin((frame - 5) << 8) << 7;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0, c2 >> 14, c2 >> 12);
            }
            if (frame == 3) {
                ex->func_800E60A4(1, 6);
            }
            if (frame >= 9) {
                work->unk0 = 2;
                entry->unk0 = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 2, 0xFF, 0xFF, 0);
                ex->func_800E60A4(-1, 2);
                work->unk6C = NULL;
            }
            break;
        case 2:
            t = (frame << 12) / 43;
            if (frame == 0) {
                ex->func_800E60A4(-1, 1);
            }
            if (frame == 2) {
                ex->func_800E60A4(0, 3);
            }
            if (frame < 4) {
                c3 = 0xFF - frame * 0xFF / 4;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, c3 >> 2, c3, c3);
            }
            if (frame >= 16 && frame < 22) {
                if (frame == 16) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x80, 0x80, 0);
                }
                c4 = 0xFF - (frame - 16) * 0xFF / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0, c4 >> 2, c4);
            }
            f = 0x1000;
            if (frame < 16) {
                g = (ex->math_cos(frame << 6) >> 3) + 0x12C;
            } else {
                g = 0x12C;
            }
            if (frame < 20) {
                m = ex->math_cos((frame << 10) / 20);
            } else {
                m = ((frame - 20) << 11) / 23;
                if (frame & 1) {
                    m += 0x800;
                } else {
                    m += 0x400;
                }
                if (ex->btlfx_rand() & 1) {
                    m += 0x400;
                }
            }
            if (frame < 24) {
                n2 = (frame << 12) / 24;
                if (frame < 4) {
                    v = ex->math_sin((frame << 10) >> 2) * 5 / 2;
                } else {
                    v = (frame << 7) + 0x2600;
                }
                scale2.vx = scale2.vy = scale2.vz = v;
                rot2.vz = 0;
                rot2.vx = 0;
                rot2.vy = frame * 16;
                pos2.vx = work->unk8.vx;
                pos2.vy = work->unk8.vy;
                pos2.vz = work->unk8.vz;
                ex->Hi_DrawEffModel(&rot2, &pos2, (s32)&scale2, work->unk70);
                c5 = ((0x1000 - n2) >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk70, c5, c5, c5);
                if (frame < 16) {
                    ex->Hi_DrawEffModel(&rot2, &pos2, (s32)&scale2, work->unk74);
                    c5b = ((0xC00 - (frame << 8)) >> 5) - 0x80;
                    ex->Hi_ModifyEffModelRGB(work->unk74, c5b, c5b, c5b);
                }
            }
            if (frame > 0) {
                SVECTOR rot;

                keys = (Fx19fKeys *)ex->btlfx_getFileData(entry->unk8, 8);
                c6k = frame + 9;
                if (c6k >= 36) {
                    h = *(s16 *)((u8 *)keys + (36 * 6 + 4)) * 2 / 3 + ((frame - 27) << 6);
                } else {
                    keys = (Fx19fKeys *)((u8 *)keys + (c6k * 6 + 4));
                    h = keys->unk0 * 2 / 3;
                }
                FX19F_W(work->unk10, 0) = FX19F_W(work->unk8, 0);
                FX19F_W(work->unk10, 1) = FX19F_W(work->unk8, 1);
                if (frame < 16) {
                    work->unk10.vy -= (ex->math_sin((frame << 10) >> 4) * 0x56E) >> 12;
                } else {
                    work->unk10.vy -= 0x56E;
                }
                FX19F_W(D_801EA9AC, 0) = FX19F_W(work->unk10, 0);
                FX19F_W(D_801EA9AC, 1) = FX19F_W(work->unk10, 1);
                if (frame < 24) {
                    D_801EA9B4 = 0x1000;
                } else if (frame < 32) {
                    D_801EA9B4 = (h * 0x44C) >> 12;
                } else {
                    D_801EA9B4 = 0xB86;
                }
                env->unk28 = 0x140;
                if (frame >= 15) {
                    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x17), &work->unk10, NULL, h * 5 / 2,
                                            frame & 1, 1, ex->math_cos(t >> 2), 0, 0);
                }
                rot.vx = rot.vy = 0x200;
                rot.vz = frame * 0x2C;
                rot.pad = 0;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), &work->unk10, &rot,
                                  ex->btlfx_randCentered(0x1000) + 0x4000, 0, 1, 0x1000);
                env->unk28 = 0;
            }
            if (frame >= 20) {
                c7 = ((frame - 20) << 12) / 23;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1D), &work->unk10, NULL, (c7 >> 2) + 0x2800, 0, 1,
                                  ex->math_sin(c7 >> 1));
            }
            if (frame == 20) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk10);
                work->unk6C = p;
                if (p != NULL) {
                    p->unk22 = 0x96;
                }
            }
            if (frame == 14) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x16), (s32)&work->unk8);
                work->unk68 = p;
                if (p != NULL) {
                    p->unk22 = -0x20;
                }
            }
            if (frame < 8) {
                c8 = frame << 9;
                alpha = 0x1000 - c8;
                base = 0x24;
                c8 >>= 2;
                lt = ex->math_sin(c8);
                type = 0x1000 - c8;
            }
            {
                for (kk = 0; kk < 3; kk++) {
                    f2 = frame - kk * 3;
                    if (frame >= kk * 3 && f2 < 22) {
                        if (f2 < 6) {
                            arr60[kk] = 0x1000;
                            arr70[kk] = f2 * 2;
                            arr80[kk] = (f2 << 12) / 22;
                            arr90[kk] = 0x1000;
                        } else {
                            v2 = 0x1000 - ((f2 - 6) << 8);
                            arr60[kk] = v2;
                            arr70[kk] = 11;
                            arr80[kk] = (f2 << 12) / 22;
                            arr90[kk] = v2;
                        }
                    }
                }
                ch = 2;
                cw = 4;
            }
            if (frame == 22) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x80, 0x80, 0x20);
            }
            if (frame >= 23 && frame < 26) {
                c9 = 0x80 - ((frame - 23) << 6);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c9, c9, c9 >> 3);
            }
            if (frame == 24) {
                ex->func_800E60A4(-1, 0);
            }
            if (frame == 25) {
                ex->func_800E60A4(0, 5);
            }
            if (frame == 29) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x20, 0x64, 0xFF);
            }
            if (frame == 36) {
                ex->func_800E60A4(1, 7);
            }
            if (frame == 33) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x40, 0x40, 0);
            }
            if (frame == 34) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x40, 0x40, 0x40);
            }
            if (frame >= 33) {
                c10 = (frame - 33) * 0xFF / 10;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, c10 >> 2, c10 >> 1, c10);
            }
            if (frame >= 35) {
                CVECTOR col;
                SVECTOR rot;
                VECTOR sc;

                sc.vy = sc.vx = 0x1020;
                rot.vz = rot.vy = rot.vx = 0;
                sc.vz = 0x1000;
                col.r = col.g = col.b = ((frame - 31) << 5) / 12;
                ex->func_800E6764(0x200, 0xA0, 0x78, 0x50, 0x3C, cw, cw, ch, ch, &rot, &sc, &col, 1, 0);
            }
            if (work->unk6C != NULL) {
                work->unk6C->unk1C = (t >> 1) + 0x1000;
                if (work->unk6C != NULL) {
                    work->unk6C->unk20 = (0x1000 - (t >> 2)) * 3 / 4;
                }
            }
            if (frame >= 43) {
                work->unk0 = 3;
                entry->unk0 = -1;
                ex->func_800E60A4(-1, 6);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x2A), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x96;
                }
            }
            break;
        case 3: {
            t = (frame << 12) / 20;
            if (frame < 12) {
                f5 = 0xFF - (frame << 5) / 12;
                func_801E7704(env->unk20 + 2, 1, D_801EA9B8[frame], 0xC8 - frame * 0xC8 / 12, 0xC8 - frame * 0xC8 / 12,
                              f5);
            }
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x20, 0xFF, 0xFF);
            }
            if (work->unk68 != NULL && frame < 10) {
                q10 = (frame << 12) / 10;
                if (work->unk68->unk30 == -1) {
                    work->unk68 = NULL;
                }
                if (work->unk68 != NULL) {
                    c11 = 0x1000 - q10;
                    work->unk68->unk20 = c11;
                    if (work->unk68 != NULL) {
                        work->unk68->unk1C = c11;
                    }
                }
                if (frame == 9) {
                    ex->func_800E3D54(work->unk68);
                }
            }
            if (work->unk6C != NULL && frame < 8) {
                q9 = frame << 9;
                if (work->unk6C->unk30 == -1) {
                    work->unk6C = NULL;
                }
                if (work->unk6C != NULL) {
                    work->unk6C->unk1C = 0x1000 - q9;
                }
                if (frame == 7) {
                    ex->func_800E3D54(work->unk6C);
                }
            }
            if (frame < 14) {
                CVECTOR col;
                SVECTOR rot;
                VECTOR sc;

                col.r = col.g = col.b = 0x89;
                sc.vz = sc.vy = sc.vx = 0x1000;
                rot.vz = rot.vy = rot.vx = 0;
                ex->func_800E6764(2, 0xA0, 0x78, 0x50, 0x3C, 4, 4, 2, 2, &rot, &sc, &col, 0, 0);
            }
            key = (Fx19fKey12 *)((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B) + (frame * 12 + 4));
            work->unk48[0] = key->x + work->unk8.vx;
            work->unk48[1] = key->y + work->unk8.vy - 0x2EE;
            work->unk48[2] = key->z + work->unk8.vz;
            work->unk54[0] = key->rx * 2;
            work->unk54[1] = key->ry * 2;
            work->unk54[2] = key->rz * 2;
            work->unk60.vx = work->unk60.vy = work->unk60.vz = 0;
            if (frame >= 20) {
                work->unk0 = 3;
                entry->unk0 = -1;
                ex->btlfx_clearEffModels();
                return 1;
            }
        } break;
        }
        {
            VECTOR pos;
            VECTOR sc;
            SVECTOR a;
            SVECTOR b;

            pos.vx = work->unk48[0];
            pos.vy = work->unk48[1];
            pos.vz = work->unk48[2];
            pos.vy += ex->math_cos(work->unk4 * 0x70) >> 6;
            pos.vy += ex->math_sin(work->unk4 << 7) >> 6;
            ex->Hi_DrawEffModel(&work->unk60, &pos, (s32)work->unk54, work->unk78);
            if (c6 != 0) {
                sc.vx = work->unk54[0];
                sc.vy = work->unk54[1];
                sc.vz = work->unk54[2];
                sc.vx += d7;
                sc.vy += e;
                sc.vz += d7;
                ex->Hi_DrawEffModel(&work->unk60, &pos, (s32)&sc, work->unk7C);
                c12 = (c6 >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk7C, c12, c12, c12);
                ex->func_800E6624(work->unk4 * 5, 0x260, 0x100, 0x20, 0x80, 0x240, 0x100);
            }
            if (m != 0) {
                ex->Hi_DrawEffModel(&work->unk60, &pos, (s32)work->unk54, work->unk80);
                c13 = (((m * 2) - 0x1000) >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk80, c13, c13, c13);
            }
            if (f != 0) {
                n3 = work->unk4 * 0x29E;
                a.vx = pos.vx;
                a.vy = pos.vy;
                a.vz = pos.vz;
                FX19F_W(b, 0) = FX19F_W(a, 0);
                FX19F_W(b, 1) = FX19F_W(a, 1);
                work->unk60.vx -= 0x400;
                ex->func_800CFD78((s16 *)&a, (s16 *)&work->unk60, 0x5DC, (s16 *)&a);
                work->unk60.vx += 0x400;
                func_801E7894(entry, &D_801EAA58, &a, &b, g, n3, 0xF0, work->unk4 & 7, f, 0x40);
            }
        }
        env->unk28 = 0x1EA;
        if (alpha != 0) {
            for (k = 0; k < 3; k++) {
                pad = 0x168;
                s1p = (s16 *)ex->btlfx_getFileData(entry->unk8, D_801EA9D0[k]) + 2;
                s2p = (s16 *)ex->btlfx_getFileData(entry->unk8, D_801EA9DC[k]) + 2;
                if (k != 0) {
                    pad = 0x80;
                    if (k == 1) {
                        pad = 0x58;
                    }
                }
                ta = &D_801EAF18[k * 2];
                pt = ta->unk4;
                ta->unk0 = ta->unk2 = 0x10;
                ta->unk1 = 0;
                tb = &D_801EAF18[k * 2 + 1];
                tb->unk0 = tb->unk2 = 0x10;
                tb->unk1 = 0;
                pb = tb->unk4;
                for (j = n = 0; j < 34; j++, n++) {
                    if (j == 17) {
                        pt = pb;
                        n--;
                    }
                    i = base - n;
                    if (i < 0) {
                        i = 0;
                    }
                    i *= 3;
                    pt->vx = work->unk8.vx + ex->btlfx_lerp(s1p[i], s2p[i], lt);
                    pt->vy = work->unk8.vy + ex->btlfx_lerp(s1p[i + 1], s2p[i + 1], lt);
                    pt->vz = work->unk8.vz + ex->btlfx_lerp(s1p[i + 2], s2p[i + 2], lt);
                    pt->pad = pad;
                    pt++;
                }
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 6), 0, NULL, (work->unk4 + k) & 7, type, alpha,
                                    1, (u8 *)ta, 2);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0xF), 0, NULL, (work->unk4 + k) & 7, type,
                                    alpha, 1, (u8 *)tb, 2);
            }
        }
        for (k2 = 0; k2 < 3; k2++) {
            if (arr60[k2] != 0) {
                q1 = (s16 *)ex->btlfx_getFileData(entry->unk8, D_801EA9E8[k2]) + 8;
                q2 = (s16 *)ex->btlfx_getFileData(entry->unk8, D_801EA9F4[k2]);
                q2 += 8;
                tc = &D_801EB268[k2];
                tc->unk0 = tc->unk2 = 0xC;
                tc->unk1 = 0;
                pt2 = tc->unk4;
                for (j2 = 0; j2 < 13; j2++) {
                    i2 = arr70[k2] - j2;
                    if (i2 < 0) {
                        i2 = 0;
                    }
                    i2 *= 3;
                    pt2->vx = work->unk8.vx + ex->btlfx_lerp(q1[i2], q2[i2], arr80[k2]) * 2;
                    pt2->vy = work->unk8.vy + ex->btlfx_lerp(q1[i2 + 1], q2[i2 + 1], arr80[k2]) * 3 / 2;
                    pt2->vz = work->unk8.vz + ex->btlfx_lerp(q1[i2 + 2], q2[i2 + 2], arr80[k2]) * 2;
                    pt2->pad = 0xC8;
                    pt2++;
                }
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x18), 0, (s32 *)&work->unk8,
                                    (work->unk4 + k2) & 7, arr90[k2], arr60[k2], 1, (u8 *)tc, 1);
            }
        }
        env->unk28 = 0;
        work->unk4++;
    }
    return 0;
}

s32 fx_19f_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    Fx19fKeys *k;
    s32 frame;
    s32 f;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801EAA48 = env;
    } else {
        frame = entry->unk0;
        k = (Fx19fKeys *)ex->btlfx_getFileData(entry->unk8, 0x2B);
        f = frame;
        if (f >= k->unk2) {
            f = k->unk2 - 1;
        }
        k = (Fx19fKeys *)((u8 *)k + (f * 6 + 4));
        v.vx = k->unk0;
        v.vy = k->unk2 - 100;
        v.vz = ((s16 *)k)[2];
        env->unk2C->unk0[0].vx = v.vx;
        env->unk2C->unk0[0].vy = v.vy;
        env->unk2C->unk0[0].vz = v.vz;
        k = (Fx19fKeys *)ex->btlfx_getFileData(entry->unk8, 0x2C);
        f = frame;
        if (f >= k->unk2) {
            f = k->unk2 - 1;
        }
        k = (Fx19fKeys *)((u8 *)k + (f * 6 + 4));
        v.vx = k->unk0;
        v.vy = k->unk2 - 100;
        v.vz = ((s16 *)k)[2];
        env->unk2C->unk0[1].vx = v.vx;
        env->unk2C->unk0[1].vy = v.vy;
        env->unk2C->unk0[1].vz = v.vz;
        if (frame >= 0x5A) {
            return 1;
        }
    }
    return 0;
}

void func_801E9968(Unk80100C78Entry *entry, s32 file, s32 ot, Fx19fTrail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip) {
    Unk800F78A4 *env = D_801EAA48;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR rot;
    s32 i;
    s32 cnt;
    s32 t;
    s32 h;
    SVECTOR *pt;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    cnt = (n - 4) / 8;
    trail->unk0 = trail->unk2 = cnt - 1;
    trail->unk1 = 0;
    pt = trail->unk4;
    for (i = 0; i < cnt; i++, pt++) {
        t = 0x1000 - (i << 12) / (cnt - 1);
        t += *noise++ * 2 / 3;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = *noise++;
        a = t >> 1;
        h = (ex->math_sin(a) * h) >> 12;
        h += ex->btlfx_lerp(h0, h1, t);
        ex->func_800D044C(&pos, (s32)pt, 0, (XY32 *)&rot, h, ang);
        if (dip != 0) {
            pt->vy -= (ex->math_sin(a) * dip) >> 12;
        }
        pt->pad = pad;
        ang += dang + *noise++;
    }
    env->unk28 = 0x80;
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, file), 0, NULL, type, 0x1000, alpha, 1, (u8 *)trail, ot);
    env->unk28 = 0;
}

void func_801E9C08(Unk80100C78Entry *entry, s32 file, s32 ot, Fx19fTrail *path, Fx19fTrail *trail, s32 n, s32 *noise,
                   s32 i, s32 type, s32 alpha, s32 pad) {
    SVECTOR *pt = path->unk4;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EAA48);
    SVECTOR mid;

    pt += i;
    ex->btlfx_lerpVec((s16 *)(pt + 1), (s16 *)pt, 0x4000, (s16 *)&mid);
    func_801E9968(entry, file, ot, trail, n, noise, pt, &mid, 0, 0x20, ex->btlfx_rand(), 0x33, type, alpha, pad, 0);
}

s32 func_801E9D20(s32 mode, Fx19fBolt *b, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 c;
    s32 k;
    s32 s;
    s32 kind;
    Fx19fTrail *tr;
    Fx19fTrail *tr2;

    if (mode != 1) {
        kind = b->unk11;
        if (kind < 2) {
            if (kind < 0) {
                return 0;
            }
            t = frame << 9;
            c = ex->math_cos(t >> 2);
            tr = (Fx19fTrail *)&D_801EB3B8[b->unk10];
            tr2 = (Fx19fTrail *)&D_801EC538[b->unk10];
            if (b->unk11 == 0) {
                b->unk8.vx = D_801EA9AC.vx;
                b->unk8.vy = D_801EA9AC.vy;
                b->unk8.vz = D_801EA9AC.vz;
                func_801E9968(entry, 0x1E, 0, tr, 0x8C, b->unk18, &b->unk0, &b->unk8, 0, 0x180, frame * 0x97, 0x140, frame,
                              c, ((0x1000 - t) * b->unk15) >> 12, ((t * 3 * 128) >> 12) + 0x80);
            } else {
                if (frame == 0) {
                    k = 2;
                } else if (frame < 4) {
                    k = 3;
                } else {
                    k = 0;
                }
                s = ((0x1000 - t) * b->unk15) >> 12;
                if (frame == 0) {
                    s = (s * 3) >> 1;
                }
                func_801E9968(entry, 2, 0, tr, 0x8C, b->unk18 + frame * k, &b->unk0, &b->unk8, 0, 0x180, frame * 0xC0,
                              0x168, frame, c, s, 0);
                func_801E9C08(entry, 3, 0, tr, tr2, 0x24, b->unk18, b->unk12, frame, c, s >> 2);
            }
            if (frame >= 8) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_801E9FC4(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EAA48);
    SVECTOR *p = D_801EB3BC[ex->btlfx_rand() % 32].unk0;

    p += ex->btlfx_rand() % 17;

    out->vx = p->vx;
    out->vy = p->vy;
    out->vz = p->vz;
    out->pad = 1;
    return 0;
}

s32 fx_19f_op83(s32 mode, Fx19fWork83 *work, Unk800F78A4 *env, Fx19fInst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR base;
    SVECTOR p;
    SVECTOR q;
    SVECTOR c;
    SVECTOR d;
    s32 frame;
    s32 i;
    s32 n;
    s32 r;
    s32 a;
    s32 col;
    s32 dist;
    Fx19fBolt *b;
    Fx19fBolt *b2;
    s32 *np;
    s32 k;

    if (mode == 0) {
        init->unk0 = 0xC;
        init->unk8 = 0x1C;
        init->unk14 = (s32)func_801E9D20;
        init->unk4 = 0x20;
    } else if (mode == 1) {
        i = init->unk0;
        D_801EAA48 = env;
        work->unk0 = D_801EC9B8[0];
        np = D_801EC9B8[0];
        for (n = 0; n < 64; n++) {
            *np++ = ex->btlfx_randCentered(0xD2);
            *np++ = ex->btlfx_randCentered(0x1C3);
            *np++ = ex->btlfx_randCentered(0xDC);
        }
        work->unk8 = i;
        work->unk4 = 0;
    } else {
        frame = inst->unk0;
        switch (work->unk8) {
        case 0:
            FX19F_W(base, 0) = FX19F_W(D_801EA9AC, 0);
            FX19F_W(base, 1) = FX19F_W(D_801EA9AC, 1);
            if (frame >= 0x14) {
                return 1;
            }
            work->unk4++;
            FX19F_W(p, 0) = FX19F_W(base, 0);
            p.vy = 0;
            FX19F_W(p, 1) = FX19F_W(base, 1);
            FX19F_W(q, 0) = FX19F_W(base, 0);
            FX19F_W(q, 1) = FX19F_W(base, 1);
            b = ex->btlfx_poolSpawn(inst->unk10);
            if (b != NULL) {
                r = ex->btlfx_randCentered(0x200) + 0x7D0;
                k = ex->btlfx_randCentered(0x200) - 0x200;
                a = ((work->unk4 / 2) << 10) + k;
                p.vx += (ex->math_cos(a) * r) >> 12;
                p.vz += (ex->math_sin(a) * r) >> 12;
                ((s32 *)&b->unk0)[0] = FX19F_W(p, 0);
                ((s32 *)&b->unk0)[1] = FX19F_W(p, 1);
                ((s32 *)&b->unk8)[0] = FX19F_W(q, 0);
                ((s32 *)&b->unk8)[1] = FX19F_W(q, 1);
                b->unk11 = 0;
                b->unk10 = work->unk4 % 32;
                b->unk18 = work->unk0 + (ex->btlfx_rand() & 0x1F);
                b->unk12 = ex->btlfx_rand() % 15;
                b->unk13 = ex->btlfx_rand() % 15;
                b->unk15 = 0x3E;
                b->unk14 = 0;
            }
            work->unk4++;
            FX19F_W(p, 0) = FX19F_W(base, 0);
            FX19F_W(q, 0) = FX19F_W(base, 0);
            p.vy = 0;
            FX19F_W(p, 1) = FX19F_W(base, 1);
            FX19F_W(q, 1) = FX19F_W(base, 1);
            if (frame >= 0xB) {
                b = ex->btlfx_poolSpawn(inst->unk10);
                if (b != NULL) {
                    r = ex->btlfx_randCentered(0x100) + 0x3E8;
                    a = ((work->unk4 / 2) << 10) + ex->btlfx_randCentered(0x200);
                    p.vx += (ex->math_cos(a) * r) >> 12;
                    p.vz += (ex->math_sin(a) * r) >> 12;
                    ((s32 *)&b->unk0)[0] = FX19F_W(p, 0);
                    ((s32 *)&b->unk0)[1] = FX19F_W(p, 1);
                    ((s32 *)&b->unk8)[0] = FX19F_W(q, 0);
                    ((s32 *)&b->unk8)[1] = FX19F_W(q, 1);
                    b->unk11 = 0;
                    b->unk10 = work->unk4 % 32;
                    b->unk18 = work->unk0 + (ex->btlfx_rand() & 0xF) * 3;
                    b->unk12 = ex->btlfx_rand() % 15;
                    b->unk13 = ex->btlfx_rand() % 15;
                    b->unk15 = 0x32;
                    b->unk14 = 0;
                }
            }
            if (frame >= 0x14) {
                return 1;
            }
            break;
        case 1:
            FX19F_W(p, 0) = FX19F_W(D_801EA9AC, 0);
            FX19F_W(p, 1) = FX19F_W(D_801EA9AC, 1);
            if (frame < 0x18) {
                col = D_801EA9B4 * 100 / 2048 + 0x3C;
                if (col >= 0x100) {
                    col = 0xFF;
                }
                for (i = 0; i < 2; i++) {
                    work->unk4++;
                    FX19F_W(c, 0) = FX19F_W(p, 0);
                    FX19F_W(d, 0) = FX19F_W(p, 0);
                    FX19F_W(c, 1) = FX19F_W(p, 1);
                    FX19F_W(d, 1) = FX19F_W(p, 1);
                    b2 = ex->btlfx_poolSpawn(inst->unk10);
                    if (b2 != NULL) {
                        dist = D_801EA9B4;
                        ex->btlfx_randRotation(&q);
                        ex->func_800CFD78((s16 *)&d, (s16 *)&q, 0x180, (s16 *)&c);
                        ex->func_800CFD78((s16 *)&d, (s16 *)&q, dist + 0x180, (s16 *)&d);
                        ((s32 *)&b2->unk0)[0] = FX19F_W(c, 0);
                        ((s32 *)&b2->unk0)[1] = FX19F_W(c, 1);
                        ((s32 *)&b2->unk8)[0] = FX19F_W(d, 0);
                        ((s32 *)&b2->unk8)[1] = FX19F_W(d, 1);
                        b2->unk11 = 1;
                        b2->unk10 = work->unk4 % 32;
                        b2->unk18 = work->unk0 + (ex->btlfx_rand() & 0x1F);
                        b2->unk12 = ex->btlfx_rand() % 15;
                        b2->unk13 = ex->btlfx_rand() % 15;
                        b2->unk15 = col;
                        b2->unk14 = 0;
                    }
                }
                if (frame >= 0xD) {
                    for (i = 0; i < 2; i++) {
                        work->unk4++;
                        FX19F_W(c, 0) = FX19F_W(p, 0);
                        FX19F_W(d, 0) = FX19F_W(p, 0);
                        FX19F_W(c, 1) = FX19F_W(p, 1);
                        FX19F_W(d, 1) = FX19F_W(p, 1);
                        b2 = ex->btlfx_poolSpawn(inst->unk10);
                        if (b2 != NULL) {
                            dist = D_801EA9B4;
                            ex->btlfx_randRotation(&q);
                            ex->func_800CFD78((s16 *)&d, (s16 *)&q, 0x180, (s16 *)&c);
                            ex->func_800CFD78((s16 *)&d, (s16 *)&q, dist + 0x180, (s16 *)&d);
                            ((s32 *)&b2->unk0)[0] = FX19F_W(c, 0);
                            ((s32 *)&b2->unk0)[1] = FX19F_W(c, 1);
                            ((s32 *)&b2->unk8)[0] = FX19F_W(d, 0);
                            ((s32 *)&b2->unk8)[1] = FX19F_W(d, 1);
                            b2->unk11 = 1;
                            b2->unk10 = work->unk4 % 32;
                            b2->unk18 = work->unk0 + (ex->btlfx_rand() & 0xF) * 3;
                            b2->unk12 = ex->btlfx_rand() % 15;
                            b2->unk13 = ex->btlfx_rand() % 15;
                            b2->unk15 = col - 0x32;
                            b2->unk14 = 0;
                        }
                    }
                }
            }
            if (frame >= 0x18) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_19f/nonmatchings/fx_19f", fx_19f_headerPtr);
