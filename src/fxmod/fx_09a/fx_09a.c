#include "fxmod/fx_09a.h"

void func_801E7704(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx09aSeg *buf, s32 abr) {
    Unk800F78A4 *env = D_801E8858;
    CVECTOR c0;
    CVECTOR c1;
    s32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    POLY_G4 *prim;
    Fx09aSeg *s;
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
    Unk800F78A4 *env = D_801E8858;
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

s32 fx_09a_op80(s32 mode, Fx09aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    s32 file;
    Pool6CEntry *p;
    s32 frame;
    s32 k;
    s32 t;
    s32 i;
    s32 v;
    s32 rgb;
    SVECTOR *sv;

    if (mode == 0) {
        *arg4 = sizeof(Fx09aWork);
    } else if (mode == 1) {
        work->unk0 = 1;
        work->unk4 = 0;
        D_801E8858 = env;
        file = ex->btlfx_getFileData(entry->file, 2);
        ex->btlfx_registerSummonModel((Unk800E9000 *)ex->btlfx_getFileData(entry->file, 8), &file, 1, &D_801E8868);
        work->unk8 = -1;
        work->unkC = -1;
        work->unk10 = env->targets->targetCount;
        env->state->pad10[0] = 0;
    } else {
        frame = entry->frame;
        k = 0;
        switch (work->unk0) {
        case 1:
            work->unkC++;
            work->unk8++;
            if (work->unkC >= work->unk10) {
                work->unk0 = 4;
                entry->frame = -1;
                break;
            }
            ex->btlfx_getUnitPos(work->unk8, (s16 *)&work->unk14);
            ex->btlfx_getUnitRot(work->unk8, (s16 *)&work->unk1C);
            work->unk1C.vy &= 0xFFF;
            if (work->unk1C.vy - 0x400U > 0x800) {
                work->unk1C.vy = 0;
            } else {
                work->unk1C.vy = 0x800;
            }
            ex->btlfx_rotateOffset(&work->unk1C, ex->btlfx_getUnitRadius(work->unk8) + 0x78, (s32)&a);
            work->unk24.vx = work->unk14.vx + a.vx;
            work->unk24.vy = work->unk14.vy + a.vy;
            work->unk24.vz = work->unk14.vz + a.vz;
            ex->btlfx_getUnitBodyPos(work->unk8, 0, (s16 *)&work->unk14);
            BTLFX_COPY_SVEC(&b, &work->unk24);
            b.vy -= 0x140;
            env->camera->points[0].vx = b.vx;
            env->camera->points[0].vy = b.vy;
            env->camera->points[0].vz = b.vz;
            ex->btlfx_rotateOffset(&work->unk1C, -0xD2, (s32)&a);
            b.vx = work->unk24.vx + a.vx;
            b.vy = work->unk24.vy + a.vy - 0x140;
            b.vz = work->unk24.vz + a.vz;
            env->camera->points[1].vx = b.vx;
            env->camera->points[1].vy = b.vy;
            env->camera->points[1].vz = b.vz;
            ex->btlfx_rotateOffset(&work->unk1C, -ex->btlfx_getUnitRadius(0), (s32)&a);
            b.vx = work->unk14.vx + a.vx;
            b.vy = a.vy - 0x140;
            b.vz = work->unk14.vz + a.vz;
            env->camera->points[2].vx = b.vx;
            env->camera->points[2].vy = b.vy;
            env->camera->points[2].vz = b.vz;
            ex->Hi_SetSummonMotion(((Fx09aSummon *)env->loader)->unk54->unk180, ((Fx09aSummon *)env->loader)->unk60);
            ex->Hi_SetSummonMotFrame(((Fx09aSummon *)env->loader)->unk60, 6);
            work->unk3C[2] = 0x1000;
            work->unk3C[1] = 0x1000;
            work->unk3C[0] = 0x1000;
            work->unk34.vx = 0;
            work->unk34.vz = 0;
            work->unk34.vy = work->unk1C.vy;
            ex->btlfx_playFileSound(0xF, entry->file, -1);
            ex->btlfx_playFileSound(0x10, entry->file, -1);
            ex->btlfx_playFileSound(0x11, entry->file, -1);
            work->unk0 = 2;
            entry->frame = -1;
            break;
        case 2:
            pos.vx = work->unk24.vx;
            pos.vy = work->unk24.vy;
            pos.vz = work->unk24.vz;
            ex->Hi_DrawSummonModel(&work->unk34, &pos, (s32)work->unk3C, ((Fx09aSummon *)env->loader)->unk60, 1);
            t = frame << 12;
            /* Dead store: only its sign word survives. */
            k = t / 30;
            if (frame < 6) {
                rgb = (t / 6 >> 5) - 0x80;
                ex->Hi_ModifySummonModelRGB(((Fx09aSummon *)env->loader)->unk60, rgb, rgb, rgb);
            }
            if (frame == 0) {
                k = -1;
            } else if (frame < 22) {
                k = t / 22;
            } else {
                k = 0x1000;
            }
            if (frame == 24) {
                ex->btlfx_playFileSound(0x12, entry->file, -1);
                ex->btlfx_playFileSound(0x13, entry->file, -1);
                ex->btlfx_playFileSound(0x14, entry->file, -1);
            }
            if (frame == 26) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 2, 0xFF, 0xFF, 0);
            }
            if (frame == 27) {
                s32 ang;

                p = (Pool6CEntry *)ex->btlfx_startEmitter(
                    ex->btlfx_getFileData(entry->file,
                                          *(D_801E87E4 + (((ang = work->unk1C.vy & 0xFFF) - 0x400U > 0x800) !=
                                                          ((ang = env->camera->yaw & 0xFFF) - 0x400U > 0x800)))),
                    (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(work->unk8, 0x40);
                }
                ex->btlfx_playFileSound(0x15, entry->file, -1);
                ex->btlfx_playFileSound(0x16, entry->file, -1);
                ex->btlfx_playFileSound(0x17, entry->file, -1);
            }
            if (frame == 28) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xDC, 0, 0x20);
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(work->unk8, 0x40);
                }
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(work->unk8, 0x40);
                }
            }
            if (frame == 29) {
                ex->btlfx_execCmd(work->unk8);
            }
            if (frame >= 30) {
                work->unk0 = 3;
                entry->frame = -1;
            }
            break;
        case 3:
            v = (frame << 12) / 6;
            if (frame == 2) {
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk24);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E89F8;
                    p->unk12 = 0x18;
                    p->unkC = 0x44;
                    p->unk22 = 0x40;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->unk24);
            }
            work->unk3C[0] = work->unk3C[2] = ((0x1000 - ex->math_sin(v >> 2)) << 12) >> 12;
            pos.vx = work->unk24.vx;
            pos.vy = work->unk24.vy;
            pos.vz = work->unk24.vz;
            ex->Hi_SetSummonMotFrame(((Fx09aSummon *)env->loader)->unk60, 41 - frame);
            ex->Hi_DrawSummonModel(&work->unk34, &pos, (s32)work->unk3C, ((Fx09aSummon *)env->loader)->unk60, 1);
            k = ex->math_cos(v >> 2);
            ex->Hi_ModifySummonModelRGB(((Fx09aSummon *)env->loader)->unk60, (k >> 5) - 0x80, (k >> 5) - 0x80,
                                        (k >> 5) - 0x80);
            if (frame >= 6) {
                work->unk0 = 1;
                entry->frame = -1;
            }
            break;
        case 4:
            if (frame >= 8) {
                env->state->pad10[0] = 1;
                return 1;
            }
            break;
        }
        if (k != 0) {
            func_801E7B74(((Fx09aSummon *)env->loader)->unk60, 9, &D_801E87F8, &a);
            func_801E7B74(((Fx09aSummon *)env->loader)->unk60, 9, &D_801E8800, &b);
            func_801E7B74(((Fx09aSummon *)env->loader)->unk60, 9, &D_801E8808, &c);
            if (k < 0) {
                func_801E7704(&a, &b, 0x3F8, (CVECTOR *)&D_801E87EC, (CVECTOR *)&D_801E87F0, 0, D_801E9058, 1);
                func_801E7704(&b, &c, 0x3F8, (CVECTOR *)&D_801E87F0, (CVECTOR *)&D_801E87F4, 0, D_801E9168, 1);
            } else {
                func_801E7704(&a, &b, 0x10, (CVECTOR *)&D_801E87EC, (CVECTOR *)&D_801E87F0, k, D_801E9058, 1);
                func_801E7704(&b, &c, 0x10, (CVECTOR *)&D_801E87F0, (CVECTOR *)&D_801E87F4, k, D_801E9168, 1);
                BTLFX_COPY_SVEC(&work->unk2C, &a);
                {
                    s32 o;
                    s32 p2;
                    SVECTOR *sv2;

                    sv = (SVECTOR *)D_801E9058;
                    for (i = 0, o = 8; i < 17; o += 16, i++) {
                        sv[i * 2].vy -= sv[i * 2].vy >> 6;
                        ((SVECTOR *)(o + (s32)sv))->vy -= ((SVECTOR *)(o + (s32)sv))->vy >> 4;
                    }
                    sv2 = (SVECTOR *)D_801E9168;
                    i = 0;
                    p2 = 8;
                    for (; i < 17; p2 += 16, i++) {
                        sv2[i * 2].vy -= sv2[i * 2].vy >> 4;
                        ((SVECTOR *)(p2 + (s32)sv2))->vy -= ((SVECTOR *)(p2 + (s32)sv2))->vy >> 3;
                    }
                }
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 3), work->unk4);
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_09a/nonmatchings/fx_09a", fx_09a_headerPtr);
