#include "fxmod/fx_0a0.h"

/* Draws a gouraud line from a to b, colors c0/c1 faded towards black by p (0x1000 = full; < 0 = full). */
void fx_0a0_drawGouraudLine(SVECTOR *a, SVECTOR *b, CVECTOR *c0, CVECTOR *c1, s32 p, s32 abr) {
    Unk800F78A4 *env = fx_0a0_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    LINE_G2 *prim;
    CVECTOR col0;
    CVECTOR col1;
    s32 otz;

    if (p < 0) {
        p = 0x1000;
    }
    prim = (LINE_G2 *)env->prim;
    env->prim += sizeof(LINE_G2);
    gte_DpqColorFar_nst(c0, p);
    gte_strgb(&col0);
    gte_DpqColorFar_nst(c1, p);
    gte_strgb(&col1);
    col0.cd = (abr == 0xFF) ? 0x50 : 0x52;
    gte_ldv3(a, b, a);
    gte_rtpt();
    *(u32 *)&prim->r0 = *(u32 *)&col0;
    *(u32 *)&prim->r1 = *(u32 *)&col1;
    gte_stsxy01(&prim->x0, &prim->x1);
    gte_avsz3();
    gte_stszotz(&otz);
    ex->btlfx_addPrimAbr(0x4000000, otz, prim, abr);
}

static void fxm_drawTrailLine(u8 *file, SVECTOR *from, SVECTOR *to, s32 frame, s32 fade, s32 abr, s32 lineWidth) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0a0_env);
    SVECTOR *pts;

    fx_0a0_line.lastSlot0 = fx_0a0_line.count2 = 1;
    fx_0a0_line.head1 = 0;
    pts = fx_0a0_line.pts4;
    pts[0].vx = from->vx;
    pts[0].vy = from->vy;
    pts[0].vz = from->vz;
    pts[0].pad = lineWidth;
    pts[1].vx = to->vx;
    pts[1].vy = to->vy;
    pts[1].vz = to->vz;
    pts[1].pad = lineWidth;
    ex->btlfx_drawTrail(file, 0, NULL, frame, 0x1000, fade, abr, (u8 *)&fx_0a0_line, 0);
}

static void fx_0a0_op80ElemDraw(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 rot, s32 *scale, s32 alpha, s32 blend, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0a0_env);
    Unk80100C78Entry *entry = fx_0a0_task;
    CVECTOR col0;
    CVECTOR col1;
    s32 trail;
    s32 fade;

    trail = ex->btlfx_getParticleTrail(pt, p);
    if (frame == 0) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_WEB_STRANDS), (s32)pos, NULL, -1, 0, 0, BTLFX_ABR_HALF, (u8 *)trail, 2);
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_WEB_STRANDS), (s32)pos, (s32 *)p->origin, frame, *scale, fx_0a0_trailAlpha, BTLFX_ABR_ADD,
                      (u8 *)trail, 2);
    if (frame < 0x28) {
        fade = ex->math_cos((frame << 10) / 40);
        col0.r = col0.g = col0.b = 0x80;
        col1.r = col1.g = col1.b = 0x31;
        fxm_drawTrailLine((u8 *)ex->btlfx_getFileData(entry->file, FX_WEB_STRAND_LINE), pos, &fx_0a0_bonePos, 0, fade, 1, 0x40);
    }
}

s32 fx_0a0_op80(s32 mode, Fx0a0Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;

    if (mode == 0) {
        *args = 0x24;
    } else if (mode == 1) {
        fx_0a0_env = env;
        work->state0 = 0;
        fx_0a0_trailAlpha = 0x1000;
        fx_0a0_task = entry;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos4);
        BTLFX_COPY_SVEC(&fx_0a0_bonePos, &work->pos4);
        BTLFX_COPY_SVEC(&work->posC, &work->pos4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
        work->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WEB_CALLBACK_EMITTER), (s32)&work->pos14);
        if (work->emit1C != NULL) {
            work->emit1C->otZ = ex->btlfx_addRadiusSixth(0, 0x10);
        }
        if (work->emit1C != NULL) {
            work->emit1C->trails = (s32)fx_0a0_trails;
            work->emit1C->trailSlots = 0x10;
            work->emit1C->trailStride = 0xAC;
        }
        work->emit1C->elems->draw = fx_0a0_op80ElemDraw;
        work->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WEB_WHITE_PUFFS), (s32)&work->posC);
        if (work->emit20 != NULL) {
            work->emit20->otZ = 0x20;
        }
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos4);
        BTLFX_COPY_SVEC(&fx_0a0_bonePos, &work->pos4);
        switch (work->state0) {
        case 0:
            if (work->emit20 != NULL && work->emit20->state == BTLFX_EMIT_DONE) {
                work->emit20 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit20, (s32)&work->pos4);
            if (frame >= 0x29) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = frame * 0x200;
            fx_0a0_trailAlpha = 0x1000 - t;
            if (frame >= 8) {
                ex->btlfx_stopEmitter(work->emit1C);
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a0/nonmatchings/fx_0a0", fx_0a0_headerPtr);
