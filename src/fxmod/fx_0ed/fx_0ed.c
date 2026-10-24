#include "fxmod/fx_0ed.h"

void func_801E7704(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx0edSeg *buf, s32 abr) {
    Unk800F78A4 *env = D_801E9224;
    CVECTOR c0;
    CVECTOR c1;
    s32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    POLY_G4 *prim;
    Fx0edSeg *s;
    u32 i;
    s32 p0;
    s32 p1;

    if (p < 0) {
        p = 0x1000;
    }
    if (n >= 1000) {
        n -= 999;
        for (i = 0; i < n; i++) {
            buf[i].unk0.pad = 0;
        }
        return;
    }
    s = &buf[(s32)(n - 1)];
    for (i = 0; i < n; i++) {
        BTLFX_COPY_SVEC(&s[1].unk0, &s[0].unk0);
        BTLFX_COPY_SVEC(&s[1].unk8, &s[0].unk8);
        s--;
    }
    s = buf;
    for (i = 0; i < n; i++, s++) {
        if (!(s[0].unk0.pad & s[1].unk0.pad)) {
            break;
        }
    }
    BTLFX_COPY_SVEC(&buf->unk0, a);
    buf->unk0.pad = 1;
    BTLFX_COPY_SVEC(&buf->unk8, b);
    n = i;
    prim = (POLY_G4 *)env->prim;
    env->prim = (s32)(prim + n);
    if (ca == NULL) {
        c0.r = c0.g = c0.b = 0;
    } else {
        gte_DpqColorFar_nst(ca, p);
        gte_strgb(&c0);
    }
    if (cb == NULL) {
        c1.r = c1.g = c1.b = 0;
    } else {
        gte_DpqColorFar_nst(cb, p);
        gte_strgb(&c1);
    }
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    c0.cd = abr != 0xFF ? 0x3A : 0x38;
    c1.cd = abr != 0xFF ? 0x3A : 0x38;
    for (i = 0; i < n; i++, buf++, prim++) {
        gte_ldv3c(buf);
        p0 = 0x1000 - (i << 12) / n;
        gte_rtpt_b();
        p1 = 0x1000 - ((i + 1) << 12) / n;
        gte_stopz(&otz);
        if (otz != 0) {
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_avsz3();
            gte_stszotz(&otz);
            gte_ldv0(&buf[1].unk8);
            gte_rtps();
            ex->btlfx_addPrimAbr(0x8000000, otz, prim, abr);
            gte_stsxy(&prim->x3);
            gte_DpqColorFar_nst(&c0, p0);
            gte_strgb(&prim->r0);
            gte_DpqColorFar_nst(&c1, p0);
            gte_strgb(&prim->r1);
            gte_DpqColorFar_nst(&c0, p1);
            gte_strgb(&prim->r2);
            gte_DpqColorFar_nst(&c1, p1);
            gte_strgb(&prim->r3);
        }
    }
}

