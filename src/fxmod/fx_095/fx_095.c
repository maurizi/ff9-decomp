#include "fxmod/fx_095.h"

INCLUDE_ASM("asm/us/fx_095/nonmatchings/fx_095", func_801E7704);

s32 fx_095_op80(s32 mode, Fx095Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx095Part *part;
    s32 frame;
    s32 i;

    if (mode == 0) {
        ((EffInit *)arg4)->unk0 = sizeof(Fx095Work80);
        ((EffInit *)arg4)->unk8 = sizeof(Fx095Part);
        ((EffInit *)arg4)->unk14 = (s32)func_801E7704;
        ((EffInit *)arg4)->unk4 = 0xC;
    } else if (mode == 1) {
        D_801E8904 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&D_801E890C);
        work->unk4 = 0;
        work->unk0 = 0;
        work->unkC = 0;
    } else {
        frame = entry->unk0;
        gte_SetRotMatrix(env->unk14);
        gte_SetTransMatrix(env->unk14);
        gte_ldv0(&D_801E890C);
        gte_rtps();
        gte_stsxy(&work->unk8);
        switch (work->unk0) {
        case 0:
            for (i = 0; i < 2; i++) {
                part = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (part != NULL) {
                    part->unk8 = work->unk4;
                    part->unk0 = ((work->unk4 + 1) << 12) / 12;
                }
                work->unk4++;
            }
            if (work->unk4 >= 12) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            if (frame >= 16) {
                work->unk0 = 2;
                entry->unk0 = -1;
                work->unkC = 1;
            }
            break;
        case 2:
            if (frame >= 16) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            if (frame >= 8) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_095/nonmatchings/fx_095", func_801E7CBC);

INCLUDE_ASM("asm/us/fx_095/nonmatchings/fx_095", func_801E7F48);

s32 fx_095_op81(s32 mode, Fx095Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    DVECTOR xy;
    RECT rect;
    s32 frame;
    s32 c;

    if (mode == 0) {
        *arg4 = sizeof(Fx095Work81);
    } else if (mode == 1) {
        D_801E8904 = env;
        work->unk0 = 0;
        work->unk4 = 0;
    } else {
        frame = entry->unk0;
        rect.x = 0x1C0;
        rect.y = 0x100;
        rect.w = 0xC0;
        rect.h = 0xC0;
        gte_SetRotMatrix(env->unk14);
        gte_SetTransMatrix(env->unk14);
        gte_ldv0(&D_801E890C);
        gte_rtps();
        gte_stsxy(&xy);
        switch (work->unk0) {
        case 0:
            func_801E7CBC(frame, xy.vx, xy.vy, work->unk8, &rect);
            if (frame >= 6) {
                work->unk0 = 1;
                entry->unk0 = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 2, 0x9B, 0xFF, 0);
            }
            break;
        case 1:
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x80, 0x80, 0xFF);
            }
            func_801E7F48(8, xy.vx, xy.vy, 0x60, 0x80, &rect);
            ex->func_800D1BC4(7, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), xy.vx, xy.vy, 0, 1, 0x1000);
            if (frame >= 16) {
                work->unk0 = 2;
                entry->unk0 = -1;
            }
            break;
        case 2:
            c = 0x1000 - (frame << 9);
            func_801E7F48(8, xy.vx, xy.vy, 0x60, c >> 5, &rect);
            ex->func_800D1BC4(7, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), xy.vx, xy.vy, 0, 1, c);
            if (frame >= 8) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_095/nonmatchings/fx_095", fx_095_headerPtr);
