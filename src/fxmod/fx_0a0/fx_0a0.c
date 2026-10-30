#include "fxmod/fx_0a0.h"

/* Draws a gouraud line from a to b, colors c0/c1 faded towards black by p (0x1000 = full; < 0 = full). */
void func_801E7704(SVECTOR *a, SVECTOR *b, CVECTOR *c0, CVECTOR *c1, s32 p, s32 abr) {
    Unk800F78A4 *env = D_801E7D98;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    LINE_G2 *prim;
    CVECTOR col0;
    CVECTOR col1;
    s32 z;

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
    gte_stszotz(&z);
    ex->btlfx_addPrimAbr(0x4000000, z, prim, abr);
}

void func_801E783C(u8 *file, SVECTOR *a, SVECTOR *b, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7D98);
    SVECTOR *v;

    D_801E7D84.unk0 = D_801E7D84.unk2 = 1;
    D_801E7D84.unk1 = 0;
    v = D_801E7D84.unk4;
    v[0].vx = a->vx;
    v[0].vy = a->vy;
    v[0].vz = a->vz;
    v[0].pad = arg6;
    v[1].vx = b->vx;
    v[1].vy = b->vy;
    v[1].vz = b->vz;
    v[1].pad = arg6;
    ex->btlfx_drawTrail(file, 0, NULL, arg3, 0x1000, arg4, arg5, (u8 *)&D_801E7D84, 0);
}

void func_801E78F8(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 arg3, s32 *arg4, s32 arg5, s32 arg6, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7D98);
    Unk80100C78Entry *entry = D_801E886C;
    CVECTOR c0;
    CVECTOR c1;
    s32 r;
    s32 s;

    r = ex->btlfx_getParticleTrail(pt, p);
    if (frame == 0) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 7), (s32)pos, NULL, -1, 0, 0, 0, (u8 *)r, 2);
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 7), (s32)pos, (s32 *)p->origin, frame, *arg4, D_801E8870, 1,
                      (u8 *)r, 2);
    if (frame < 0x28) {
        s = ex->math_cos((frame << 10) / 40);
        c0.r = c0.g = c0.b = 0x80;
        c1.r = c1.g = c1.b = 0x31;
        func_801E783C((u8 *)ex->btlfx_getFileData(entry->file, 6), pos, &D_801E8864, 0, s, 1, 0x40);
    }
}

s32 fx_0a0_op80(s32 mode, Fx0a0Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        D_801E7D98 = env;
        work->unk0 = 0;
        D_801E8870 = 0x1000;
        D_801E886C = entry;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk4);
        BTLFX_COPY_SVEC(&D_801E8864, &work->unk4);
        BTLFX_COPY_SVEC(&work->unkC, &work->unk4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk14);
        work->unk1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk14);
        if (work->unk1C != NULL) {
            work->unk1C->unk22 = ex->btlfx_addRadiusSixth(0, 0x10);
        }
        if (work->unk1C != NULL) {
            work->unk1C->unk14 = (s32)D_801E7DA4;
            work->unk1C->unk12 = 0x10;
            work->unk1C->unkC = 0xAC;
        }
        work->unk1C->elems->unk24 = func_801E78F8;
        work->unk20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unkC);
        if (work->unk20 != NULL) {
            work->unk20->unk22 = 0x20;
        }
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk4);
        BTLFX_COPY_SVEC(&D_801E8864, &work->unk4);
        switch (work->unk0) {
        case 0:
            if (work->unk20 != NULL && work->unk20->state == -1) {
                work->unk20 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->unk20, (s32)&work->unk4);
            if (frame >= 0x29) {
                work->unk0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = frame * 0x200;
            D_801E8870 = 0x1000 - t;
            if (frame >= 8) {
                ex->btlfx_stopEmitter(work->unk1C);
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a0/nonmatchings/fx_0a0", fx_0a0_headerPtr);
