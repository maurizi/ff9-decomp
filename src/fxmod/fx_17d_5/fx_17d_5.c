#include "fxmod/fx_17d_5.h"

s32 func_801EC704(s32 mode, Fx17d5Spark *w, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx17d5Work80 *parent) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ang;
    VECTOR pos;
    VECTOR scale;
    SVECTOR *p;
    Pool6CEntry *e;
    s32 k;
    s32 a;
    s32 t; /* one variable for cases 2 and 3: register allocation */
    s32 s;
    s32 c;
    s32 g; /* separate from c: each stays block-local (allocation) */
    s32 m;
    s32 b;

    if (mode == 1) {
        w->unk4 = 0x18;
        return 0;
    }
    switch (w->unk0) {
    case 0:
        if (frame == 0) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 5), (s32)&w->unk8, NULL, -1, 0, 0, 0, w->unk18, 0);
        }
        if (frame < 20) {
            w->unk2 += ex->math_cos((frame << 10) / 20) >> 7;
            w->unk4 += 0x20;
        } else if (frame < 24) {
            w->unk2 -= 0x20;
            w->unk4 += 0x80;
        } else {
            w->unk2 += 0x20;
            if (w->unk2 > 2000) {
                w->unk2 = 2000;
            }
            w->unk4 += 0x18;
            if (w->unk4 > 0x991) {
                w->unk4 = 0x992;
            }
        }
        p = &w->unk8;
        if (w->unk14 == 0) {
            ex->btlfx_aimAngles((s16 *)p, (s16 *)&parent->unk14, (s16 *)&ang);
            ex->btlfx_turnToward((s16 *)&ang, &w->unk10, w->unk4);
            k = frame * 12;
            if (k > 0xF5) {
                k = 0xF5;
            }
            a = w->unk6 * 0x9B + (frame << 7);
            w->unk10 += (ex->math_sin(a + w->unk12) * k) >> 12;
            w->unk12 -= (ex->math_cos(a - w->unk10) * k) >> 12;
            ex->func_800CFD78((s16 *)p, &w->unk10, w->unk2, (s16 *)p);
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 5), (s32)p, NULL, 0, k * 16 + 0x140, 0x1000, 1,
                                w->unk18, 0);
            if (parent->unk0 > 0) {
                w->unk14 = 1;
                ((s16 *)w)[-1] = -1;
            }
        } else {
            ex->btlfx_aimAngles((s16 *)p, (s16 *)&parent->unk14, (s16 *)&ang);
            ex->btlfx_turnToward((s16 *)&ang, &w->unk10, w->unk4);
            k = frame * 4;
            if (k > 0x80) {
                k = 0x80;
            }
            a = w->unk6 * 0x9B + frame * 24;
            w->unk10 += (ex->math_sin(a + w->unk12) * k) >> 12;
            w->unk12 -= (ex->math_cos(a - w->unk10) * k) >> 12;
            ex->func_800CFD78((s16 *)p, &w->unk10, w->unk2, (s16 *)p);
            env->unk28 = 0xFF;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 5), (s32)p, NULL, 0, w->unk22 - frame * 0x23A,
                                0x1000, 1, w->unk18, 0);
            env->unk28 = 0;
            if (w->unk6 == 1) {
                env->camera->points[1].vx = w->unk8.vx;
                env->camera->points[1].vy = w->unk8.vy;
                env->camera->points[1].vz = w->unk8.vz;
                env->camera->points[1].vy -= frame << 5;
                env->camera->points[1].vz -= frame << 5;
            }
        }
        break;
    case 1:
        if (frame == 0) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 9), (s32)&w->unk8, NULL, -1, 0, 0, 0, w->unk18, 0);
        }
        m = 0x1800;
        w->unk8.vz += 0x300;
        w->unk8.vy += 0x300;
        if (w->unk10 == 2) {
            m = 0x5000;
        }
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 9), (s32)&w->unk8, NULL, 0, m, 0x1000, 1, w->unk18, 0);
        if (frame >= 7) {
            w->unk0 = 2;
            ((s16 *)w)[-1] = -1;
            if (w->unk10 == 2) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 2, 0x40, 0x40, 0x40);
                w->unk0 = 3;
            } else if (w->unk10 != 0) {
                c = (ex->btlfx_rand() & 0x1F) + 0x28;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, c, c, c);
            } else {
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&w->unk8);
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
            }
            w->unk8.vy = 0;
        }
        break;
    case 2:
        t = (frame << 12) / 24;
        if (frame < 8) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 9), (s32)&w->unk8, NULL, 0, 0x1000, 0x1000, 1,
                                w->unk18, 0);
        }
        env->unk28 = 0x140;
        if (w->unk10 != 0) {
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &w->unk8, NULL, 0x1000, frame, 1, 0xAAA);
            env->unk28 = 0;
        } else {
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 8), &w->unk8, NULL, 0x1000, frame, 1, 0xC00,
                                    0, 0);
            env->unk28 = 0;
        }
        if (w->unk10 != 0) {
            ang.vz = 0;
            ang.vx = 0;
            ang.vy = frame * 8;
            if (frame == 0) {
                s = 0x555;
            } else {
                s = ((ex->math_sin(t >> 2) * 9) >> 3) + 0x1000;
            }
            scale.vx = scale.vy = scale.vz = (s * 0x578) >> 12;
            pos.vx = w->unk8.vx;
            pos.vy = w->unk8.vy;
            pos.vz = w->unk8.vz;
            ex->Hi_DrawEffModel(&ang, &pos, (s32)&scale, parent->unk88);
            b = t >> 2;
            ex->Hi_ModifyEffModelRGB(parent->unk88, (ex->math_cos(b) >> 5) - 0x80, (ex->math_cos(b) >> 5) - 0x80,
                                     (ex->math_cos(b) >> 5) - 0x80);
        }
        if (frame >= 24) {
            w->unk0 = 2;
            return 1;
        }
        break;
    case 3:
        t = frame << 7;
        if (frame < 8) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 9), (s32)&w->unk8, NULL, 0, 0x5000, 0x1000, 1,
                                w->unk18, 0);
            if (frame < 6) {
                g = 0x80 - t / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, g, g, g);
            }
        }
        env->unk28 = 0x400;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &w->unk8, NULL, 0x1800, frame, 1, 0x1000);
        env->unk28 = 0;
        ang.vz = 0;
        ang.vx = 0;
        ang.vy = frame * 12;
        if (frame == 0) {
            s = 0x800;
        } else if (t < 0xC00) {
            s = ex->math_sin(t / 3) * 2 / 3 + 0xC00;
        } else {
            s = 0x16AA;
        }
        scale.vx = scale.vy = scale.vz = s >> 2;
        pos.vx = w->unk8.vx;
        pos.vy = w->unk8.vy;
        pos.vz = w->unk8.vz;
        ex->Hi_DrawEffModel(&ang, &pos, (s32)&scale, parent->unk90);
        ex->Hi_DrawEffModel(&ang, &pos, (s32)&scale, parent->unk94);
        ex->Hi_DrawEffModel(&ang, &pos, (s32)&scale, parent->unk8C);
        if (frame >= 32) {
            w->unk0 = 2;
            return 1;
        }
        break;
    }
    return 0;
}

