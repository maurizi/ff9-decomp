#include "fxmod/fx_0b2.h"

void func_801E7704(Unk80100C78Entry *entry, Fx0b2Trail *trail, SVECTOR *from, SVECTOR *to, s32 arg4, s32 arg5, s32 arg6,
                   s32 arg7, s32 arg8, s32 arg9, s32 arg10) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E90A0);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 h;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    i = 0;
    p = trail->unk4;
    trail->unk0 = trail->unk2 = 0x18;
    trail->unk1 = 0;
    do {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x100);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = ex->btlfx_randCentered(0xA0);
        i++;
        h = (ex->math_sin(t >> 1) * h) >> 12;
        h += ex->btlfx_lerp(arg4, arg5, t);
        ex->func_800D044C(&pos, (s32)p, 0, (XY32 *)&rot, h, arg6);
        p->pad = arg10;
        arg6 += arg7 + ex->btlfx_randCentered(0x80);
        p++;
    } while (i < 25);
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 4), 0, NULL, arg8, 0x1000, arg9, 1, (u8 *)trail, 0);
}

void func_801E78FC(SVECTOR *arg0, SVECTOR *arg1, VECTOR *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 arg10, s32 arg11, s32 arg12, s32 arg13, s32 arg14, s32 arg15) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E90A0);
    Fx0b2Rgbc c0;
    Fx0b2Rgbc c1;
    Fx0b2Rgbc c2;
    MATRIX m;
    SVECTOR v[4];
    s32 z;
    POLY_GT4 *gt4;
    POLY_FT4 *ft4;
    s32 r0;
    s32 r1;
    s32 j;
    s32 row;
    s32 ang;
    s32 h0;
    s32 h1;
    s32 a;
    s32 x;
    s32 y;
    s32 d;
    s32 e;
    s32 ft;
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
    s32 ju;
    s32 rv;

    gte_SetRotMatrix(D_801E90A0->viewMatrix);
    gte_SetTransMatrix(D_801E90A0->viewMatrix);
    gte_ldv0(arg0);
    gt4 = (POLY_GT4 *)D_801E90A0->prim;
    *(s32 *)&c2 = 0;
    ft4 = (POLY_FT4 *)((u8 *)gt4 + arg15 * 0x68);
    D_801E90A0->prim = (u8 *)ft4 + arg15 * 0xC8;
    c0.r = c0.g = c0.b = arg5;
    c1.r = c1.g = c1.b = arg5;
    c0.code = arg14 != 0xFF ? 0x2E : 0x2C;
    c2.code = arg14 != 0xFF ? 0x3E : 0x3C;
    arg6 |= (arg14 & 3) << 5;
    c1.code = c2.code;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(arg1, &m);
    ex->ScaleMatrix(&m, arg2);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    ang = 0;
    /* row * arg10, (row + 1) * arg10, row + 1 and the two heights are loop-reduced into five stack temporaries;
     * h0/h1 are set here (not at their use) so their lifetime makes them worth reducing. */
    for (row = 0; row < 7; row++) {
        h1 = row * 0x90;
        h0 = (row + 1) * 0x90;
        ang += arg13;
        arg11 += arg12;
        r0 = ex->btlfx_lerp(arg3, arg4, (row << 12) / 7);
        r1 = ex->btlfx_lerp(arg3, arg4, ((row + 1) << 12) / 7);
        for (j = 0; j < arg15; j++) {
            ang += arg13;
            x = (ex->math_cos(arg11 + arg12) * ((row + 1) * arg10 / 7)) >> 12;
            y = (ex->math_sin(arg11 + arg12) * ((row + 1) * arg10 / 7)) >> 12;
            v[0].vx = x + ((ex->math_cos(ang) * r1) >> 12);
            v[0].vy = y + ((ex->math_sin(ang) * r1) >> 12);
            d = 0x1000 / arg15;
            a = ang + d;
            v[1].vx = x + ((ex->math_cos(a) * r1) >> 12);
            v[1].vy = y + ((ex->math_sin(a) * r1) >> 12);
            v[0].vz = v[1].vz = h0;
            ang -= arg13;
            e = ang + d; /* a new variable keeps (plus ang d) in that operand order */
            x = (ex->math_cos(arg11) * (row * arg10 / 7)) >> 12;
            y = (ex->math_sin(arg11) * (row * arg10 / 7)) >> 12;
            v[2].vx = x + ((ex->math_cos(ang) * r0) >> 12);
            v[2].vy = y + ((ex->math_sin(ang) * r0) >> 12);
            v[3].vx = x + ((ex->math_cos(e) * r0) >> 12);
            v[3].vy = y + ((ex->math_sin(e) * r0) >> 12);
            v[2].vz = v[3].vz = h1;
            gte_ldv3c(v);
            gte_rtpt();
            /* j & 3 and row & 3 in variables: the original computes each once, across the first ternary's label */
            ju = j & 3;
            u0 = arg8 + (ju * 7 * 16 >> 2);
            rv = row & 3;
            v0 = arg9 + (rv * 7 * 16 >> 2);
            /* a default and an override, not a ternary: the order global-alloc sees puts v1 before u1 */
            u1 = u0 + 0x1C;
            if (ju == 3) {
                u1 = u0 + 0x1B;
            }
            v1 = v0 + 0x1C;
            if (rv == 3) {
                v1 = v0 + 0x1B;
            }
            gte_nclip();
            if (row == 0) {
                ft = 0;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&c1;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&c2;
            } else if (row >= 6) {
                ft = 0;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&c1;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&c2;
            } else {
                ft = 1;
                *(s32 *)&ft4->r0 = *(s32 *)&c0;
            }
            gte_stopz(&z);
            if (z != 0) {
                if (ft) {
                    ft4->u0 = ft4->u2 = u0;
                    ft4->u1 = ft4->u3 = u1;
                    ft4->v2 = ft4->v3 = v0;
                    ft4->v0 = ft4->v1 = v1;
                    ft4->tpage = arg6;
                    ft4->clut = arg7;
                    gte_stsxy3(&ft4->x0, &ft4->x1, &ft4->x2);
                } else {
                    gt4->u0 = gt4->u2 = u0;
                    gt4->u1 = gt4->u3 = u1;
                    gt4->v2 = gt4->v3 = v0;
                    gt4->v0 = gt4->v1 = v1;
                    gt4->tpage = arg6;
                    gt4->clut = arg7;
                    gte_stsxy3(&gt4->x0, &gt4->x1, &gt4->x2);
                }
                gte_avsz3();
                gte_stszotz(&z);
                gte_ldv0(&v[3]);
                gte_rtps();
                if (ft) {
                    ex->btlfx_addPrim(0x09000000, z, ft4);
                    gte_stsxy(&ft4->x3);
                    ft4++;
                } else {
                    ex->btlfx_addPrim(0x0C000000, z, gt4);
                    gte_stsxy(&gt4->x3);
                    gt4++;
                }
            }
            ang += 0x1000 / arg15;
        }
    }
}

