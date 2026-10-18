/* Draft of fx_0fb_0 op80 (summon sequence: rocks rise, summon model drops in, bone trails, final model burst).
 * Status (pass 3): 16 mismatched instructions by opcode+register alignment (plus the 7 jump-table words and two
 * li/ori spellings of 0xFFFF that the row count includes), all in case 2's unk64 block after the sin call: the
 * original computes q2 * 4 (a1), q2 * 5, unkC.vx - q2 * 5 (a0) and q2 - 0x2800 (in place in q2's s0) before the
 * c_1 chain, and stores env->unk2C->unk0[2].vx through a copy of a0 (move v0,a0) instead of reloading it.
 * Pass 3 levers: case 3's x/y start at 0 (`y_1 = x_x0 = 0;` after b8); the second 4-loop of case 4 reuses i1;
 * case 5's tint is one `c = ((frame - 0x11) << 7) / 3` with c >> 2 / c >> 1; `u = t * 3 + 0x1000` before case
 * 5's first loop; i11 for both case 5 loops; case 6's `v_1 = u_2 >> 2` up front, `w = (u_3 - 8) << 5` after
 * sc.vy, and the r1/r2 branches only pick rp (one Hi_DrawEffModel call after the join); one block-level pos for
 * the ac and a8 tails (rot/sc after it), col declared first in the b4 tail and assigned first; pos.vy, vx, vz in
 * the ac tail; &m2 and t >> 8 written inline in case 4's 10-loop and 0x1000 - t inline in case 5's second loop
 * (loop.c hoists them like the original, and with fewer hoisted movables D_801EB70C is no longer hoisted); the
 * drawTrail trail argument as `(u8 *)(pp = &D_801EB70C[i])` with a block-local pp and `s = (s16 *)pp + 3`
 * afterwards (pp ties the %hi/%lo pseudos into the original's s-register); `i1 = 0;` before c8's store via
 * `for (i1 = 0; ...)`; the case 4 dead initializer z1 moved to case 3 (keeps the spill slot, frees the schedule).
 * Earlier findings:
 * - Block locals only share stack slots between cases if no expression statement at the block's own level runs
 *   after their address is taken (expand_expr_stmt -> preserve_temp_slots promotes addr_taken slots one level up).
 *   So each case body sits in an inner { } under its declarations; that gives the original 0x38..0xA0 layout.
 * - Spill slots follow pseudo (declaration) order.
 * - The env->unk2C->unk0[2] stores sit in both branches of case 3; jump2 cross-jumps the identical vy/vz tails. */
#include "fxmod/fx_0fb_0.h"

typedef struct {
    s32 vx, vy, vz;
} Fx0fbVec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    s32 unk8; /* rocks spawned */
    SVECTOR unkC; /* base position */
    SVECTOR unk14;
    SVECTOR unk1C;
    SVECTOR unk24;
    Fx0fbVec3 unk2C; /* model position */
    Fx0fbVec3 unk38; /* model scale */
    SVECTOR unk44; /* model rotation */
    SVECTOR unk4C;
    SVECTOR unk54;
    SVECTOR unk5C; /* summon position */
    SVECTOR unk64;
    s32 unk6C[27]; /* rock models */
    Fx0fbVec3 unkD8;
    Fx0fbVec3 unkE4;
    SVECTOR unkF0;
    Fx0fbVec3 unkF8;
    Fx0fbVec3 unk104;
    SVECTOR unk110;
    s32 unk118; /* models */
    s32 unk11C;
    s32 unk120;
    s32 unk124;
} Fx0fb0Work80; /* 0x128 */

typedef struct {
    u8 data[0x84];
} Fx0fb0Trail16; /* 4 + 16 points */

typedef struct {
    Fx0fb0Seg seg[12];
} Fx0fb0SegBuf; /* 0xC0 */

typedef struct {
    s16 x;
    s16 z;
} Fx0fb0Ofs;

