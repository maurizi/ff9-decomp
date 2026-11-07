#include "fxmod/fx_131.h"

s32 fx_131_op80(s32 mode, Fx131Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx131Work);
    } else if (mode == 1) {
        fx_131_env = env;
        ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, FX_GOLEM_REVIVAL_GLOW_BONE, (u16 *)&work->pos8);
        work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_GOLEM_REVIVAL_PINK_GLOW), (s32)&work->pos8);
        if (work->emit18 != NULL) {
            work->emit18->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
        }
    } else {
        frame = entry->frame;
        if (work->emit18 != NULL && work->emit18->state == BTLFX_EMIT_DONE) {
            work->emit18 = NULL;
        }
        ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, FX_GOLEM_REVIVAL_GLOW_BONE, (u16 *)&work->pos10);
        ex->btlfx_moveEmitterTo((s32)work->emit18, (s32)&work->pos10);
        if (frame == FX_GOLEM_REVIVAL_CLOUD_FRAME) {
            ex->btlfx_getUnitPos(BTLFX_UNIT_CASTER, (s16 *)&work->pos0);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_GOLEM_REVIVAL_SAND_CLOUD), (s32)&work->pos0);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
            }
        }
        return frame >= FX_GOLEM_REVIVAL_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_131/nonmatchings/fx_131", fx_131_headerPtr);
