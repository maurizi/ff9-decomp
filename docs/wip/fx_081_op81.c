/* Draft of fx_081 fx_081_op81 (twin of fx_057 op81, 0.78). Status: 4 aligned mismatches (2 instructions).
 * Round 5 levers: case 2's `a = j >> 2` reuses the loop variable a (the original's s0; case 0/1's t keeps s4), the
 * case 1 copy block with work->unk14.vy -= 0xC00 before work->unk1C.vz, `for (j = 0, pp = &pos; ...)`,
 * `for (i = 0; i < 12; i++, a += 0x155)`, and ang.vx = vx after ang.vy in the second loop.
 * Open: the second loop's ang.vy add has its operands swapped (original addu v0,s0(a),s6(w << 4)); writing
 * (a = ...) + (w << 4) or an explicit w4 changes loop.c's hoisting and the global allocation; and at the bottom of
 * the outer loop the original orders the D_801E8E7C giv increment, frame's reload and j++ as s2, lw t0, j++
 * (here s2, j++, lw t0; `w += frame, j++` gives lw t0, s2, j++).
 * Earlier: frame << 7 computed into j before the tint, pp2 = pp for the second loop (spilled), h declared before c
 * (spill slots). */
#include "btlfx_mod.h"

extern Unk800F78A4 *D_801E8ED8;
extern SVECTOR D_801EA728;
extern SVECTOR D_801EA730;
extern Pool6CEntry *D_801EA738;
extern Unk80100C78Entry *D_801EA9B0;
extern s32 D_801EA73C; /* glow level */
extern Pool6CEntry *D_801EA740;
extern SVECTOR D_801EA748; /* caster body position */
extern BtlModel D_801EA758[];
extern u8 D_801E8EE8[];

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx081Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    s32 unk8; /* model */
    s32 unkC; /* model */
    SVECTOR unk10;
    Fx081Vec3 unk18;
    SVECTOR unk24;
    Pool6CEntry *unk2C;
    Pool6CEntry *unk30;
    Pool6CEntry *unk34;
} Fx081Work80; /* 0x38 */

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    s32 unk8; /* priority base */
    SVECTOR unkC; /* target position */
    SVECTOR unk14;
    Fx081Vec3 unk1C;
    s32 unk28; /* model */
} Fx081Work81; /* 0x2C */

extern s16 D_801E8E7C[10]; /* ring point counts */

void func_801E7704(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 arg3, s32 *arg4, s32 arg5, s32 arg6, s32 frame,
                   s32 n);

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
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, NULL, 0x13000, frame, 1, s);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &work->unkC, NULL, (t >> 1) + 0x3000, 0, 1,
                              s);
            env->unk28 = 0;
            ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &work->unkC, work->unk4,
                              ex->math_sin(t >> 2));
            if (frame >= 7) {
                work->unk0 = 1;
                entry->unk0 = -1;
                work->unk28 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 0xB9,
                                                         0x3E00, 0x80, 0);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0, 0, 0xFF);
            }
            break;
        case 1:
            rot.vx = 0;
            rot.vy = 0;
            rot.pad = 0;
            rot.vz = 0x400;
            env->unk28 = work->unk8 + 0x40;
            t = (frame << 12) / 7;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x24), &work->unkC, &rot, 0x3000, frame, 1,
                              0x1000);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unkC, NULL, t + 0x13000, frame, 1,
                              0x1000);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &work->unkC, NULL, (t >> 1) + 0x3800, 0, 1,
                              0x1000);
            env->unk28 = 0;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0, 0, 0x80);
            ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &work->unkC, work->unk4, 0x1000);
            if (frame >= 7) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xFF, 0xFF, 0);
                work->unk0 = 2;
                entry->unk0 = -1;
                ex->func_800E3D54(D_801EA740);
                BTLFX_COPY_SVEC(&work->unkC, &env->unkC->unk38);
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
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, u >> 1, u, u);
            }
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
            for (j = 0, w = 0; j < 10; j++, w += frame) {
                vx = 0x400 - (j << 11) / 9;
                for (i = 0; i < D_801E8E7C[j]; i++) {
                    ang.vy = (w << 4) + (a = (i << 12) / D_801E8E7C[j] - (work->unk4 << 5));
                    ang.vx = vx;
                    ex->func_800CFD78((s16 *)&work->unkC, (s16 *)&ang, h, (s16 *)pp2);
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