typedef struct {
    u8 pad0[0x38];
    BtlModel *unk38;
    u8 pad3C[0x18];
    struct {
        u8 pad0[0x180];
        BtlUnit9CC *unk180[8]; /* summon motions */
    } *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx0fb0Summon;

#define FX0FB_W(v, i) (((s32 *)&(v))[i])
#define SUMMON0FB(env) ((Fx0fb0Summon *)(env)->unk4)

void func_801E7B90(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 col, s32 tpage, s32 clut, s32 u, s32 v, s32 amp,
                   s32 ph, s32 step, s32 abr);
void func_801E809C(SVECTOR *pos, s32 dy, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 ph, s32 step,
                   s32 abr, s32 ph2);
extern SVECTOR D_801EB248;
extern s16 D_801EB250[4][2];
extern s16 D_801EB260[10];
extern u16 D_801EB274[27];
extern s16 D_801EB2AC[25];
extern s16 D_801EB2E0[];
extern CVECTOR D_801EB34C[];
extern CVECTOR D_801EB358[];
extern CVECTOR D_801EB35C[];
extern SVECTOR D_801EB3BC[10];
extern SVECTOR D_801EB3C4[];
extern Fx0fb0SegBuf D_801EB40C[4];
extern Fx0fb0Trail16 D_801EB70C[10];

s32 fx_0fb_0_op80(s32 mode, Fx0fb0Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0fb0Rock *r;
    Pool6CEntry *p;
    s16 *s;
    s16 *s_0;
    s16 *s_1;
    SVECTOR *rp;
    s32 frame;
    s32 i;
    s32 i0;
    s32 i3;
    s32 i11;
    s32 i2;
    s32 i1;
    s32 j;
    s32 k;
    s32 u;
    s32 u_0;
    s32 u_1;
    s32 u_2;
    s32 u_4;
    s32 u_3;
    s32 v;
    s32 v_0;
    s32 v_1;
    s32 vb;
    s32 vb_0;
    s32 vb_1;
    s32 c;
    s32 c_0;
    s32 c_1;
    s32 c_4;
    s32 c_5;
    s32 c_6;
    s32 c_7;
    s32 w;
    s32 q2;
    s32 x;
    s32 x_x0;
    s32 y;
    s32 y_0;
    s32 y_1;
    s32 big;
    s32 idx;
    s32 a8;
    s32 ac;
    s32 b0;
    s32 b4;
    s32 b8;
    s32 bc;
    s32 t;

    if (mode == 0) {
        init->unk0 = 0x128;
        init->unk8 = 0x28;
        init->unk14 = (s32)func_801E8540;
        init->unk4 = 0x32;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EB3AC = env;
        ex->btlfx_initEffModels(SUMMON0FB(env)->unk38, 0x1F);
        work->unkC.vy = 0;
        work->unkC.vx = 0;
        work->unkC.vz = 0;
        work->unkF0.vy = -0x400;
        work->unkE4.vx = 0x1800;
        work->unkE4.vy = 0x1800;
        work->unkF0.vx = 0;
        work->unkF0.vz = 0;
        work->unkE4.vz = 0x2000;
        work->unkD8.vx = work->unkC.vx;
        work->unkD8.vy = work->unkC.vy;
        work->unkD8.vz = work->unkC.vz;
        work->unkD8.vy -= 0x5000;
        env->unk2C->unk0[3].vx = work->unkC.vx;
        env->unk2C->unk0[3].vy = work->unkC.vy;
        env->unk2C->unk0[3].vz = work->unkC.vz;
        FX0FB_W(work->unk5C, 0) = FX0FB_W(work->unkC, 0);
        FX0FB_W(work->unk5C, 1) = FX0FB_W(work->unkC, 1);
    } else {
        a8 = 0;
        ac = 0;
        b0 = 0;
        b4 = 0;
        b8 = 0;
        bc = 0;
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            a8 = (frame << 12) / 30;
            work->unk5C.vy = work->unkC.vy - ((a8 << 9) >> 12);
            env->unk2C->unk0[3].vx = work->unk5C.vx;
            env->unk2C->unk0[3].vy = work->unk5C.vy;
            env->unk2C->unk0[3].vz = work->unk5C.vz;
            env->unk2C->unk0[2].vx = env->unk2C->unk54.vx;
            env->unk2C->unk0[2].vy = env->unk2C->unk54.vy;
            env->unk2C->unk0[2].vz = env->unk2C->unk54.vz;
            work->unk64.vx = env->unk2C->unk54.vx;
            work->unk64.vy = env->unk2C->unk54.vy;
            work->unk64.vz = env->unk2C->unk54.vz;
            if (frame >= 0xE) {
                ex->func_800D0B18((s32)(env->unk20 + 2), 0xF0 - (((frame - 0xE) * 0xE600) >> 12), 8, 2, 0x78, 1,
                                  D_801EB34C, 0x20, work->unk4 << 7, 0x100, 0x400);
            }
            if (frame == 0x18) {
                work->unk24.vx = D_801EB248.vx;
                work->unk24.vy = D_801EB248.vy;
                work->unk24.vz = D_801EB248.vz;
                work->unk24.vy += 0x640;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk24);
                if (p != NULL) {
                    p->unk22 = 0x20;
                }
            }
            if (frame >= 0x1E) {
                work->unk0 = 1;
                entry->unk0 = -1;
                for (i0 = 0; i0 < 27; i0++) {
                    work->unk6C[i0] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, D_801EB274[i0]),
                                                                0x9A, 0x3E00, 0, 0);
                }
                work->unk8 = 0;
                work->unk118 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x82), 0x96, 0x3D40, 0, 0);
                work->unk11C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x87), 0x9A, 0x3E00, 0, 0);
                work->unk120 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x84), 0x7C, 0x3D02, 0, 0);
                work->unk124 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x94), 0xBD, 0x3E40, 0, 0);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk1C);
                if (p != NULL) {
                    p->unk22 = 0x180;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x200;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk1C);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
                frame = 0;
            }
            break;
        case 1:
            t = (frame << 12) / 55;
            if (frame < 8) {
                a8 = ex->math_cos((frame << 10) >> 3);
            }
            ac = ex->math_sin(t >> 2);
            b0 = 0x1000;
            b4 = 0x78;
            if (frame < 0x19) {
                ex->func_800D0B18((s32)(env->unk20 + 2), ((((frame << 12) / 24) * 0x118) >> 12) + 0xA, 8, 2, 0x8C, 1,
                                  D_801EB34C, 0x20, work->unk4 << 7, 0x100, 0x400);
            }
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0, 0x23, 0);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xFF, 0xFF, 0);
            } else if (frame < 0xC) {
                c_0 = 0xFF - ((frame - 1) * 0xFF) / 11;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c_0 >> 1, c_0, c_0);
            }
            work->unk5C.vy = work->unkC.vy + (-((0x1000 - ex->math_cos(t >> 1)) * 0x3000) >> 13) - 0x180;
            env->unk2C->unk0[3].vx = work->unk5C.vx;
            env->unk2C->unk0[3].vy = work->unk5C.vy;
            env->unk2C->unk0[3].vz = work->unk5C.vz;
            work->unk64.vy += frame * 10;
            work->unk64.vz -= frame * 9;
            env->unk2C->unk0[2].vx = work->unk64.vx;
            env->unk2C->unk0[2].vy = work->unk64.vy;
            env->unk2C->unk0[2].vz = work->unk64.vz;
            work->unk14.vy = work->unkC.vy + (-(t * 0x1800) >> 12) - 0x100;
            work->unk1C.vy = work->unkC.vy + (-(t << 13) >> 12) - 0x100;
            if (frame < 0x1E) {
                for (k = 0; frame < 0xD ? k < 2 : k < 1; k++) {
                    if (work->unk8 < 0x2F) {
                        r = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                        if (r != NULL) {
                            big = work->unk8 >= 25;
                            idx = D_801EB2AC[work->unk8 % 25];
                            r->unk4 = idx + big * 25;
                            if (big) {
                                r->unk20 = work->unk6C[(idx & 1) + 25];
                            } else {
                                r->unk20 = work->unk6C[idx];
                            }
                            r->unkC.vx = D_801EB248.vx;
                            r->unkC.vy = D_801EB248.vy;
                            r->unkC.vz = D_801EB248.vz;
                            r->unkC.vx -= ((D_801EB2E0[idx * 2] << 2) * (big + 3)) >> 1;
                            if (!big) {
                                r->unkC.vy += 0x300;
                            } else {
                                r->unkC.vy = work->unk64.vy + 0x300;
                            }
                            r->unkC.vz -= ((D_801EB2E0[idx * 2 + 1] << 2) * (big + 3)) >> 1;
                            work->unk8++;
                        }
                    }
                }
            }
            if (frame >= 0x37) {
                work->unk0 = 2;
                entry->unk0 = -1;
                D_801EB344 = 1;
                ex->Hi_SetSummonMotion(SUMMON0FB(env)->unk54->unk180[0], SUMMON0FB(env)->unk60);
                ex->Hi_SetSummonMotFrame(SUMMON0FB(env)->unk60, 0);
                work->unk44.vy = 0xA70;
                work->unk38.vx = 0x1000;
                work->unk38.vy = 0x1000;
                work->unk38.vz = 0x1000;
                work->unk2C.vx = work->unkC.vx - 0x5780;
                work->unk2C.vz = work->unkC.vz - 0x3880;
                work->unk2C.vy = work->unkC.vy - 0x16DD;
                work->unk44.vx = 0;
                work->unk44.vz = 0;
                work->unk14.vx = work->unkC.vx;
                work->unk14.vz = work->unkC.vz;
                work->unk14.vy = work->unkC.vy - 0x2800;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                work->unk104.vx = 0x2000;
                work->unk104.vy = 0x555;
                work->unk104.vz = 0x2000;
                work->unk110.vx = 0;
                work->unk110.vy = 0;
                work->unk110.vz = 0;
                work->unkF8.vx = work->unkD8.vx + 0x2000;
                work->unkF8.vz = work->unkD8.vz + 0x2000;
                work->unkF8.vy = work->unkC.vy - 0x1500;
            }
            break;
        case 2:
            ac = 0x1000;
            b0 = 0x1000;
            b4 = 0x78;
            t = (frame << 12) / 40;
            bc = ex->math_sin(t >> 2);
            if (frame >= 0x20) {
                ex->Hi_DrawEffModel(&work->unk44, (VECTOR *)&work->unk2C, (s32)&work->unk38, work->unk11C);
                b8 = ((frame - 0x20) << 12) >> 3;
            }
            u_0 = t >> 1;
            work->unk5C.vx = work->unkC.vx;
            work->unk5C.vz = work->unkC.vz;
            work->unk5C.vy = work->unkC.vy - 0x2800;
            q2 = (b0 - ex->math_cos(u_0)) >> 1;
            c_1 = (ex->math_sin(u_0) * 3) / 2 + 0x800;
            work->unk64.vz = work->unkC.vz - q2 * 4 + 0x800;
            work->unk64.vx = work->unkC.vx - q2 * 5 - c_1;
            work->unk64.vy = work->unkC.vy + (q2 - 0x2800);
            env->unk2C->unk0[2].vx = work->unk64.vx;
            env->unk2C->unk0[2].vy = work->unk64.vy;
            env->unk2C->unk0[2].vz = work->unk64.vz;
            if (frame >= 0x28) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3: {
            SVECTOR pos;
            struct { s32 x; } z1 = { frame };

        {
            ac = 0x1000;
            b0 = 0x1000;
            bc = 0x1000;
            b4 = 0x78;
            b8 = 0x1000;
            y_1 = x_x0 = 0;
            if (frame < 4) {
                x_x0 = 0x400 - (frame << 10) / 3;
            } else if (frame < 0xB) {
                u_1 = ((frame - 3) << 10) >> 3;
                v_0 = ex->math_cos(u_1);
                y_1 = (v_0 >> 8) * ((frame & 1) * 2 - 1) + ((b8 - ex->math_sin(u_1)) >> 6);
            }
            if (frame >= 8) {
                vb_0 = 0x1000 - ex->math_cos((((frame - 8) << 12) / 17) >> 1);
                pos.vx = work->unk64.vx - (s16)((vb_0 >> 6) - 0x120);
                pos.vz = work->unk64.vz - (vb_0 >> 5);
                pos.vy = work->unk64.vy - (((vb_0 >> 1) * 0x302) >> 12);
                env->unk2C->unk0[2].vx = pos.vx;
                env->unk2C->unk0[2].vy = pos.vy;
                env->unk2C->unk0[2].vz = pos.vz;
            } else {
                pos.vx = work->unk64.vx + ((frame << 12) >> 7) + 0x20;
                pos.vz = work->unk64.vz;
                pos.vy = work->unk64.vy;
                env->unk2C->unk0[2].vx = pos.vx;
                env->unk2C->unk0[2].vy = pos.vy;
                env->unk2C->unk0[2].vz = pos.vz;
            }
            work->unk2C.vy = work->unk2C.vy + y_1 - x_x0;
            ex->Hi_DrawSummonModel(&work->unk44, (VECTOR *)&work->unk2C, (s32)&work->unk38, SUMMON0FB(env)->unk60, 1);
            work->unk2C.vy = work->unk2C.vy + (x_x0 + 0xF0);
            ex->Hi_DrawEffModel(&work->unk44, (VECTOR *)&work->unk2C, (s32)&work->unk38, work->unk11C);
            work->unk2C.vy = work->unk2C.vy - (y_1 + 0xF0);
            if (frame == 3) {
                ex->Hi_GetSummonBonePos(SUMMON0FB(env)->unk60, 0x5B, (u16 *)&work->unk1C);
                work->unk1C.vy += 0x10;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk1C);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
            }
            if (frame >= 0x19) {
                work->unk0 = 4;
                entry->unk0 = -1;
                ex->Hi_SetSummonMotion(SUMMON0FB(env)->unk54->unk180[1], SUMMON0FB(env)->unk60);
                ex->Hi_SetSummonMotFrame(SUMMON0FB(env)->unk60, 0);
                D_801EB348 = 1;
                ex->Hi_GetSummonBonePos(SUMMON0FB(env)->unk60, 0x24, (u16 *)&work->unk5C);
                env->unk2C->unk0[3].vx = work->unk5C.vx;
                env->unk2C->unk0[3].vy = work->unk5C.vy;
                env->unk2C->unk0[3].vz = work->unk5C.vz;
                work->unkD8.vz -= 0x5C00;
                work->unkD8.vx -= 0x5C00;
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk4C);
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk54);
            }
        }
        } break;
        case 4: {
            SVECTOR a;
            SVECTOR b;
            BtlBone m;
            BtlBone m2;
            struct { s32 x; } z0 = { frame };
            s32 c8;

        {
            t = (frame << 12) / 44;
            ex->Hi_DrawSummonModel(&work->unk44, (VECTOR *)&work->unk2C, (s32)&work->unk38, SUMMON0FB(env)->unk60, 1);
            ex->Hi_SetEffModelOffset(work->unk11C, 0x80);
            ex->Hi_DrawEffModel(&work->unk44, (VECTOR *)&work->unk2C, (s32)&work->unk38, work->unk11C);
            b8 = 0x1000;
            vb_1 = 0x1000 - ex->math_cos(t >> 1);
            a.vx = work->unk5C.vx;
            a.vy = work->unk5C.vy;
            a.vz = work->unk5C.vz;
            a.vy += (vb_1 << 6) >> 12;
            env->unk2C->unk0[3].vx = a.vx;
            b0 = 0x1000;
            env->unk2C->unk0[3].vy = a.vy;
            b4 = 0x84;
            env->unk2C->unk0[3].vz = a.vz;
            if (frame == 0) {
                for (i1 = 0; i1 < 4; i1++) {
                    ex->Hi_GetSummonBonePos(SUMMON0FB(env)->unk60, D_801EB250[i1][0], (u16 *)&a);
                    ex->Hi_GetSummonBonePos(SUMMON0FB(env)->unk60, D_801EB250[i1][1], (u16 *)&b);
                    func_801E7720(&a, &b, 0x3F3, &D_801EB358[i1 & 1], &D_801EB358[(i1 & 1) + 1], 0, D_801EB40C[i1].seg, 1);
                }
                for (i2 = 0; i2 < 10; i2++) {
                    ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->unk60, D_801EB260[i2], &m);
                    ex->func_800CFE88((MATRIX *)&m, 0x46, (s32)&a);
                    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 9), (s32)&a, 0, -1, 0x1000, 0x1000, 1,
                                        (u8 *)&D_801EB70C[i2], 2);
                }
            }
            for (i1 = 0; i1 < 4; i1++) {
                ex->Hi_GetSummonBonePos(SUMMON0FB(env)->unk60, D_801EB250[i1][0], (u16 *)&a);
                ex->Hi_GetSummonBonePos(SUMMON0FB(env)->unk60, D_801EB250[i1][1], (u16 *)&b);
                func_801E7720(&a, &b, 0xB, &D_801EB358[i1 & 1], &D_801EB358[(i1 & 1) + 1], 0x1000, D_801EB40C[i1].seg, 1);
            }
            if (frame >= 0x26) {
                c8 = ((((frame - 0x26) << 12) / 6) * 2) + 0x1000;
            } else {
                c8 = (t >> 1) + 0x300;
            }
            for (i1 = 0; i1 < 10; i1++) {
                Fx0fb0Trail16 *pp;

                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->unk60, D_801EB260[i1], &m2);
                ex->func_800CFE88((MATRIX *)&m2, 0x46, (s32)&a);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 9), (s32)&a, 0, 0, 0x800, 0x1000, 1,
                                    (u8 *)(pp = &D_801EB70C[i1]), 2);
                env->unk28 = 0x18;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), &a, NULL, c8 >> 1, 0, 1,
                                        ex->math_sin(t >> 2), 0, 0);
                env->unk28 = 0;
                j = 0;
                s_0 = (s16 *)pp + 3;
                for (j = 0; j < 16; j++) {
                    *s_0 -= 8 + (t >> 8);
                    s_0 += 4;
                }
            }
            ex->Hi_GetSummonBonePos(SUMMON0FB(env)->unk60, D_801EB260[0], (u16 *)&work->unk4C);
            ex->Hi_GetSummonBonePos(SUMMON0FB(env)->unk60, D_801EB260[5], (u16 *)&work->unk54);
            if (frame >= 0x2C) {
                work->unk0 = 5;
                entry->unk0 = -1;
            }
        }
        } break;
        case 5: {
            SVECTOR a;
            BtlBone m;
            BtlBone m2;

        {
            b0 = 0x1000;
            b8 = 0x1000;
            b4 = 0x78;
            t = (frame << 12) / 21;
            if (frame == 0x11) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0, 0, 0x40);
            } else if (frame >= 0x11) {
                c = ((frame - 0x11) << 7) / 3;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c >> 2, c >> 1, c);
            }
            if (frame >= 5) {
                ex->Hi_SetSummonMotFrame(SUMMON0FB(env)->unk60, 0x31);
            }
            ex->Hi_DrawSummonModel(&work->unk44, (VECTOR *)&work->unk2C, (s32)&work->unk38, SUMMON0FB(env)->unk60, 1);
            u = t * 3 + 0x1000;
            for (i11 = 0; i11 < 10; i11++) {
                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->unk60, D_801EB260[i11], &m);
                ex->func_800CFE88((MATRIX *)&m, 0x46, (s32)&a);
                env->unk28 = 0x18;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), &a, NULL, u, 0, 1,
                                        0x1000, 0, 0);
                env->unk28 = 0;
                if (i11 == 0) {
                    FX0FB_W(work->unk14, 0) = FX0FB_W(a, 0);
                    FX0FB_W(work->unk14, 1) = FX0FB_W(a, 1);
                }
                if (i11 == 5) {
                    FX0FB_W(work->unk1C, 0) = FX0FB_W(a, 0);
                    FX0FB_W(work->unk1C, 1) = FX0FB_W(a, 1);
                }
            }
            for (i11 = 0; i11 < 10; i11++) {
                Fx0fb0Trail16 *pp;

                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->unk60, D_801EB260[i11], &m2);
                ex->func_800CFE88((MATRIX *)&m2, 0x46, (s32)&a);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 9), (s32)&a, 0, 0, 0x800, 0x1000 - t, 1,
                                    (u8 *)(pp = &D_801EB70C[i11]), 2);
                j = 0;
                s_1 = (s16 *)pp + 3;
                for (j = 0; j < 16; j++) {
                    *s_1 -= 0x18;
                    s_1 += 4;
                }
            }
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x14), &work->unk14, NULL, (t >> 1) + 0xAAA, 0, 1,
                              t >> 1);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x14), &work->unk1C, NULL, (t >> 1) + 0xAAA, 0, 1,
                              t >> 1);
            if (frame == 4) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk1C);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            if (frame == 7) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk1C);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            if (frame >= 0x15) {
                work->unk0 = 6;
                entry->unk0 = -1;
                ex->Hi_SetSummonMotion(SUMMON0FB(env)->unk54->unk180[2], SUMMON0FB(env)->unk60);
            }
        }
        } break;
        case 6: {
            VECTOR pos;
            VECTOR sc;
            SVECTOR r1;
            SVECTOR r2;
            BtlBone m;
            BtlBone m2;

        {
            b4 = 0xF1;
            if (frame < 0xD) {
                ex->Hi_SetSummonMotFrame(SUMMON0FB(env)->unk60, frame);
            } else {
                ex->Hi_SetSummonMotFrame(SUMMON0FB(env)->unk60, 0xC);
            }
            ex->Hi_DrawSummonModel(&work->unk44, (VECTOR *)&work->unk2C, (s32)&work->unk38, SUMMON0FB(env)->unk60, 1);
            ex->Hi_DrawEffModel(&work->unk44, (VECTOR *)&work->unk2C, (s32)&work->unk38, work->unk11C);
            env->unk28 = 0x40;
            for (i3 = 0; i3 < 10; i3++) {
                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->unk60, D_801EB260[i3], (BtlBone *)&pos);
                ex->func_800CFE88((MATRIX *)&pos, 0x46, (s32)&D_801EB3BC[i3]);
            }
            for (i3 = 0; i3 < 2; i3++) {
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &D_801EB3C4[i3 * 5], NULL, 0x1000, 0, 1,
                                  0x1000);
            }
            env->unk28 = 0;
            if (frame >= 4) {
                w = 0;
                if (frame < 5) {
                    u_2 = frame << 12;
                    v_1 = u_2 >> 2;
                    sc.vx = (0x1000 - ex->math_cos(u_2 >> 4)) / 12;
                    sc.vy = (ex->math_cos(u_2 >> 4) >> 1) + 0xC00;
                    sc.vz = v_1 / 8;
                    x = 0;
                    if (frame == 4) {
                        ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0x80, 0x80, 0x80);
                    }
                } else {
                    u_3 = frame - 4;
                    if (frame < 8) {
                        c_1 = 0x80 - ((frame << 7) >> 3);
                        if (frame == 0) {
                            c_1 = 0xFF;
                        }
                        ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c_1, c_1, c_1);
                    }
                    if (u_3 < 0x20) {
                        x = (ex->math_sin((u_3 << 10) >> 5) >> 5) + 0x2E;
                    } else {
                        x = 0xAE;
                    }
                    if (u_3 < 8) {
                        v_1 = (u_3 << 12) >> 5;
                        sc.vx = ex->math_cos(v_1) + 0x155;
                        sc.vy = 0xC00 - v_1;
                        sc.vz = ((u_3 << 12) >> 6) + 0x200;
                    } else {
                        sc.vx = ((u_3 - 8) << 6) + 0x155;
                        sc.vy = ((u_3 - 8) << 5) + 0x800;
                        w = (u_3 - 8) << 5;
                        sc.vz = ((u_3 - 8) << 6) + 0x400;
                    }
                }
                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->unk60, 7, &m);
                ex->func_800CFF40((MATRIX *)&m, (s16 *)&r1);
                r1.vz = 0xA00;
                r1.vx -= 0x2F8;
                r1.vy += 0x300;
                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->unk60, 0x31, &m2);
                ex->func_800CFF40((MATRIX *)&m2, (s16 *)&r2);
                r2.vz = 0x600;
                r2.vx -= 0x2F8;
                r2.vy -= 0x300;
                for (i3 = 9; i3 >= 0; i3--) {
                    pos.vx = D_801EB3BC[i3].vx;
                    pos.vy = D_801EB3BC[i3].vy;
                    pos.vz = D_801EB3BC[i3].vz;
                    if (i3 < 5) {
                        r2.vx += x;
                        sc.vx += w;
                        rp = &r2;
                    } else {
                        r1.vx += x;
                        sc.vx += w;
                        rp = &r1;
                    }
                    ex->Hi_DrawEffModel(rp, &pos, (s32)&sc, work->unk124);
                    if (i3 == 4) {
                        sc.vx -= w * 5;
                    }
                }
            }
            if (frame == 0) {
                work->unk5C.vy += 0x20;
                env->unk2C->unk0[3].vx = work->unk5C.vx;
                env->unk2C->unk0[3].vy = work->unk5C.vy;
                env->unk2C->unk0[3].vz = work->unk5C.vz;
            }
            if (frame == 4) {
                for (i3 = 0; i3 < 2; i3++) {
                    p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&D_801EB3C4[i3 * 5]);
                    if (p != NULL) {
                        p->unk22 = 0x100;
                    }
                }
            }
            if (frame < 2) {
                c_4 = 0xFF - frame * 0xFF / 2;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c_4, c_4, c_4 >> 1);
            }
            if (frame >= 0x1C) {
                c_5 = (frame - 0x1B) * 0x33;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c_5, c_5, c_5 >> 1);
            }
            if (frame >= 0x20) {
                for (i11 = 0; i11 < 27; i11++) {
                    ex->Hi_FreeEffModel(work->unk6C[i11]);
                }
                return 1;
            }
        }
        } break;
        }
        {
        SVECTOR pos;

        if (ac != 0) {
            pos.vx = D_801EB248.vx;
            u_4 = ac >> 5;
            pos.vy = D_801EB248.vy;
            pos.vz = D_801EB248.vz;
            pos.vy -= 0x79C0;
            pos.vx += 0xA00;
            pos.vz -= 0x1400;
            func_801E809C(&pos, 0x4000, u_4, 0x118, 0xFFFF, 0x10, 0x90, 0x7D0, work->unk4 * 0x5C, 0xF0, 1,
                          work->unk4 * 0x37);
            pos.vy += 0x640;
            pos.vx -= 0x1400;
            pos.vz += 0x2800;
            func_801E809C(&pos, 0x4000, u_4, 0x118, 0xFFFF, 0x10, 0x90, 0x76C, work->unk4 * 0x5C + 0x400, 0xF0, 1,
                          work->unk4 * 0x37 - 0x200);
        }
        if (a8 != 0) {
            SVECTOR rot;
            VECTOR sc;

            env->unk28 = -0x300;
            rot.vx = 0x400;
            sc.vx = 0x1000;
            rot.vz = 0x400;
            sc.vy = 0x1000;
            sc.vz = 0x1000;
            rot.vy = 0;
            pos.vz = (0x1000 - a8) * 2;
            pos.vx = D_801EB248.vx;
            pos.vy = D_801EB248.vy;
            func_801E7B90(&pos, &rot, &sc, ex->math_sin(a8 >> 2) >> 5, 0x118, 0xFFFF, 0x10, 0x90, 0x400, a8, 0x180, 1);
            env->unk28 = 0;
        }
        if (ac != 0 || a8 != 0) {
            if (work->unk0 == 0 || (work->unk0 == 1 && frame < 0x10)) {
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xD), work->unk4);
            } else {
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0xE), work->unk4);
            }
        }
        if (b0 != 0) {
            work->unkF0.vz += 7;
            ex->Hi_DrawEffModel(&work->unkF0, (VECTOR *)&work->unkD8, (s32)&work->unkE4, work->unk118);
            if (b0 != 0x1000) {
                c_6 = (b0 >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk118, c_6, c_6, c_6);
            }
            if (bc != 0) {
                work->unk110.vy += 4;
                ex->Hi_DrawEffModel(&work->unk110, (VECTOR *)&work->unkF8, (s32)&work->unk104, work->unk120);
                if (bc != 0x1000) {
                    c_7 = (bc >> 5) - 0x80;
                    ex->Hi_ModifyEffModelRGB(work->unk120, c_7, c_7, c_7);
                }
            }
        }
        if (b8 != 0) {
            u_3 = (work->unk4 * 16) % 240;
            ex->func_800D1BC4(8, (u8 *)ex->btlfx_getFileData(entry->unk8, 3), 0xA0 - u_3, 0xA0, 0, 3, b8);
            ex->func_800D1BC4(8, (u8 *)ex->btlfx_getFileData(entry->unk8, 3), 0x280 - u_3, 0xA0, 0, 3, b8);
        }
        if (b4 != 0) {
            CVECTOR col;
            SVECTOR rot;
            VECTOR sc;

            col.r = col.g = col.b = b4;
            sc.vx = 0x1060;
            sc.vy = 0x1060;
            sc.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->func_800E6764(0x400, 0xA0, 0x70, 0x4D, 0x37, 4, 4, 2, 2, &rot, &sc, &col, 0, 0);
        }
        }
        work->unk4++;
    }
    return 0;
}
