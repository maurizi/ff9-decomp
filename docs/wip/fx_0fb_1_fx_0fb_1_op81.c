/* Draft of fx_0fb_1_op81 (module fx_0fb_1). Status: 206/274 aligned instructions with ccprobe; register allocation only.
 * Declarations it needs that are not yet in include/fxmod/fx_0fb_1.h are repeated below. */
#include "fxmod/fx_0fb_1.h"

void func_801EC738(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r, s32 p, CVECTOR *rgb, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 dang, s32 abr);

s32 fx_0fb_1_op81(s32 mode, Fx0fb1Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0fb1Vec3 sc;
    s32 i;
    s32 a;
    s32 r;

    if (mode == 0) {
        *arg4 = 0x1A8;
    } else if (mode == 1) {
        D_801F0D2C = env;
        work->unk1A0 = env->unkC->unk24;
        for (i = 0; i < work->unk1A0; i++) {
            if (ex->btlfx_isEnemyUnit(i) != 0) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk60[i]);
                if (work->unk1A0 >= 2) {
                    work->unk1A4 = ex->btlfx_randCentered(0x200);
                    a = (i << 12) / work->unk1A0 + 0x200;
                    r = ex->btlfx_randCentered(0x100) + 0x200;
                    setVector(&work->unkA0[i], (ex->math_cos(a) * r) >> 12, work->unk1A4 - 0x1000,
                              (ex->math_sin(a) * r) >> 12);
                } else {
                    work->unk1A4 = 0;
                    setVector(&work->unkA0[i], 0, -0x1000, 0);
                }
                ex->btlfx_setUnitPos(i, (s16 *)&work->unkA0[i]);
                ex->btlfx_getUnitRot(i, (s16 *)&work->unkE0[i]);
                BTLFX_COPY_SVEC(&work->unk160[i], &work->unkE0[i]);
                work->unk120[i].vx = ex->btlfx_randCentered(0x80);
                work->unk120[i].vy = ex->btlfx_randCentered(0x80);
                work->unk120[i].vz = ex->btlfx_randCentered(0x20);
                work->unk160[i].vx = ex->btlfx_rand();
                work->unk160[i].vy = ex->btlfx_rand();
                ex->btlfx_getUnitScale(i, (VECTOR *)&work->unk0[i]);
                sc.vx = work->unk0[i].vx / 2;
                sc.vy = work->unk0[i].vy / 2;
                sc.vz = work->unk0[i].vz / 2;
                ex->func_800E7ACC(i, (s32 *)&sc);
            }
        }
    } else {
        for (i = 0; i < work->unk1A0; i++) {
            if (ex->btlfx_isEnemyUnit(i) != 0) {
                work->unk160[i].vx += work->unk120[i].vx;
                work->unk160[i].vy += work->unk120[i].vy;
                work->unk160[i].vz += work->unk120[i].vz;
                ex->btlfx_setUnitRot(i, (s16 *)&work->unk160[i]);
                if (D_801F0D34 != 0x1000) {
                    ex->func_800E80CC(i, D_801F0D34 >> 5);
                }
            }
        }
        if (D_801F0D30 != 0) {
            for (i = 0; i < work->unk1A0; i++) {
                if (ex->btlfx_isEnemyUnit(i) != 0) {
                    ex->btlfx_setUnitRot(i, (s16 *)&work->unkE0[i]);
                    ex->btlfx_getUnitHomePos(i, (s16 *)&sc);
                    ex->btlfx_setUnitPos(i, (s16 *)&sc);
                    ex->func_800E7ACC(i, (s32 *)&work->unk0[i]);
                }
            }
            return 1;
        }
    }
    return 0;
}
