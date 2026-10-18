/* Draft of fx_1a8 op80 (summon ring: three draws per state plus the four func_801E7950 bands). Status: 10 aligned
 * mismatches (5 pairs), all in case 2's last model draw: the original loads 0x1000 into s0 for s4.vy and computes
 * a4 = s0 - t in place just before the func_800E6624 call, then (a4 >> 5) - 0x80 in place after the draw; here the
 * constant lands in a0 and the subtraction is scheduled right after the sin call. Tried: a4 = 0x1000 - t before the
 * call, a4 = 0x1000 / s4.vy = a4 / a4 -= t, reusing other case-2 temporaries for a4, every placement of the
 * subtraction; the permuter (40 min) found nothing. Levers that got it here: case 1 braced with its SVECTOR
 * declared at the case-block top (so the final ring VECTOR reuses offset 0 after the switch), separate temporaries
 * per case-2 block, A - (C + B) to get (A - C) - B past fold, (-x << 8) for negu-then-sll, block-local alpha/size
 * statement order, sin(q2 >> 2)/cos(q2 >> 2) instead of q2 >>= 2. The remaining rows are scheduling/regalloc only. */
#include "fxmod/fx_1a8.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx1a8Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    SVECTOR unk8;
    SVECTOR unk10;
    u8 pad18[0x20 - 0x18];
    SVECTOR unk20; /* ring position */
    s32 unk28; /* model */
    SVECTOR unk2C; /* rotation */
    Fx1a8Vec3 unk34; /* scale */
    Fx1a8Vec3 unk40; /* position */
    s32 unk4C[4]; /* models */
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s32 unk68;
    u8 pad6C[0x74 - 0x6C];
} Fx1a8Work80; /* 0x74 */

extern SVECTOR D_801E937C;
extern u32 D_801E93CC[];
extern BtlModel D_801E97DC[];
extern u8 D_801EA13C[];

