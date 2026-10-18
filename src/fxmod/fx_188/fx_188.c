#include "fxmod/fx_188.h"

s32 func_801E7704(s32 mode, Fx188Elem *e, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ang;
    s32 t;

    if (mode == 1) {
        e->unk0 = 0;
        e->unk1C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&e->unkC);
        if (e->unk1C != NULL) {
            e->unk1C->unk22 = 0;
        }
        e->unk4 = (ex->btlfx_rand() & 0x1F) + 0x27;
        e->unk8 = (ex->btlfx_rand() & 0x1F) + 0x20;
    } else if (e->unk0 == 0) {
        e->unk4 += 0xC;
        if (e->unk4 > 0x140) {
            e->unk4 = 0x140;
        }
        e->unk8 += 0x1D;
        if (e->unk8 > 0x22C) {
            e->unk8 = 0x22D;
        }
        t = 0x1000;
        if (frame < 5) {
            t = frame << 10;
        }
        t = (t * D_801E86CC) >> 12;
        ex->btlfx_aimAngles((s16 *)&e->unkC, (s16 *)&e->unk20, (s16 *)&ang);
        ex->btlfx_turnToward((s16 *)&ang, (s16 *)&e->unk14, e->unk8);
        ex->func_800CFD78((s16 *)&e->unkC, (s16 *)&e->unk14, e->unk4, (s16 *)&e->unkC);
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0xE), &e->unkC, 0, t + 0x800, frame, 1, -1, 0, 0);
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &e->unkC, 0, t + 0x800, frame, 2, -1, 0, 0);
        if (e->unk1C != NULL && e->unk1C->unk30 == -1) {
            e->unk1C = NULL;
        }
        if (D_801E86CC == 0) {
            if (e->unk1C != NULL) {
                e->unk1C->unk38 = e->unk1C->unk24->unk4.unk2;
            }
            e->unk0 = 1;
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_188/nonmatchings/fx_188", fx_188_op80);

INCLUDE_RODATA("asm/us/fx_188/nonmatchings/fx_188", fx_188_headerPtr);
