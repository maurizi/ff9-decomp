#include "fxmod/fx_081.h"

void func_801E7704(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 arg3, s32 *arg4, s32 arg5, s32 arg6, s32 frame,
                   s32 n) {
    Unk800F78A4 *env = D_801E8ED8;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Unk80100C78Entry *entry = D_801EA9B0;
    SVECTOR v;
    s32 r;
    s32 t;
    s32 k;

    r = ex->func_800E0134(pt, p);
    if (frame == 0) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), (s32)pos, NULL, -1, 0, 0, 0, (u8 *)r, 0);
    }
    if (frame < n - 4) {
        t = 0x1000;
    } else {
        t = 0x1000 - (((frame - (n - 4)) << 12) >> 2);
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), (s32)pos, (s32 *)p->unk28, frame,
                      0x1C00 - (frame << 12) / n, t, 1, (u8 *)r, 1);
    if (pt->unk1F == 1) {
        if (D_801EA738 != 0) {
            ex->func_800E3D20(D_801EA738, (s32)pos);
        }
        BTLFX_COPY_SVEC(&D_801EA730, pos);
        if (frame < 0x1D) {
            env->camera->points[3].vx = pos->vx;
            env->camera->points[3].vy = pos->vy;
            env->camera->points[3].vz = pos->vz;
            return;
        }
        k = frame - 0x1D;
        if (k < 0x20) {
            ex->btlfx_lerpVec((s16 *)pos, (s16 *)&D_801EA728, 0x1000 - ex->math_cos(k << 5), (s16 *)&v);
        } else {
            BTLFX_COPY_SVEC(&v, &D_801EA728);
        }
        env->camera->points[3].vx = v.vx;
        env->camera->points[3].vy = v.vy;
        env->camera->points[3].vz = v.vz;
    }
}