void func_801E7B74(s32 model, s32 bone, SVECTOR *in, SVECTOR *out) {
    Unk800F78A4 *env = D_801E9224;
    MATRIX m;

    BTLFX_EXPORTS(env)->Hi_GetSummonBoneMatrix(model, bone, (BtlBone *)&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    gte_ldv0(in);
    gte_rtv0tr();
    gte_stlvnl(m.t);
    out->vx = m.t[0];
    out->vy = m.t[1];
    out->vz = m.t[2];
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
}

s32 fx_0ed_op80(s32 mode, Fx0edWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    u8 *file;
    s32 state;
    s32 frame;
    s32 amp;
    s32 t;
    s32 t0;
    s32 t3;
    s32 k;
    s32 i;
    Pool6CEntry *p;
    SVECTOR *sv;

    if (mode == 0) {
        *arg4 = 0x40;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E9224 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        ex->btlfx_getUnitRot(0, (s16 *)&work->unk10);
        work->unk10.vy &= 0xFFF;
        if ((u32)(work->unk10.vy - 0x400) > 0x800) {
            work->unk10.vy = 0;
        } else {
            work->unk10.vy = 0x800;
        }
        ex->btlfx_rotateOffset(&work->unk10, ex->btlfx_getUnitRadius(0) + 0x78, (s32)&pos);
        work->unk18.vx = work->unk8.vx + ((SVECTOR *)&pos)->vx;
        work->unk18.vy = work->unk8.vy + ((SVECTOR *)&pos)->vy;
        work->unk18.vz = work->unk8.vz + ((SVECTOR *)&pos)->vz;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        BTLFX_COPY_SVEC(&((SVECTOR *)&pos)[1], &work->unk18);
        ((SVECTOR *)&pos)[1].vy -= 0x140;
        env->camera->points[0].vx = ((SVECTOR *)&pos)[1].vx;
        env->camera->points[0].vy = ((SVECTOR *)&pos)[1].vy;
        env->camera->points[0].vz = ((SVECTOR *)&pos)[1].vz;
        ex->btlfx_rotateOffset(&work->unk10, -0xD2, (s32)&pos);
        ((SVECTOR *)&pos)[1].vx = work->unk18.vx + ((SVECTOR *)&pos)->vx;
        ((SVECTOR *)&pos)[1].vy = work->unk18.vy + ((SVECTOR *)&pos)->vy - 0x140;
        ((SVECTOR *)&pos)[1].vz = work->unk18.vz + ((SVECTOR *)&pos)->vz;
        env->camera->points[1].vx = ((SVECTOR *)&pos)[1].vx;
        env->camera->points[1].vy = ((SVECTOR *)&pos)[1].vy;
        env->camera->points[1].vz = ((SVECTOR *)&pos)[1].vz;
        ex->btlfx_rotateOffset(&work->unk10, -ex->btlfx_getUnitRadius(0), (s32)&pos);
        ((SVECTOR *)&pos)[1].vx = work->unk8.vx + ((SVECTOR *)&pos)->vx;
        ((SVECTOR *)&pos)[1].vy = ((SVECTOR *)&pos)->vy - 0x140;
        ((SVECTOR *)&pos)[1].vz = work->unk8.vz + ((SVECTOR *)&pos)->vz;
        env->camera->points[2].vx = ((SVECTOR *)&pos)[1].vx;
        env->camera->points[2].vy = ((SVECTOR *)&pos)[1].vy;
        env->camera->points[2].vz = ((SVECTOR *)&pos)[1].vz;
        file = (u8 *)ex->btlfx_getFileData(entry->unk8, 2);
        ex->btlfx_registerSummonModel((Unk800E9000 *)ex->btlfx_getFileData(entry->unk8, 7), (s32 *)&file, 1, &D_801E9234);
        ex->Hi_SetSummonMotion(((Fx0edSummon *)env->unk4)->unk54->unk180, ((Fx0edSummon *)env->unk4)->unk60);
        work->unk34[0] = work->unk34[1] = work->unk34[2] = 0x1000;
        work->unk2C.vx = 0;
        work->unk2C.vz = 0;
        work->unk2C.vy = work->unk10.vy;
        ex->btlfx_initEffModels(D_801E93C4, 1);
        work->unk28 = (s16)ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 8), 0);
    } else {
        state = work->unk0;
        frame = entry->unk0;
        amp = 0;
        switch (state) {
        case 0:
            t0 = (frame << 12) / 24;
            if (frame == 0) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&work->unk18);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1D), (s32)&work->unk18);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E8C84;
                    p->unk12 = 0x18;
                    p->unkC = 0x3C;
                    p->unk22 = 0x40;
                }
            }
            if (frame > 0) {
                s32 h;

                pos.vx = work->unk18.vx;
                pos.vy = work->unk18.vy;
                pos.vz = work->unk18.vz;
                h = ((0x1000 - ex->math_sin(t0 >> 2)) * 0x514) >> 12;
                pos.vy += h;
                ex->Hi_SetEffModelSlice(work->unk28, h);
                ex->Hi_DrawSliceEffModel(&work->unk2C, &pos, (s32)work->unk34, work->unk28);
                ex->Hi_ModifyEffModelRGB(work->unk28, (((t0 >> 1) + 0x800) >> 5) - 0x80, (((t0 >> 1) + 0x800) >> 5) - 0x80,
                                         (((t0 >> 1) + 0x800) >> 5) - 0x80);
            }
            if (frame >= 0x18) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            pos.vx = work->unk18.vx;
            pos.vy = work->unk18.vy;
            pos.vz = work->unk18.vz;
            t0 = ((Fx0edSummon *)env->unk4)->unk60;
            ex->Hi_SetSummonMotFrame(t0, 0);
            ex->Hi_DrawSummonModel(&work->unk2C, &pos, (s32)work->unk34, ((Fx0edSummon *)env->unk4)->unk60, 1);
            if (frame >= 8) {
                work->unk0 = 2;
                entry->unk0 = -1;
                ex->Hi_SetSummonMotFrame(((Fx0edSummon *)env->unk4)->unk60, 0);
            }
            break;
        case 2:
            t = frame << 12;
            /* Dead store: only its sign word survives. */
            amp = t / 30;
            pos.vx = work->unk18.vx;
            pos.vy = work->unk18.vy;
            pos.vz = work->unk18.vz;
            ex->Hi_DrawSummonModel(&work->unk2C, &pos, (s32)work->unk34, ((Fx0edSummon *)env->unk4)->unk60, 1);
            if (frame == 0) {
                amp = -1;
            } else if (frame < 0x18) {
                amp = t / 24;
            } else {
                amp = 0x1000;
            }
            if (frame == 0x20) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 2, 0xFF, 0xFF, 0);
            }
            if (frame == 0x1F) {
                s32 ang;

                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8,
                                                           *(D_801E8C10 + (((ang = work->unk10.vy & 0xFFF) - 0x400U > 0x800) !=
                                                                      ((ang = env->camera->yaw & 0xFFF) - 0x400U > 0x800)))),
                                      (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x40);
                }
            }
            if (frame == 0x21) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xDC, 0, 0x20);
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x40);
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x40);
                }
            }
            if (frame == 0x26) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk18);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E9494;
                    p->unk12 = 0x18;
                    p->unkC = 0x44;
                    p->unk22 = 0x40;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk18);
            }
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 4), frame);
            if (frame >= 0x29) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            t0 = frame << 9;
            t3 = t0 >> 2;
            work->unk34[0] = work->unk34[2] = ((0x1000 - ex->math_sin(t3)) * 0x1000) >> 12;
            pos.vx = work->unk18.vx;
            pos.vy = work->unk18.vy;
            pos.vz = work->unk18.vz;
            ex->Hi_SetSummonMotFrame(((Fx0edSummon *)env->unk4)->unk60, 0x29 - frame);
            ex->Hi_DrawSummonModel(&work->unk2C, &pos, (s32)work->unk34, ((Fx0edSummon *)env->unk4)->unk60, 1);
            amp = ex->math_cos(t3);
            ex->Hi_ModifySummonModelRGB(((Fx0edSummon *)env->unk4)->unk60, (amp >> 5) - 0x80, (amp >> 5) - 0x80,
                                        (amp >> 5) - 0x80);
            if (frame >= 8) {
                work->unk0 = 3;
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (amp != 0) {
            func_801E7B74(((Fx0edSummon *)env->unk4)->unk60, 9, &D_801E8C24, &a);
            func_801E7B74(((Fx0edSummon *)env->unk4)->unk60, 9, &D_801E8C2C, &b);
            func_801E7B74(((Fx0edSummon *)env->unk4)->unk60, 9, &D_801E8C34, &c);
            if (amp < 0) {
                func_801E7704(&a, &b, 0x3F8, &D_801E8C18, &D_801E8C1C, 0, D_801E9AF4, 1);
                func_801E7704(&b, &c, 0x3F8, &D_801E8C1C, &D_801E8C20, 0, D_801E9C04, 1);
            } else {
                func_801E7704(&a, &b, 0x10, &D_801E8C18, &D_801E8C1C, amp, D_801E9AF4, 1);
                func_801E7704(&b, &c, 0x10, &D_801E8C1C, &D_801E8C20, amp, D_801E9C04, 1);
                BTLFX_COPY_SVEC(&work->unk20, &a);
                {
                    s32 o;
                    s32 p2;
                    SVECTOR *sv2;

                    sv = (SVECTOR *)D_801E9AF4;
                    for (i = 0, o = 8; i < 17; o += 16, i++) {
                        sv[i * 2].vy -= sv[i * 2].vy >> 6;
                        ((SVECTOR *)(o + (s32)sv))->vy -= ((SVECTOR *)(o + (s32)sv))->vy >> 4;
                    }
                    sv2 = (SVECTOR *)D_801E9C04;
                    i = 0;
                    p2 = 8;
                    for (; i < 17; p2 += 16, i++) {
                        sv2[i * 2].vy -= sv2[i * 2].vy >> 4;
                        ((SVECTOR *)(p2 + (s32)sv2))->vy -= ((SVECTOR *)(p2 + (s32)sv2))->vy >> 3;
                    }
                }
            }
        }
        work->unk4++;
    }
    return 0;
}

