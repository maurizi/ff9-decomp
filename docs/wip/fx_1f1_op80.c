/* Draft of fx_1f1 fx_1f1_op80. Status: 42 aligned mismatches, 20 ignoring registers (was 263/45). The vectors at
 * 0x20 are a nested Fx1f1Model (rotations, scales, position), as the original's mode-2 addressing shows
 * ((D_801E7F14 + 0x20 + off) + i * 0x60). Levers: mode 1 loads D_801E7E9C[i] into k before the u->unk4/unk14
 * stores (found by the permuter: it lets sched hoist the four setVector addresses); mode 2 has m = &D[i].unk20 at
 * the top of the loop body, &D[i].unkC everywhere (one reduced giv, s5), v.vy = u->...vy = func_801E7704(...).
 * Open: the original reduces m = &D[i].unk20 into its own register (s6, initialised to D_801E7F34) and computes
 * the unk50/unk30 draw arguments as i * 0x60 + (R + 0x30 / 0x10) with R = D + 0x20 hoisted and rematerialised
 * (lui t0 each time); here R is not hoisted ("not desirable", life 2), so m is no giv and is recomputed. A sweep
 * over explicit &D[i].unk20.x vs m->x for the six draw arguments (build/p6/f1/combo.py) did not find it. */
#include "btlfx_mod.h"

typedef struct {
    SVECTOR unk0; /* model rotations */
    SVECTOR unk8;
    VECTOR unk10; /* model scales */
    VECTOR unk20;
    VECTOR unk30; /* model position */
} Fx1f1Model; /* 0x40 */

typedef struct {
    s32 unk0; /* delay */
    s32 unk4; /* countdown */
    s32 unk8; /* saved height */
    SVECTOR unkC; /* unit position */
    s32 unk14;
    s32 unk18; /* models */
    s32 unk1C;
    Fx1f1Model unk20;
} Fx1f1Unit; /* 0x60 */

extern s32 D_801E7E9C[];
extern Unk800F78A4 *D_801E7F04;
extern Fx1f1Unit D_801E7F14[];
extern BtlModel D_801E8214[];

s32 func_801E7704(s32 curve, s32 n, s32 t, s32 from, s32 to);

s32 fx_1f1_op80(s32 mode, s32 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx1f1Unit *u;
    Pool6CEntry *p;
    SVECTOR v;
    s32 frame;
    s32 i;
    s32 c;
    s32 k;
    Fx1f1Model *m;

    if (mode == 0) {
        *arg4 = 0x40;
    } else if (mode == 1) {
        D_801E7F04 = env;
        ex->func_800E8274(0x10);
        ex->btlfx_initEffModels(D_801E8214, 8);
        for (i = 15; i >= 0; i--) {
            work[i] = 0;
        }
        for (i = 0; i < env->unkC->unk24; i++) {
            u = &D_801E7F14[i];
            u->unk18 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 2), 1);
            u->unk1C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 4), 1);
            ex->btlfx_getUnitPos(i, (s16 *)&D_801E7F14[i].unkC);
            setVector(&D_801E7F14[i].unk20.unk0, 0, 0, 0);
            setVector(&D_801E7F14[i].unk20.unk8, 0, 0, 0);
            setVector(&D_801E7F14[i].unk20.unk10, 0x1100, 0x1000, 0x1100);
            setVector(&D_801E7F14[i].unk20.unk20, 0x1000, 0x1000, 0x1000);
            setVector(&D_801E7F14[i].unk20.unk30, u->unkC.vx, -0x800, u->unkC.vz);
            k = D_801E7E9C[i];
            u->unk4 = 0x20;
            u->unk14 = 0;
            u->unk0 = k;
        }
    } else {
        frame = entry->unk0;
        if (frame == 0) {
            work[0] = 0x50;
        }
        if (work[0] != 0) {
            work[0]--;
            for (i = 0; i < env->unkC->unk24; i++) {
                u = &D_801E7F14[i];
                m = &D_801E7F14[i].unk20;
                if (u->unk0 == 0) {
                    if (u->unk4 != 0) {
                        u->unk4--;
                        if (u->unk4 == 0x1F) {
                            ex->btlfx_getUnitPos(i, (s16 *)&D_801E7F14[i].unkC);
                            u->unk8 = u->unkC.vy;
                            u->unkC.vy = 0;
                            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&D_801E7F14[i].unkC);
                            if (p != NULL) {
                                p->unk22 = ex->func_800E7CC4(i, 0x100);
                            }
                            ex->func_800E9930(6, entry->unk8, -1);
                            ex->func_800E9930(7, entry->unk8, -1);
                            ex->func_800E9930(8, entry->unk8, -1);
                        }
                        ex->Hi_DrawEffModel(&D_801E7F14[i].unk20.unk0, &D_801E7F14[i].unk20.unk30, (s32)&D_801E7F14[i].unk20.unk10, u->unk18);
                        ex->Hi_DrawEffModel(&D_801E7F14[i].unk20.unk8, &D_801E7F14[i].unk20.unk30, (s32)&m->unk20, u->unk1C);
                        if (u->unk4 >= 0x10) {
                            c = (0x10 - u->unk4) * 0x10;
                            ex->Hi_ModifyEffModelRGB(u->unk18, c, c, c);
                            ex->Hi_ModifyEffModelRGB(u->unk1C, c, c, c);
                            u->unk20.unk30.vy = func_801E7704(2, 0x10, u->unk4 - 0x10, -0x800, 0);
                            v.vy = func_801E7704(2, 0x10, u->unk4 - 0x10, u->unk8, 0);
                            v.vx = u->unkC.vx;
                            v.vz = u->unkC.vz;
                            ex->btlfx_setUnitPos(i, (s16 *)&v);
                        }
                        if (u->unk4 < 0x10) {
                            c = (u->unk4 - 0x10) * 0x10;
                            ex->Hi_ModifyEffModelRGB(u->unk18, c, c, c);
                            ex->Hi_ModifyEffModelRGB(u->unk1C, c, c, c);
                            ex->func_800E806C(i, u->unk4 * 8);
                            v.vy = u->unk20.unk30.vy = func_801E7704(1, 0x10, u->unk4, 0, -0x800);
                            v.vx = u->unk20.unk30.vx;
                            v.vz = u->unk20.unk30.vz;
                            ex->btlfx_setUnitPos(i, (s16 *)&v);
                        }
                        if (u->unk4 == 0) {
                            u->unkC.vy = 0x3000;
                            ex->btlfx_setUnitPos(i, (s16 *)&D_801E7F14[i].unkC);
                            ex->func_800E806C(i, 0x80);
                            ex->func_800E8274(i);
                        }
                        u->unk20.unk8.vy += 0x13;
                    }
                } else {
                    u->unk0--;
                }
            }
        }
        ex->func_800E6624(frame * 8, 0x1C0, 0x100, 0x20, 0x80, 0x1E0, 0x100);
        ex->func_800E6624(frame * 4, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
        return frame >= 0x50;
    }
    return 0;
}
