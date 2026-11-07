#include "fxmod/fx_190.h"

s32 fx_190_op80(s32 mode, Fx190Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    SVECTOR *aim;

    if (mode == 0) {
        *args = sizeof(Fx190Work);
    } else if (mode == 1) {
        work->count0 = 0;
        fx_190_env = env;
        ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, FX_VIRAL_SMOKE_GREEN_BONE, (u16 *)&work->pos44);
        ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, FX_VIRAL_SMOKE_BROWN_BONE, (u16 *)&work->pos4C);
        ex->btlfx_getUnitPos(BTLFX_UNIT_CASTER, (s16 *)&work->pos34);
        work->pos3C.vx = env->targets->centerX;
        work->pos3C.vz = env->targets->centerZ;
        work->pos3C.vy = 0;
        ex->btlfx_aimAngles((s16 *)&work->pos34, (s16 *)&work->pos3C, (s16 *)&work->rot4);
        ex->btlfx_aimAngles((s16 *)&work->pos44, (s16 *)&work->pos3C, (s16 *)&work->rotC);
        work->rot14[0].vy = 0x80;
        work->rot14[1].vy = -0x80;
        work->rot14[2].vy = 0xC0;
        work->rot14[3].vy = -0xC0;
        work->rot14[0].vx = 0x80;
        work->rot14[1].vx = 0x40;
        work->rot14[2].vx = 0x80;
        work->rot14[3].vx = 0x40;
        work->rotC.vy = work->rot4.vy;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
        case FX_VIRAL_SMOKE_PUFF_INTERVAL:
        case 2 * FX_VIRAL_SMOKE_PUFF_INTERVAL:
        case (FX_VIRAL_SMOKE_PUFF_COUNT - 1) * FX_VIRAL_SMOKE_PUFF_INTERVAL:
            work->emit54[work->count0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VIRAL_SMOKE_GREEN_PUFFS), (s32)&work->pos44);
            work->emit64[work->count0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VIRAL_SMOKE_BROWN_PUFFS), (s32)&work->pos4C);
            work->rot14[work->count0].vy += work->rotC.vy;
            work->rot14[work->count0].vx += work->rotC.vx;
            work->rot14[work->count0].vz += work->rotC.vz;
            work->count0++;
            break;
        }
        if (frame == FX_VIRAL_SMOKE_HELIX_FRAME) {
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VIRAL_SMOKE_PUFF_HELIX), (s32)&work->pos3C);
        }
        for (i = 0; i < work->count0; i++) {
            if (work->emit54[i] != NULL && work->emit54[i]->state == BTLFX_EMIT_DONE) {
                work->emit54[i] = NULL;
            }
            if (work->emit64[i] != NULL && work->emit64[i]->state == BTLFX_EMIT_DONE) {
                work->emit64[i] = NULL;
            }
            if (work->emit54[i] != NULL) {
                aim = (SVECTOR *)((u8 *)work + i * 8 + 0x14);
                work->emit54[i]->rot.vx = aim->vx;
                work->emit54[i]->rot.vy = aim->vy;
                work->emit54[i]->rot.vz = aim->vz;
            }
            if (work->emit64[i] != NULL) {
                aim = (SVECTOR *)((u8 *)work + i * 8 + 0x14);
                work->emit64[i]->rot.vx = aim->vx;
                work->emit64[i]->rot.vy = aim->vy;
                work->emit64[i]->rot.vz = aim->vz;
            }
        }
        return frame >= FX_VIRAL_SMOKE_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_190/nonmatchings/fx_190", fx_190_headerPtr);
