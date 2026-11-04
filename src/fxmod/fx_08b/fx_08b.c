#include "fxmod/fx_08b.h"

INCLUDE_RODATA("asm/us/fx_08b/nonmatchings/fx_08b", fx_08b_headerPtr);

s32 fx_08b_op80(s32 mode, Fx08bWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    union {
        BtlBone bone;
        struct {
            VECTOR scale;
            SVECTOR rot;
            CVECTOR col;
        } s;
    } u;
    BtlBone bone2;
    s32 frame;
    s32 gridScale;

    if (mode == 0) {
        *args = 0x5C;
    } else if (mode == 1) {
        fx_08b_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos18);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot20);
        ex->btlfx_pointAhead((s16 *)&work->pos18, (s16 *)&work->rot20, 0x708, (s16 *)&work->pos38);
        ex->btlfx_pointAhead((s16 *)&work->pos18, (s16 *)&work->rot20, 0x7D0, (s16 *)&work->aimPos50);
        work->pos38.vy = -0xC8;
        work->aimPos50.vy = -0xC8;
        ex->btlfx_getUnitBoneMatrix(0x10, 5, &u.bone);
        ex->btlfx_matrixPointAhead((MATRIX *)&u.bone, 0x190, (s32)&work->pos28);
        ex->btlfx_getUnitBoneMatrix(0x10, 0x22, &bone2);
        ex->btlfx_matrixPointAhead((MATRIX *)&bone2, 0x190, (s32)&work->pos30);
        work->pos28.vy += 0x32;
        work->pos30.vy += 0x32;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_CRESCENT), (s32)&work->pos28);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_CRESCENT_2), (s32)&work->pos30);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x100);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x100);
        }
        ex->btlfx_aimAngles((s16 *)&work->pos28, (s16 *)&work->aimPos50, (s16 *)&work->aim40);
        ex->btlfx_aimAngles((s16 *)&work->pos30, (s16 *)&work->aimPos50, (s16 *)&work->aim48);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 11:
            ex->printf("flash\n");
            work->tint58 = 0x80;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, work->tint58, work->tint58, 0x80);
            break;
        case 10:
            work->tint58 = 0x80;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, work->tint58, work->tint58, 0x80);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_SPARKLE_CURTAIN), (s32)&work->pos38);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_SMOKE_RING), (s32)&work->pos38);
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_BLUE_GUST), (s32)&work->pos38);
            break;
        }
        if (frame >= 0xD) {
            gridScale = (frame << 7) + 0x980;
            u.s.scale.vx = gridScale;
            u.s.scale.vy = gridScale;
            u.s.scale.vz = gridScale;
            u.s.col.r = u.s.col.g = u.s.col.b = 0x28;
            u.s.rot.vx = 0;
            u.s.rot.vy = 0;
            u.s.rot.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &u.s.rot, &u.s.scale, &u.s.col, 1, NULL);
        }
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->aim40.vx;
                work->emit0->rot.vy = work->aim40.vy;
                work->emit0->rot.vz = work->aim40.vz;
            }
        }
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->aim48.vx;
                work->emit4->rot.vy = work->aim48.vy;
                work->emit4->rot.vz = work->aim48.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_08b_op81(s32 mode, Fx08bWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x10;
    } else if (mode == 1) {
        s32 target = *args;
        work->unitC = target;
        fx_08b_env = env;
        if (target >= env->targets->targetCount) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
            return 1;
        }
        fxm_setSignal(env, BTLFX_SIG_WAIT, 0);
        ex->btlfx_getUnitPos(work->unitC, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_VIOLET_PUFFS), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(work->unitC, 0x20);
        }
    } else {
        frame = entry->frame;
        if (frame == 1) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
        }
        return frame >= 0x28;
    }
    return 0;
}

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value) {
    env->state->signals[signal] = value;
}
