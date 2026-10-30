#include "fxmod/fx_1f1.h"

s32 func_801E7704(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7F04);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

s32 fx_1f1_op80(s32 mode, s32 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx1f1Unit *u;
    Pool6CEntry *p;
    SVECTOR v;
    s32 frame;
    s32 i;
    s32 c;
    s32 k;
    Fx1f1Model *mm;
    VECTOR *q;
    VECTOR *m;

    if (mode == 0) {
        *arg4 = 0x40;
    } else if (mode == 1) {
        D_801E7F04 = env;
        ex->btlfx_clearUnitStatus(0x10);
        ex->btlfx_initEffModels(D_801E8214, 8);
        for (i = 15; i >= 0; i--) {
            work[i] = 0;
        }
        for (i = 0; i < env->targets->targetCount; i++) {
            u = &D_801E7F14[i];
            u->unk18 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 2), 1);
            u->unk1C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 4), 1);
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
        frame = entry->frame;
        if (frame == 0) {
            work[0] = 0x50;
        }
        if (work[0] != 0) {
            work[0]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                u = &D_801E7F14[i];
                if (u->unk0 == 0) {
                    if (u->unk4 != 0) {
                        u->unk4--;
                        if (u->unk4 == 0x1F) {
                            ex->btlfx_getUnitPos(i, (s16 *)&D_801E7F14[i].unkC);
                            u->unk8 = u->unkC.vy;
                            u->unkC.vy = 0;
                            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&D_801E7F14[i].unkC);
                            if (p != NULL) {
                                p->otZ = ex->btlfx_addRadiusSixth(i, 0x100);
                            }
                            ex->btlfx_playFileSound(6, entry->file, -1);
                            ex->btlfx_playFileSound(7, entry->file, -1);
                            ex->btlfx_playFileSound(8, entry->file, -1);
                        }
                        /* mm holds the base so q and m are computed from it (base + 0x30, + 0x10) while its last
                         * use is the &mm[i] giv: loop.c then hoists the base without dragging + 0x10 along */
                        mm = D_801E7F34;
                        q = &D_801E7F34[i].unk30;
                        m = D_801E7F34[i].unk10;
                        ex->Hi_DrawEffModel(mm[i].unk0, q, (s32)m, u->unk18);
                        ex->Hi_DrawEffModel(mm[i].unk0 + 1, q, (s32)(m + 1), u->unk1C);
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
                            ex->btlfx_fadeUnitById(i, u->unk4 * 8);
                            v.vy = u->unk50.vy = func_801E7704(1, 0x10, u->unk4, 0, -0x800);
                            v.vx = u->unk50.vx;
                            v.vz = u->unk50.vz;
                            ex->btlfx_setUnitPos(i, (s16 *)&v);
                        }
                        if (u->unk4 == 0) {
                            u->unkC.vy = 0x3000;
                            ex->btlfx_setUnitPos(i, (s16 *)&D_801E7F14[i].unkC);
                            ex->btlfx_fadeUnitById(i, 0x80);
                            ex->btlfx_clearUnitStatus(i);
                        }
                        u->unk28.vy += 0x13;
                    }
                } else {
                    u->unk0--;
                }
            }
        }
        ex->btlfx_drawScrollMove(frame * 8, 0x1C0, 0x100, 0x20, 0x80, 0x1E0, 0x100);
        ex->btlfx_drawScrollMove(frame * 4, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f1/nonmatchings/fx_1f1", fx_1f1_headerPtr);
