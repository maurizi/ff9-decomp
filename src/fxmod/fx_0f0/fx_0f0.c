#include "fxmod/fx_0f0.h"

s32 fx_0f0_op80(s32 mode, Fx0f0Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0f0Work80);
    } else if (mode == 1) {
        fx_0f0_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos1C);
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ZOMBIE_POWDER_BLUE_SPECKS), (s32)&work->pos1C);
            break;
        case FX_ZOMBIE_POWDER_SPRAY_FRAME:
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos24);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ZOMBIE_POWDER_MIST_TRAIL), (s32)&work->pos24);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ZOMBIE_POWDER_MIST_TRAIL_2), (s32)&work->pos24);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ZOMBIE_POWDER_VIOLET_CLOUD), (s32)&work->pos24);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ZOMBIE_POWDER_SPORE_TRAIL), (s32)&work->pos24);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            if (work->emit10 != NULL) {
                work->emit10->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_0f0_op80Trails0;
                work->emit4->trailSlots = FX_ZOMBIE_POWDER_MIST_TRAIL_TRAIL_SLOTS;
                work->emit4->trailStride = FX_ZOMBIE_POWDER_MIST_TRAIL_TRAIL_STRIDE;
            }
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_0f0_op80Trails1;
                work->emit8->trailSlots = FX_ZOMBIE_POWDER_MIST_TRAIL_TRAIL_SLOTS;
                work->emit8->trailStride = FX_ZOMBIE_POWDER_MIST_TRAIL_TRAIL_STRIDE;
            }
            if (work->emit10 != NULL) {
                work->emit10->trails = (s32)fx_0f0_op80Trails2;
                work->emit10->trailSlots = FX_ZOMBIE_POWDER_SPORE_TRAIL_TRAIL_SLOTS;
                work->emit10->trailStride = FX_ZOMBIE_POWDER_SPORE_TRAIL_TRAIL_STRIDE;
            }
            break;
        }
        if (frame >= FX_ZOMBIE_POWDER_SPRAY_FRAME) {
            if (work->emit10 != NULL) {
                if (work->emit10->state == BTLFX_EMIT_DONE) {
                    work->emit10 = NULL;
                }
                if (work->emit10 != NULL) {
                    work->emit10->target = (s32)&work->pos14;
                }
            }
        }
        return frame >= FX_ZOMBIE_POWDER_FRAMES;
    }
    return 0;
}

s32 fx_0f0_op81(s32 mode, Fx0f0Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0f0Work81);
    } else if (mode == 1) {
        fx_0f0_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->posC);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ZOMBIE_POWDER_YELLOW_PUFFS), (s32)&work->posC);
        work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ZOMBIE_POWDER_YELLOW_CLOUD), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        if (work->emit8 != NULL) {
            work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == FX_ZOMBIE_POWDER_WAVE_FRAME) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ZOMBIE_POWDER_DUST_WAVE), (s32)&work->posC);
        }
        return frame >= FX_ZOMBIE_POWDER_HIT_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0f0/nonmatchings/fx_0f0", fx_0f0_headerPtr);
