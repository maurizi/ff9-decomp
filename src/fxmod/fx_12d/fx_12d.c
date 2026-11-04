#include "fxmod/fx_12d.h"

s32 fx_12d_op80(s32 mode, Fx12dWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x14;
    } else if (mode == 1) {
        fx_12d_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->posC);
        fx_12d_casterPos.vx = work->posC.vx;
        fx_12d_casterPos.vy = work->posC.vy;
        fx_12d_casterPos.vz = work->posC.vz;
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_SAND_RING), (s32)&work->posC);
    } else {
        frame = entry->frame;
        if (frame == 4) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_SAND_DUST), (s32)&work->posC);
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_12d_op81(s32 mode, Fx12dWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x20;
    } else if (mode == 1) {
        fx_12d_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos10);
        work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_PINK_GLOW_TRAIL), (s32)&work->pos10);
        if (work->emitC != NULL) {
            work->emitC->trails = (s32)fx_12d_op81Trails1;
            work->emitC->trailSlots = 8;
            work->emitC->trailStride = 0x60;
        }
        work->unk1C = -1;
    } else {
        frame = entry->frame;
        if (frame == 8) {
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_SAND_CLOUD), (s32)&work->pos10);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_SAND_SWIRL), (s32)&work->pos10);
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_12d_op81Trails0;
                work->emit8->trailSlots = 0xC;
                work->emit8->trailStride = 0x70;
            }
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_12d_op82(s32 mode, Fx12dWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = 0x38;
    } else if (mode == 1) {
        fx_12d_env = env;
        ex->btlfx_initEffModels(fx_12d_effModels, 1);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos2C);
        work->rot0.vx = 0;
        work->rot0.vy = 0;
        work->rot0.vz = 0;
        work->pos8.vx = work->pos2C.vx;
        work->pos8.vy = work->pos2C.vy;
        work->pos8.vz = work->pos2C.vz;
        ex->btlfx_getUnitScale(0x10, &work->scale18);
        work->pos8.vx -= 0x28;
        work->pos8.vz += 0x304;
        ex->btlfx_getCasterTexInfo(&fx_12d_casterTexPages, &fx_12d_casterUvOffsets, 0x15E);
        for (i = 0; i < 6; i++) {
            fx_12d_texInfo[5 - i][0] = fx_12d_casterTexPages.pages[i][0];
            fx_12d_texInfo[5 - i][1] = fx_12d_casterTexPages.pages[i][1];
            fx_12d_texPos[5 - i][0] = fx_12d_casterUvOffsets.pages[i][0];
            fx_12d_texPos[5 - i][1] = fx_12d_casterUvOffsets.pages[i][1];
        }
        /* passes three arguments; the fourth parameter keeps whatever is in $a3 */
        work->model34 = ((s32(*)())ex->Hi_RegisterTexListModel)(ex->btlfx_getFileData(entry->file, FX_DEATH_ANTLION),
                                                               fx_12d_texInfo, fx_12d_texPos);
        ex->btlfx_setUnitVisible(0x10, 0);
        return 0;
    } else {
        frame = entry->frame;
        if (frame > 0) {
            ex->Hi_SetEffModelSlice(work->model34, work->pos8.vy);
            ex->Hi_DrawSliceEffModel(&work->rot0, &work->pos8, (s32)&work->scale18, work->model34);
            if (frame - 1 >= 0xC) {
                work->pos8.vy += 0xF;
            }
        }
        if (frame >= 0x50) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_12d_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_12d/nonmatchings/fx_12d", fx_12d_headerPtr);