s32 fx_081_op80(s32 mode, Fx081Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ang;
    SVECTOR rot;
    VECTOR scale;
    SVECTOR unused;
    s32 frame;
    s32 b;
    s32 sc;
    s32 c;
    s32 t;
    s32 u;
    s32 c4;
    Pool6CEntry *e;

    if (mode == 0) {
        *arg4 = sizeof(Fx081Work80);
    } else if (mode == 1) {
        D_801E8ED8 = env;
        D_801EA73C = 0;
        D_801EA9B0 = entry;
        work->unk10.vx = 0;
        work->unk10.vy = 0;
        work->unk10.vz = 0;
        work->unk0 = 0;
        work->unk4 = 0;
        BTLFX_COPY_SVEC(&D_801EA728, &work->unk10);
        work->unk18.vx = work->unk10.vx;
        work->unk18.vy = work->unk10.vy;
        work->unk18.vz = work->unk10.vz;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&D_801EA748);
        work->unk2C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&work->unk10);
        if (work->unk2C != NULL) {
            work->unk2C->unk22 = 0x18;
            if (work->unk2C != NULL) {
                work->unk2C->unk14 = (s32)D_801E8EE8;
                work->unk2C->unk12 = 0x10;
                work->unk2C->unkC = 0x184;
                if (work->unk2C != NULL) {
                    work->unk2C->unk18->unk24 = func_801E7704;
                }
            }
        }
        D_801EA738 = work->unk30 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk24);
        if (work->unk30 != NULL) {
            work->unk30->unk22 = 0x18;
        }
        work->unk34 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk24);
        if (work->unk34 != NULL) {
            work->unk34->unk22 = 0x10;
        }
        ex->btlfx_initEffModels(D_801EA758, 3);
        work->unk8 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x21), 0x137,
                                                0xFFFF, 0,
                                                0x80);
        work->unkC = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x17));
    } else {
        frame = entry->unk0;
        sc = b = 0;
        switch (work->unk0) {
        case 0:
            b = 0x1000;
            t = (frame << 12) / 22;
            if (work->unk2C != NULL && work->unk2C->unk30 == -1) {
                work->unk2C = NULL;
            }
            if (work->unk30 != NULL && work->unk30->unk30 == -1) {
                work->unk30 = NULL;
            }
            if (work->unk34 != NULL && work->unk34->unk30 == -1) {
                work->unk34 = NULL;
            }
            if (work->unk2C != NULL) {
                work->unk2C->unk20 = D_801EA73C;
            }
            if (work->unk30 != NULL) {
                work->unk30->unk20 = 0x1000;
            }
            if (work->unk34 != NULL) {
                work->unk34->unk20 = 0x1000;
            }
            sc = (((0x1000 - t) * 3) >> 1) + 0x1000;
            if (frame < 4) {
                u = 0xFF - frame * 0xFF / 4;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, u >> 1, u, u);
            }
            if (frame >= 0x16) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 20;
            b = 0x1000 - t;
            sc = 0x1000;
            D_801EA73C = t;
            if (work->unk2C != NULL && work->unk2C->unk30 == -1) {
                work->unk2C = NULL;
            }
            if (work->unk30 != NULL && work->unk30->unk30 == -1) {
                work->unk30 = NULL;
            }
            if (work->unk34 != NULL && work->unk34->unk30 == -1) {
                work->unk34 = NULL;
            }
            if (work->unk2C != NULL) {
                work->unk2C->unk20 = D_801EA73C;
            }
            if (work->unk30 != NULL) {
                work->unk30->unk20 = b;
            }
            if (work->unk34 != NULL) {
                work->unk34->unk20 = b;
            }
            if (frame >= 0x14) {
                work->unk0 = 2;
                entry->unk0 = -1;
                ex->func_800E3D54(work->unk30);
                ex->func_800E3D54(work->unk34);
                D_801EA738 = NULL;
            }
            break;
        case 2:
            D_801EA73C = 0x1000;
            if (work->unk2C != NULL) {
                if (work->unk2C->unk30 == -1) {
                    work->unk2C = NULL;
                }
                if (work->unk2C != NULL) {
                    work->unk2C->unk20 = D_801EA73C;
                }
            }
            if (frame == 0x13) {
                D_801EA740 = e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&work->unk10);
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x80, 0xFF);
            }
            if (frame >= 0x16) {
                ex->func_800E3D54(work->unk2C);
                return 1;
            }
            break;
        }
        if (b != 0) {
            ang.pad = 0;
            ang.vy = 0;
            ang.vx = 0;
            ang.vz = -work->unk4 * 20;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), &D_801EA730, &ang, sc, 0, 1, b);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 6), &D_801EA730, &ang, 0xE00, 0, 1, b);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &D_801EA730, &ang, 0x1000, 0, 1, b);
        }
        if (D_801EA73C != 0) {
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = -0x400;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&work->unk18, (s32)&scale, work->unk8);
            ex->Hi_ModifyEffModelAbr(work->unk8, 0xFF);
            ex->Hi_ModifyEffModelRGB(work->unk8, (D_801EA73C >> 5) - 0x80, (D_801EA73C >> 5) - 0x80,
                                     (D_801EA73C >> 5) - 0x80);
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&work->unk18, (s32)&scale, work->unkC);
            ex->Hi_ModifyEffModelAbr(work->unkC, 0xFF);
            ex->Hi_ModifyEffModelRGB(work->unkC, (D_801EA73C >> 5) - 0x80, (D_801EA73C >> 5) - 0x80,
                                     (D_801EA73C >> 5) - 0x80);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x20), work->unk4);
            env->unk28 = 0x80;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0x16), &work->unk10, NULL, 0x1600, 0, 1,
                              D_801EA73C);
            env->unk28 = 0;
            rot.pad = 1;
            rot.vx = -0x200;
            rot.vy = work->unk4 * 0x18 + 0x580;
            rot.vz = 0;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), &work->unk10, &rot, 0x16000, 0, 1,
                              D_801EA73C);
            c = ex->math_cos(((work->unk4 - 0x16) << 10) / 42);
            c4 = c * 4;
            rot.vx = -0x180;
            rot.vy = (work->unk4 * 32) + 0x180;
            rot.vz = work->unk4 * 0x18;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23), &work->unk10, &rot, c4, 0, 1, D_801EA73C);
            rot.vx = 0x200;
            rot.vy = 0x200 - work->unk4 * 32;
            rot.vz = -work->unk4 * 16;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23), &work->unk10, &rot, (c * 8 + c4) >> 1,
                              0, 1,
                              D_801EA73C);
            rot.vx = 0x100;
            rot.vy = -0x180 - work->unk4 * 0x18;
            rot.vz = -work->unk4 * 16;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23), &work->unk10, &rot, (c * 16 + c4) >> 1,
                              0, 1,
                              D_801EA73C);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x22), work->unk4);
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x10), work->unk4);
        work->unk4++;
    }
    return 0;
}

