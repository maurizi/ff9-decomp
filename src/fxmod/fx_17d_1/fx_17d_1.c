#include "fxmod/fx_17d_1.h"

/* Matching notes: q is a separate pointer biv (the original's s1 giv), with the addPrim argument taken first
 * (pp = p++). nu = uu + dw, copied back to uu after the call, is a block-local pseudo crossing the call that
 * local-alloc puts in s0; that gives uu an s0 preference and keeps s0 from the giv in global-alloc. i = 0 before the
 * row header and q++ before i++ order the giv inits; j = 0 before dw = m places sra fp late in the prologue. */
void func_801EC704(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr) {
    POLY_FT4 *p;
    BtlfxExports *ex;
    u8 col[4];
    s32 x0;
    s32 y0;
    s32 y1;
    s32 dw;
    s32 dh;
    s32 v1;
    s32 abrBits;
    s32 uu;
    s32 xa;
    s32 xs;
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    s32 ta;
    s32 ts;
    POLY_FT4 *q;
    POLY_FT4 *pp;
    s32 nu;

    x0 = x - (w >> 1) * 3 / 2;
    abrBits = (abr & 3) << 5;
    m = w >> 2;
    ex = BTLFX_EXPORTS(D_801ED420);
    p = (POLY_FT4 *)D_801ED420->prim;
    dh = h >> 2;
    D_801ED420->prim = (u8 *)(p + 16);
    y0 = y - (h >> 1);
    j = 0;
    dw = m;
    for (; j < 4; j++) {
        i = 0;
        y1 = y0 + dh;
        v1 = v + dh;
        uu = u;
        q = p;
        for (; i < 4;) {
            col[0] = col[1] = col[2] = c;
            if (abr != 0xFF) {
                col[3] = 0x2E;
            } else {
                col[3] = 0x2C;
            }
            pp = p++;
            xs = dw * i;
            xa = xs + dw;
            ta = xa * 3;
            ts = xs * 3;
            k = i < 3;
            q->x0 = q->x2 = x0 + ts / 2;
            q->u0 = q->u2 = uu;
            nu = uu + dw;
            q->u1 = q->u3 = nu - (k ^ 1);
            m = j < 3;
            q->v2 = q->v3 = v1 - (m ^ 1);
            q->x1 = q->x3 = x0 + ta / 2;
            q->v0 = q->v1 = v;
            q->y0 = q->y1 = y0;
            q->y2 = q->y3 = y1;
            tpage |= abrBits;
            q->clut = clut;
            q->tpage = tpage;
            *(u32 *)&q->r0 = *(u32 *)col;
            q++;
            i++;
            ex->btlfx_addPrim(0x09000000, ot, pp);
            uu = nu;
        }
        v += dh;
        y0 += dh;
    }
}

void func_801EC944(u32 *ot, s32 tpage, s32 x, s32 r, s32 g, s32 b) {
    BtlfxExports *ex;
    POLY_G4 *prim;
    POLY_G4 *p;
    s32 i;
    s32 k;

    prim = (POLY_G4 *)D_801ED420->prim;
    ex = BTLFX_EXPORTS(D_801ED420);
    p = prim;
    D_801ED420->prim = (u8 *)(prim + 4);
    for (i = 0; i < 4; i++) {
        setlen(p, 8);
        p->code = 0x3A;
        p->x1 = x + (s16)(i * 16 + 0x80);
        p->r0 = p->r2 = r;
        p->g0 = p->g2 = g;
        p->b0 = p->b2 = b;
        p->r1 = p->r3 = 0;
        p->g1 = p->g3 = 0;
        p->b1 = p->b3 = 0;
        p->x0 = p->x2 = x;
        k = i + 1;
        p->x3 = x + (s16)(k * 16 + 0x80);
        p->y0 = p->y1 = i * 60;
        p->y2 = p->y3 = i * 60 + 60;
        p++;
        ex->btlfx_linkPrimAbr(0x08000000, ot, prim, tpage);
        prim++;
    }
}