s32 fx_1a8_op80(s32 mode, Fx1a8Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 alpha;
    s32 size;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x74;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E97CC = env;
        ex->btlfx_initEffModels(D_801E97DC, 0xC);
        work->unk28 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x19), 0);
        work->unk4C[0] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x10), 0);
        work->unk4C[1] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 7), 0);
        work->unk4C[2] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 8), 0);
        work->unk4C[3] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 9), 0);
        work->unk5C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x15), 2);
        work->unk64 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x11), 1);
        D_801E97D4[0] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xA), 0);
        D_801E97D4[1] = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 6), 0);
        work->unk60 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x16), 3);
        work->unk68 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x14), 1);
        work->unk8.vx = D_801E937C.vx;
        work->unk8.vy = D_801E937C.vy - 0x2000;
        work->unk8.vz = D_801E937C.vz - 0x2000;
        env->unk2C->unk0[3].vx = work->unk8.vx;
        env->unk2C->unk0[3].vy = work->unk8.vy;
        env->unk2C->unk0[3].vz = work->unk8.vz;
    } else {
        alpha = 0;
        size = 0;
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            t = (frame << 12) / 10;
            if (frame == 0) {
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk8);
            }
            if (frame < 0x10) {
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk8, NULL, 0, 0x555,
                                    ex->math_cos((frame << 8) >> 2), 1, D_801EA13C, 0);
            }
            if (frame == 0) {
                work->unk2C.vx = 0x600;
                work->unk2C.vy = 0;
                work->unk2C.vz = 0;
            }
            work->unk40.vx = work->unk8.vx;
            work->unk40.vy = work->unk8.vy;
            work->unk40.vz = work->unk8.vz;
            work->unk40.vy = work->unk40.vy - (ex->math_cos(t >> 2) - 0x1000);
            work->unk40.vz = work->unk40.vz - (ex->math_cos(t >> 2) - 0x1000);
            work->unk2C.vz += 0x80;
            work->unk34.vx = work->unk34.vy = work->unk34.vz = ((((0x1000 - ex->math_cos(t >> 2)) << 2) + 0x100) * 6) >> 2;
            ex->Hi_DrawEffModel(&work->unk2C, (VECTOR *)&work->unk40, (s32)&work->unk34, work->unk28);
            work->unk34.vx = work->unk34.vy = work->unk34.vz = work->unk34.vx / 3;
            ex->Hi_DrawEffModel(&work->unk2C, (VECTOR *)&work->unk40, (s32)&work->unk34, work->unk60);
            work->unk34.vx *= 2;
            work->unk34.vy = work->unk34.vz = work->unk34.vx;
            size = work->unk34.vx;
            alpha = 0x1000;
            work->unk20.vx = work->unk40.vx;
            work->unk20.vy = work->unk40.vy;
            work->unk20.vz = work->unk40.vz;
            if (frame >= 10) {
                work->unk0 = 1;
                entry->unk0 = -1;
                ex->func_800E6E1C(0x240, 0x1C0, 0x40, 0x40, D_801E93CC, D_801E937C.vx, D_801E937C.vz, 0x800);
            }
            break;
        case 1: {
            SVECTOR v;

            size = work->unk34.vx;
            work->unk40.vx = D_801E937C.vx;
            work->unk40.vy = D_801E937C.vy;
            alpha = 0x1000;
            work->unk40.vz = D_801E937C.vz;
            t = ((frame + 1) << 12) / 5;
            work->unk40.vy = work->unk40.vy + (t - 0x1000);
            work->unk40.vz = work->unk40.vz + (t - 0x1000);
            work->unk2C.vz += 0x80;
            work->unk20.vx = work->unk40.vx;
            work->unk20.vy = work->unk40.vy;
            work->unk20.vz = work->unk40.vz;
            if (frame == 0) {
                v.vx = work->unk40.vx;
                v.vy = work->unk40.vy;
                v.vz = work->unk40.vz;
                ex->btlfx_aimAngles((s16 *)&v, (s16 *)&D_801E937C, (s16 *)&work->unk2C);
                work->unk34.vx = work->unk34.vy = work->unk34.vz = 0x5000;
                work->unk2C.vx += 0x800;
                env->unk2C->unk0[3].vx = D_801E937C.vx;
                env->unk2C->unk0[3].vy = D_801E937C.vy;
                env->unk2C->unk0[3].vz = D_801E937C.vz;
                env->unk2C->unk0[3].vy -= 0x140;
            }
            ex->Hi_DrawEffModel(&work->unk2C, (VECTOR *)&work->unk40, (s32)&work->unk34, work->unk28);
            work->unk34.vx = work->unk34.vy = work->unk34.vz = work->unk34.vx / 2;
            ex->Hi_DrawEffModel(&work->unk2C, (VECTOR *)&work->unk40, (s32)&work->unk34, work->unk60);
            work->unk34.vx = work->unk34.vy = work->unk34.vz = work->unk34.vx * 2;
            if (frame >= 4) {
                Pool6CEntry *p;

                work->unk0 = 2;
                entry->unk0 = -1;
                ex->btlfx_drawScreenTint((u32 *)&env->unk20[1], 2, 0x46, 0x46, 0x46);
                BTLFX_COPY_SVEC(&work->unk10, &D_801E937C);
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk10);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x200;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x17), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk18[0].unk24 = (void (*)())func_801E7704;
                    p->unk18[1].unk24 = (void (*)())func_801E7828;
                }
            }
            break;
        }
        case 2: {
            SVECTOR r1;
            VECTOR s1;
            SVECTOR r2;
            VECTOR s2;
            SVECTOR r3;
            VECTOR s3;
            SVECTOR r4;
            VECTOR s4;
            SVECTOR r5;
            s32 d;
            s32 i;
            s32 at;
            s32 q1;
            s32 a1;
            s32 q2;
            s32 c2;
            s32 a2;
            s32 q3;
            s32 a3;
            s32 y3;
            s32 p3;
            s32 sv;
            s32 a4;

            t = frame << 7;
            if (frame < 8) {
                env->unk2C->unk0[3].vx = D_801E937C.vx;
                d = ((frame & 1) * 2 - 1) * (0x20 - frame * 4);
                env->unk2C->unk0[3].vy = D_801E937C.vy;
                env->unk2C->unk0[3].vz = D_801E937C.vz;
                env->unk2C->unk0[3].vy = env->unk2C->unk0[3].vy - (0x140 + d);
                env->unk2C->unk0[3].vx += d;
            }
            work->unk40.vx = D_801E937C.vx;
            work->unk40.vy = D_801E937C.vy;
            work->unk40.vz = D_801E937C.vz;
            if (frame < 6) {
                work->unk40.vy = (((frame & 1) * 2 - 1) * ex->math_cos((frame << 10) / 6)) >> 6;
            }
            ex->Hi_DrawEffModel(&work->unk2C, (VECTOR *)&work->unk40, (s32)&work->unk34, work->unk28);
            if (t > 0xC00) {
                s32 k;

                ex->Hi_ModifyEffModelAbr(work->unk28, 1);
                k = ((0x1000 - (t - 0xC00) * 4) >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk28, k, k, k);
            }
            if (frame < 0x10) {
                alpha = 0x1000 - (frame << 8);
                work->unk20.vx = work->unk40.vx;
                work->unk20.vy = work->unk40.vy;
                size = work->unk34.vx + t;
                work->unk20.vz = work->unk40.vz;
            }
            if (frame < 6) {
                at = 0xFF - (frame * 0xFF) / 6;
                ex->btlfx_drawScreenTint((u32 *)&env->unk20[1], 1, at, at >> 1, at >> 1);
            }
            if (frame < 0x10) {
                work->unk40.vx = D_801E937C.vx;
                work->unk40.vy = D_801E937C.vy;
                work->unk40.vz = D_801E937C.vz;
                s1.vx = s1.vz = 0x1800;
                r1.vx = 0;
                r1.vy = 0;
                r1.vz = 0;
                q1 = (frame << 12) / 12;
                s1.vy = q1 + 0x2000;
                a1 = (ex->math_cos(q1 >> 2) >> 5) - 0x80;
                for (i = 0; i < 4; i++) {
                    ex->Hi_DrawEffModel(&r1, (VECTOR *)&work->unk40, (s32)&s1, work->unk4C[i]);
                    ex->Hi_ModifyEffModelRGB(work->unk4C[i], a1, a1, a1);
                }
                ex->func_800E6624(work->unk4 << 4, 0x1C0, 0x100, 0x38, 0x70, 0x1C0, 0x170);
            }
            if (frame < 0xC) {
                work->unk40.vx = D_801E937C.vx;
                work->unk40.vy = D_801E937C.vy;
                work->unk40.vz = D_801E937C.vz;
                q2 = (frame << 12) / 12;
                s2.vx = s2.vz = q2 / 8 + 0x555;
                s2.vy = ex->math_sin(q2 >> 2);
                r2.vx = 0;
                r2.vy = 0;
                r2.vz = 0;
                c2 = ex->math_cos(q2 >> 2);
                ex->Hi_DrawEffModel(&r2, (VECTOR *)&work->unk40, (s32)&s2, work->unk64);
                a2 = (c2 >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk64, a2, a2, a2);
                s2.vy >>= 1;
                s2.vx = s2.vz = (s2.vx * 4) / 3;
                ex->Hi_DrawEffModel(&r2, (VECTOR *)&work->unk40, (s32)&s2, work->unk64);
                ex->Hi_ModifyEffModelRGB(work->unk64, a2, a2, a2);
            }
            if (frame < 0x14) {
                work->unk40.vx = D_801E937C.vx;
                work->unk40.vy = D_801E937C.vy;
                work->unk40.vz = D_801E937C.vz;
                q3 = (frame << 12) / 20;
                p3 = q3 >> 2;
                s3.vx = s3.vz = ex->math_sin(p3) / 2;
                a3 = ((0x1000 - q3) >> 5) - 0x80;
                s3.vy = 0x800 - p3;
                r3.vx = 0;
                r3.vy = frame * 0x18;
                r3.vz = 0;
                y3 = frame * 0x18;
                work->unk40.vy -= y3;
                for (i = 0; i < 4; i++) {
                    ex->Hi_DrawEffModel(&r3, (VECTOR *)&work->unk40, (s32)&s3, work->unk5C);
                    ex->Hi_ModifyEffModelRGB(work->unk5C, a3, a3, a3);
                    s3.vx = s3.vz = (s3.vx * 5) >> 2;
                    s3.vy = (s3.vy * 3) >> 2;
                    work->unk40.vy -= y3;
                    r3.vy = (r3.vy * 9) >> 3;
                }
            }
            work->unk40.vx = D_801E937C.vx;
            work->unk40.vy = D_801E937C.vy;
            work->unk40.vz = D_801E937C.vz;
            s4.vx = s4.vz = ex->math_sin(t >> 2) / 2 + 0x1800;
            s4.vy = 0x1000;
            r4.vx = 0;
            r4.vy = frame * 0x18;
            r4.vz = 0;
            ex->func_800E6624(10000 - (work->unk4 << 3), 0x240, 0x140, 0x20, 0x80, 0x260, 0x140);
            ex->Hi_DrawEffModel(&r4, (VECTOR *)&work->unk40, (s32)&s4, work->unk68);
            a4 = ((0x1000 - t) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unk68, a4, a4, a4);
            if (frame < 0x10) {
                r5.vx = 0;
                r5.vy = 0;
                r5.vz = 0;
                r5.pad = 0;
                env->unk28 = 0x200;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &work->unk10, &r5,
                                  ex->math_sin((frame << 8) >> 2), 0, 1, ex->math_cos((frame << 8) >> 2));
                env->unk28 = 0;
            }
            if (frame == 0xC) {
                Pool6CEntry *p;

                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x190;
                }
            }
            if (frame >= 0x20) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        }
        if (alpha != 0) {
            VECTOR sc;
            s32 c;

            sc.vx = sc.vy = sc.vz = size;
            c = alpha >> 5;
            func_801E7950(&work->unk20, &work->unk2C, &sc, 0x100, 0xA0, 0x200, 0x200, c, 0x19, 0x3D02, 0, 0,
                          work->unk4 << 8, 0xC00, 1, 1);
            func_801E7950(&work->unk20, &work->unk2C, &sc, 0x100, 0xA0, 0x100, 0x100, c, 0x19, 0x3D02, 0, 0,
                          (work->unk4 << 8) + 0x800, 0xC00, 1, 1);
            func_801E7950(&work->unk20, &work->unk2C, &sc, 0x100, 0xA0, 0x200, 0x200, c, 0x19, 0x3D02, 0, 0,
                          -work->unk4 << 8, 0xC00, 1, -1);
            func_801E7950(&work->unk20, &work->unk2C, &sc, 0x100, 0xA0, 0x100, 0x100, c, 0x19, 0x3D02, 0, 0,
                          (-work->unk4 << 8) - 0x800, 0xC00, 1, -1);
            ex->func_800E6624(10000 - (work->unk4 << 4), 0x240, 0x100, 0x20, 0x40, 0x260, 0x100);
        }
        work->unk4++;
    }
    return 0;
}
