/* Draft of fx_0e0 fx_0e0_op80. Status: 234 aligned mismatches (92 ignoring registers and spill offsets); equivalent
 * by construction, untested in the emulator. Pass 3 fixes: a dead-store-only if/else after the col/scl/xo = 0 inits
 * ends cse's block, so the per-case `xo = 0` stores survive (spill slot 0x7C as in the original); mode 1 stores all
 * three unk24 copies before the two `-=` (flow then keeps both stores and reload_cse turns the re-reads into the
 * original's register copies); case 8's spark loop counter is r1, not i (the original keeps case 6's counter in s3
 * and case 8's in s1, so they are different variables; frame size now matches); variable roles searched
 * (build/w/roles.py in the pass-3 worktree) and adjacent statement swaps. Open: case 6's counter i is spilled here
 * (s3 in the original; local-alloc gives s3/s4 to case 6 block-locals first), case 6/7 statement order.
 * Earlier: one temporary per case block; one t for the frame-derived angle of cases 6, 7, 8 and 10; the 0x248 call's
 * col/rot share stack space with case 6's position (union Fx0e0Tmp). */
#include "fxmod/fx_0e0.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0e0Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* position */
    SVECTOR unk10; /* target */
    SVECTOR unk18; /* camera target */
    u8 pad20[4];
    Fx0e0Vec3 unk24; /* model position */
    Fx0e0Vec3 unk30; /* model scale */
    SVECTOR unk3C; /* model rotation */
    Fx0e0Vec3 unk44; /* scale */
    SVECTOR unk50; /* rotation */
    u8 pad58[4];
    s32 unk5C; /* models */
    s32 unk60;
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    u8 pad78[0xC];
} Fx0e0Work80; /* 0x84 */

typedef struct {
    s16 unk0; /* state */
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
} Fx0e0Spark;

typedef union {
    VECTOR v;
    struct {
        CVECTOR col;
        s32 pad;
        SVECTOR rot;
    } s;
} Fx0e0Tmp;

extern SVECTOR D_801EA8B0;
extern CVECTOR D_801EA8C8[];
extern Fx0e0Bolt D_801EA928[];
extern BtlModel D_801EAB98[];
s32 func_801E8628(s32 mode, void *o, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 arg5, void *inst);