s32 fx_17d_1_op80(s32 mode, Fx17d1Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sc;
    s32 frame;
    s32 state;
    s32 t;
    s32 k;
    s32 tint;
    s32 lo;
    s32 hi;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x7C;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801ED420 = env;
        ex->btlfx_initEffModels(D_801ED430, 2);
        work->unk78 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x9E), 1);
        ex->Hi_SetSummonMotion(((Fx17d1Summon *)env->unk4)->unk54->unk180, ((Fx17d1Summon *)env->unk4)->unk60);
        work->unk28.vx = 0;
        work->unk28.vy = 0;
        work->unk68.vx = -0x400;
        work->unk5C.vx = work->unk5C.vy = work->unk5C.vz = 0x1000;
        work->unk50.vz = 0x100;
        work->unk34.vx = work->unk34.vy = 0x800;
        work->unk28.vz = 0;
        work->unk34.vz = 0x555;
        work->unk68.vy = 0;
        work->unk68.vz = 0;
        work->unk50.vx = 0;
        work->unk50.vy = 0;
        work->unk40.vx = 0;
        work->unk40.vy = 0;
        work->unk40.vz = 0;
        work->unk18.vx = work->unk28.vx;
        work->unk18.vy = work->unk28.vy;
        work->unk18.vz = work->unk28.vz - 0x3000;
        work->unk18.vx += 0x400;
        env->camera->points[1].vx = work->unk50.vx;
        env->camera->points[1].vy = work->unk50.vy;
        env->camera->points[1].vz = work->unk50.vz;
        work->unk18.vx += 0x20;
        env->camera->points[2].vx = work->unk18.vx;
        env->camera->points[2].vy = work->unk18.vy;
        env->camera->points[2].vz = work->unk18.vz;
    } else {
        state = work->unk0;
        frame = entry->unk0;
        switch (state) {
        case 0:
            t = (frame << 12) / 24;
            work->unk50.vz -= 0x26C;
            work->unk28.vz -= 0x26C;
            if (frame != 0) {
                ex->Hi_SetSummonMotFrame(((Fx17d1Summon *)env->unk4)->unk60, 0);
                ex->Hi_DrawSummonModel(&work->unk68, (VECTOR *)&work->unk50, (s32)&work->unk5C,
                                       ((Fx17d1Summon *)env->unk4)->unk60, 0);
                ex->Hi_ModifySummonModelRGB(((Fx17d1Summon *)env->unk4)->unk60, ((0x800 - (t >> 2)) >> 5) - 0x80,
                                            ((0x800 - (t >> 2)) >> 5) - 0x80, ((0x800 - (t >> 2)) >> 5) - 0x80);
            }
            work->unk10.vx = work->unk28.vx;
            work->unk10.vy = work->unk28.vy;
            work->unk10.vz = work->unk28.vz - 0x258;
            ex->Hi_DrawEffModel(&work->unk40, (VECTOR *)&work->unk28, (s32)&work->unk34, work->unk78);
            ex->Hi_ModifyEffModelRGB(work->unk78, (t >> 6) - 0x80, (t >> 6) - 0x80, (t >> 6) - 0x80);
            env->camera->points[1].vx = work->unk50.vx;
            env->camera->points[1].vy = work->unk50.vy;
            env->camera->points[1].vz = work->unk50.vz;
            work->unk18.vx += 0x20;
            env->camera->points[2].vx = work->unk18.vx;
            env->camera->points[2].vy = work->unk18.vy;
            env->camera->points[2].vz = work->unk18.vz;
            ex->btlfx_drawScrollMove(frame * 8, 0x340, 0x100, 0x40, 0x80, 0x340, 0x180);
            if (frame < 12) {
                tint = (ex->math_cos((frame << 10) / 12) << 7) >> 12;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 0x800), 0xFF, tint, tint, tint);
            }
            if (frame >= 13) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, ((frame - 12) << 7) / 12, 0, 0);
            }
            if (frame == 8) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x9D), (s32)&work->unk10);
                work->unk74 = p;
                if (p != NULL) {
                    p->unk14 = (s32)D_801ED5C0;
                    work->unk74->unk12 = 0xE0;
                    work->unk74->unkC = 0x1C;
                    if (work->unk74 != NULL) {
                        work->unk74->unk22 = 0x80;
                    }
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            } else if (frame >= 9) {
                if (work->unk74 != NULL) {
                    if (work->unk74->unk30 == -1) {
                        work->unk74 = NULL;
                    }
                    if (work->unk74 != NULL) {
                        work->unk74->unk5C.vx = work->unk40.vx;
                        work->unk74->unk5C.vy = work->unk40.vy;
                        work->unk74->unk5C.vz = work->unk40.vz;
                    }
                }
            }
            if (frame >= 5) {
                ex->btlfx_rangeProgress(frame, 4, 0x14, 0, 0x1000, &k);
                env->unk28 = 0x100;
                work->unk10.vz += 0x258;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &work->unk10, NULL, 0x1000, 0, 1, k);
                work->unk10.vz -= 0x258;
                env->unk28 = 0;
            }
            if (frame >= 18) {
                func_801EC944((u32 *)(env->ot + 0xFFE), 0xFF, 0, 0, 0x18, 0xC8);
            }
            if (frame >= 24) {
                work->unk0 = 1;
                entry->unk0 = -1;
                ex->btlfx_stopEmitter(work->unk74);
                lo = ((s32 *)&work->unk8)[0];
                hi = ((s32 *)&work->unk8)[1];
                ((s32 *)&work->unk18)[0] = lo;
                ((s32 *)&work->unk18)[1] = hi;
                work->unk28.vx = 0;
                work->unk28.vz = 0;
                work->unk18.vy -= 0x1000;
                work->unk40.vx = 0x400;
                work->unk40.vy = 0;
                work->unk40.vz = 0;
                work->unk28.vy = -0x2000;
                work->unk34.vx = work->unk34.vy = 0x333;
                work->unk34.vz = 0xC00;
                env->camera->points[3].vx = work->unk18.vx;
                env->camera->points[3].vy = work->unk18.vy;
                env->camera->points[3].vz = work->unk18.vz;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x9F), (s32)&work->unk10);
                work->unk70 = p;
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                work->unk48.vx = 0x400;
                work->unk48.vy = 0;
                work->unk48.vz = 0;
            }
            break;
        case 1:
            func_801EC704(0xFFE, frame / 2 + 0x96, 0x70, 0x100, 0xF0, 0x80, 0x9B, 0x3EC0, 0, 0, 1);
            work->unk28.vy += 0x58;
            sc.vx = sc.vy = work->unk34.vx * 2 / 3;
            sc.vz = work->unk34.vz;
            ex->Hi_DrawEffModel(&work->unk40, (VECTOR *)&work->unk28, (s32)&sc, work->unk78);
            ex->Hi_DrawEffModel(&work->unk40, (VECTOR *)&work->unk28, (s32)&work->unk34, work->unk78);
            ex->Hi_ModifyEffModelRGB(work->unk78, -0x40, -0x40, -0x40);
            work->unk10.vx = work->unk28.vx;
            work->unk10.vy = work->unk28.vy;
            work->unk10.vz = work->unk28.vz;
            env->unk28 = 0x100;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &work->unk10, NULL, 0x1000, 0, 1, -1);
            env->unk28 = 0;
            if (work->unk70 != NULL) {
                if (work->unk70->unk30 == -1) {
                    work->unk70 = NULL;
                }
                if (work->unk70 != NULL) {
                    work->unk70->unk5C.vx = work->unk48.vx;
                    work->unk70->unk5C.vy = work->unk48.vy;
                    work->unk70->unk5C.vz = work->unk48.vz;
                }
            }
            ex->btlfx_drawScrollMove(frame * 12, 0x340, 0x100, 0x40, 0x80, 0x340, 0x180);
            if (frame >= 0x50) {
                ex->btlfx_clearEffModels();
                env->camera->points[3].vx = 0;
                env->camera->points[3].vy = -0x1000;
                env->camera->points[3].vz = 0;
                return 1;
            }
            break;
        }
        env->unk28 = 0;
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17d_1/nonmatchings/fx_17d_1", fx_17d_1_headerPtr);
