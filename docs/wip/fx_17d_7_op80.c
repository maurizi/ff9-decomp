/* Draft of fx_17d_7_op80. Status: 4 aligned mismatches. Only difference: case 2 frame<21 tint, c3 = c2 >> 1 is
 * "srav t2,a2,a1" in the original (a1 = 1 is the first insn of the block after sched1, so reload_cse turns the 1 into
 * $a1); here sched1 keeps c3/s9C before the call's a1 load. Notes: case 3 braced (else the tail's locals do not
 * reuse case 1/2 slots), separate per-use temporaries, loop1 is a for loop over an explicit pointer, loop2 a plain
 * i loop, yy block-local, m/mm split (birthing-insn priority in sched1 depends on REG_N_SETS == 1). */
#include "fxmod/fx_17d_7.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* counter */
    SVECTOR unk8; /* position */
    SVECTOR unk10;
    SVECTOR unk18; /* camera */
    u8 pad20[8];
    s32 unk28[3]; /* model position */
    SVECTOR unk34;
    SVECTOR unk3C;
    s32 unk44[3]; /* model scale */
    s32 unk50[3];
    SVECTOR unk5C;
    Pool6CEntry *unk64;
    Pool6CEntry *unk68;
    Pool6CEntry *unk6C;
    s32 unk70; /* models */
    s32 unk74;
    s32 unk78;
    s32 unk7C;
    s32 unk80;
    s32 unk84;
    s32 unk88;
    s32 unk8C;
    s32 unk90;
    s32 unk94;
    s32 unk98;
    s32 unk9C;
    s32 unkA0;
} Fx17d7Work80; /* 0xA4 */

extern BtlModel D_801EF2EC[];
extern SVECTOR D_801EF258;
extern s16 D_801EF264[];
s32 func_801EC704(SVECTOR *v, SVECTOR *rot);
s32 func_801EC7C8(s32 mode, Fx17d7Rock *o, Unk800F78A4 *env, s32 arg3, s32 frame, s32 arg5, Fx17d7RockInst *inst);