s32 fx_081_op81(s32 mode, Fx081Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR ang;
    SVECTOR pos;
    Pool6CEntry *e;
    s32 frame;
    s32 t;
    s32 s;
    s32 u;
    s32 h;
    s32 c;
    s32 a;
    s32 i;
    s32 j;
    s32 w;
    s32 vx;
    SVECTOR *pp;
    SVECTOR *pp2;
    s32 w4;
    s32 n;

    if (mode == 0) {
        *arg4 = sizeof(Fx081Work81);
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk8 = 0x46;
        work->unkC.vx = 0;
        work->unkC.vy = 0;
        work->unkC.vz = 0;
        D_801E8ED8 = env;
        e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unkC);
        if (e != NULL) {
            e->unk22 = work->unk8 + 0x40;
        }
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            rot.vx = 0;
            rot.vy = 0;
            rot.pad = 0;
            rot.vz = 0x400;
            t = (frame << 12) / 7;
            s = ex->math_sin(t >> 2);
            env->unk28 = work->unk8 + 0x40;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, NULL, 0x13000, frame, 1, s);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &work->unkC, NULL, (t >> 1) + 0x3000, 0, 1,
                              s);
            env->unk28 = 0;
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &work->unkC, work->unk4,
                              ex->math_sin(t >> 2));
            if (frame >= 7) {
                work->unk0 = 1;
                entry->unk0 = -1;
                work->unk28 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 0xB9,
                                                         0x3E00, 0x80, 0);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0, 0xFF);
            }
            break;
        case 1:
            rot.vx = 0;
            rot.vy = 0;
            rot.pad = 0;
            rot.vz = 0x400;
            env->unk28 = work->unk8 + 0x40;
            t = (frame << 12) / 7;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0x24), &work->unkC, &rot, 0x3000, frame, 1,
                              0x1000);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, NULL, t + 0x13000, frame, 1,
                              0x1000);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &work->unkC, NULL, (t >> 1) + 0x3800, 0, 1,
                              0x1000);
            env->unk28 = 0;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0, 0x80);
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &work->unkC, work->unk4, 0x1000);
            if (frame >= 7) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
                work->unk0 = 2;
                entry->unk0 = -1;
                ex->func_800E3D54(D_801EA740);
                BTLFX_COPY_SVEC(&work->unkC, &env->targets->centerX);
                work->unkC.vy -= 0x200;
                BTLFX_COPY_SVEC(&work->unk14, &work->unkC);
                work->unk1C.vx = work->unkC.vx;
                work->unk1C.vy = work->unkC.vy;
                work->unk14.vy -= 0xC00;
                work->unk1C.vz = work->unkC.vz;
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&work->unkC);
                if (e != NULL) {
                    e->unk22 = work->unk8 + 0x40;
                }
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1C), (s32)&work->unkC);
                if (e != NULL) {
                    e->unk22 = work->unk8 + 0x40;
                }
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x19), (s32)&work->unkC);
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1D), (s32)&work->unkC);
                if (e != NULL) {
                    e->unk22 = work->unk8 + 0x80;
                }
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1A), (s32)&work->unk14);
                if (e != NULL) {
                    e->unk22 = 0x46;
                }
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x27), (s32)&work->unkC);
                if (e != NULL) {
                    e->unk22 = 0x46;
                }
            }
            break;
        case 2:
            j = frame << 7;
            env->unk28 = 0;
            if (frame < 6) {
                u = 0xFF - frame * 0xFF / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, u >> 1, u, u);
            }
            /* the first loop's a (the original keeps it in s0; t keeps its own register) */
            a = j >> 2;
            c = ex->math_cos(a);
            h = ((ex->math_sin(a) * 0x60E) >> 12) + 0x100;
            ang.vx = 0;
            ang.vy = 0;
            ang.vz = 0;
            for (j = 0, pp = &pos; j < 2; j++) {
                if (j == 0) {
                    ang.vx = work->unk4 * 0x18 + 0x200;
                } else {
                    ang.vx = -0x200 - work->unk4 * 0x18;
                }
                a = work->unk4 * 41 + (j << 7);
                for (i = 0; i < 12; i++, a += 0x155) {
                    ex->func_800D044C(&work->unkC, (s32)pp, 0, (XY32 *)&ang, h, a);
                    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0xC), pp, NULL, 0x1000, frame & 0xF, 1,
                                      c, 0, 0);
                }
            }
            h = h * 2 / 3;
            pp2 = pp;
            /* n indexes D_801E8E7C: loop.c drops it for the reduced giv, whose increment then comes before
             * w += frame and j++ as in the original */
            for (j = 0, n = 0, w = 0; j < 10; n++, w += frame, j++) {
                vx = 0x400 - (j << 11) / 9;
                for (i = 0; i < D_801E8E7C[n]; i++) {
                    /* a long enough lifetime for loop.c to hoist it, and the add's operand order */
                    w4 = w << 4;
                    ang.vy = (a = (i << 12) / D_801E8E7C[n] - (work->unk4 << 5)) + w4;
                    ang.vx = vx;
                    ex->btlfx_pointAhead((s16 *)&work->unkC, (s16 *)&ang, h, (s16 *)pp2);
                    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x25), pp2, NULL, c + 0x800, 0, 1, c, 0,
                                      0);
                }
            }
            if (frame >= 0x20) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            if (frame >= 15) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_081/nonmatchings/fx_081", fx_081_headerPtr);
