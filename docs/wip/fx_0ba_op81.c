/* Draft of fx_0ba fx_0ba_op81. Status: 715 strict aligned mismatches (was 1405), 189 with registers and spill-slot
 * offsets masked (sdiff --noreg after spnorm 0xE8); equivalent by construction. Splitting the reused temporaries into
 * one variable per block (t_1.., c_1.., i_1.. for every loop counter, ...) fixed most of the global allocation.
 * The original frame is 0x170 (ours 0x160): it keeps glow/gk/orb in s6/s7/s4, ex in fp, and spills trail..amp
 * (0xEC-0x104), h and the loop pointers. Open: the remaining allocation; the original holds 2 and 4 in s1/s0 after the
 * a60 loop (used by the frame-0x16 tint and the 0x248 call); trail loop 1 increments its trail/id pointers before the
 * two drawTrail calls. */
#include "fxmod/fx_0ba.h"

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* home position */
    SVECTOR unk10; /* orb position */
    SVECTOR unk18; /* camera target */
    u8 pad20[0x48 - 0x20];
    Fx0baVec3 unk48; /* model position */
    Fx0baVec3 unk54; /* model scale */
    SVECTOR unk60; /* model rotation */
    Pool6CEntry *unk68;
    Pool6CEntry *unk6C;
    s32 unk70; /* models */
    s32 unk74;
    s32 unk78;
    s32 unk7C;
    s32 unk80;
} Fx0baWork81; /* 0x84 */

extern BtlModel D_801ECB90[];
extern s16 D_801EC560[];
extern s32 D_801EC578[];
extern s32 D_801EC584[];
extern s32 D_801EC590[];
extern s32 D_801EC59C[];
extern Fx0baStrip D_801ECAC0[];
extern Fx0baTrail D_801ECF80[];
extern u8 D_801ED2D0[];

