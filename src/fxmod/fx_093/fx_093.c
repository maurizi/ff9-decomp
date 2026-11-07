#include "fxmod/fx_093.h"

s32 fx_093_op80(s32 mode, Fx093Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone bone;
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx093Work);
    } else if (mode == 1) {
        fx_093_env = env;
        ex->btlfx_getUnitArgBonePos(BTLFX_UNIT_CASTER, 0, (u16 *)&work->pos0);
        ex->btlfx_getUnitBoneMatrix(BTLFX_UNIT_CASTER, env->targets->fxArgs[0], &bone);
        ex->btlfx_matrixPointAhead((MATRIX *)&bone, FX_CANNON_MUZZLE_REACH, (s32)&work->pos8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos18);
        ex->btlfx_aimAngles((s16 *)&work->pos0, (s16 *)&work->pos8, (s16 *)&work->aim20);
        work->emit28 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CANNON_SMOKE), (s32)&work->pos0);
        if (work->emit28 != NULL) {
            work->emit28->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x40);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CANNON_BLAST_RING), (s32)&work->pos0);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x40);
        }
    } else {
        frame = entry->frame;
        if (work->emit28 != NULL) {
            if (work->emit28->state == BTLFX_EMIT_DONE) {
                work->emit28 = NULL;
            }
            if (work->emit28 != NULL) {
                work->emit28->rot.vx = work->aim20.vx;
                work->emit28->rot.vy = work->aim20.vy;
                work->emit28->rot.vz = work->aim20.vz;
            }
        }
        if (frame == FX_CANNON_EXPLOSION_FRAME) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CANNON_EXPLOSION), (s32)&work->pos10);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CANNON_DUST_RING), (s32)&work->pos18);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, -0x40);
            }
        }
        return frame >= FX_CANNON_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_093/nonmatchings/fx_093", fx_093_headerPtr);