s32 fx_17d_7_op80(s32 mode, Fx17d7Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sc;
    VECTOR pos;
    SVECTOR rot;
    SVECTOR p60;
    s32 frame;
    s32 t;
    s32 b;
    s32 s9C;
    s32 c3;
    s32 c0;
    s32 c1;
    s32 c2;
    s32 c4;
    s32 c5;
    s32 s;
    s32 q;
    s32 d;
    s32 e;
    s32 k;
    s32 i;
    s32 y;
    s32 a;
    Pool6CEntry *p;
    Fx17d7Rock *o;
    s16 *rp;
    s32 sh1;
    s32 sh2;
    s32 y2;
    s32 x2;
    Fx17d7Rock *o2;
    Fx17d7Rock *o1;

    if (mode == 0) {
        arg4->unk0 = 0xA4;
        arg4->unk8 = 0x24;
        arg4->unk14 = (s32)func_801EC7C8;
        arg4->unk4 = 0x28;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EF2DC = env;
        ex->func_800E3C58();
        ex->btlfx_initEffModels(D_801EF2EC, 0xE);
        work->unk70 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x90), 0);
        work->unk74 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8E), 0);
        work->unk78 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8C), 1);
        work->unk7C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x97), 1);
        work->unk80 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x8F), 1);
        work->unk84 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x96), 0);
        work->unk88 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x98), 0);
        work->unk8C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x99), 0);
        work->unk90 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x95), 1);
        work->unk94 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x92), 1);
        work->unk98 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x9A), 1);
        work->unk9C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x93), 1);
        work->unkA0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x94), 1);
        work->unk34.vx = 0;
        work->unk34.vy = 0x320;
        work->unk34.vz = 0;
        work->unk3C.vx = 0;
        work->unk3C.vy = 0;
        work->unk3C.vz = 0;
        work->unk28[0] = D_801EF258.vx;
        work->unk28[1] = D_801EF258.vy;
        work->unk28[2] = D_801EF258.vz;
        work->unk44[1] = 0x1800;
        work->unk44[2] = 0x1800;
        work->unk44[0] = 0x1800;
        BTLFX_COPY_SVEC(&work->unk8, &D_801EF258);
        work->unk8.vy -= 0x200;
        work->unk18.vx = D_801EF258.vx;
        work->unk18.vy = D_801EF258.vy;
        work->unk18.vz = D_801EF258.vz;
        work->unk18.vy -= 0x300;
        env->unk2C->unk0[3].vx = work->unk18.vx;
        env->unk2C->unk0[3].vy = work->unk18.vy;
        env->unk2C->unk0[3].vz = work->unk18.vz;
    } else {
        b = 0;
        s9C = 0;
        c3 = 0;
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            t = frame << 8;
            if (frame < 18) {
                c0 = 0xFF - frame * 0xFF / 18;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, c0, c0 * 2 / 3, c0 / 2);
                if (frame < 2) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0xFF, 0xFF, 0xFF);
                }
            }
            work->unk18.vy -= 4 + ((ex->math_cos(t >> 2) * 10) >> 12);
            env->unk2C->unk0[3].vx = work->unk18.vx;
            env->unk2C->unk0[3].vy = work->unk18.vy;
            env->unk2C->unk0[3].vz = work->unk18.vz;
            b = 0x1000;
            if (frame >= 16) {
                work->unk0 = 1;
                entry->unk0 = -1;
                work->unk5C.vx = -0x400;
                work->unk5C.vy = 0x400;
                work->unk5C.vz = 0;
                work->unk50[0] = work->unk8.vx;
                work->unk50[1] = work->unk8.vy;
                work->unk50[2] = work->unk8.vz;
                BTLFX_COPY_SVEC(&D_801EFDE4, &work->unk8);
                BTLFX_COPY_SVEC(&D_801EFDEC, &work->unk8);
                D_801EFDEC.vy -= 0xDAC;
            }
            break;
        case 1: {
            VECTOR v;

            t = frame << 8;
            if (frame < 6) {
                d = (frame << 12) / 6;
                s = d >> 2;
                q = d;
            } else {
                d = ((frame - 6) << 12) / 10;
                s = 0xC00 - ((d << 11) >> 13);
                q = 0x1000;
                if (frame == 6) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xFF, 0x80, 0x80);
                } else {
                    c1 = (d << 7) >> 12;
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c1, c1, c1 * 2 / 3);
                }
                e = d >> 2;
                c3 = (d << 6) >> 12;
                sc.vx = sc.vz = ex->math_sin(e) * 2 + 0x3000;
                sc.vy = ex->math_sin(e) * 4;
                pos.vx = work->unk8.vx;
                pos.vy = work->unk8.vy;
                pos.vz = work->unk8.vz;
                rot.vx = 0;
                rot.vy = d >> 3;
                rot.vz = 0;
                s9C = 0x300;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk7C);
                ex->Hi_ModifyEffModelRGB(work->unk7C, (ex->math_cos(e) >> 5) - 0x80, (ex->math_cos(e) >> 5) - 0x80,
                                         (ex->math_cos(e) >> 5) - 0x80);
                ex->func_800E6624(frame * 12, 0x340, 0x100, 0x10, 0x40, 0x350, 0x100);
            }
            v.vx = v.vy = s;
            v.vz = 0x800;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)work->unk50, (s32)&v, work->unk78);
            ex->Hi_ModifyEffModelRGB(work->unk78, (q >> 5) - 0x80, (q >> 5) - 0x80, (q >> 5) - 0x80);
            if (frame == 6) {
                work->unk64 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk8);
                if (work->unk64 != NULL) {
                    work->unk64->unk22 = 0x80;
                }
            }
            work->unk18.vy -= 3;
            env->unk2C->unk0[3].vx = work->unk18.vx;
            env->unk2C->unk0[3].vy = work->unk18.vy;
            b = 0x1001 - t;
            env->unk2C->unk0[3].vz = work->unk18.vz;
            if (frame >= 16) {
                work->unk0 = 2;
                entry->unk0 = -1;
                BTLFX_COPY_SVEC(&work->unk10, &work->unk8);
                work->unk10.vy -= 0x800;
                work->unk68 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk10);
                if (work->unk68 != NULL) {
                    work->unk68->unk22 = 0x80;
                }
            }
            break;
        }
        case 2: {
            VECTOR v;
            SVECTOR cam;
            SVECTOR cam2;
            s32 w;
            s32 m;
            s32 mm;
            s32 z;
            s32 g;
            s32 h;

            t = (frame << 12) / 70;
            if (frame < 5) {
                q = 0x800;
                m = (frame << 12) / 5;
                if (work->unk68 != NULL && work->unk68->unk30 == -1) {
                    work->unk68 = NULL;
                }
                BTLFX_COPY_SVEC(&p60, &work->unk8);
                p60.vy -= ((0x1000 - m) << 11) >> 12;
                ex->func_800E3D20((s32)work->unk68, (s32)&p60);
                sc.vx = sc.vz = q * 4;
                rot.vx = 0;
                rot.vy = m >> 3;
                rot.vz = 0;
                pos.vx = p60.vx;
                pos.vy = p60.vy;
                pos.vz = p60.vz;
                sc.vy = (q * (m + 0x400)) >> 8;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk7C);
                ex->func_800E6624(frame * 12, 0x340, 0x100, 0x10, 0x40, 0x350, 0x100);
                if (frame == 1 || frame == 3) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x80, 0x60, 0x10);
                }
            } else {
                w = frame - 5;
                m = (w << 12) / 65;
                q = ((0x1000 - m) * 0x1800) >> 12;
                if (frame < 21) {
                    g = 0x1000 - ex->math_cos((w << 10) >> 4);
                    pos.vx = work->unk8.vx;
                    pos.vy = work->unk8.vy;
                    pos.vz = work->unk8.vz;
                    h = g >> 2;
                    pos.vy -= h;
                    rot.vx = 0;
                    rot.vy = frame << 5;
                    rot.vz = 0;
                    sc.vx = sc.vz = (g * 5) >> 2;
                    sc.vy = 0x1000;
                    ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unkA0);
                    ex->Hi_ModifyEffModelRGB(work->unkA0, ((ex->math_cos(h) - 0x800) >> 5) - 0x80,
                                             ((ex->math_cos(h) - 0x800) >> 5) - 0x80,
                                             ((ex->math_cos(h) - 0x800) >> 5) - 0x80);
                }
                if (frame == 5) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x80, 0x80, 0x80);
                    p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk8);
                    if (p != NULL) {
                        p->unk22 = 0x100;
                    }
                    p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk8);
                    if (p != NULL) {
                        p->unk22 = 0x100;
                    }
                    BTLFX_COPY_SVEC(&work->unk10, &work->unk8);
                    work->unk10.vy -= 0x400;
                    p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk10);
                    if (p != NULL) {
                        p->unk24->unk0 = -1;
                        *(s32 *)&p->unk24->unk3C = (s32)func_801EC704;
                        p->unk22 = 0x100;
                    }
                } else if (frame < 21) {
                    c2 = 0x80 - ((w << 7) >> 4);
                    c3 = c2 >> 1;
                    s9C = 0x400;
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 16), 1, c2, c2, c2 >> 2);
                }
            }
            v.vx = v.vy = q;
            v.vz = 0x800;
            ex->Hi_DrawEffModel(&work->unk5C, (VECTOR *)work->unk50, (s32)&v, work->unk78);
            ex->Hi_ModifyEffModelRGB(work->unk78, (ex->math_cos(t >> 2) >> 5) - 0x80, (ex->math_cos(t >> 2) >> 5) - 0x80,
                                     (ex->math_cos(t >> 2) >> 5) - 0x80);
            work->unk18.vy -= 2;
            env->unk2C->unk0[3].vx = work->unk18.vx;
            env->unk2C->unk0[3].vy = work->unk18.vy;
            b = ((0x1000 - t) >> 1) + 1;
            env->unk2C->unk0[3].vz = work->unk18.vz;
            if (frame >= 6) {
                mm = frame - 6;
                sc.vx = sc.vy = sc.vz = mm * 0x60;
                pos.vx = work->unk8.vx;
                pos.vy = work->unk8.vy;
                pos.vz = work->unk8.vz;
                pos.vy -= 0x400;
                rot.vy = frame << 2;
                rot.vx = 0;
                rot.vz = 0;
                D_801EFDDC = (sc.vx * 0xA28) >> 12;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk80);
                ex->func_800E6624(frame * 8, 0x360, 0x100, 0x20, 0x80, 0x360, 0x180);
                z = mm * 0x122B / 64;
                sc.vx = sc.vy = sc.vz = z;
                pos.vx = work->unk8.vx;
                pos.vy = work->unk8.vy;
                pos.vz = work->unk8.vz;
                rot.vx = 0;
                rot.vy = frame * 8;
                rot.vz = 0;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk90);
                ex->Hi_ModifyEffModelRGB(work->unk90, -0x30, -0x30, -0x30);
                rot.vy = 0;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk94);
                ex->Hi_ModifyEffModelRGB(work->unk94, -0x20, -0x20, -0x20);
                sc.vx = sc.vy = sc.vz = z / 2;
                rot.vx = 0;
                rot.vy = frame << 4;
                rot.vz = 0;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk90);
                ex->func_800E6624(frame * ((t >> 9) + 12), 0x200, 0x180, 0x20, 0x80, 0x220, 0x180);
                rot.vx = (frame << 4) + 0x200;
                rot.vy = frame << 5;
                rot.vz = 0;
                sc.vx = sc.vy = sc.vz = z;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk98);
                ex->Hi_ModifyEffModelRGB(work->unk98, -0x20, -0x20, -0x20);
                rot.vx = (frame << 5) - 0x200;
                rot.vy = frame << 6;
                rot.vz = 0;
                sc.vx = sc.vy = sc.vz = z * 2 / 3;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->unk98);
                ex->func_800E6624(frame * ((t >> 8) + 8), 0x200, 0x100, 0x20, 0x80, 0x220, 0x100);
            }
            if (frame >= 35) {
                s32 yy;

                env->unk28 = 0x100;
                yy = ((frame - 35) << 12) / 35;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), &work->unk8, NULL, yy + 0xAAA, 0, 1, yy);
                env->unk28 = 0;
            }
            if (frame >= 58) {
                c4 = (frame - 58) * 0xFF / 12;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 256), 1, c4, c4, c4 * 2 / 3);
            }
            if (frame == 41) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            if (frame >= 6 && frame < 14) {
                sh1 = ex->math_cos(((frame - 6) << 10) >> 3) >> 7;
                BTLFX_COPY_SVEC(&cam, &work->unk18);
                cam.vy += ex->btlfx_randCentered(sh1);
                cam.vx += ex->btlfx_randCentered(sh1);
                env->unk2C->unk0[3].vx = cam.vx;
                env->unk2C->unk0[3].vy = cam.vy;
                env->unk2C->unk0[3].vz = cam.vz;
            }
            if (frame >= 35) {
                sh2 = 0x1000 - ex->math_cos(((frame - 35) << 10) / 35);
                sh2 >>= 6;
                BTLFX_COPY_SVEC(&cam2, &work->unk18);
                cam2.vy += ex->btlfx_randCentered(sh2);
                cam2.vx += ex->btlfx_randCentered(sh2);
                env->unk2C->unk0[3].vx = cam2.vx;
                env->unk2C->unk0[3].vy = cam2.vy;
                env->unk2C->unk0[3].vz = cam2.vz;
            }
            if (frame == 58) {
                BTLFX_COPY_SVEC(&work->unk10, &work->unk8);
                work->unk10.vz -= 0x5DC;
                work->unk10.vy -= 0x1AE;
                work->unk6C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk10);
                if (work->unk6C != NULL) {
                    work->unk6C->unk22 = 0x200;
                }
            }
            if (frame >= 70) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        }
        case 3: {
            s32 t;
            s32 n;
            s32 n5;
            s32 nz;

            t = (frame << 12) / 41;
            if (work->unk6C != NULL) {
                if (work->unk6C->unk30 == -1) {
                    work->unk6C = NULL;
                }
                if (work->unk6C != NULL) {
                    work->unk6C->unk22 = -10 - ((t << 5) >> 12);
                }
            }
            if (frame < 42) {
                if (frame == 0) {
                    for (i = 0, rp = D_801EF264, y = 0; i < 24; rp++, i++, y -= 0x18) {
                        o = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                        if (o != NULL) {
                            n = (i << 7) + 0x400;
                            o->unkC.vx = work->unk8.vx + *rp * 3 / 4;
                            o->unkC.vz = work->unk8.vz + y;
                            o->unkC.vy = work->unk8.vy + 0x300;
                            o->unk14.vx = 0x400;
                            o->unk14.vy = 0x400;
                            o->unk14.vz = 0;
                            o->unk0 = 0;
                            n5 = n * 5;
                            o->unkA = n5 >> 2;
                            o->unk22 = y;
                            o->unk20 = i % 3;
                            o->unk4 = ex->btlfx_randCentered(0xAA);
                            o->unk8 = ex->btlfx_randCentered(0xAA);
                            o->unk6 = ex->btlfx_randCentered(0x80) - ((n5 << 6) >> 12);
                            o->unk1C = 0;
                            o->unk1E = 0;
                        }
                    }
                }
                D_801EF260 = -((t * 0x241) >> 12);
                if (!(frame & 1)) {
                    o1 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (o1 != NULL) {
                        a = (0x1000 - ex->math_cos(t >> 2)) * 3 / 4 + 0x400;
                        nz = -(t >> 3);
                        o1->unkC.vx = work->unk8.vx + ex->btlfx_randCentered(a >> 1) * 5;
                        o1->unkC.vz = work->unk8.vz + nz;
                        o1->unkC.vy = work->unk8.vy + 0x200;
                        o1->unk14.vx = 0x400;
                        o1->unk0 = 1;
                        o1->unk14.vy = 0;
                        o1->unk14.vz = 0;
                        o1->unkA = a * 3 / 2;
                        o1->unk20 = ex->btlfx_rand() % 3;
                        o1->unk1C = ex->btlfx_randCentered(0x40);
                        o1->unk1E = ex->btlfx_randCentered(0x40);
                        o1->unk4 = ex->btlfx_randCentered(0x80);
                        o1->unk8 = ex->btlfx_randCentered(0x80);
                        o1->unk6 = ex->btlfx_randCentered(0x80) - ((a << 9) >> 12);
                    }
                }
                if (frame == 40) {
                    for (i = 0; i < 2; i++) {
                        o2 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                        if (o2 != NULL) {
                            o2->unkC.vx = work->unk8.vx + i * 0x280 - 0x140;
                            o2->unkC.vz = work->unk8.vz + 0x2BC;
                            o2->unkC.vy = work->unk8.vy + 0xBB8;
                            o2->unk0 = 1;
                            o2->unkA = 0x4000;
                            o2->unk1C = 0x32 - i * 0x64;
                            o2->unk1E = 0x18;
                            o2->unk14.vx = 0x400;
                            o2->unk14.vy = 0x400;
                            o2->unk14.vz = 0;
                            o2->unk20 = 0;
                            o2->unk4 = 0;
                            o2->unk8 = 0;
                            o2->unk6 = -0x11A;
                        }
                    }
                }
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 512), 1, 0xFF, 0xFF, 0xFF);
            } else {
                c5 = 0xFF - (frame - 41) * 0xFF / 8;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 512), 1, c5, c5, c5);
            }
            c3 = ((t << 6) >> 12) + 0x20;
            s9C = (t >> 3) + 0x100;
            b = 0;
            if (frame >= 49) {
                ex->btlfx_poolClear((Unk800DA1B8 *)entry->unk10);
                ex->func_800E3C58();
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        }
        env->unk28 = 0;
        if (c3 != 0) {
            CVECTOR col;
            SVECTOR r2;
            VECTOR s2;

            col.r = col.g = col.b = c3;
            r2.vx = 0;
            r2.vy = 0;
            r2.vz = 0;
            s2.vx = s2.vy = s2.vz = s9C + 0x1000;
            ex->func_800E6764(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &r2, &s2, &col, 1, 0);
        }
        if (b != 0) {
            work->unk34.vy += 2;
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)work->unk28, (s32)work->unk44, work->unk74);
            k = (b >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk74, k, k, k);
            work->unk3C.vy += 1;
            ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)work->unk28, (s32)work->unk44, work->unk70);
            ex->Hi_ModifyEffModelRGB(work->unk70, k, k, k);
        }
        work->unk4++;
    }
    return 0;
}