s32 fx_0ba_op81(s32 mode, Fx0baWork81 *w, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos2;
    VECTOR scale2;
    SVECTOR rot2;
    s32 a60[3];
    s32 a70[3];
    s32 a80[3];
    s32 a90[3];
    VECTOR pos;
    VECTOR scale;
    SVECTOR p0;
    SVECTOR p1;
    SVECTOR r3;
    VECTOR s3v;
    s32 trail;
    s32 len;
    s32 lerp;
    s32 tw;
    s32 gk2;
    s32 bolt;
    s32 amp;
    s32 glow;
    s32 gk;
    s32 orb;
    s32 t_1;
    s32 t_2;
    s32 t_3;
    s32 t2_1;
    s32 t2_2;
    s32 c_1;
    s32 c_2;
    s32 c_3;
    s32 c_4;
    s32 c_5;
    s32 c_6;
    s32 c_7;
    s32 n_1;
    s32 n_2;
    s32 n_3;
    s32 m_1;
    s32 m_2;
    s32 m_3;
    s32 m_4;
    s32 i_1;
    s32 i_2;
    s32 frame;
    s32 state;
    s32 t;
    s32 t2;
    s32 c;
    s32 v;
    s32 i;
    s32 j;
    s32 k;
    s32 n;
    s32 m;
    s32 h;
    s16 *src0;
    s16 *src1;
    s16 *a;
    s16 *b;
    Fx0baPt *pt;
    Fx0baTrail *tr;
    SVECTOR *sv;
    Fx0baStrip *st;
    Pool6CEntry *p;
    s32 tc;
    s32 q;
    s16 *ps;

    if (mode == 0) {
        *arg4 = 0x84;
    } else if (mode == 1) {
        w->unk0 = 0;
        w->unk4 = 0;
        D_801EC5F0 = env;
        ex->btlfx_initEffModels(D_801ECB90, 5);
        w->unk70 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1F), 1);
        w->unk74 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x26), 0);
        w->unk78 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x18), 0);
        w->unk80 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 4), 0);
        w->unk7C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x83), 1);
        w->unk8.vx = 0;
        w->unk8.vy = 0;
        w->unk8.vz = 0;
        ((s32 *)&w->unk18)[0] = ((s32 *)&w->unk8)[0];
        ((s32 *)&w->unk18)[1] = ((s32 *)&w->unk8)[1];
        w->unk18.vy -= 0x1EA;
        env->unk2C->unk0[3].vx = w->unk18.vx;
        env->unk2C->unk0[3].vy = w->unk18.vy;
        env->unk2C->unk0[3].vz = w->unk18.vz;
    } else {
        orb = 0;
        trail = 0;
        len = 0;
        lerp = 0;
        tw = 0;
        gk2 = 0;
        frame = entry->unk0;
        glow = 0;
        bolt = 0;
        a60[0] = a60[1] = a60[2] = 0;
        state = w->unk0;
        gk = 0;
        amp = 0;
        switch (state) {
        case 0:
            pt = (Fx0baPt *)(ex->btlfx_getFileData(entry->unk8, 0x1A) + 4) + frame;
            w->unk48.vx = pt->vx + w->unk8.vx;
            w->unk48.vy = pt->vy + w->unk8.vy;
            w->unk48.vz = pt->vz + w->unk8.vz;
            w->unk60.vx = pt->pad[0];
            w->unk60.vy = pt->pad[1];
            w->unk54.vx = 0x2000;
            w->unk54.vy = 0x2000;
            w->unk54.vz = 0x2000;
            w->unk60.vz = pt->pad[2];
            if (frame >= 7) {
                ex->func_800CFC2C(frame, 7, 8, 0, 0x1000, &tc);
                env->unk28 = -0x80;
                ((SVECTOR *)&pos)->vx = 0x400;
                ((SVECTOR *)&pos)->vy = 0;
                ((SVECTOR *)&pos)->vz = frame * 0x18;
                ((SVECTOR *)&pos)->pad = 1;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x21), &w->unk8, (SVECTOR *)&pos,
                                  ex->math_sin(tc >> 2), 0, 1, ex->math_cos(tc >> 2));
                env->unk28 = 0;
                glow = ex->math_cos(tc >> 2) >> 1;
                gk = ex->math_sin(tc >> 2);
                gk2 = ex->math_sin(tc >> 2) >> 2;
            } else {
                glow = (ex->math_cos((frame << 10) / 7) >> 1) + 0x800;
            }
            if (frame >= 0xF) {
                w->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            trail = 0x1000;
            t_1 = (frame << 12) / 9;
            len = ((trail - ex->math_cos(t_1 >> 2)) << 5) >> 12;
            tw = (t_1 >> 3) + 0xE00;
            if (frame < 4) {
                c_1 = ex->math_cos(frame << 8);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 2, (c_1 * 0x46) >> 12, (c_1 * 0x46) >> 12, (c_1 * 0x46) >> 12);
            }
            if (frame >= 5) {
                c_2 = ex->math_sin((frame - 5) << 8) << 7;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0, c_2 >> 14, c_2 >> 12);
            }
            if (frame == 3) {
                ex->func_800E60A4(1, 6);
            }
            if (frame >= 9) {
                w->unk0 = 2;
                entry->unk0 = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 2, 0xFF, 0xFF, 0);
                ex->func_800E60A4(-1, 2);
                w->unk6C = NULL;
            }
            break;
        case 2:
            t_2 = (frame << 12) / 51;
            if (frame == 0) {
                ex->func_800E60A4(-1, 1);
            }
            if (frame == 2) {
                ex->func_800E60A4(0, 3);
            }
            if (frame < 4) {
                c_3 = 0xFF - frame * 0xFF / 4;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, c_3 >> 2, c_3, c_3);
            }
            if ((u32)(frame - 0x10) < 6) {
                if (frame == 0x10) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x80, 0x80, 0);
                }
                c_4 = 0xFF - (frame - 0x10) * 0xFF / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0, c_4 >> 2, c_4);
            }
            bolt = 0x1000;
            if (frame < 0x10) {
                amp = (ex->math_cos(frame << 6) >> 3) + 0x12C;
            } else {
                amp = 0x12C;
            }
            if (frame < 0x14) {
                orb = ex->math_cos((frame << 10) / 20);
            } else {
                orb = ((frame - 0x14) << 11) / 31;
                if (frame & 1) {
                    orb += 0x800;
                } else {
                    orb += 0x400;
                }
                if (ex->btlfx_rand() & 1) {
                    orb += 0x400;
                }
            }
            if (frame < 0x18) {
                t2_1 = (frame << 12) / 24;
                if (frame < 4) {
                    v = ex->math_sin((frame << 10) >> 2) * 5 / 2;
                } else {
                    v = (frame << 7) + 0x2600;
                }
                scale2.vx = scale2.vy = scale2.vz = v;
                rot2.vz = 0;
                rot2.vx = 0;
                rot2.vy = frame * 16;
                pos2.vx = w->unk8.vx;
                pos2.vy = w->unk8.vy;
                pos2.vz = w->unk8.vz;
                ex->Hi_DrawEffModel(&rot2, &pos2, (s32)&scale2, w->unk70);
                ex->Hi_ModifyEffModelRGB(w->unk70, ((0x1000 - t2_1) >> 5) - 0x80, ((0x1000 - t2_1) >> 5) - 0x80,
                                         ((0x1000 - t2_1) >> 5) - 0x80);
                if (frame < 0x10) {
                    ex->Hi_DrawEffModel(&rot2, &pos2, (s32)&scale2, w->unk74);
                    ex->Hi_ModifyEffModelRGB(w->unk74, ((0xC00 - (frame << 8)) >> 5) - 0x80,
                                             ((0xC00 - (frame << 8)) >> 5) - 0x80, ((0xC00 - (frame << 8)) >> 5) - 0x80);
                }
            }
            if (frame > 0) {
                a = (s16 *)ex->btlfx_getFileData(entry->unk8, 7);
                q = frame + 9;
                if (q >= 0x24) {
                    k = a[0x6E] * 2 / 3 + ((frame - 0x1B) << 6);
                } else {
                    k = *(s16 *)((u8 *)a + q * 6 + 4) * 2 / 3;
                }
                ((s32 *)&w->unk10)[0] = ((s32 *)&w->unk8)[0];
                ((s32 *)&w->unk10)[1] = ((s32 *)&w->unk8)[1];
                if (frame < 0x10) {
                    w->unk10.vy -= (ex->math_sin((frame << 10) >> 4) * 0x56E) >> 12;
                } else {
                    w->unk10.vy -= 0x56E;
                }
                ((s32 *)&D_801EC554)[0] = ((s32 *)&w->unk10)[0];
                ((s32 *)&D_801EC554)[1] = ((s32 *)&w->unk10)[1];
                if (frame < 0x18) {
                    D_801EC55C = 0x1000;
                } else if (frame < 0x20) {
                    D_801EC55C = (k * 0x44C) >> 12;
                } else {
                    D_801EC55C = 0xB86;
                }
                env->unk28 = 0x140;
                if (frame >= 0xF) {
                    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x15), &w->unk10, NULL, k * 5 / 2,
                                            frame & 1, 1, ex->math_cos(t_2 >> 2), 0, 0);
                }
                ((SVECTOR *)&pos)->vy = ((SVECTOR *)&pos)->vx = 0x200;
                ((SVECTOR *)&pos)->vz = frame * 0x2C;
                ((SVECTOR *)&pos)->pad = 0;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1D), &w->unk10, (SVECTOR *)&pos,
                                  ex->btlfx_randCentered(0x1000) + 0x4000, 0, 1, 0x1000);
                env->unk28 = 0;
            }
            if (frame >= 0x14) {
                t2_2 = ((frame - 0x14) << 12) / 31;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), &w->unk10, NULL, (t2_2 >> 2) + 0x2800,
                                  0, 1, ex->math_sin(t2_2 >> 1));
            }
            if (frame == 0x14) {
                w->unk6C = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&w->unk10);
                if (p != NULL) {
                    p->unk22 = 0x96;
                }
            }
            if (frame == 0xE) {
                w->unk68 = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&w->unk8);
                if (p != NULL) {
                    p->unk22 = -0x20;
                }
            }
            if (frame < 8) {
                trail = 0x1000 - (frame << 9);
                len = 0x24;
                tw = 0x1000 - ((frame << 9) >> 2);
                lerp = ex->math_sin((frame << 9) >> 2);
            }
            for (i_1 = 0; i_1 < 3; i_1++) {
                q = frame - i_1 * 3;
                if (frame >= i_1 * 3 && q < 0x16) {
                    if (q < 6) {
                        a60[i_1] = 0x1000;
                        a70[i_1] = q * 2;
                        a80[i_1] = (q << 12) / 22;
                        a90[i_1] = 0x1000;
                    } else {
                        c_5 = 0x1000 - ((q - 6) << 8);
                        a60[i_1] = c_5;
                        a70[i_1] = 0xB;
                        a80[i_1] = (q << 12) / 22;
                        a90[i_1] = c_5;
                    }
                }
            }
            if (frame == 0x16) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x80, 0x80, 0x20);
            }
            if ((u32)(frame - 0x17) < 3) {
                c_6 = 0x80 - ((frame - 0x17) << 6);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c_6, c_6, c_6 >> 3);
            }
            if (frame == 0x18) {
                ex->func_800E60A4(-1, 0);
            }
            if (frame == 0x19) {
                ex->func_800E60A4(0, 5);
            }
            if (frame == 0x1D) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x20, 0x64, 0xFF);
            }
            if (frame == 0x29) {
                ex->func_800E60A4(1, 8);
            }
            if (frame == 0x21) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x40, 0x40, 0);
            }
            if (frame == 0x22) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x40, 0x40, 0x40);
            }
            if (frame >= 0x29) {
                c_7 = (frame - 0x29) * 0xFF / 10;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, c_7 >> 2, c_7 >> 1, c_7);
            }
            if (frame >= 0x2B) {
                ((VECTOR *)&scale.vz)->vx = 0x1020;
                ((VECTOR *)&scale.vz)->vy = 0x1020;
                ((SVECTOR *)&scale)->vx = 0;
                ((SVECTOR *)&scale)->vy = 0;
                ((SVECTOR *)&scale)->vz = 0;
                ((VECTOR *)&scale.vz)->vz = 0x1000;
                ((CVECTOR *)&pos.vz)->r = ((CVECTOR *)&pos.vz)->g = ((CVECTOR *)&pos.vz)->b = ((frame - 0x27) << 5) / 12;
                ex->func_800E6764(0x200, 0xA0, 0x78, 0x50, 0x3C, 4, 4, 2, 2, (SVECTOR *)&scale, (VECTOR *)&scale.vz,
                                  (CVECTOR *)&pos.vz, 1, 0);
            }
            if (w->unk6C != NULL) {
                w->unk6C->unk1C = (t_2 >> 1) + 0x1000;
                if (w->unk6C != NULL) {
                    w->unk6C->unk20 = (0x1000 - (t_2 >> 2)) * 3 / 4;
                }
            }
            if (frame >= 0x33) {
                w->unk0 = 3;
                entry->unk0 = -1;
                ex->func_800E60A4(-1, 6);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x2C), (s32)&w->unk10);
                if (p != NULL) {
                    p->unk22 = 0x96;
                }
            }
            break;
        case 3:
            t_3 = (frame << 12) / 10;
            if (frame < 0xC) {
                func_801E7704((u32 *)(env->unk20 + 2), 1, D_801EC560[frame], 0xC8 - frame * 0xC8 / 12,
                              0xC8 - frame * 0xC8 / 12, 0xFF - (frame << 5) / 12);
            }
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x20, 0xFF, 0xFF);
            }
            if (w->unk68 != NULL && frame < 0xA) {
                if (w->unk68->unk30 == -1) {
                    w->unk68 = NULL;
                }
                if (w->unk68 != NULL) {
                    w->unk68->unk20 = 0x1000 - t_3;
                    if (w->unk68 != NULL) {
                        w->unk68->unk1C = 0x1000 - t_3;
                    }
                }
                if (frame == 9) {
                    ex->func_800E3D54(w->unk68);
                }
            }
            if (w->unk6C != NULL && frame < 8) {
                if (w->unk6C->unk30 == -1) {
                    w->unk6C = NULL;
                }
                if (w->unk6C != NULL) {
                    w->unk6C->unk1C = 0x1000 - (frame << 9);
                }
                if (frame == 7) {
                    ex->func_800E3D54(w->unk6C);
                }
            }
            if (frame < 0xE) {
                ((CVECTOR *)&p1)->r = ((CVECTOR *)&p1)->g = ((CVECTOR *)&p1)->b = 0x89;
                s3v.vx = 0x1000;
                s3v.vy = 0x1000;
                s3v.vz = 0x1000;
                r3.vx = 0;
                r3.vy = 0;
                r3.vz = 0;
                ex->func_800E6764(2, 0xA0, 0x78, 0x50, 0x3C, 4, 4, 2, 2, &r3, &s3v, (CVECTOR *)&p1, 0, 0);
            }
            pt = (Fx0baPt *)(ex->btlfx_getFileData(entry->unk8, 0x19) + 4) + frame;
            w->unk48.vx = pt->vx + w->unk8.vx;
            w->unk48.vy = pt->vy + w->unk8.vy - 0x2EE;
            w->unk48.vz = pt->vz + w->unk8.vz;
            w->unk54.vx = pt->pad[0] * 2;
            w->unk54.vy = pt->pad[1] * 2;
            w->unk60.vx = w->unk60.vy = w->unk60.vz = 0;
            w->unk54.vz = pt->pad[2] * 2;
            if (frame >= 0x14) {
                w->unk0 = 3;
                entry->unk0 = -1;
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        pos.vx = w->unk48.vx;
        pos.vy = w->unk48.vy;
        pos.vz = w->unk48.vz;
        pos.vy += ex->math_cos(w->unk4 * 0x70) >> 6;
        pos.vy += ex->math_sin(w->unk4 << 7) >> 6;
        ex->Hi_DrawEffModel(&w->unk60, &pos, (s32)&w->unk54, w->unk78);
        if (glow != 0) {
            scale.vx = w->unk54.vx;
            scale.vy = w->unk54.vy;
            scale.vz = w->unk54.vz;
            scale.vx += gk;
            scale.vy += gk2;
            scale.vz += gk;
            ex->Hi_DrawEffModel(&w->unk60, &pos, (s32)&scale, w->unk7C);
            ex->Hi_ModifyEffModelRGB(w->unk7C, (glow >> 5) - 0x80, (glow >> 5) - 0x80, (glow >> 5) - 0x80);
            ex->func_800E6624(w->unk4 * 5, 0x160, 0x100, 0x20, 0x80, 0x140, 0x100);
        }
        if (orb != 0) {
            ex->Hi_DrawEffModel(&w->unk60, &pos, (s32)&w->unk54, w->unk80);
            ex->Hi_ModifyEffModelRGB(w->unk80, ((orb * 2 - 0x1000) >> 5) - 0x80, ((orb * 2 - 0x1000) >> 5) - 0x80,
                                     ((orb * 2 - 0x1000) >> 5) - 0x80);
        }
        if (bolt != 0) {
            p0.vz = pos.vz;
            p0.vy = pos.vy;
            p0.vx = pos.vx;
            ((s32 *)&p1)[1] = ((s32 *)&p0)[1];
            ((s32 *)&p1)[0] = ((s32 *)&p0)[0];
            n_1 = w->unk4 * 0x29E;
            w->unk60.vx -= 0x400;
            ex->func_800CFD78((s16 *)&p0, (s16 *)&w->unk60, 0x5DC, (s16 *)&p0);
            w->unk60.vx += 0x400;
            func_801E8778(entry, D_801ECAC0, &p0, &p1, amp, n_1, 0xF0, w->unk4 & 7, bolt, 0x40);
        }
        env->unk28 = 0x1EA;
        if (trail != 0) {
            for (j = 0; j < 3; j++) {
                h = 0x168;
                src0 = (s16 *)(ex->btlfx_getFileData(entry->unk8, D_801EC578[j]) + 4);
                src1 = (s16 *)(ex->btlfx_getFileData(entry->unk8, D_801EC584[j]) + 4);
                if (j != 0) {
                    h = 0x80;
                    if (j == 1) {
                        h = 0x58;
                    }
                }
                tr = &D_801ECF80[j * 2];
                sv = tr[0].unk4;
                k = 0;
                n_2 = 0;
                tr[0].unk2 = tr[0].unk0 = 0x10;
                tr[0].unk1 = 0;
                ps = &tr[0].unk4[0].pad;
                tr[1].unk2 = tr[1].unk0 = 0x10;
                tr[1].unk1 = 0;
                do {
                    if (n_2 == 0x11) {
                        ps = &tr[1].unk4[0].pad;
                        sv = tr[1].unk4;
                        k--;
                    }
                    m_1 = len - k;
                    if (m_1 < 0) {
                        m_2 = 0;
                    }
                    a = src0 + m_2 * 3;
                    b = src1 + m_2 * 3;
                    n_2++;
                    sv->vx = w->unk8.vx + ex->btlfx_lerp(a[0], b[0], lerp);
                    k++;
                    ps[-2] = w->unk8.vy + ex->btlfx_lerp(a[1], b[1], lerp);
                    sv++;
                    ps[-1] = w->unk8.vz + ex->btlfx_lerp(a[2], b[2], lerp);
                    ps[0] = h;
                    ps += 4;
                } while (n_2 < 0x22);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 5), 0, NULL, (w->unk4 + j) & 7, tw, trail,
                                    1, (u8 *)&tr[0], 2);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0xD), 0, NULL, (w->unk4 + j) & 7, tw,
                                    trail, 1, (u8 *)&tr[1], 2);
            }
        }
        for (i_2 = 0; i_2 < 3; i_2++) {
            if (a60[i_2] != 0) {
                src0 = (s16 *)(ex->btlfx_getFileData(entry->unk8, D_801EC590[i_2]) + 0x10);
                src1 = (s16 *)(ex->btlfx_getFileData(entry->unk8, D_801EC59C[i_2]) + 0x10);
                st = (Fx0baStrip *)(D_801ED2D0 + i_2 * 0x6C);
                sv = st->unk4;
                ps = &st->unk4[0].pad;
                st->unk2 = st->unk0 = 0xC;
                st->unk1 = 0;
                n_3 = 0;
                do {
                    m_3 = a70[i_2] - n_3;
                    if (m_3 < 0) {
                        m_4 = 0;
                    }
                    a = src0 + m_4 * 3;
                    b = src1 + m_4 * 3;
                    n_3++;
                    sv->vx = w->unk8.vx + ex->btlfx_lerp(a[0], b[0], a80[i_2]) * 2;
                    sv++;
                    ps[-2] = w->unk8.vy + ex->btlfx_lerp(a[1], b[1], a80[i_2]) * 3 / 2;
                    ps[0] = 0xC8;
                    ps[-1] = w->unk8.vz + ex->btlfx_lerp(a[2], b[2], a80[i_2]) * 2;
                    ps += 4;
                } while (n_3 < 0xD);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x16), 0, (s32 *)&w->unk8, (w->unk4 + i_2) & 7,
                                    a90[i_2], a60[i_2], 1, (u8 *)st, 1);
            }
        }
        env->unk28 = 0;
        w->unk4++;
    }
    return 0;
}
