#include "fxmod/fx_0ad.h"

s32 fx_0ad_op80(s32 mode, Fx0adWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        work->unk0 = 0;
        fx_0ad_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos8);
        ex->btlfx_aimAngles((s16 *)&work->pos8, (s16 *)&work->pos10, (s16 *)&work->unk18);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos8);
        if (work->emit4 != NULL) {
            work->emit4->trails = (s32)D_801E793C;
            work->emit4->trailSlots = 0x24;
            work->emit4->trailStride = 0x5C;
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = 0x20;
        }
    } else {
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->unk18.vx;
                work->emit4->rot.vy = work->unk18.vy;
                work->emit4->rot.vz = work->unk18.vz;
            }
            if (frame == 0x16) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos10);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0, 0x10);
                }
            }
            if (frame >= 0x28) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ad/nonmatchings/fx_0ad", fx_0ad_headerPtr);
