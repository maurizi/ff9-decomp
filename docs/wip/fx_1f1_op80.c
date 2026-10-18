/* Draft of fx_1f1 fx_1f1_op80. Status: 171 strict mismatches, mostly register allocation; structure follows the
 * original. */
#include "fxmod/fx_1f1.h"

s32 fx_1f1_op80(s32 mode, s32 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx1f1Unit *u;
    Pool6CEntry *p;
    SVECTOR v;
    s32 frame;
    s32 i;
    s32 c;

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
            D_801E7F14[i].unk20.vx = 0;
            D_801E7F14[i].unk20.vy = 0;
            D_801E7F14[i].unk20.vz = 0;
            D_801E7F14[i].unk28.vx = 0;
            D_801E7F14[i].unk28.vy = 0;
            D_801E7F14[i].unk28.vz = 0;
            D_801E7F14[i].unk30.vx = 0x1100;
            D_801E7F14[i].unk30.vy = 0x1000;
            D_801E7F14[i].unk30.vz = 0x1100;
            D_801E7F14[i].unk40.vx = 0x1000;
            D_801E7F14[i].unk40.vy = 0x1000;
            D_801E7F14[i].unk40.vz = 0x1000;
            u->unk50.vy = -0x800;
            u->unk50.vx = u->unkC.vx;
            u->unk50.vz = u->unkC.vz;
            u->unk4 = 0x20;
            u->unk14 = 0;
            u->unk0 = D_801E7E9C[i];
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
                if (u->unk0 == 0) {
                    if (u->unk4 != 0) {
                        u->unk4--;
                        if (u->unk4 == 0x1F) {
                            ex->btlfx_getUnitPos(i, (s16 *)&u->unkC);
                            u->unk8 = u->unkC.vy;
                            u->unkC.vy = 0;
                            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&u->unkC);
                            if (p != NULL) {
                                p->unk22 = ex->func_800E7CC4(i, 0x100);
                            }
                            ex->func_800E9930(6, entry->unk8, -1);
                            ex->func_800E9930(7, entry->unk8, -1);
                            ex->func_800E9930(8, entry->unk8, -1);
                        }
                        ex->Hi_DrawEffModel(&D_801E7F14[i].unk20, &D_801E7F14[i].unk50, (s32)&D_801E7F14[i].unk30,
                                            u->unk18);
                        ex->Hi_DrawEffModel(&D_801E7F14[i].unk28, &D_801E7F14[i].unk50, (s32)&D_801E7F14[i].unk40,
                                            u->unk1C);
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
                            u->unk50.vy = func_801E7704(1, 0x10, u->unk4, 0, -0x800);
                            v.vy = u->unk50.vy;
                            v.vx = u->unk50.vx;
                            v.vz = u->unk50.vz;
                            ex->btlfx_setUnitPos(i, (s16 *)&v);
                        }
                        if (u->unk4 == 0) {
                            u->unkC.vy = 0x3000;
                            ex->btlfx_setUnitPos(i, (s16 *)&u->unkC);
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