s32 fx_0ed_op81(s32 mode, Fx0edWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    union {
        SVECTOR v;
        CVECTOR c;
    } u;
    SVECTOR rot;
    VECTOR sc;
    s32 tmp;
    s32 frame;
    s32 r;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        work->unk4 = 0xA;
        work->unk0 = 0;
        work->unkC = (BtlUnit *)-1;
        work->unk8 = 0;
        D_801E9224 = env;
        ((Fx0edBattleState *)env->unk0)->unk10 = 0;
    } else {
        frame = entry->unk0;
        c = 0;
        switch (work->unk0) {
        case 0:
            do {
                r = ex->btlfx_nextUnitPos(0, (s16 *)&work->unk10, &work->unkC, &tmp);
                if (r == -2) {
                    work->unkC = (BtlUnit *)-1;
                }
            } while (r == -2 || r == -1);
            env->camera->points[0].vx = work->unk10.vx;
            env->camera->points[0].vy = work->unk10.vy;
            env->camera->points[0].vz = work->unk10.vz;
            u.v.vx = work->unk10.vx;
            u.v.vy = work->unk10.vy;
            u.v.vz = work->unk10.vz;
            if (u.v.vz < 0) {
                u.v.vz += 0x640;
            } else {
                u.v.vz -= 0x640;
            }
            u.v.vy -= 0x200;
            env->camera->points[1].vx = u.v.vx;
            env->camera->points[1].vy = u.v.vy;
            env->camera->points[1].vz = u.v.vz;
            ex->btlfx_playFileSound(3, entry->unk8, -1);
            if (++work->unk8 >= 0x10 && r == 1) {
                work->unk0 = 2;
            } else {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            if (work->unk8 < 0x10) {
                c = work->unk8 * 100 / 16;
            } else {
                c = 100;
            }
            break;
        case 1:
            env->unk28 = 0x80;
            ex->btlfx_drawSprite((u8 *)ex->btlfx_getFileData(entry->unk8, 6), &work->unk10, 0, 0xFF, -1);
            env->unk28 = 0;
            if (work->unk8 < 0x10) {
                c = work->unk8 * 100 / 16;
            } else {
                c = 100;
            }
            if (frame >= work->unk4) {
                work->unk0 = 0;
                work->unk4--;
            }
            break;
        case 2:
            ((Fx0edBattleState *)env->unk0)->unk10 = 1;
            env->camera->unk52 = 0;
            return 1;
        }
        if (c != 0) {
            sc.vx = 0x1000;
            sc.vy = 0x1000;
            sc.vz = 0x1000;
            u.c.r = u.c.g = u.c.b = c;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(6, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &sc, &u.c, 1, 0);
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ed/nonmatchings/fx_0ed", fx_0ed_headerPtr);