s32 fx_17d_5_op80(s32 mode, Fx17d5Work80 *w, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v; /* also the fade colour */
    SVECTOR rot;
    VECTOR scale;
    Fx17d5Spark *sp;
    Fx17d5Spark *sq;
    Fx17d5Spark *sr;
    Pool6CEntry *e;
    u8 *d;
    s32 frame;
    s32 fade;
    s32 flag;
    s32 i;
    s32 j;
    s32 t;
    s32 a;
    s32 b;
    s32 c;
    s32 k;
    s32 q;
    s32 r;
    s32 u;
    s32 m;

    if (mode == 0) {
        init->unk0 = 0x98;
        init->unk8 = 0x24;
        init->unk14 = (s32)func_801EC704;
        init->unk4 = 0x39;
    } else if (mode == 1) {
        w->unk0 = 0;
        w->unk4 = 0;
        w->unk8 = 0;
        D_801EE52C = env;
        ex->func_800E3C58();
        ex->btlfx_initEffModels(D_801EE53C, 5);
        w->unk84 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x85), 1);
        w->unk88 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x87), 1);
        w->unk8C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x88), 1);
        w->unk90 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x9B), 1);
        w->unk94 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x91), 1);
        ex->Hi_SetSummonMotion(((Fx17d5Summon *)env->unk4)->unk54->unk1A4, ((Fx17d5Summon *)env->unk4)->unk60);
        ex->Hi_SetSummonMotFrame(((Fx17d5Summon *)env->unk4)->unk60, 3);
        w->unk64.vx = 0;
        w->unk64.vy = -0x400;
        w->unk64.vz = 0;
        w->unk70.vx = w->unk70.vy = w->unk70.vz = 0x800;
        w->unkC.vx = w->unk64.vx;
        w->unkC.vy = w->unk64.vy;
        w->unkC.vz = w->unk64.vz;
        w->unk14.vx = w->unk64.vx;
        w->unk14.vy = w->unk64.vy;
        w->unk14.vz = w->unk64.vz - 0xC00;
        w->unk7C.vx = 0;
        w->unk7C.vy = 0;
        w->unk7C.vz = 0;
        w->unk50.vx = 0;
        w->unk50.vy = 0;
        w->unk50.vz = 0;
        w->unk44.vx = w->unk64.vx;
        w->unk44.vy = w->unk64.vy;
        w->unk44.vz = w->unk64.vz;
        w->unkC.vy -= 0x3CA;
        w->unk44.vy += 0xC00;
        w->unk58.vx = w->unk58.vz = w->unk58.vy = 0x1400;
        w->unk3C.vx = w->unkC.vx;
        w->unk3C.vy = w->unkC.vy;
        w->unk3C.vz = w->unkC.vz;
        env->camera->points[3].vx = w->unk3C.vx;
        env->camera->points[3].vy = w->unk3C.vy;
        env->camera->points[3].vz = w->unk3C.vz;
    } else {
        fade = 0;
        flag = 0;
        frame = entry->unk0;
        switch (w->unk0) {
        case 0:
            ex->Hi_DrawSummonModel(&w->unk7C, (VECTOR *)&w->unk64, (s32)&w->unk70, ((Fx17d5Summon *)env->unk4)->unk60, 0);
            w->unk50.vy += 6;
            ex->Hi_DrawEffModel(&w->unk50, (VECTOR *)&w->unk44, (s32)&w->unk58, w->unk84);
            ex->Hi_ModifyEffModelAbr(w->unk84, 1);
            ex->Hi_ModifyEffModelRGB(w->unk84, 0, 0, 0);
            if (frame == 0x1F) {
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&w->unkC);
                if (e != NULL) {
                    e->unk22 = -0xF0;
                }
            }
            if (frame == 0x1B) {
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&w->unkC);
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
            }
            u = frame - 0x20;
            if ((u32)u < 0x10) {
                d = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xD);
                u <<= 12;
                ex->func_800D3B6C(d, &w->unkC, u >> 12, ex->math_cos(u >> 6));
            }
            if (frame >= 0x1F && frame < 0x27) {
                m = 0xFF - (((frame - 0x1F) * 0xFF) >> 3);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, m, m, m * 2 / 3);
            }
            if (frame >= 0x31) {
                flag = 1;
                fade = ((frame - 0x31) << 12) >> 4;
            }
            if (frame >= 0x21 && w->unk8 < 0x39) {
                for (i = 0; i < 3; i++) {
                    if (w->unk8 < 0x39) {
                        w->unk8++;
                        sp = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                        if (sp != NULL) {
                            sp->unk0 = 0;
                            BTLFX_COPY_SVEC(&sp->unk8, &w->unkC);
                            sp->unk1C = w->unk14.vx;
                            sp->unk20 = w->unk14.vz;
                            sp->unk1E = w->unk14.vy;
                            sp->unk2 = 0x90;
                            {
                                s32 m = w->unk8 % 3;
                                sp->unk12 = w->unk7C.vy + (m * 0x300 - 0x300);
                                m = w->unk8 / 3;
                                sp->unk10 = w->unk7C.vx + 0x80 + m * 0x58;
                            }
                            sp->unk14 = 0;
                            sp->unk22 = 0x2800;
                            sp->unk6 = w->unk8 - 1;
                            sp->unk18 = D_801EE860[w->unk8];
                        }
                    }
                }
            } else {
                w->unk14.vy += 0xC;
            }
            if (frame >= 0x41) {
                w->unk0 = 1;
                entry->unk0 = -1;
                ex->func_800E3C58();
                w->unk3C.vx = w->unk24.vx;
                w->unk3C.vy = w->unk24.vy;
                w->unk3C.vz = w->unk24.vz;
                env->camera->points[3].vx = w->unk3C.vx;
                env->camera->points[3].vy = w->unk3C.vy;
                env->camera->points[3].vz = w->unk3C.vz;
                w->unk50.vx = 0;
                w->unk50.vy = 0;
                w->unk50.vz = 0;
                w->unk8 = 0;
                w->unk14.vz += 0x2000;
                w->unk14.vy += 0x2000;
                w->unk58.vx = w->unk58.vz = w->unk58.vy = 0x2000;
                w->unk44.vx = w->unk14.vx;
                w->unk44.vy = w->unk14.vy;
                w->unk44.vz = w->unk14.vz;
            }
            break;
        case 1:
            w->unk50.vx += 0x20;
            w->unk50.vy += 0x20;
            ex->Hi_DrawEffModel(&w->unk50, (VECTOR *)&w->unk44, (s32)&w->unk58, w->unk84);
            ex->Hi_ModifyEffModelAbr(w->unk84, 1);
            ex->Hi_ModifyEffModelRGB(w->unk84, 0, 0, 0);
            if (frame >= 0x10) {
                w->unk0 = 2;
                entry->unk0 = -1;
                ex->func_800E3C58();
                BTLFX_COPY_SVEC(&w->unk3C, &D_801EE4DC);
                env->camera->points[3].vx = w->unk3C.vx;
                env->camera->points[3].vy = w->unk3C.vy;
                env->camera->points[3].vz = w->unk3C.vz;
                w->unk24.vx = D_801EE4DC.vx;
                w->unk24.vy = D_801EE4DC.vy;
                w->unk24.vz = D_801EE4DC.vz;
                w->unk24.vy -= 0x5000;
                w->unk24.vz -= 0x5000;
                w->unkC.vx = w->unk24.vx;
                w->unkC.vy = w->unk24.vy;
                w->unkC.vz = w->unk24.vz;
                ex->btlfx_aimAngles((s16 *)&w->unkC, (s16 *)&D_801EE4DC, (s16 *)&w->unk1C);
                BTLFX_COPY_SVEC(&w->unk3C, &w->unk24);
                w->unk3C.vy += 0x200;
                w->unk3C.vz += 0x200;
                env->camera->points[2].vx = w->unk3C.vx;
                env->camera->points[2].vy = w->unk3C.vy;
                env->camera->points[2].vz = w->unk3C.vz;
                w->unk8 = 0;
            }
            break;
        case 2:
            t = frame << 7;
            if (frame == 0) {
                BTLFX_COPY_SVEC(&w->unk14, &D_801EE4DC);
                ex->btlfx_poolClear(entry->unk10);
                for (j = 0; j < 0x20; j++) {
                    if (w->unk8 < 0x39) {
                        w->unk8++;
                        sq = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                        if (sq != NULL) {
                            sq->unk0 = 0;
                            BTLFX_COPY_SVEC(&sq->unk8, &w->unkC);
                            BTLFX_COPY_SVEC(&v, &w->unk1C);
                            b = w->unk8 * 0x145 + 0x100;
                            sq->unk8.vx += ex->math_cos(b) * 5 / 2;
                            sq->unk8.vy += ex->math_sin(b) * 5 / 2;
                            v.vy += ((w->unk8 + 3) & 0xF) * 32 - 0x100;
                            v.vx += (w->unk8 & 1) * 512 - 0x100;
                            sq->unk1C = w->unk14.vx;
                            sq->unk20 = w->unk14.vz;
                            sq->unk1E = w->unk14.vy;
                            sq->unk2 = 0x320;
                            sq->unk10 = v.vx;
                            sq->unk12 = v.vy;
                            sq->unk14 = 1;
                            sq->unk22 = 0x5000;
                            sq->unk6 = w->unk8 - 1;
                            sq->unk18 = D_801EE860[w->unk8];
                        }
                    }
                }
            }
            a = t >> 2;
            w->unk3C.vy += (ex->math_sin(a) * 600) >> 12;
            w->unk3C.vz += (ex->math_sin(a) * 600) >> 12;
            env->camera->points[2].vx = w->unk3C.vx;
            env->camera->points[2].vy = w->unk3C.vy;
            env->camera->points[2].vz = w->unk3C.vz;
            if (frame != 0) {
                rot.vx = 0x400;
                rot.vz = 0;
                rot.vy = 0;
                rot.pad = 1;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &D_801EE4DC, &rot, 0x5000, 0, 1, -1);
            }
            if (frame >= 0x20) {
                w->unk0 = 3;
                entry->unk0 = -1;
                ex->func_800E3C58();
                BTLFX_COPY_SVEC(&w->unk24, &D_801EE4DC);
                w->unk24.vy -= 0x80;
                w->unk44.vx = w->unk24.vx;
                w->unk44.vy = w->unk24.vy;
                w->unk44.vz = w->unk24.vz;
                BTLFX_COPY_SVEC(&w->unk3C, &w->unk24);
                w->unk50.vx = 0;
                w->unk50.vy = 0;
                w->unk50.vz = 0;
                w->unk3C.vx += 0x800;
                w->unk3C.vy -= 0x20;
                w->unk3C.vz = D_801EE4DC.vz;
                env->camera->points[2].vx = w->unk3C.vx;
                env->camera->points[2].vy = w->unk3C.vy;
                env->camera->points[2].vz = w->unk3C.vz;
                w->unk8 = 0;
            }
            break;
        case 3:
            t = (frame << 12) / 72;
            if (frame == 0) {
                ex->btlfx_poolClear(entry->unk10);
            }
            if (w->unk8 < 0x20) {
                w->unk8++;
                if (w->unk8 < 0x1F) {
                    sr = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (sr != NULL) {
                        sr->unk0 = 1;
                        BTLFX_COPY_SVEC(&sr->unk1C, &w->unk24);
                        sr->unk1C -= t * 3 / 2 - 0x800 + ex->btlfx_randCentered(1000);
                        sr->unk1E = 0;
                        sr->unk20 += (w->unk8 % 3) * 1200 - 0x2B0 + ex->btlfx_randCentered(0x6A4);
                        sr->unk8.vx = sr->unk1C;
                        sr->unk8.vy = sr->unk1E - 0x1800;
                        sr->unk8.vz = sr->unk20 - 0x1800;
                        sr->unk10 = w->unk8 & 1;
                        sr->unk6 = w->unk8 - 1;
                        sr->unk18 = D_801EE860[w->unk8];
                    }
                } else {
                    sr = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (sr != NULL) {
                        sr->unk0 = 1;
                        BTLFX_COPY_SVEC(&sr->unk1C, &w->unk24);
                        sr->unk1E = 0;
                        sr->unk1C += 0x6A4 - t * 3 / 2;
                        sr->unk10 = 2;
                        sr->unk8.vx = sr->unk1C;
                        sr->unk20 += 0x400;
                        sr->unk8.vy = sr->unk1E - 0x1800;
                        sr->unk8.vz = sr->unk20 - 0x1800;
                        sr->unk6 = w->unk8 - 1;
                        sr->unk18 = D_801EE860[w->unk8];
                    }
                }
            }
            BTLFX_COPY_SVEC(&w->unk3C, &w->unk24);
            w->unk3C.vx += 0x800 - t;
            w->unk3C.vy -= 0x20;
            w->unk3C.vz = D_801EE4DC.vz;
            if (frame >= 9 && frame < 0x31) {
                if (frame < 0x28) {
                    k = (t >> 5) + 0x26;
                } else if (frame < 0x2D) {
                    k = (ex->math_cos(((frame - 0x28) << 10) / 5) * 400) >> 12;
                } else {
                    k = 0;
                }
                w->unk3C.vy += ex->btlfx_randCentered(k);
                w->unk3C.vz += ex->btlfx_randCentered(k);
            }
            env->camera->points[2].vx = w->unk3C.vx;
            env->camera->points[2].vy = w->unk3C.vy;
            env->camera->points[2].vz = w->unk3C.vz;
            if (frame < 0x10) {
                c = ex->math_cos((frame << 10) >> 4);
                rot.vx = 0x400;
                rot.vz = 0;
                rot.vy = 0;
                rot.pad = 1;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &D_801EE4DC, &rot, 0x5000, 0, 1, c);
            }
            ex->func_800E6624(frame * 29, 0x200, 0x100, 0x40, 0x80, 0x200, 0x180);
            if (frame >= 0x38) {
                r = (frame - 0x38) * 0xFF;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, r >> 4, (r >> 4) * 2 / 3, r >> 5);
            }
            if (frame >= 0x48) {
                w->unk0 = 3;
                entry->unk0 = -1;
                ex->func_800E3C58();
                ex->btlfx_poolClear(entry->unk10);
                w->unk24.vx = 0;
                w->unk24.vy = -0x800;
                w->unk24.vz = 0;
                w->unk50.vx = 0;
                w->unk50.vy = 0;
                w->unk50.vz = 0;
                w->unk44.vx = w->unk24.vx;
                w->unk44.vy = w->unk24.vy;
                w->unk44.vz = w->unk24.vz;
                w->unk3C.vx = w->unk24.vx;
                w->unk3C.vy = w->unk24.vy;
                w->unk3C.vz = w->unk24.vz;
                env->camera->points[3].vx = w->unk3C.vx;
                env->camera->points[3].vy = w->unk3C.vy;
                env->camera->points[3].vz = w->unk3C.vz;
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        env->unk28 = 0;
        if (fade != 0) {
            if (flag == 0) {
                q = ((fade << 5) >> 12) + 0x89;
            } else {
                q = (fade * 70) >> 12;
            }
            ((CVECTOR *)&v)->r = ((CVECTOR *)&v)->g = ((CVECTOR *)&v)->b = q;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            scale.vx = scale.vy = scale.vz = (fade >> 7) + 0x1000;
            ex->func_800E6764(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &scale, (CVECTOR *)&v, flag, 0);
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 1), w->unk4);
        w->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17d_5/nonmatchings/fx_17d_5", fx_17d_5_headerPtr);
