/* Draft of fx_19f op81 (summon sequence: four states of a model path, bolts, ribbons and screen tints). Status: 4
 * aligned mismatches, one instruction scheduled differently: in case 3's func_801E7704 call the original places
 * sll t0,frame,5 (the (frame << 5) / 12 argument) before the D_801EA9B8 address, here after it. Everything else
 * matches. Levers that got here:
 * - temp slots: case 0 and case 3 braced, case 2 unbraced, case 2's kk loop and the cw/ch constants in their own block
 *   so no expression statement at switch level promotes the address-taken rot/col/sc slots;
 * - spE8 is an ADDRESSOF slot allocated last; spill slot order follows declaration order of the spilled pseudos;
 * - fresh variables per temporary (c1..c13) so they stay call-clobbered;
 * - loop 1's ta/tb are set after the pad if-chain (not every iteration, so the user vars stay copies of the reduced
 *   givs), with ta's header stores between them so the D_801EAF18 high part lives long enough to be hoisted;
 *   pb = tb->unk4 in a variable so pt = pb counts as a biv reset;
 * - case 3 has a dead (frame << 12) / 20 whose highpart cse reuses for the later / 10, and per-block q10/q9;
 * - i *= 3 then s16 indexing gives the original's register ties; q2 = gfd(); q2 += 8; in two statements raises q2's
 *   refs so it beats the k*4 giv for s7 (q1 = ... + 8 in one).
 * cw/ch = 4/2 are variables in the original too (reload_cse turns the + 4 of env->unk20 + 1 into s0). */
#include "fxmod/fx_19f.h"

typedef struct {
    s16 x, y, z, rx, ry, rz;
} Fx19fKey12; /* 0xC */

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* base position */
    SVECTOR unk10;
    SVECTOR unk18;
    u8 pad20[0x28];
    s32 unk48[3]; /* model position */
    s32 unk54[3]; /* model scale */
    SVECTOR unk60; /* model rotation */
    Pool6CEntry *unk68;
    Pool6CEntry *unk6C;
    s32 unk70; /* models */
    s32 unk74;
    s32 unk78;
    s32 unk7C;
    s32 unk80;
} Fx19fWork81; /* 0x84 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    SVECTOR unk4[13];
} Fx19fTrail13; /* 0x6C */

extern s16 D_801EA9B8[];
extern s32 D_801EA9D0[3];
extern s32 D_801EA9DC[3];
extern s32 D_801EA9E8[3];
extern s32 D_801EA9F4[3];
extern Fx19fTrail D_801EAA58;
extern BtlModel D_801EAB28[];
extern Fx19fTrail17 D_801EAF18[6];
extern Fx19fTrail13 D_801EB268[3];
void func_801E7704(s32 *ot, s32 n, s32 a, s32 r, s32 g, s32 b);
void func_801E7894(Unk80100C78Entry *entry, Fx19fTrail *trail, SVECTOR *from, SVECTOR *to, s32 amp, s32 arg5, s32 arg6,
                   s32 arg7, s32 arg8, s32 arg9);

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
            func_801E7704(env->unk20 + 2, 1, D_801EA9B8[frame], 0xC8 - frame * 0xC8 / 12, 0xC8 - frame * 0xC8 / 12,
                          0xFF - (frame << 5) / 12);
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
