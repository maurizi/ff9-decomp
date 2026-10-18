/* Draft of fx_1f1 fx_1f1_op80. Status: 39 aligned mismatches, 9 ignoring registers (was 263/45).
 * Levers: mode 1 loads D_801E7E9C[i] into k before the u->unk4/unk14 stores (found by the permuter: it lets
 * sched hoist the four setVector addresses); the draw arguments go through D_801E7F34, the model part of the
 * records seen as its own array (its data label exists: the original computes (D_801E7F34 + 0x30) + i * 0x60);
 * rotations and scales as SVECTOR[2] / VECTOR[2] with the second draw's arguments written r + 1 and unk10 + 1
 * (the original computes them as m + 8 and unk30 + 0x10); v.vy = u->unk50.vy = func_801E7704(...).
 * Open: the original reduces r = &D_801E7F34[i] into its own register (s6, initialised to D_801E7F34) and keeps
 * D_801E7F34 in a spilled pseudo that reload rematerialises (lui t0 before each + 0x30 / + 0x10); here r is not
 * a giv (its D_801E7F34 load is "not desirable" for loop.c: savings 2, life 5) and is recomputed each pass, so
 * the registers shift (u/&u->unkC/i * 0x60 in s2/s5/s7 there). With r assigned right before the draws (instead of
 * at the top) everything but those registers matches (212/8). */
#include "fxmod/fx_1f1.h"

/* the same records seen from their model part at 0x20 */
typedef struct {
    SVECTOR unk0[2]; /* model rotations */
    VECTOR unk10[2]; /* model scales */
    VECTOR unk30; /* model position */
    u8 pad40[0x20];
} Fx1f1Model; /* 0x60 */

extern Fx1f1Model D_801E7F34[];

s32 fx_1f1_op80(s32 mode, s32 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx1f1Unit *u;
    Pool6CEntry *p;
    SVECTOR v;
    s32 frame;
    s32 i;
    s32 c;
    s32 k;
    SVECTOR *r;

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
            setVector(&D_801E7F14[i].unk20, 0, 0, 0);
            setVector(&D_801E7F14[i].unk28, 0, 0, 0);
            setVector(&D_801E7F14[i].unk30, 0x1100, 0x1000, 0x1100);
            setVector(&D_801E7F14[i].unk40, 0x1000, 0x1000, 0x1000);
            setVector(&D_801E7F14[i].unk50, u->unkC.vx, -0x800, u->unkC.vz);
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
                r = D_801E7F34[i].unk0;
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
                        ex->Hi_DrawEffModel(r, &D_801E7F34[i].unk30, (s32)D_801E7F34[i].unk10, u->unk18);
                        ex->Hi_DrawEffModel(r + 1, &D_801E7F34[i].unk30, (s32)(D_801E7F34[i].unk10 + 1), u->unk1C);
                        if (u->unk4 >= 0x10) {
                            c = (0x10 - u->unk4) * 0x10;
                            ex->Hi_ModifyEffModelRGB(u->unk18, c, c, c);
                            ex->Hi_ModifyEffModelRGB(u->unk1C, c, c, c);
                            u->unk50.vy = func_801E7704(2, 0x10, u->unk4 - 0x10, -0x800, 0);
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
                            v.vy = u->unk50.vy = func_801E7704(1, 0x10, u->unk4, 0, -0x800);
                            v.vx = u->unk50.vx;
                            v.vz = u->unk50.vz;
                            ex->btlfx_setUnitPos(i, (s16 *)&v);
                        }
                        if (u->unk4 == 0) {
                            u->unkC.vy = 0x3000;
                            ex->btlfx_setUnitPos(i, (s16 *)&D_801E7F14[i].unkC);
                            ex->func_800E806C(i, 0x80);
                            ex->func_800E8274(i);
                        }
                        u->unk28.vy += 0x13;
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
