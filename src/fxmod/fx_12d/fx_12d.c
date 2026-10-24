#include "fxmod/fx_12d.h"

s32 fx_12d_op80(s32 mode, Fx12dWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        D_801E7D68 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unkC);
        D_801E7E40.vx = work->unkC.vx;
        D_801E7E40.vy = work->unkC.vy;
        D_801E7E40.vz = work->unkC.vz;
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unkC);
    } else {
        frame = entry->frame;
        if (frame == 4) {
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unkC);
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_12d_op81(s32 mode, Fx12dWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        D_801E7D68 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk10);
        work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->unk10);
        if (work->unkC != NULL) {
            work->unkC->unk14 = (s32)D_801E8388;
            work->unkC->unk12 = 8;
            work->unkC->unkC = 0x60;
        }
        work->unk1C = -1;
    } else {
        frame = entry->frame;
        if (frame == 8) {
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk10);
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk10);
            if (work->unk8 != NULL) {
                work->unk8->unk14 = (s32)D_801E7E48;
                work->unk8->unk12 = 0xC;
                work->unk8->unkC = 0x70;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_12d_op82(s32 mode, Fx12dWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x38;
    } else if (mode == 1) {
        D_801E7D68 = env;
        ex->btlfx_initEffModels(D_801E7D78, 1);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk2C);
        work->unk0.vx = 0;
        work->unk0.vy = 0;
        work->unk0.vz = 0;
        work->unk8.vx = work->unk2C.vx;
        work->unk8.vy = work->unk2C.vy;
        work->unk8.vz = work->unk2C.vz;
        ex->btlfx_getUnitScale(0x10, &work->unk18);
        work->unk8.vx -= 0x28;
        work->unk8.vz += 0x304;
        ex->btlfx_getCasterTexInfo(&D_801E7CC0, &D_801E7CF0, 0x15E);
        for (i = 0; i < 6; i++) {
            D_801E7CD8[5 - i][0] = D_801E7CC0.unk0[i][0];
            D_801E7CD8[5 - i][1] = D_801E7CC0.unk0[i][1];
            D_801E7D08[5 - i][0] = D_801E7CF0.unk0[i][0];
            D_801E7D08[5 - i][1] = D_801E7CF0.unk0[i][1];
        }
        /* passes three arguments; the fourth parameter keeps whatever is in $a3 */
        work->unk34 = ((s32(*)())ex->Hi_RegisterTexListModel)(ex->btlfx_getFileData(entry->file, 0xE),
                                                               D_801E7CD8, D_801E7D08);
        ex->btlfx_setUnitVisible(0x10, 0);
        return 0;
    } else {
        frame = entry->frame;
        if (frame > 0) {
            ex->Hi_SetEffModelSlice(work->unk34, work->unk8.vy);
            ex->Hi_DrawSliceEffModel(&work->unk0, &work->unk8, (s32)&work->unk18, work->unk34);
            if (frame - 1 >= 0xC) {
                work->unk8.vy += 0xF;
            }
        }
        if (frame >= 0x50) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

void func_801E7C24(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7D68)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7C68(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_12d/nonmatchings/fx_12d", fx_12d_headerPtr);
