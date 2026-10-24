#include "fxmod/fx_1af.h"

s32 fx_1af_op80(s32 mode, Fx1afWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    VECTOR pos;
    Pool6CEntry *p;
    s32 frame;
    s32 state;
    s32 t;
    s32 s;
    s32 i;
    s32 j;
    s32 y;

    if (mode == 0) {
        *arg4 = 0x54;
    } else if (mode == 1) {
        D_801E7F48 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&v);
        work->unk8[0] = v.vx;
        work->unk8[1] = v.vy;
        work->unk8[2] = v.vz;
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk14);
        ex->btlfx_getUnitScale(0x10, (VECTOR *)work->unk1C);
        D_801E7F4C = entry;
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk28.vx = work->unk8[0];
        work->unk28.vy = work->unk8[1];
        work->unk28.vz = work->unk8[2];
        work->unk28.vy = 0;
        work->unk14.vy += 0x800;
        work->unk38.vx = work->unk28.vx;
        work->unk38.vy = work->unk28.vy;
        work->unk38.vz = work->unk28.vz;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            if (frame > 0) {
                work->unk0 = 1;
                entry->unk0 = -1;
                ex->btlfx_fadeUnitById(0x10, 0);
                ex->func_800E87C4(0x50);
                ex->btlfx_initEffModels(D_801E7F58, 5);
                for (i = 0; i < 5; i++) {
                    work->unk40[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, D_801E7EF4[i]), 0);
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x80, 0x80, 0xAA);
            }
            break;
        case 1:
            t = (frame << 12) / 180;
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x40, 0x40, 0x55);
            }
            if (frame == 1) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x2A, 0x2A, 0x38);
            }
            if (frame == 0x14) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x2A, 0x2A, 0x38);
            }
            if (frame == 0x28) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x2A, 0x2A, 0x38);
            }
            if (frame >= 0) {
                y = t;
                if (frame < 4) {
                    work->unk14.vz += 0xC;
                    work->unk38.vx += 0xE;
                    work->unk28.vx += 0xC;
                } else {
                    work->unk14.vz += 2;
                    work->unk38.vx += 8;
                    work->unk28.vx += 6;
                }
                pos.vx = work->unk8[0];
                pos.vy = work->unk8[1];
                pos.vz = work->unk8[2];
                s = t >> 2;
                pos.vy = y;
                pos.vx += ex->btlfx_randCentered(ex->math_sin(s) >> 6);
                pos.vz += ex->btlfx_randCentered(ex->math_sin(s) >> 6);
                work->unk30.vx = pos.vx;
                work->unk30.vy = pos.vy;
                work->unk30.vz = pos.vz;
                work->unk30.vx += t >> 4;
                for (j = 0; j < 5; j++) {
                    ex->Hi_SetEffModelSlice(work->unk40[j], y);
                    ex->Hi_DrawSliceEffModel(&work->unk14, &pos, (s32)work->unk1C, work->unk40[j]);
                }
            }
            if (frame == 0xA) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk38);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x1E) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x45) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk22 = 0x140;
                }
            }
            if (frame == 0x4B) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk28);
                if (p != NULL) {
                    p->unk14 = (s32)D_801E8348;
                    p->unk12 = 0x2C;
                    p->unkC = 0x4C;
                    p->unk22 = 0x140;
                }
            }
            if (frame >= 0xB4) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->unk4++;
    }
    return 0;
}

s32 fx_1af_op83(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E7F48 = env;
        ex->btlfx_clearUnitStatus(0x10);
    } else {
        return 1;
    }
    return 0;
}

s32 fx_1af_op87(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801E7F48 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&v);
        env->camera->points[3].vx = v.vx;
        env->camera->points[3].vy = v.vy;
        env->camera->points[3].vz = v.vz;
        return 1;
    } else {
        return 0;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1af/nonmatchings/fx_1af", fx_1af_headerPtr);