s32 fx_0b2_op80(s32 mode, Fx0b2Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR scale;
    s32 frame;
    s32 alpha;
    s32 r;
    s32 a;
    s32 i;
    s32 t; /* one variable for cases 1-3 (fp in the original) */
    s32 state;
    s32 one = 1; /* a variable, not a literal: the original reloads it for `mode == 1` and `return 1` */

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == one) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E90A0 = env;
        ((s32 *)&work->unk8)[0] = ((s32 *)&env->targets->centerX)[0];
        ((s32 *)&work->unk8)[1] = ((s32 *)&env->targets->centerX)[1];
        work->unk18.vx = 0x400;
        work->unk18.vy = 0x20;
        work->unk18.vz = 0;
    } else {
        alpha = 0x1000;
        r = work->unk4 * 0x300 / 72;
        frame = entry->frame;
        a = alpha - ex->math_cos(work->unk4 * 1500 / 72);
        work->unk10.vx = work->unk8.vx + ((ex->math_cos(a) * r) >> 12);
        work->unk10.vz = work->unk8.vz + ((ex->math_sin(a) * r) >> 12);
        work->unk10.vy = work->unk8.vy;
        state = work->unk0;
        /* Dead stores only (flow deletes them, jump2 the branch): the labels end cse's block here, so case 3
         * computes 0x1000 - t from the constant instead of reusing alpha (which cse would know is 0x1000). */
        if (a) {
            r = 0;
            a = 0;
        } else {
            r = 1;
            a = 1;
        }
        switch (state) {
        case 0: {
            SVECTOR rot;
            s32 w;
            s32 t;

            scale.vz = 0x2000;
            alpha = (frame << 12) / 12;
            scale.vy = alpha;
            scale.vx = alpha;
            w = alpha >> 6;
            func_801E78FC(&work->unk10, &work->unk18, &scale, 0xC8, 0x180, 0x80, 0x11A, 0xFFFF, 0x10, 0x90, w,
                          work->unk4 * 0xC8, 0x190, 0xC8, 1, 0x10);
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = work->unk4 * 90;
            rot.pad = 1;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->unk10, &rot, 0x6000, 0, 1, alpha / 2, 0,
                              0);
            scale.vy = 0x2000;
            scale.vx = 0x2000;
            t = alpha >> 2;
            scale.vz = ex->math_sin(t) * 2;
            func_801E78FC(&work->unk10, &work->unk18, &scale, (ex->math_cos(t) >> 4) + 0x1C8, 0x280, alpha >> 5, 0x1A,
                          0x3D01, 0, 0, w, work->unk4 * 0xC8, 0x190, 0x80, 1, 8);
            scale.vx = scale.vy = scale.vx * 3 / 2;
            func_801E78FC(&work->unk10, &work->unk18, &scale, (ex->math_cos(t) >> 4) + 0x1C8, 0x280, w, 0x17, 0x3D03, 0,
                          0x70, w, work->unk4 * 0xC8, 0x190, 0x40, 1, 8);
            if (frame >= 12) {
                work->unk0 = 1;
                entry->frame = -1;
            }
            break;
        }
        case 1: {
            SVECTOR rot;
            s32 y;

            scale.vz = 0x2000;
            t = (frame << 12) / 12;
            scale.vx = scale.vy = t + 0x1000;
            func_801E78FC(&work->unk10, &work->unk18, &scale, 0xC8, 0x180, 0x80, 0x11A, 0xFFFF, 0x10, 0x90, (t >> 5) + 0x40,
                          work->unk4 * 0xC8, 0x190, 0xC8, 1, 0x10);
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = work->unk4 * 90;
            rot.pad = 1;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->unk10, &rot, 0x6000, 0, 1,
                              t / 2 + 0x800, 0, 0);
            scale.vz = scale.vx = scale.vy = 0x2000;
            y = 0x1C8 - (t >> 5);
            func_801E78FC(&work->unk10, &work->unk18, &scale, y, 0x280, 0x80, 0x1A, 0x3D01, 0, 0, (t >> 5) + 0x40, work->unk4 * 0xC8,
                          0x190, 0x80, 1, 8);
            scale.vx = scale.vy = scale.vx * 3 / 2;
            func_801E78FC(&work->unk10, &work->unk18, &scale, y, 0x280, 0x40, 0x17, 0x3D03, 0, 0x70, (t >> 5) + 0x40,
                          work->unk4 * 0xC8, 0x190, 0x40, 1, 8);
            if (frame >= 12) {
                Pool6CEntry *p;

                work->unk0 = 2;
                entry->frame = -1;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x58;
                }
            }
            break;
        }
        case 2: {
            SVECTOR rot;
            s32 s;
            s32 w;

            t = frame << 7;
            s = (t >> 1) + 0x2000;
            scale.vx = scale.vy = scale.vz = s;
            w = (t >> 5) + 0xC0;
            func_801E78FC(&work->unk10, &work->unk18, &scale, 0xC8 - (t >> 6), 0x180, 0x80, 0x11A, 0xFFFF, 0x10, 0x90, w,
                          work->unk4 * 0xC8, 0x190, (t >> 5) + 0xA0, 1, 0x10);
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = work->unk4 * 90;
            rot.pad = 1;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->unk10, &rot, 0x6000, 0, 1, alpha, 0, 0);
            scale.vx = scale.vy = scale.vz = s;
            func_801E78FC(&work->unk10, &work->unk18, &scale, 0x148, 0x280, 0x80, 0x1A, 0x3D01, 0, 0, w,
                          work->unk4 * 0xC8, 0x190, 0x80, 1, 8);
            scale.vx = scale.vy = scale.vx * 3 / 2;
            func_801E78FC(&work->unk10, &work->unk18, &scale, 0x148, 0x280, 0x40, 0x17, 0x3D03, 0, 0x70, w,
                          work->unk4 * 0xC8, 0x190, 0x40, 1, 8);
            if (frame >= 0x20) {
                work->unk0 = 3;
                entry->frame = -1;
            }
            break;
        }
        case 3: {
            SVECTOR rot;
            s32 u;

            scale.vx = scale.vy = scale.vz = 0x2800;
            t = frame << 8;
            u = t >> 2;
            func_801E78FC(&work->unk10, &work->unk18, &scale, 0x88, 0x180, ex->math_cos(u) >> 5, 0x11A, 0xFFFF, 0x10, 0x90,
                          0x140, work->unk4 * 0xC8, 0x190, 0x120, 1, 0x10);
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = work->unk4 * 90;
            rot.pad = 1;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->unk10, &rot, 0x6000, 0, 1, 0x1000 - t, 0,
                              0);
            scale.vx = scale.vy = scale.vz = 0x2800;
            func_801E78FC(&work->unk10, &work->unk18, &scale, 0x148, 0x280, ex->math_cos(u) >> 5, 0x1A, 0x3D01, 0, 0, 0x140,
                          work->unk4 * 0xC8, 0x190, 0x80, 1, 8);
            alpha -= t;
            scale.vx = scale.vy = scale.vx * 3 / 2;
            func_801E78FC(&work->unk10, &work->unk18, &scale, 0x148, 0x280, ex->math_cos(u) >> 6, 0x17, 0x3D03, 0, 0x70,
                          0x140, work->unk4 * 0xC8, 0x190, 0x40, 1, 8);
            if (frame >= 0x10) {
                return one;
            }
            break;
        }
        }
        work->unk18.vz += -0x10 - work->unk4 * 2;
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 2), work->unk4);
        ex->btlfx_drawScrollMove(10000 - work->unk4 * 8, 0x1C0, 0x170, 0x1C, 0x70, 0x1DC, 0x170);
        ex->btlfx_drawScrollMove(10000 - work->unk4 * 12, 0x280, 0x100, 0x1C, 0x70, 0x29C, 0x100);
        i = 0;
        if (alpha != 0) {
            SVECTOR from;
            SVECTOR to;
            Fx0b2Trail *p;
            s32 ang;
            s32 h;
            s32 d;
            s32 e;

            from.vx = work->unk10.vx;
            from.vy = work->unk10.vy;
            from.vz = work->unk10.vz;
            from.vy -= 0x640;
            ang = work->unk4 * 0x180 + (ex->btlfx_rand() & 0xFF);
            p = D_801E90B0;
            do {
                i++;
                h = ((ex->btlfx_rand() & 0xFFF) * alpha) >> 12;
                d = (ex->btlfx_rand() & 0x1FF) + 0x400;
                BTLFX_COPY_SVEC(&to, &work->unk10);
                to.vx += (ex->math_cos(ang) * d) >> 12;
                to.vz += (ex->math_sin(ang) * d) >> 12;
                e = ang + ex->btlfx_rand();
                func_801E7704(entry, p, &from, &to, 0x100, 0x180, e, 0x100, 0, h, (ex->btlfx_rand() & 0xF) + 0xF);
                p++;
                ang += 0x300 + (ex->btlfx_rand() & 0x3FF);
            } while (i < 3);
        }
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b2/nonmatchings/fx_0b2", fx_0b2_headerPtr);
