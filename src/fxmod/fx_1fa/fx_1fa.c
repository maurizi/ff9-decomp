#include "fxmod/fx_1fa.h"

s32 fx_1fa_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E80E4.vx = 0;
        D_801E80E4.vy = 0;
        D_801E80E4.vz = -0x618;
        D_801E80DC = env;
        ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1E), (s32)&D_801E80E4);
    } else {
        return entry->unk0 >= 0x40;
    }
    return 0;
}

#define LOAD(r, a, b) \
    D_801E811C[r][0].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, a); \
    D_801E811C[r][1].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, b); \
    D_801E811C[r][2].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, b); \
    D_801E811C[r][3].unk8 = (u8 *)ex->btlfx_getFileData(entry->unk8, b);

s32 fx_1fa_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx1faRing *e;
    Pool6CEntry *p;
    s32 frame;
    s32 i;
    s32 j;
    s32 n;
    s32 r;
    s32 s;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E80EC[0] = 0;
        D_801E80DC = env;
        LOAD(0, 3, 0x12)
        LOAD(1, 8, 0xF)
        LOAD(2, 0x15, 0x10)
        LOAD(3, 0x16, 0xD)
        LOAD(4, 0x18, 0xE)
        LOAD(5, 0x19, 0xB)
        LOAD(6, 0x1A, 0xC)
        LOAD(7, 0x1B, 9)
        LOAD(8, 0x1C, 0xA)
        LOAD(9, 1, 6)
        LOAD(10, 4, 7)
        LOAD(11, 0x11, 5)
        for (i = 0; i < 12; i++) {
            for (j = 0; j < 4; j++) {
                D_801E811C[i][j].unkC = i * -0x2A9 + j * 0xC0;
                D_801E811C[i][j].unk10 = j * 16 + 16;
                D_801E811C[i][j].unk0 = i + 4 - j;
                D_801E811C[i][j].unk4 = 8;
            }
        }
        D_801E80F4.vx = env->camera->unk0[3].vx;
        D_801E80F4.vy = env->camera->unk0[3].vy;
        D_801E80F4.vz = env->camera->unk0[3].vz;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            D_801E80EC[0] = 0x20;
            break;
        case 8:
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&D_801E80F4);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
            break;
        case 0x18:
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&D_801E80FC[i]);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&D_801E80FC[i]);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(i, 0x80);
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&D_801E80FC[i]);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(i, 0x80);
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x17), (s32)&D_801E80FC[i]);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(i, 0x80);
                }
            }
            break;
        }
        if (D_801E80EC[0] != 0) {
            D_801E80EC[0]--;
            env->unk28 = 0x200;
            for (i = 0; i < 12; i++) {
                for (j = 0; j < 4; j++) {
                    e = (Fx1faRing *)(i * 0x50 + j * 0x14 + (s32)D_801E811C);
                    if (e->unk0 != 0) {
                        e->unk0--;
                    } else if (e->unk4 != 0) {
                        e->unkC -= 0x40;
                        e->unk10 -= 2;
                        e->unk4--;
                        r = (e->unk10 << 8) + 0x4000;
                        n = e->unk4 << 9;
                        s = (ex->math_sin(e->unkC) * e->unk10) >> 12;
                        ex->btlfx_drawParticles(e->unk8, &D_801E80F4, NULL, r, 0, 1, n, s,
                                                (ex->math_cos(e->unkC) * e->unk10) >> 12);
                    }
                }
            }
            env->unk28 = 0;
        }
        return frame >= 0x60;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1fa/nonmatchings/fx_1fa", fx_1fa_headerPtr);
