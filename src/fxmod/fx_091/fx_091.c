#include "fxmod/fx_091.h"

s32 fx_091_op80(s32 mode, Fx091Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        work->unk0 = 0;
        fx_091_env = env;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk8);
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0xC8);
        }
        ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&work->unk10, (s16 *)&work->unk18);
    } else {
        frame = entry->frame;
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->unk18.vx;
                work->emit4->rot.vy = work->unk18.vy;
                work->emit4->rot.vz = work->unk18.vz;
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 1), work->unk0);
        work->unk0++;
        return frame >= 0x42;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_091/nonmatchings/fx_091", fx_091_headerPtr);