s32 fx_0e0_op80(s32 mode, Fx0e0Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0e0Tmp u;
    VECTOR sc;
    SVECTOR rot;
    SVECTOR from;
    SVECTOR to;
    Fx0e0Bolt *bolt;
    Fx0e0Spark *sp;
    s32 a1;
    s32 a2;
    s32 a3;
    s32 a4;
    s32 c1;
    s32 r1;
    s32 r2;
    s32 d1;
    s32 tc;
    s32 frame;
    s32 col;
    s32 scl;
    s32 xo;
    s32 t;
    s32 a;
    s32 c;
    s32 i;
    s32 r;
    s32 d;
    s32 state;
    s32 n;
    s32 one;

    if (mode == 0) {
        init->unk0 = 0x84;
        init->unk8 = 0x24;
        init->unk14 = (s32)func_801E8628;
        init->unk4 = 0x31;
    } else if (one = 1, mode == one) {
        work->unk0 = 6;
        work->unk4 = 0;
        D_801EA918 = env;
        ex->btlfx_initEffModels(D_801EAB98, 8);
        work->unk5C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 8), 0x98, 0x3D80, 0, 0);
        work->unk60 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 0x118, 0xFFFF, 0x40, 0);
        work->unk64 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x14), 0x118, 0xFFFF, 0x40, 0);
        work->unk68 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x11), 0x37, 0x3D07, 0, 0);
        work->unk6C = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x13));
        work->unk70 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x12), 0x37, 0x3D07, 0, 0);
        work->unk74 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x19), 0xBC, 0x3E00, 0x80, 0);
        D_801EA8B0.vz += 0x400;
        BTLFX_COPY_SVEC(&work->unk8, &D_801EA8B0);
        D_801EA8B0.vz -= 0x400;
        work->unk30.vy = work->unk30.vx = 0x2800;
        work->unk24.vx = work->unk8.vx;
        work->unk24.vy = work->unk8.vy;
        work->unk24.vz = work->unk8.vz;
        work->unk24.vy -= 0x3800;
        work->unk24.vz -= 0x1C00;
        work->unk30.vz = 0x1800;
        work->unk10.vx = work->unk24.vx;
        work->unk10.vy = work->unk24.vy;
        work->unk10.vz = work->unk24.vz;
        ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&work->unk10, (s16 *)&work->unk3C);
        BTLFX_COPY_SVEC(&work->unk50, &work->unk3C);
    } else {
        state = work->unk0;
        frame = entry->unk0;
        col = 0;
        scl = 0;
        xo = 0;
        if (mode) {
            a1 = 1;
            a2 = 1;
        } else {
            a1 = 2;
            a2 = 2;
        }
        switch (state) {
        case 6:
            t = (frame << 12) / 66;
            col = ((t << 5) >> 12) + 0x8D;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, (t * 0x58) >> 12, 0, 0);
            a1 = t >> 2;
            BTLFX_COPY_SVEC(&work->unk18, &work->unk8);
            work->unk18.vy -= 0x400 + (ex->math_sin(a1) >> 1);
            scl = 0x80;
            xo = 0;
            env->unk2C->unk0[3].vx = work->unk18.vx;
            env->unk2C->unk0[3].vy = work->unk18.vy;
            env->unk2C->unk0[3].vz = work->unk18.vz;
            work->unk3C.vz += 8;
            work->unk24.vy += 0x90;
            work->unk24.vz += 0x46;
            ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x1C), work->unk4);
            work->unk44.vx = work->unk44.vy = 0x3800;
            work->unk44.vz = 0x2000;
            work->unk24.vy -= 0x5DC;
            work->unk24.vz += 0x2EE;
            work->unk50.vz += 0x14;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (((ex->math_cos(a1) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(a1) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(a1) >> 1) + 0x800) >> 5) - 0x80);
            work->unk50.vz -= 0x200;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (((ex->math_cos(a1) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(a1) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(a1) >> 1) + 0x800) >> 5) - 0x80);
            work->unk44.vx = work->unk44.vy = 0x2000;
            work->unk50.vz += 0x200;
            work->unk24.vy += 0x5DC;
            work->unk24.vz -= 0x2EE;
            work->unk44.vz = 0x3000;
            work->unk24.vy -= 0x125C;
            work->unk24.vz -= 0x8FC;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
            c1 = (((t * 2) - 0x1000) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk6C, c1, c1, c1);
            work->unk24.vy += 0x125C;
            work->unk24.vz += 0x8FC;
            if (frame > 0x32) {
                n = 0;
                a = (frame - 0x32) << 8;
                sc.vx = sc.vy = (ex->math_sin(a >> 2) * 5) / 2 + 0x400;
                a2 = a >> 2;
                sc.vz = a + 0x1800;
                rot.vx = -0x400;
                rot.vy = 0;
                u.v.vx = D_801EA8B0.vx;
                rot.vz = frame * 0x8D;
                u.v.vy = D_801EA8B0.vy;
                u.v.vz = D_801EA8B0.vz;
                u.v.vy -= 0x100;
                do {
                    ex->Hi_DrawEffModel(&rot, &u.v, (s32)&sc, work->unk70);
                    ex->Hi_ModifyEffModelRGB(work->unk70, (ex->math_sin(a2) >> 5) - 0x80, (ex->math_sin(a2) >> 5) - 0x80,
                                             (ex->math_sin(a2) >> 5) - 0x80);
                    n++;
                    sc.vx = sc.vy = (sc.vx * 3) >> 2;
                    rot.vz += 0x400;
                    u.v.vy += 0x20;
                } while (n <= 0);
                sc.vx = sc.vz = (ex->math_sin(a >> 2) * 5) / 2 + 0x1000;
                sc.vy = ((a * 0x300) >> 12) + 0x800;
                rot.vx = 0;
                rot.vy = frame * 0x8D;
                rot.vz = 0;
                u.v.vx = D_801EA8B0.vx;
                u.v.vy = D_801EA8B0.vy;
                u.v.vz = D_801EA8B0.vz;
                ex->Hi_DrawEffModel(&rot, &u.v, (s32)&sc, work->unk74);
                a3 = a >> 1;
                ex->Hi_ModifyEffModelRGB(work->unk74, (ex->math_sin(a3) >> 5) - 0x80, (ex->math_sin(a3) >> 5) - 0x80,
                                         (ex->math_sin(a3) >> 5) - 0x80);
            }
            i = 0;
            from.vx = work->unk24.vx;
            from.vy = work->unk24.vy;
            from.vz = work->unk24.vz;
            from.vy += 0x1838;
            from.vz += 0xC1C;
            bolt = D_801EA928;
            do {
                i++;
                r2 = ex->btlfx_rand();
                d1 = ex->btlfx_randCentered(0x400) + 0xC00;
                ((s32 *)&to)[0] = work->unk24.vx;
                ((s32 *)&to)[1] = work->unk24.vy;
                ex->func_800D044C(&from, (s32)&to, 0x1000, (XY32 *)&work->unk3C, 0x3000, r2);
                func_801E8430(entry, bolt, &from, &to, 0x100, 0x200, r2, 0xC0, 0, d1, (ex->btlfx_rand() & 0x1F) + 0x58);
                bolt++;
            } while (i <= 0);
            if (frame >= 0x42) {
                work->unk0 = 7;
                entry->unk0 = -1;
                work->unk8.vx = work->unk24.vx;
                work->unk8.vy = work->unk24.vy;
                work->unk8.vz = work->unk24.vz;
                work->unk10.vx = work->unk24.vx;
                work->unk10.vy = work->unk24.vy;
                work->unk10.vz = work->unk24.vz;
            }
            break;
        case 7:
            xo = 0;
            t = frame << 8;
            a4 = t >> 2;
            col = 0xAD;
            scl = a4 + 0x80;
            D_801EA8C8[1].r = (ex->math_sin(a4) * 0xFF) >> 12;
            D_801EA8C8[1].g = (ex->math_sin(a4) * 0xFF) >> 12;
            D_801EA8C8[1].b = (u32)ex->math_sin(a4) >> 6;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x58, 0, 0);
            ex->func_800D0B18((s32)(env->unk20 + 0xFF0), 0x28, 2, 2, 0xC8, 0xFF, D_801EA8C8, 0, 0, 0, 0);
            if (frame > 8) {
                c = ((frame - 8) * 0xFF) / 8;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, c, c, c);
            }
            work->unk24.vy = work->unk8.vy + (t >> 6) + ((((frame & 1) * 2 - 1) * ex->math_cos(t >> 2)) >> 6);
            ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
            ex->Hi_ModifyEffModelRGB(work->unk64, ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80,
                                     ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80,
                                     ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x1C), work->unk4);
            work->unk44.vx = work->unk44.vy = 0x3800;
            work->unk44.vz = 0x2000;
            work->unk24.vy -= 0x5DC;
            work->unk24.vz += 0x2EE;
            work->unk50.vz += 8;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (ex->math_cos(t >> 2) >> 6) - 0x80, (ex->math_cos(t >> 2) >> 6) - 0x80,
                                     (ex->math_cos(t >> 2) >> 6) - 0x80);
            work->unk50.vz -= 0x200;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (ex->math_cos(t >> 2) >> 6) - 0x80, (ex->math_cos(t >> 2) >> 6) - 0x80,
                                     (ex->math_cos(t >> 2) >> 6) - 0x80);
            work->unk44.vx = work->unk44.vy = 0x2000;
            work->unk50.vz += 0x200;
            work->unk24.vy += 0x5DC;
            work->unk24.vz -= 0x2EE;
            work->unk44.vz = 0x3000;
            work->unk24.vy -= 0x1388;
            work->unk24.vz -= 0x9C4;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
            ex->Hi_ModifyEffModelRGB(work->unk6C, (ex->math_cos(t >> 2) >> 5) - 0x80, (ex->math_cos(t >> 2) >> 5) - 0x80,
                                     (ex->math_cos(t >> 2) >> 5) - 0x80);
            work->unk24.vy += 0x1388;
            work->unk24.vz += 0x9C4;
            env->unk28 = 0x898;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A), &D_801EA8B0, NULL, t * 3 + 0x1000,
                              frame, 1, ex->math_sin(t >> 2));
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), &D_801EA8B0, NULL, t * 2 + 0x4000, frame, 1,
                              ex->math_sin(t >> 2));
            env->unk28 = 0;
            if (frame >= 0x10) {
                work->unk0 = 8;
                entry->unk0 = -1;
                work->unk10.vx = work->unk18.vx;
                work->unk10.vy = work->unk18.vy;
                work->unk10.vz = work->unk18.vz;
                work->unk10.vx -= 0x2800;
                work->unk10.vy += 0xB86;
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x20), (s32)&work->unk10);
            }
            break;
        case 8:
            t = frame << 7;
            col = 0xBC;
            scl = 0x200;
            xo = 0;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 0xFF8), 0xFF, 0xFF, 0xFF, 0xC8);
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x40, 0, 0);
            if (frame >= 0x1A) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, ((frame - 0x1A) << 7) / 6, ((frame - 0x1A) << 7) / 6,
                                         ((frame - 0x1A) << 7) / 6);
            }
            if (frame >= 0xA) {
                for (r1 = 0; r1 < 3; r1++) {
                    sp = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (sp != NULL) {
                        sp->unkC = work->unk18.vx - ((t >> 4) + 0x1800) + ex->btlfx_randCentered(0x200);
                        sp->unkE = 0x6A4 - ((0x1000 - t) >> 1);
                        sp->unk10 = work->unk18.vz + ex->btlfx_randCentered(0x1C00);
                        sp->unk4 = ex->btlfx_randCentered(0x100);
                        sp->unk6 = -0x140 - (ex->btlfx_rand() & 0xFF);
                        sp->unk8 = ex->btlfx_randCentered(0x100);
                        sp->unk14 = ex->btlfx_rand();
                        sp->unk16 = ex->btlfx_rand();
                        sp->unk18 = ex->btlfx_rand();
                        sp->unk0 = 3;
                        sp->unkA = t / 5 + (s16)((ex->btlfx_rand() & 0x3FF) + 0x80);
                    }
                }
            }
            if (frame == 1) {
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x21), (s32)&work->unk10);
            }
            if (frame == 4) {
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x21), (s32)&work->unk10);
            }
            if (frame >= 0x20) {
                work->unk0 = 0xA;
                entry->unk0 = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0xFF, 0xFF, 0xFF);
                ex->btlfx_poolClear(entry->unk10);
                ex->func_800E3C58();
                work->unk18.vx = 0;
                work->unk18.vy = -0x200;
                work->unk18.vz = 0;
                env->unk2C->unk0[3].vx = work->unk18.vx;
                env->unk2C->unk0[3].vy = work->unk18.vy;
                env->unk2C->unk0[3].vz = work->unk18.vz;
            }
            break;
        case 10:
            t = frame << 8;
            tc = 0xF5 - ((t * 0xF5) >> 12);
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, tc, tc, tc);
            u.s.rot.vx = 0;
            u.s.rot.vy = 0;
            u.s.rot.vz = 0x20;
            sc.vy = sc.vx = 0x1100;
            sc.vz = 0x1000;
            tc = 0xA0 - ((frame << 15) >> 12);
            u.s.col.r = u.s.col.g = u.s.col.b = tc;
            ex->func_800E6764(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &u.s.rot, &sc, &u.s.col, one, NULL);
            if (frame >= 0x10) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (col != 0) {
            u.s.col.r = u.s.col.g = u.s.col.b = col;
            u.s.rot.vx = 0;
            u.s.rot.vy = 0;
            u.s.rot.vz = 0;
            sc.vy = sc.vx = scl + 0x1000;
            sc.vz = 0x1000;
            ex->func_800E6764(1, xo + 0xA0, 0x78, 0x50, 0x3C, 4, 4, 2, 2, &u.s.rot, &sc, &u.s.col, 0, NULL);
        }
        if ((u32)(work->unk0 - 3) < 5) {
            ex->func_800E6624((0x1F4 - work->unk4) * 8, 0x1E0, 0x100, 0x20, 0x80, 0x300, 0x100);
        }
        work->unk4++;
    }
    return 0;
}
